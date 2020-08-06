mod linked_list;

use linked_list::LinkedList;

use std::cell::RefCell;
use std::rc::Rc;

type Link = Option<Rc<RefCell<Node>>>;

struct Node {
    chars: LinkedList<(char, Link)>,
    is_end: bool,
}

impl Node {
    fn new() -> Node {
        Node {
            chars: LinkedList::new(),
            is_end: false,
        }
    }

    fn insert<T>(&mut self, mut iter: T)
    where
        T: Iterator<Item=char>
    {
        if let Some(ch) = iter.next() {
            let node = Rc::new(RefCell::new(Node::new()));
            node.borrow_mut().insert(iter);
            self.chars.insert((ch, Some(node)));
        } else {
            self.is_end = true;
        }
    }
}
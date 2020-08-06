mod linked_list;

use linked_list::LinkedList;

use std::cell::RefCell;
use std::rc::Rc;

type Link = RefCell<Option<Rc<Node>>>;

struct Node {
    chars: LinkedList<(char, Link)>,
    is_end: bool,
}

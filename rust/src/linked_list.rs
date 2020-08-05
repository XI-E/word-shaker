use std::cell::RefCell;
use std::fmt;
use std::rc::Rc;

type Link<T> = RefCell<Option<Rc<Node<T>>>>;

struct Node<T>
where
    T: fmt::Display,
{
    data: T,
    next: Link<T>,
}

impl<T: fmt::Display> Node<T> {
    fn new(data: T) -> Node<T> {
        Node {
            data,
            next: RefCell::new(None),
        }
    }

    fn insert_after(&self, data: T) -> Rc<Node<T>> {
        let new_node = Rc::new(Node::new(data));
        *new_node.next.borrow_mut() = Some(Rc::clone(&new_node));
        self.next.swap(&new_node.next);
        new_node
    }
}

impl<T: fmt::Display> fmt::Display for Node<T> {
    fn fmt(&self, f: &mut fmt::Formatter<'_>) -> fmt::Result {
        match self.next.borrow().as_ref() {
            None => write!(f, "{} -> {}", self.data, "nil"),
            Some(node) => write!(f, "{} -> {}", self.data, node),
        }
    }
}

struct LinkedList<T>
where
    T: fmt::Display,
{
    head: Link<T>,
    tail: Link<T>,
}

impl<T: fmt::Display> LinkedList<T> {
    fn new() -> LinkedList<T> {
        LinkedList {
            head: RefCell::new(None),
            tail: RefCell::new(None),
        }
    }

    fn insert(&self, data: T) {
        if let Some(tail) = self.tail.borrow().as_ref() {
            tail.insert_after(data);
        } else {
            let new_node = Rc::new(Node::new(data));
            *self.head.borrow_mut() = Some(Rc::clone(&new_node));
            *self.head.borrow_mut() = Some(new_node);
        }
    }
}

impl<T: fmt::Display> fmt::Display for LinkedList<T> {
    fn fmt(&self, f: &mut fmt::Formatter<'_>) -> fmt::Result {
        match self.head.borrow().as_ref() {
            None => write!(f, "{}", "nil"),
            Some(head) => write!(f, "{}", head),
        }
    }
}

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn node_insert_end() {
        let head = Node::new(10);
        let new_node = head.insert_after(5);
        assert_eq!(head.data, 10);
        assert_eq!(new_node.data, head.next.borrow().as_ref().unwrap().data);
        assert_eq!(new_node.data, 5);
        println!("{}", head);
    }

    #[test]
    fn list_insert() {

    }
}

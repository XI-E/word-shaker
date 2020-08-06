use std::cell::RefCell;
use std::rc::Rc;

type Link<T> = RefCell<Option<Rc<Node<T>>>>;

struct Node<T> {
    data: T,
    next: Link<T>,
}

impl<T> Node<T> {
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

pub struct LinkedList<T> {
    head: Link<T>,
    tail: Link<T>,
}

impl<T> LinkedList<T> {
    pub fn new() -> LinkedList<T> {
        LinkedList {
            head: RefCell::new(None),
            tail: RefCell::new(None),
        }
    }

    pub fn insert(&self, data: T) {
        let new_node;
        if let Some(tail) = self.tail.borrow().as_ref() {
            new_node = tail.insert_after(data);
        } else {
            new_node = Rc::new(Node::new(data));
            *self.head.borrow_mut() = Some(Rc::clone(&new_node));
        }
        *self.tail.borrow_mut() = Some(new_node);
    }
}

#[cfg(test)]
mod tests {
    use super::*;

    /// Asserts that node pointed by this link exists and returns a reference to it
    fn assert_exists<T>(node: &Link<T>) -> Rc<Node<T>> {
        Rc::clone(node.borrow().as_ref().unwrap())
    }

    /// Asserts that node pointed by this link does not exist
    fn assert_not_exists<T>(node: &Link<T>) {
        assert!(node.borrow().is_none());
    }

    #[test]
    fn node_insert_end() {
        let head = Node::new(10);
        let new_node = head.insert_after(5);
        assert_eq!(head.data, 10);
        let head_next = assert_exists(&head.next);
        assert_eq!(new_node.data, head_next.data);
        assert_eq!(new_node.data, 5);
        assert_not_exists(&head_next.next);
        assert_not_exists(&new_node.next);
    }

    #[test]
    fn list_insert() {
        let list = LinkedList::new();
        list.insert(5);
        list.insert(10);
        list.insert(15);

        let node = assert_exists(&list.head);
        assert_eq!(node.data, 5);
        let node = assert_exists(&node.next);
        assert_eq!(node.data, 10);
        let node = assert_exists(&node.next);
        assert_eq!(node.data, 15);
        assert_not_exists(&node.next);
    }
}

use std::cell::RefCell;
use std::rc::Rc;

type Link<T> = Option<Rc<RefCell<Node<T>>>>;

struct Node<T> {
    data: T,
    next: Link<T>,
}

impl<T> Node<T> {
    fn new(data: T) -> Node<T> {
        Node {
            data,
            next: None,
        }
    }

    fn insert_after(&mut self, data: T) -> Rc<RefCell<Node<T>>> {
        let new_node = Rc::new(RefCell::new(Node::new(data)));
        new_node.borrow_mut().next = if let Some(node) = &self.next {
            Some(Rc::clone(&node))
        } else {
            None
        };
        self.next = Some(Rc::clone(&new_node));
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
            head: None,
            tail: None,
        }
    }

    pub fn insert(&mut self, data: T) {
        let new_node;
        if let Some(tail) = &self.tail {
            new_node = tail.borrow_mut().insert_after(data);
        } else {
            new_node = Rc::new(RefCell::new(Node::new(data)));
            self.head = Some(Rc::clone(&new_node));
        }
        self.tail = Some(new_node);
    }
}

/* struct LinkedListIterator<'a, T> {
    node: Option<&'a Node<T>>,
}

impl<'a, T> Iterator for LinkedListIterator<'a, T> {
    type Item = &'a T;

    fn next(&mut self) -> Option<&'a T> {
        if let Some(node) = self.node {
            self.node = node.next.borrow().as_ref().map(|node| &**node);
            Some(&node.data)
        } else {
            None
        }
    }
} */

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn node_insert_end() {
        let mut head = Node::new(10);
        let new_node = head.insert_after(5);
        assert_eq!(head.data, 10);
        let head_next = head.next.expect("Node should exist");
        assert_eq!(new_node.borrow().data, head_next.borrow().data);
        assert_eq!(new_node.borrow().data, 5);
        assert!(head_next.borrow().next.is_none());
        assert!(new_node.borrow().next.is_none());
    }

    #[test]
    fn list_insert() {
        let mut list = LinkedList::new();
        list.insert(5);
        list.insert(10);
        list.insert(15);

        let node = list.head.expect("Lists head should exist");
        assert_eq!(node.borrow().data, 5);
        let node = Rc::clone(node.borrow().next.as_ref().expect("Node should exist"));
        assert_eq!(node.borrow().data, 10);
        let node = Rc::clone(node.borrow().next.as_ref().expect("Node should exist"));
        assert_eq!(node.borrow().data, 15);
        assert!(node.borrow().next.is_none());
    }
}

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

    /// Asserts that node pointed by this link exists and returns a reference to it
    fn assert_exists<T>(node: &Link<T>) -> Rc<Node<T>>
    where
        T: fmt::Display
    {
        Rc::clone(node.borrow().as_ref().unwrap())
    }

    /// Asserts that node pointed by this link does not exist
    fn assert_not_exists<T>(node: &Link<T>)
    where
        T: fmt::Display
    {
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
        println!("{}", head);
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

        println!("{}", list);
    }
}

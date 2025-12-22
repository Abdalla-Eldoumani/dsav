//! Educational linked list implementation with visualization support.
//!
//! This implementation demonstrates linked list operations with visual
//! representation of nodes and pointer connections.

use crate::error::{DsavError, Result};
use crate::state::{RenderElement, RenderState};
use crate::traits::{Operation, Step, Visualizable};

#[derive(Debug, Clone)]
struct Node {
    value: i32,
    next: Option<Box<Node>>,
}

impl Node {
    fn new(value: i32) -> Self {
        Self { value, next: None }
    }
}

#[derive(Debug, Clone)]
pub struct VisualizableLinkedList {
    head: Option<Box<Node>>,
    length: usize,
}

impl VisualizableLinkedList {
    pub fn new() -> Self {
        Self {
            head: None,
            length: 0,
        }
    }

    pub fn insert_front(&mut self, value: i32) {
        let mut new_node = Box::new(Node::new(value));
        new_node.next = self.head.take();
        self.head = Some(new_node);
        self.length += 1;
    }

    pub fn insert_back(&mut self, value: i32) {
        let new_node = Box::new(Node::new(value));

        if self.head.is_none() {
            self.head = Some(new_node);
        } else {
            let mut current = self.head.as_mut().unwrap();
            while current.next.is_some() {
                current = current.next.as_mut().unwrap();
            }
            current.next = Some(new_node);
        }

        self.length += 1;
    }

    pub fn insert_at(&mut self, index: usize, value: i32) -> Result<()> {
        if index > self.length {
            return Err(DsavError::IndexOutOfBounds {
                index,
                size: self.length,
            });
        }

        if index == 0 {
            self.insert_front(value);
            return Ok(());
        }

        let mut current = self.head.as_mut().unwrap();
        for _ in 0..index - 1 {
            if current.next.is_none() {
                return Err(DsavError::IndexOutOfBounds {
                    index,
                    size: self.length,
                });
            }
            current = current.next.as_mut().unwrap();
        }

        let mut new_node = Box::new(Node::new(value));
        new_node.next = current.next.take();
        current.next = Some(new_node);
        self.length += 1;

        Ok(())
    }

    pub fn delete_front(&mut self) -> Result<i32> {
        if let Some(mut old_head) = self.head.take() {
            self.head = old_head.next.take();
            self.length -= 1;
            Ok(old_head.value)
        } else {
            Err(DsavError::EmptyStructure)
        }
    }

    pub fn delete_back(&mut self) -> Result<i32> {
        if self.head.is_none() {
            return Err(DsavError::EmptyStructure);
        }

        if self.head.as_ref().unwrap().next.is_none() {
            let value = self.head.take().unwrap().value;
            self.length -= 1;
            return Ok(value);
        }

        let mut current = self.head.as_mut().unwrap();
        while current.next.as_ref().unwrap().next.is_some() {
            current = current.next.as_mut().unwrap();
        }

        let value = current.next.take().unwrap().value;
        self.length -= 1;
        Ok(value)
    }

    pub fn search(&self, target: i32) -> Option<usize> {
        let mut current = self.head.as_ref();
        let mut index = 0;

        while let Some(node) = current {
            if node.value == target {
                return Some(index);
            }
            current = node.next.as_ref();
            index += 1;
        }

        None
    }

    pub fn get(&self, index: usize) -> Result<i32> {
        if index >= self.length {
            return Err(DsavError::IndexOutOfBounds {
                index,
                size: self.length,
            });
        }

        let mut current = self.head.as_ref().unwrap();
        for _ in 0..index {
            current = current.next.as_ref().unwrap();
        }

        Ok(current.value)
    }

    pub fn len(&self) -> usize {
        self.length
    }

    pub fn is_empty(&self) -> bool {
        self.length == 0
    }

    pub fn clear(&mut self) {
        self.head = None;
        self.length = 0;
    }

    fn to_vec(&self) -> Vec<i32> {
        let mut result = Vec::new();
        let mut current = self.head.as_ref();

        while let Some(node) = current {
            result.push(node.value);
            current = node.next.as_ref();
        }

        result
    }
}

impl Default for VisualizableLinkedList {
    fn default() -> Self {
        Self::new()
    }
}

impl Visualizable for VisualizableLinkedList {
    fn execute_with_steps(&mut self, operation: Operation) -> Result<Vec<Step>> {
        match operation {
            Operation::Insert(index, value) => {
                let mut steps = Vec::new();

                steps.push(Step {
                    description: format!("Inserting {} at position {}", value, index),
                    highlight_indices: vec![],
                    active_indices: vec![],
                    metadata: serde_json::json!({
                        "operation": "insert",
                        "value": value,
                        "index": index
                    }),
                });

                if index == 0 {
                    steps.push(Step {
                        description: "Inserting at head of list".to_string(),
                        highlight_indices: vec![0],
                        active_indices: vec![],
                        metadata: serde_json::json!({}),
                    });

                    self.insert_front(value);
                } else {
                    for i in 0..index.min(self.length) {
                        steps.push(Step {
                            description: format!("Traversing to position {}", i),
                            highlight_indices: vec![i],
                            active_indices: vec![],
                            metadata: serde_json::json!({}),
                        });
                    }

                    self.insert_at(index, value)?;
                }

                steps.push(Step {
                    description: format!("Successfully inserted {} at position {}", value, index),
                    highlight_indices: vec![],
                    active_indices: vec![index],
                    metadata: serde_json::json!({}),
                });

                Ok(steps)
            }

            Operation::Delete(index) => {
                let mut steps = Vec::new();

                if index >= self.length {
                    return Err(DsavError::IndexOutOfBounds {
                        index,
                        size: self.length,
                    });
                }

                let value = self.get(index)?;

                steps.push(Step {
                    description: format!("Deleting node at position {}", index),
                    highlight_indices: vec![index],
                    active_indices: vec![],
                    metadata: serde_json::json!({
                        "operation": "delete",
                        "index": index
                    }),
                });

                for i in 0..index {
                    steps.push(Step {
                        description: format!("Traversing to position {}", i),
                        highlight_indices: vec![i],
                        active_indices: vec![],
                        metadata: serde_json::json!({}),
                    });
                }

                if index == 0 {
                    self.delete_front()?;
                } else if index == self.length - 1 {
                    self.delete_back()?;
                } else {
                    let mut current = self.head.as_mut().unwrap();
                    for _ in 0..index - 1 {
                        current = current.next.as_mut().unwrap();
                    }
                    current.next = current.next.as_mut().unwrap().next.take();
                    self.length -= 1;
                }

                steps.push(Step {
                    description: format!("Deleted node with value {}", value),
                    highlight_indices: vec![],
                    active_indices: vec![],
                    metadata: serde_json::json!({}),
                });

                Ok(steps)
            }

            Operation::Search(target) => {
                let mut steps = Vec::new();

                steps.push(Step {
                    description: format!("Searching for value {}", target),
                    highlight_indices: vec![],
                    active_indices: vec![],
                    metadata: serde_json::json!({
                        "operation": "search",
                        "target": target
                    }),
                });

                let mut current = self.head.as_ref();
                let mut index = 0;
                let mut found = false;

                while let Some(node) = current {
                    steps.push(Step {
                        description: format!("Checking node at position {} (value: {})", index, node.value),
                        highlight_indices: vec![index],
                        active_indices: vec![],
                        metadata: serde_json::json!({}),
                    });

                    if node.value == target {
                        steps.push(Step {
                            description: format!("Found {} at position {}", target, index),
                            highlight_indices: vec![],
                            active_indices: vec![index],
                            metadata: serde_json::json!({
                                "found": true,
                                "index": index
                            }),
                        });
                        found = true;
                        break;
                    }

                    current = node.next.as_ref();
                    index += 1;
                }

                if !found {
                    steps.push(Step {
                        description: format!("Value {} not found in list", target),
                        highlight_indices: vec![],
                        active_indices: vec![],
                        metadata: serde_json::json!({
                            "found": false
                        }),
                    });
                }

                Ok(steps)
            }

            Operation::Traverse => {
                let mut steps = Vec::new();

                steps.push(Step {
                    description: "Starting list traversal".to_string(),
                    highlight_indices: vec![],
                    active_indices: vec![],
                    metadata: serde_json::json!({
                        "operation": "traverse"
                    }),
                });

                let mut current = self.head.as_ref();
                let mut index = 0;

                while let Some(node) = current {
                    steps.push(Step {
                        description: format!("Visiting node {} (value: {})", index, node.value),
                        highlight_indices: vec![index],
                        active_indices: vec![],
                        metadata: serde_json::json!({
                            "index": index,
                            "value": node.value
                        }),
                    });

                    current = node.next.as_ref();
                    index += 1;
                }

                steps.push(Step {
                    description: "Traversal complete".to_string(),
                    highlight_indices: vec![],
                    active_indices: vec![],
                    metadata: serde_json::json!({}),
                });

                Ok(steps)
            }

            _ => Err(DsavError::Visualization(
                "Operation not supported for linked list".to_string(),
            )),
        }
    }

    fn render_state(&self) -> RenderState {
        let elements: Vec<RenderElement> = self
            .to_vec()
            .into_iter()
            .enumerate()
            .map(|(i, value)| {
                RenderElement::new(value)
                    .with_sublabel(format!("Node {}", i))
            })
            .collect();

        let connections: Vec<(usize, usize)> = (0..elements.len().saturating_sub(1))
            .map(|i| (i, i + 1))
            .collect();

        RenderState {
            elements,
            connections,
        }
    }
}

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn test_linked_list_insert_front() {
        let mut list = VisualizableLinkedList::new();
        list.insert_front(10);
        list.insert_front(20);
        list.insert_front(30);

        assert_eq!(list.len(), 3);
        assert_eq!(list.get(0).unwrap(), 30);
        assert_eq!(list.get(1).unwrap(), 20);
        assert_eq!(list.get(2).unwrap(), 10);
    }

    #[test]
    fn test_linked_list_insert_back() {
        let mut list = VisualizableLinkedList::new();
        list.insert_back(10);
        list.insert_back(20);
        list.insert_back(30);

        assert_eq!(list.len(), 3);
        assert_eq!(list.get(0).unwrap(), 10);
        assert_eq!(list.get(1).unwrap(), 20);
        assert_eq!(list.get(2).unwrap(), 30);
    }

    #[test]
    fn test_linked_list_insert_at() {
        let mut list = VisualizableLinkedList::new();
        list.insert_back(10);
        list.insert_back(30);

        assert!(list.insert_at(1, 20).is_ok());

        assert_eq!(list.len(), 3);
        assert_eq!(list.get(0).unwrap(), 10);
        assert_eq!(list.get(1).unwrap(), 20);
        assert_eq!(list.get(2).unwrap(), 30);
    }

    #[test]
    fn test_linked_list_delete_front() {
        let mut list = VisualizableLinkedList::new();
        list.insert_back(10);
        list.insert_back(20);
        list.insert_back(30);

        assert_eq!(list.delete_front().unwrap(), 10);
        assert_eq!(list.len(), 2);
        assert_eq!(list.get(0).unwrap(), 20);
    }

    #[test]
    fn test_linked_list_delete_back() {
        let mut list = VisualizableLinkedList::new();
        list.insert_back(10);
        list.insert_back(20);
        list.insert_back(30);

        assert_eq!(list.delete_back().unwrap(), 30);
        assert_eq!(list.len(), 2);
        assert_eq!(list.get(1).unwrap(), 20);
    }

    #[test]
    fn test_linked_list_search() {
        let mut list = VisualizableLinkedList::new();
        list.insert_back(10);
        list.insert_back(20);
        list.insert_back(30);

        assert_eq!(list.search(20), Some(1));
        assert_eq!(list.search(40), None);
    }

    #[test]
    fn test_linked_list_empty() {
        let list = VisualizableLinkedList::new();
        assert!(list.is_empty());
        assert_eq!(list.len(), 0);
    }

    #[test]
    fn test_linked_list_delete_empty() {
        let mut list = VisualizableLinkedList::new();
        assert!(list.delete_front().is_err());
        assert!(list.delete_back().is_err());
    }
}

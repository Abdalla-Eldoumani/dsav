//! Educational binary search tree implementation with visualization support.
//!
//! This implementation demonstrates BST operations with visual
//! representation of nodes and tree structure.

use crate::error::{DsavError, Result};
use crate::state::{RenderElement, RenderState};
use crate::traits::{Operation, Step, Visualizable};

#[derive(Debug, Clone)]
struct Node {
    value: i32,
    left: Option<Box<Node>>,
    right: Option<Box<Node>>,
}

impl Node {
    fn new(value: i32) -> Self {
        Self {
            value,
            left: None,
            right: None,
        }
    }
}

#[derive(Debug, Clone)]
pub struct VisualizableBST {
    root: Option<Box<Node>>,
    size: usize,
}

impl VisualizableBST {
    pub fn new() -> Self {
        Self {
            root: None,
            size: 0,
        }
    }

    pub fn insert(&mut self, value: i32) {
        if self.root.is_none() {
            self.root = Some(Box::new(Node::new(value)));
            self.size += 1;
        } else {
            Self::insert_recursive(&mut self.root, value);
            self.size += 1;
        }
    }

    fn insert_recursive(node: &mut Option<Box<Node>>, value: i32) {
        if let Some(n) = node {
            if value < n.value {
                if n.left.is_none() {
                    n.left = Some(Box::new(Node::new(value)));
                } else {
                    Self::insert_recursive(&mut n.left, value);
                }
            } else if value > n.value {
                if n.right.is_none() {
                    n.right = Some(Box::new(Node::new(value)));
                } else {
                    Self::insert_recursive(&mut n.right, value);
                }
            }
            // If value == n.value, we don't insert duplicates
        }
    }

    pub fn search(&self, value: i32) -> bool {
        Self::search_recursive(&self.root, value)
    }

    fn search_recursive(node: &Option<Box<Node>>, value: i32) -> bool {
        match node {
            None => false,
            Some(n) => {
                if value == n.value {
                    true
                } else if value < n.value {
                    Self::search_recursive(&n.left, value)
                } else {
                    Self::search_recursive(&n.right, value)
                }
            }
        }
    }

    pub fn size(&self) -> usize {
        self.size
    }

    pub fn is_empty(&self) -> bool {
        self.size == 0
    }

    pub fn clear(&mut self) {
        self.root = None;
        self.size = 0;
    }

    // Helper to collect nodes for visualization (in-order traversal)
    fn collect_nodes(&self) -> Vec<i32> {
        let mut nodes = Vec::new();
        Self::inorder_collect(&self.root, &mut nodes);
        nodes
    }

    fn inorder_collect(node: &Option<Box<Node>>, nodes: &mut Vec<i32>) {
        if let Some(n) = node {
            Self::inorder_collect(&n.left, nodes);
            nodes.push(n.value);
            Self::inorder_collect(&n.right, nodes);
        }
    }
}

impl Default for VisualizableBST {
    fn default() -> Self {
        Self::new()
    }
}

impl Visualizable for VisualizableBST {
    fn execute_with_steps(&mut self, operation: Operation) -> Result<Vec<Step>> {
        match operation {
            Operation::Insert(_, value) => {
                let mut steps = Vec::new();

                steps.push(Step {
                    description: format!("Inserting {} into BST", value),
                    highlight_indices: vec![],
                    active_indices: vec![],
                    metadata: serde_json::json!({
                        "operation": "insert",
                        "value": value
                    }),
                });

                if self.root.is_none() {
                    steps.push(Step {
                        description: format!("Tree is empty, {} becomes root", value),
                        highlight_indices: vec![],
                        active_indices: vec![0],
                        metadata: serde_json::json!({}),
                    });
                    self.insert(value);
                } else {
                    // Traverse to find insertion point
                    let mut path = Vec::new();
                    let mut current = self.root.as_ref();
                    let mut idx = 0;

                    while let Some(node) = current {
                        path.push(idx);

                        steps.push(Step {
                            description: format!("Comparing {} with {}", value, node.value),
                            highlight_indices: path.clone(),
                            active_indices: vec![],
                            metadata: serde_json::json!({}),
                        });

                        if value < node.value {
                            current = node.left.as_ref();
                            idx = idx * 2 + 1; // Left child
                        } else if value > node.value {
                            current = node.right.as_ref();
                            idx = idx * 2 + 2; // Right child
                        } else {
                            // Duplicate value
                            steps.push(Step {
                                description: format!("{} already exists in tree", value),
                                highlight_indices: path.clone(),
                                active_indices: vec![],
                                metadata: serde_json::json!({}),
                            });
                            return Ok(steps);
                        }
                    }

                    self.insert(value);

                    steps.push(Step {
                        description: format!("Inserted {} successfully", value),
                        highlight_indices: vec![],
                        active_indices: vec![idx],
                        metadata: serde_json::json!({}),
                    });
                }

                Ok(steps)
            }

            Operation::Search(target) => {
                let mut steps = Vec::new();

                steps.push(Step {
                    description: format!("Searching for {} in BST", target),
                    highlight_indices: vec![],
                    active_indices: vec![],
                    metadata: serde_json::json!({
                        "operation": "search",
                        "target": target
                    }),
                });

                let mut current = self.root.as_ref();
                let mut idx = 0;
                let mut found = false;

                while let Some(node) = current {
                    steps.push(Step {
                        description: format!("Checking node with value {}", node.value),
                        highlight_indices: vec![idx],
                        active_indices: vec![],
                        metadata: serde_json::json!({}),
                    });

                    if target == node.value {
                        steps.push(Step {
                            description: format!("Found {} at node", target),
                            highlight_indices: vec![],
                            active_indices: vec![idx],
                            metadata: serde_json::json!({
                                "found": true,
                                "index": idx
                            }),
                        });
                        found = true;
                        break;
                    } else if target < node.value {
                        current = node.left.as_ref();
                        idx = idx * 2 + 1;
                    } else {
                        current = node.right.as_ref();
                        idx = idx * 2 + 2;
                    }
                }

                if !found {
                    steps.push(Step {
                        description: format!("Value {} not found in tree", target),
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
                    description: "Starting in-order traversal (left, root, right)".to_string(),
                    highlight_indices: vec![],
                    active_indices: vec![],
                    metadata: serde_json::json!({
                        "operation": "traverse"
                    }),
                });

                Self::inorder_traverse(&self.root, 0, &mut steps);

                steps.push(Step {
                    description: "In-order traversal complete".to_string(),
                    highlight_indices: vec![],
                    active_indices: vec![],
                    metadata: serde_json::json!({}),
                });

                Ok(steps)
            }

            Operation::PreOrderTraverse => {
                let mut steps = Vec::new();

                steps.push(Step {
                    description: "Starting pre-order traversal (root, left, right)".to_string(),
                    highlight_indices: vec![],
                    active_indices: vec![],
                    metadata: serde_json::json!({
                        "operation": "preorder_traverse"
                    }),
                });

                Self::preorder_traverse(&self.root, 0, &mut steps);

                steps.push(Step {
                    description: "Pre-order traversal complete".to_string(),
                    highlight_indices: vec![],
                    active_indices: vec![],
                    metadata: serde_json::json!({}),
                });

                Ok(steps)
            }

            Operation::PostOrderTraverse => {
                let mut steps = Vec::new();

                steps.push(Step {
                    description: "Starting post-order traversal (left, right, root)".to_string(),
                    highlight_indices: vec![],
                    active_indices: vec![],
                    metadata: serde_json::json!({
                        "operation": "postorder_traverse"
                    }),
                });

                Self::postorder_traverse(&self.root, 0, &mut steps);

                steps.push(Step {
                    description: "Post-order traversal complete".to_string(),
                    highlight_indices: vec![],
                    active_indices: vec![],
                    metadata: serde_json::json!({}),
                });

                Ok(steps)
            }

            Operation::LevelOrderTraverse => {
                let mut steps = Vec::new();

                steps.push(Step {
                    description: "Starting level-order traversal (breadth-first)".to_string(),
                    highlight_indices: vec![],
                    active_indices: vec![],
                    metadata: serde_json::json!({
                        "operation": "levelorder_traverse"
                    }),
                });

                Self::levelorder_traverse(&self.root, &mut steps);

                steps.push(Step {
                    description: "Level-order traversal complete".to_string(),
                    highlight_indices: vec![],
                    active_indices: vec![],
                    metadata: serde_json::json!({}),
                });

                Ok(steps)
            }

            _ => Err(DsavError::Visualization(
                "Operation not supported for BST".to_string(),
            )),
        }
    }

    fn render_state(&self) -> RenderState {
        let mut elements = Vec::new();
        let mut connections = Vec::new();

        Self::build_render_state(&self.root, 0, &mut elements, &mut connections);

        RenderState {
            elements,
            connections,
        }
    }
}

impl VisualizableBST {
    fn inorder_traverse(node: &Option<Box<Node>>, idx: usize, steps: &mut Vec<Step>) {
        if let Some(n) = node {
            Self::inorder_traverse(&n.left, idx * 2 + 1, steps);

            steps.push(Step {
                description: format!("Visiting node {}", n.value),
                highlight_indices: vec![idx],
                active_indices: vec![],
                metadata: serde_json::json!({
                    "value": n.value,
                    "index": idx
                }),
            });

            Self::inorder_traverse(&n.right, idx * 2 + 2, steps);
        }
    }

    fn preorder_traverse(node: &Option<Box<Node>>, idx: usize, steps: &mut Vec<Step>) {
        if let Some(n) = node {
            steps.push(Step {
                description: format!("Visiting node {}", n.value),
                highlight_indices: vec![idx],
                active_indices: vec![],
                metadata: serde_json::json!({
                    "value": n.value,
                    "index": idx
                }),
            });

            Self::preorder_traverse(&n.left, idx * 2 + 1, steps);
            Self::preorder_traverse(&n.right, idx * 2 + 2, steps);
        }
    }

    fn postorder_traverse(node: &Option<Box<Node>>, idx: usize, steps: &mut Vec<Step>) {
        if let Some(n) = node {
            Self::postorder_traverse(&n.left, idx * 2 + 1, steps);
            Self::postorder_traverse(&n.right, idx * 2 + 2, steps);

            steps.push(Step {
                description: format!("Visiting node {}", n.value),
                highlight_indices: vec![idx],
                active_indices: vec![],
                metadata: serde_json::json!({
                    "value": n.value,
                    "index": idx
                }),
            });
        }
    }

    fn levelorder_traverse(root: &Option<Box<Node>>, steps: &mut Vec<Step>) {
        use std::collections::VecDeque;

        if root.is_none() {
            return;
        }

        let mut queue = VecDeque::new();
        queue.push_back((root, 0)); // (node, index)

        while let Some((node_opt, idx)) = queue.pop_front() {
            if let Some(node) = node_opt {
                steps.push(Step {
                    description: format!("Visiting node {}", node.value),
                    highlight_indices: vec![idx],
                    active_indices: vec![],
                    metadata: serde_json::json!({
                        "value": node.value,
                        "index": idx
                    }),
                });

                // Enqueue left child
                if node.left.is_some() {
                    queue.push_back((&node.left, idx * 2 + 1));
                }

                // Enqueue right child
                if node.right.is_some() {
                    queue.push_back((&node.right, idx * 2 + 2));
                }
            }
        }
    }

    fn build_render_state(
        node: &Option<Box<Node>>,
        idx: usize,
        elements: &mut Vec<RenderElement>,
        connections: &mut Vec<(usize, usize)>,
    ) {
        if let Some(n) = node {
            // Ensure we have enough space in elements vector
            while elements.len() <= idx {
                elements.push(RenderElement::new(0).with_label("".to_string()));
            }

            elements[idx] = RenderElement::new(n.value)
                .with_label(n.value.to_string())
                .with_sublabel(format!("Node {}", idx));

            // Process left child
            if n.left.is_some() {
                let left_idx = idx * 2 + 1;
                connections.push((idx, left_idx));
                Self::build_render_state(&n.left, left_idx, elements, connections);
            }

            // Process right child
            if n.right.is_some() {
                let right_idx = idx * 2 + 2;
                connections.push((idx, right_idx));
                Self::build_render_state(&n.right, right_idx, elements, connections);
            }
        }
    }
}

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn test_bst_insert() {
        let mut bst = VisualizableBST::new();
        bst.insert(50);
        bst.insert(30);
        bst.insert(70);

        assert_eq!(bst.size(), 3);
        assert!(bst.search(50));
        assert!(bst.search(30));
        assert!(bst.search(70));
    }

    #[test]
    fn test_bst_search() {
        let mut bst = VisualizableBST::new();
        bst.insert(50);
        bst.insert(30);
        bst.insert(70);

        assert!(bst.search(50));
        assert!(bst.search(30));
        assert!(!bst.search(100));
    }

    #[test]
    fn test_bst_empty() {
        let bst = VisualizableBST::new();
        assert!(bst.is_empty());
        assert_eq!(bst.size(), 0);
    }

    #[test]
    fn test_bst_clear() {
        let mut bst = VisualizableBST::new();
        bst.insert(50);
        bst.insert(30);

        bst.clear();
        assert!(bst.is_empty());
        assert_eq!(bst.size(), 0);
    }

    #[test]
    fn test_bst_no_duplicates() {
        let mut bst = VisualizableBST::new();
        bst.insert(50);
        bst.insert(50);

        assert_eq!(bst.size(), 1);
    }
}

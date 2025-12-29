//! Educational Red-Black Tree implementation with visualization support.
//!
//! This implementation demonstrates RB tree operations with visual
//! representation of nodes, colors, and balancing operations.

use crate::error::{DsavError, Result};
use crate::state::{RenderElement, RenderState, ElementState};
use crate::traits::{Operation, Step, Visualizable};
use std::rc::Rc;
use std::cell::RefCell;

#[derive(Debug, Clone, Copy, PartialEq, Eq)]
pub enum Color {
    Red,
    Black,
}

#[derive(Debug, Clone)]
struct Node {
    value: i32,
    color: Color,
    left: Option<Rc<RefCell<Node>>>,
    right: Option<Rc<RefCell<Node>>>,
    parent: Option<Rc<RefCell<Node>>>,
}

impl Node {
    fn new(value: i32) -> Rc<RefCell<Self>> {
        Rc::new(RefCell::new(Self {
            value,
            color: Color::Red, // New nodes are always red
            left: None,
            right: None,
            parent: None,
        }))
    }

    fn is_red(node: &Option<Rc<RefCell<Node>>>) -> bool {
        node.as_ref()
            .map(|n| n.borrow().color == Color::Red)
            .unwrap_or(false)
    }

    fn is_black(node: &Option<Rc<RefCell<Node>>>) -> bool {
        !Self::is_red(node)
    }
}

#[derive(Debug, Clone)]
pub struct VisualizableRBTree {
    root: Option<Rc<RefCell<Node>>>,
    size: usize,
}

impl VisualizableRBTree {
    pub fn new() -> Self {
        Self {
            root: None,
            size: 0,
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

    /// Insert a value into the tree (non-visualized)
    pub fn insert(&mut self, value: i32) {
        if self.root.is_none() {
            let node = Node::new(value);
            node.borrow_mut().color = Color::Black;
            self.root = Some(node);
            self.size += 1;
            return;
        }

        // Standard BST insert
        let new_node = self.insert_bst(value);
        if new_node.is_none() {
            return; // Duplicate
        }

        let new_node = new_node.unwrap();
        self.size += 1;

        // Fix RB properties
        self.insert_fixup(new_node);
    }

    /// BST-style insertion, returns the new node
    fn insert_bst(&mut self, value: i32) -> Option<Rc<RefCell<Node>>> {
        let mut current = self.root.clone();
        let mut parent: Option<Rc<RefCell<Node>>> = None;

        while let Some(node_rc) = current {
            parent = Some(node_rc.clone());
            let node = node_rc.borrow();

            if value < node.value {
                current = node.left.clone();
            } else if value > node.value {
                current = node.right.clone();
            } else {
                return None; // Duplicate
            }
        }

        let new_node = Node::new(value);
        new_node.borrow_mut().parent = parent.clone();

        if let Some(parent_rc) = parent {
            let mut parent_node = parent_rc.borrow_mut();
            if value < parent_node.value {
                parent_node.left = Some(new_node.clone());
            } else {
                parent_node.right = Some(new_node.clone());
            }
        }

        Some(new_node)
    }

    /// RB insert fixup
    fn insert_fixup(&mut self, z: Rc<RefCell<Node>>) {
        let mut current_z = z;

        loop {
            // Check if parent exists and is red
            let parent_rc = {
                let z_borrow = current_z.borrow();
                match &z_borrow.parent {
                    Some(p) if p.borrow().color == Color::Red => p.clone(),
                    _ => break, // Parent is black or doesn't exist
                }
            };

            // Get grandparent (must exist if parent is red)
            let grandparent_rc = {
                let parent_borrow = parent_rc.borrow();
                match &parent_borrow.parent {
                    Some(gp) => gp.clone(),
                    None => break,
                }
            };

            // Determine if parent is left or right child
            let parent_is_left = {
                let gp_borrow = grandparent_rc.borrow();
                gp_borrow.left.as_ref()
                    .map(|l| Rc::ptr_eq(l, &parent_rc))
                    .unwrap_or(false)
            };

            if parent_is_left {
                // Parent is left child
                let uncle = grandparent_rc.borrow().right.clone();

                if Node::is_red(&uncle) {
                    // Case 1: Uncle is red - recolor
                    parent_rc.borrow_mut().color = Color::Black;
                    if let Some(u) = uncle {
                        u.borrow_mut().color = Color::Black;
                    }
                    grandparent_rc.borrow_mut().color = Color::Red;
                    current_z = grandparent_rc;
                } else {
                    // Uncle is black
                    let z_is_right = {
                        let parent_borrow = parent_rc.borrow();
                        parent_borrow.right.as_ref()
                            .map(|r| Rc::ptr_eq(r, &current_z))
                            .unwrap_or(false)
                    };

                    if z_is_right {
                        // Case 2: Triangle - rotate to line
                        current_z = parent_rc.clone();
                        self.rotate_left(current_z.clone());
                    }

                    // Case 3: Line - recolor and rotate
                    let parent_rc = current_z.borrow().parent.clone().unwrap();
                    let grandparent_rc = parent_rc.borrow().parent.clone().unwrap();

                    parent_rc.borrow_mut().color = Color::Black;
                    grandparent_rc.borrow_mut().color = Color::Red;
                    self.rotate_right(grandparent_rc);
                    break;
                }
            } else {
                // Parent is right child (mirror cases)
                let uncle = grandparent_rc.borrow().left.clone();

                if Node::is_red(&uncle) {
                    // Case 1: Uncle is red
                    parent_rc.borrow_mut().color = Color::Black;
                    if let Some(u) = uncle {
                        u.borrow_mut().color = Color::Black;
                    }
                    grandparent_rc.borrow_mut().color = Color::Red;
                    current_z = grandparent_rc;
                } else {
                    // Uncle is black
                    let z_is_left = {
                        let parent_borrow = parent_rc.borrow();
                        parent_borrow.left.as_ref()
                            .map(|l| Rc::ptr_eq(l, &current_z))
                            .unwrap_or(false)
                    };

                    if z_is_left {
                        // Case 2: Triangle
                        current_z = parent_rc.clone();
                        self.rotate_right(current_z.clone());
                    }

                    // Case 3: Line
                    let parent_rc = current_z.borrow().parent.clone().unwrap();
                    let grandparent_rc = parent_rc.borrow().parent.clone().unwrap();

                    parent_rc.borrow_mut().color = Color::Black;
                    grandparent_rc.borrow_mut().color = Color::Red;
                    self.rotate_left(grandparent_rc);
                    break;
                }
            }
        }

        // Ensure root is black
        if let Some(root) = &self.root {
            root.borrow_mut().color = Color::Black;
        }
    }

    /// Left rotation around node x
    fn rotate_left(&mut self, x: Rc<RefCell<Node>>) {
        let y = match x.borrow().right.clone() {
            Some(y) => y,
            None => return,
        };

        // y's left subtree becomes x's right subtree
        let y_left = y.borrow().left.clone();
        x.borrow_mut().right = y_left.clone();
        if let Some(yl) = y_left {
            yl.borrow_mut().parent = Some(x.clone());
        }

        // Link x's parent to y
        let x_parent = x.borrow().parent.clone();
        y.borrow_mut().parent = x_parent.clone();

        if x_parent.is_none() {
            self.root = Some(y.clone());
        } else if let Some(parent) = x_parent.clone() {
            let x_is_left = parent.borrow().left.as_ref()
                .map(|l| Rc::ptr_eq(l, &x))
                .unwrap_or(false);

            if x_is_left {
                parent.borrow_mut().left = Some(y.clone());
            } else {
                parent.borrow_mut().right = Some(y.clone());
            }
        }

        // Put x on y's left
        y.borrow_mut().left = Some(x.clone());
        x.borrow_mut().parent = Some(y);
    }

    /// Right rotation around node x
    fn rotate_right(&mut self, x: Rc<RefCell<Node>>) {
        let y = match x.borrow().left.clone() {
            Some(y) => y,
            None => return,
        };

        // y's right subtree becomes x's left subtree
        let y_right = y.borrow().right.clone();
        x.borrow_mut().left = y_right.clone();
        if let Some(yr) = y_right {
            yr.borrow_mut().parent = Some(x.clone());
        }

        // Link x's parent to y
        let x_parent = x.borrow().parent.clone();
        y.borrow_mut().parent = x_parent.clone();

        if x_parent.is_none() {
            self.root = Some(y.clone());
        } else if let Some(parent) = x_parent.clone() {
            let x_is_right = parent.borrow().right.as_ref()
                .map(|r| Rc::ptr_eq(r, &x))
                .unwrap_or(false);

            if x_is_right {
                parent.borrow_mut().right = Some(y.clone());
            } else {
                parent.borrow_mut().left = Some(y.clone());
            }
        }

        // Put x on y's right
        y.borrow_mut().right = Some(x.clone());
        x.borrow_mut().parent = Some(y);
    }

    /// Delete a value from the RB tree
    pub fn delete(&mut self, value: i32) -> bool {
        // Find the node to delete
        let node_to_delete = match self.find_node(&self.root, value) {
            Some(node) => node,
            None => return false, // Value not found
        };

        self.delete_node(node_to_delete);
        self.size -= 1;
        true
    }

    /// Find a node with the given value
    fn find_node(&self, start: &Option<Rc<RefCell<Node>>>, value: i32) -> Option<Rc<RefCell<Node>>> {
        match start {
            Some(node) => {
                let node_value = node.borrow().value;
                if value == node_value {
                    Some(node.clone())
                } else if value < node_value {
                    self.find_node(&node.borrow().left, value)
                } else {
                    self.find_node(&node.borrow().right, value)
                }
            }
            None => None,
        }
    }

    /// Delete a specific node from the tree
    fn delete_node(&mut self, z: Rc<RefCell<Node>>) {
        let mut y = z.clone();
        let mut y_original_color = y.borrow().color;

        // Find node to splice out and its replacement
        let (x, x_parent): (Option<Rc<RefCell<Node>>>, Option<Rc<RefCell<Node>>>);

        {
            let z_borrow = z.borrow();
            let has_left = z_borrow.left.is_some();
            let has_right = z_borrow.right.is_some();

            if !has_left {
                // Case 1: No left child - replace z with right child
                x = z_borrow.right.clone();
                x_parent = z_borrow.parent.clone();
                drop(z_borrow);
                self.transplant(z.clone(), x.clone());
            } else if !has_right {
                // Case 2: No right child - replace z with left child
                x = z_borrow.left.clone();
                x_parent = z_borrow.parent.clone();
                drop(z_borrow);
                self.transplant(z.clone(), x.clone());
            } else {
                // Case 3: Two children - find minimum in right subtree
                drop(z_borrow);
                y = self.tree_minimum(z.borrow().right.as_ref().unwrap());
                y_original_color = y.borrow().color;
                x = y.borrow().right.clone();

                let y_parent = y.borrow().parent.clone();

                if let Some(y_parent_rc) = y_parent {
                    if Rc::ptr_eq(&y_parent_rc, &z) {
                        // y is direct child of z
                        x_parent = Some(y.clone());
                    } else {
                        // y is deeper in the tree
                        x_parent = Some(y_parent_rc.clone());
                        self.transplant(y.clone(), x.clone());
                        y.borrow_mut().right = z.borrow().right.clone();
                        if let Some(right) = &y.borrow().right {
                            right.borrow_mut().parent = Some(y.clone());
                        }
                    }
                } else {
                    x_parent = Some(y.clone());
                }

                self.transplant(z.clone(), Some(y.clone()));
                y.borrow_mut().left = z.borrow().left.clone();
                if let Some(left) = &y.borrow().left {
                    left.borrow_mut().parent = Some(y.clone());
                }
                y.borrow_mut().color = z.borrow().color;
            }
        }

        // Fix RB violations if a black node was deleted
        if y_original_color == Color::Black {
            self.delete_fixup(x, x_parent);
        }
    }

    /// Replace subtree rooted at u with subtree rooted at v
    fn transplant(&mut self, u: Rc<RefCell<Node>>, v: Option<Rc<RefCell<Node>>>) {
        let u_parent = u.borrow().parent.clone();

        match &u_parent {
            None => {
                // u is root
                self.root = v.clone();
            }
            Some(parent) => {
                let u_is_left = {
                    parent.borrow().left.as_ref()
                        .map(|l| Rc::ptr_eq(l, &u))
                        .unwrap_or(false)
                };

                if u_is_left {
                    parent.borrow_mut().left = v.clone();
                } else {
                    parent.borrow_mut().right = v.clone();
                }
            }
        }

        if let Some(v_node) = v {
            v_node.borrow_mut().parent = u_parent;
        }
    }

    /// Find minimum node in subtree
    fn tree_minimum(&self, node: &Rc<RefCell<Node>>) -> Rc<RefCell<Node>> {
        let mut current = node.clone();
        loop {
            let left = current.borrow().left.clone();
            match left {
                Some(left_node) => current = left_node,
                None => break,
            }
        }
        current
    }

    /// RB delete fixup - restore RB properties after deletion
    fn delete_fixup(&mut self, mut x: Option<Rc<RefCell<Node>>>, mut x_parent: Option<Rc<RefCell<Node>>>) {
        while x.as_ref().map_or(true, |node| !Rc::ptr_eq(node, self.root.as_ref().unwrap()))
              && x.as_ref().map_or(true, |node| node.borrow().color == Color::Black) {

            let x_is_left = if let Some(parent) = &x_parent {
                parent.borrow().left.as_ref()
                    .map(|l| x.as_ref().map_or(false, |x_node| Rc::ptr_eq(l, x_node)))
                    .unwrap_or(true) // If parent.left is None, x is considered left
            } else {
                break;
            };

            if x_is_left {
                // x is left child
                let mut w = x_parent.as_ref().unwrap().borrow().right.clone();

                if let Some(w_node) = &w {
                    // Case 1: Sibling is red
                    if w_node.borrow().color == Color::Red {
                        w_node.borrow_mut().color = Color::Black;
                        x_parent.as_ref().unwrap().borrow_mut().color = Color::Red;
                        self.rotate_left(x_parent.clone().unwrap());
                        w = x_parent.as_ref().unwrap().borrow().right.clone();
                    }
                }

                if let Some(w_node) = &w {
                    let left_is_black = w_node.borrow().left.as_ref()
                        .map_or(true, |l| l.borrow().color == Color::Black);
                    let right_is_black = w_node.borrow().right.as_ref()
                        .map_or(true, |r| r.borrow().color == Color::Black);

                    if left_is_black && right_is_black {
                        // Case 2: Sibling and its children are black
                        w_node.borrow_mut().color = Color::Red;
                        x = x_parent.clone();
                        x_parent = x.as_ref().and_then(|node| node.borrow().parent.clone());
                    } else {
                        if right_is_black {
                            // Case 3: Sibling's right child is black (left is red)
                            if let Some(left) = &w_node.borrow().left {
                                left.borrow_mut().color = Color::Black;
                            }
                            w_node.borrow_mut().color = Color::Red;
                            self.rotate_right(w_node.clone());
                            w = x_parent.as_ref().unwrap().borrow().right.clone();
                        }

                        // Case 4: Sibling's right child is red
                        if let Some(w_node) = &w {
                            w_node.borrow_mut().color = x_parent.as_ref().unwrap().borrow().color;
                            x_parent.as_ref().unwrap().borrow_mut().color = Color::Black;
                            if let Some(right) = &w_node.borrow().right {
                                right.borrow_mut().color = Color::Black;
                            }
                            self.rotate_left(x_parent.clone().unwrap());
                        }
                        x = self.root.clone();
                        break;
                    }
                } else {
                    break;
                }
            } else {
                // x is right child (mirror cases)
                let mut w = x_parent.as_ref().unwrap().borrow().left.clone();

                if let Some(w_node) = &w {
                    // Case 1: Sibling is red
                    if w_node.borrow().color == Color::Red {
                        w_node.borrow_mut().color = Color::Black;
                        x_parent.as_ref().unwrap().borrow_mut().color = Color::Red;
                        self.rotate_right(x_parent.clone().unwrap());
                        w = x_parent.as_ref().unwrap().borrow().left.clone();
                    }
                }

                if let Some(w_node) = &w {
                    let left_is_black = w_node.borrow().left.as_ref()
                        .map_or(true, |l| l.borrow().color == Color::Black);
                    let right_is_black = w_node.borrow().right.as_ref()
                        .map_or(true, |r| r.borrow().color == Color::Black);

                    if left_is_black && right_is_black {
                        // Case 2: Sibling and its children are black
                        w_node.borrow_mut().color = Color::Red;
                        x = x_parent.clone();
                        x_parent = x.as_ref().and_then(|node| node.borrow().parent.clone());
                    } else {
                        if left_is_black {
                            // Case 3: Sibling's left child is black (right is red)
                            if let Some(right) = &w_node.borrow().right {
                                right.borrow_mut().color = Color::Black;
                            }
                            w_node.borrow_mut().color = Color::Red;
                            self.rotate_left(w_node.clone());
                            w = x_parent.as_ref().unwrap().borrow().left.clone();
                        }

                        // Case 4: Sibling's left child is red
                        if let Some(w_node) = &w {
                            w_node.borrow_mut().color = x_parent.as_ref().unwrap().borrow().color;
                            x_parent.as_ref().unwrap().borrow_mut().color = Color::Black;
                            if let Some(left) = &w_node.borrow().left {
                                left.borrow_mut().color = Color::Black;
                            }
                            self.rotate_right(x_parent.clone().unwrap());
                        }
                        x = self.root.clone();
                        break;
                    }
                } else {
                    break;
                }
            }
        }

        // Ensure x is black
        if let Some(x_node) = x {
            x_node.borrow_mut().color = Color::Black;
        }
    }

    /// Search for a value
    pub fn search(&self, value: i32) -> bool {
        Self::search_recursive(&self.root, value)
    }

    fn search_recursive(node: &Option<Rc<RefCell<Node>>>, value: i32) -> bool {
        match node {
            None => false,
            Some(n) => {
                let n = n.borrow();
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

    /// Helper to collect nodes in-order
    fn collect_nodes(&self) -> Vec<i32> {
        let mut nodes = Vec::new();
        Self::inorder_collect(&self.root, &mut nodes);
        nodes
    }

    fn inorder_collect(node: &Option<Rc<RefCell<Node>>>, nodes: &mut Vec<i32>) {
        if let Some(n) = node {
            let n = n.borrow();
            Self::inorder_collect(&n.left, nodes);
            nodes.push(n.value);
            Self::inorder_collect(&n.right, nodes);
        }
    }

    /// Convert tree to array representation for rendering
    fn tree_to_array(&self) -> Vec<Option<(i32, Color)>> {
        let mut result = vec![None; 128]; // Max nodes for visualization
        Self::tree_to_array_helper(&self.root, 0, &mut result);
        result
    }

    fn tree_to_array_helper(
        node: &Option<Rc<RefCell<Node>>>,
        idx: usize,
        result: &mut [Option<(i32, Color)>],
    ) {
        if let Some(n) = node {
            if idx < result.len() {
                let n = n.borrow();
                result[idx] = Some((n.value, n.color));
                Self::tree_to_array_helper(&n.left, idx * 2 + 1, result);
                Self::tree_to_array_helper(&n.right, idx * 2 + 2, result);
            }
        }
    }
}

impl Default for VisualizableRBTree {
    fn default() -> Self {
        Self::new()
    }
}

impl Visualizable for VisualizableRBTree {
    fn execute_with_steps(&mut self, operation: Operation) -> Result<Vec<Step>> {
        match operation {
            Operation::Insert(_, value) => {
                self.insert_with_steps(value)
            }

            Operation::Search(target) => {
                let mut steps = Vec::new();

                steps.push(Step {
                    description: format!("Searching for {} in Red-Black Tree", target),
                    highlight_indices: vec![],
                    active_indices: vec![],
                    metadata: serde_json::json!({
                        "operation": "search",
                        "target": target
                    }),
                });

                let mut current = self.root.clone();
                let mut idx = 0;
                let mut found = false;

                while let Some(node_rc) = current {
                    let node = node_rc.borrow();

                    steps.push(Step {
                        description: format!("Checking {} node with value {}",
                            if node.color == Color::Red { "RED" } else { "BLACK" },
                            node.value),
                        highlight_indices: vec![idx],
                        active_indices: vec![],
                        metadata: serde_json::json!({
                            "node_color": if node.color == Color::Red { "red" } else { "black" }
                        }),
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
                        current = node.left.clone();
                        idx = idx * 2 + 1;
                    } else {
                        current = node.right.clone();
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
                    description: "Starting in-order traversal of Red-Black Tree".to_string(),
                    highlight_indices: vec![],
                    active_indices: vec![],
                    metadata: serde_json::json!({
                        "operation": "traverse"
                    }),
                });

                Self::inorder_traverse_steps(&self.root, 0, &mut steps);

                steps.push(Step {
                    description: "In-order traversal complete".to_string(),
                    highlight_indices: vec![],
                    active_indices: vec![],
                    metadata: serde_json::json!({}),
                });

                Ok(steps)
            }

            Operation::Delete(value_as_idx) => {
                // For RB tree, we interpret the index as the value to delete
                self.delete_with_steps(value_as_idx as i32)
            }

            _ => Err(DsavError::Visualization(
                "Operation not supported for Red-Black Tree".to_string(),
            )),
        }
    }

    fn render_state(&self) -> RenderState {
        let mut elements = Vec::new();
        let mut connections = Vec::new();

        let array = self.tree_to_array();

        for (idx, node_opt) in array.iter().enumerate() {
            if let Some((value, color)) = node_opt {
                while elements.len() <= idx {
                    elements.push(RenderElement::new(0).with_label("".to_string()));
                }

                let state = match color {
                    Color::Red => ElementState::Comparing, // Use Comparing for red nodes
                    Color::Black => ElementState::Normal,
                };

                elements[idx] = RenderElement::new(*value)
                    .with_label(value.to_string())
                    .with_sublabel(format!("{}", if *color == Color::Red { "R" } else { "B" }))
                    .with_state(state);

                // Add connections
                let left_idx = idx * 2 + 1;
                let right_idx = idx * 2 + 2;

                if left_idx < array.len() && array[left_idx].is_some() {
                    connections.push((idx, left_idx));
                }

                if right_idx < array.len() && array[right_idx].is_some() {
                    connections.push((idx, right_idx));
                }
            }
        }

        RenderState {
            elements,
            connections,
        }
    }
}

// Step-by-step visualization methods
impl VisualizableRBTree {
    /// Insert with detailed animation steps
    fn insert_with_steps(&mut self, value: i32) -> Result<Vec<Step>> {
        let mut steps = Vec::new();

        steps.push(Step {
            description: format!("Inserting {} into Red-Black Tree", value),
            highlight_indices: vec![],
            active_indices: vec![],
            metadata: serde_json::json!({
                "operation": "insert",
                "value": value
            }),
        });

        // Handle empty tree
        if self.root.is_none() {
            steps.push(Step {
                description: format!("Tree is empty, {} becomes BLACK root", value),
                highlight_indices: vec![],
                active_indices: vec![0],
                metadata: serde_json::json!({
                    "new_root": value,
                    "color": "black"
                }),
            });

            let node = Node::new(value);
            node.borrow_mut().color = Color::Black;
            self.root = Some(node);
            self.size += 1;
            return Ok(steps);
        }

        // BST insertion with path tracking
        let mut current = self.root.clone();
        let mut parent: Option<Rc<RefCell<Node>>> = None;
        let mut path = Vec::new();
        let mut idx = 0;

        while let Some(node_rc) = current.clone() {
            path.push(idx);
            let node = node_rc.borrow();

            steps.push(Step {
                description: format!("Comparing {} with {} ({} node)",
                    value, node.value,
                    if node.color == Color::Red { "RED" } else { "BLACK" }),
                highlight_indices: path.clone(),
                active_indices: vec![],
                metadata: serde_json::json!({
                    "comparing": [value, node.value],
                    "node_color": if node.color == Color::Red { "red" } else { "black" }
                }),
            });

            parent = Some(node_rc.clone());

            if value < node.value {
                current = node.left.clone();
                idx = idx * 2 + 1;
            } else if value > node.value {
                current = node.right.clone();
                idx = idx * 2 + 2;
            } else {
                steps.push(Step {
                    description: format!("{} already exists in tree (no duplicates allowed)", value),
                    highlight_indices: path,
                    active_indices: vec![],
                    metadata: serde_json::json!({ "duplicate": true }),
                });
                return Ok(steps);
            }
        }

        // Insert new RED node
        let new_node = Node::new(value);
        let insert_idx = idx;

        if let Some(parent_rc) = parent.clone() {
            let mut parent_node = parent_rc.borrow_mut();
            new_node.borrow_mut().parent = Some(parent_rc.clone());

            if value < parent_node.value {
                parent_node.left = Some(new_node.clone());
            } else {
                parent_node.right = Some(new_node.clone());
            }
        }

        self.size += 1;

        steps.push(Step {
            description: format!("Inserted {} as RED node", value),
            highlight_indices: vec![],
            active_indices: vec![insert_idx],
            metadata: serde_json::json!({
                "inserted": value,
                "color": "red",
                "index": insert_idx
            }),
        });

        // Fixup phase with detailed steps
        self.insert_fixup_with_steps(new_node, &mut steps)?;

        steps.push(Step {
            description: "Red-Black Tree properties restored".to_string(),
            highlight_indices: vec![],
            active_indices: vec![],
            metadata: serde_json::json!({ "fixup_complete": true }),
        });

        Ok(steps)
    }

    /// Delete a value with detailed animation steps
    fn delete_with_steps(&mut self, value: i32) -> Result<Vec<Step>> {
        let mut steps = Vec::new();

        steps.push(Step {
            description: format!("Deleting {} from Red-Black Tree", value),
            highlight_indices: vec![],
            active_indices: vec![],
            metadata: serde_json::json!({
                "operation": "delete",
                "value": value
            }),
        });

        // Find the node to delete
        let node_to_delete = match self.find_node(&self.root, value) {
            Some(node) => {
                let idx = self.find_node_index(&node);
                steps.push(Step {
                    description: format!("Found {} in the tree", value),
                    highlight_indices: vec![idx],
                    active_indices: vec![],
                    metadata: serde_json::json!({
                        "found": true,
                        "index": idx
                    }),
                });
                node
            }
            None => {
                steps.push(Step {
                    description: format!("Value {} not found in tree", value),
                    highlight_indices: vec![],
                    active_indices: vec![],
                    metadata: serde_json::json!({
                        "found": false
                    }),
                });
                return Ok(steps);
            }
        };

        // Perform deletion with steps
        self.delete_node_with_steps(node_to_delete, &mut steps)?;
        self.size -= 1;

        steps.push(Step {
            description: format!("Deletion of {} complete", value),
            highlight_indices: vec![],
            active_indices: vec![],
            metadata: serde_json::json!({
                "complete": true
            }),
        });

        Ok(steps)
    }

    /// Delete a specific node with animation steps
    fn delete_node_with_steps(&mut self, z: Rc<RefCell<Node>>, steps: &mut Vec<Step>) -> Result<()> {
        let z_idx = self.find_node_index(&z);
        let z_val = z.borrow().value;

        let mut y = z.clone();
        let mut y_original_color = y.borrow().color;

        let (x, x_parent): (Option<Rc<RefCell<Node>>>, Option<Rc<RefCell<Node>>>);

        {
            let z_borrow = z.borrow();
            let has_left = z_borrow.left.is_some();
            let has_right = z_borrow.right.is_some();

            if !has_left && !has_right {
                // Case 1: No children - leaf node
                steps.push(Step {
                    description: format!("Node {} is a leaf, removing it directly", z_val),
                    highlight_indices: vec![],
                    active_indices: vec![z_idx],
                    metadata: serde_json::json!({
                        "case": "no_children",
                        "node": z_val
                    }),
                });
                x = None;
                x_parent = z_borrow.parent.clone();
                drop(z_borrow);
                self.transplant(z.clone(), x.clone());
            } else if !has_left {
                // Case 2: Only right child
                let right_val = z_borrow.right.as_ref().unwrap().borrow().value;
                steps.push(Step {
                    description: format!("Node {} has only right child {}, replacing with right child", z_val, right_val),
                    highlight_indices: vec![],
                    active_indices: vec![z_idx],
                    metadata: serde_json::json!({
                        "case": "only_right_child",
                        "node": z_val,
                        "replacement": right_val
                    }),
                });
                x = z_borrow.right.clone();
                x_parent = z_borrow.parent.clone();
                drop(z_borrow);
                self.transplant(z.clone(), x.clone());
            } else if !has_right {
                // Case 3: Only left child
                let left_val = z_borrow.left.as_ref().unwrap().borrow().value;
                steps.push(Step {
                    description: format!("Node {} has only left child {}, replacing with left child", z_val, left_val),
                    highlight_indices: vec![],
                    active_indices: vec![z_idx],
                    metadata: serde_json::json!({
                        "case": "only_left_child",
                        "node": z_val,
                        "replacement": left_val
                    }),
                });
                x = z_borrow.left.clone();
                x_parent = z_borrow.parent.clone();
                drop(z_borrow);
                self.transplant(z.clone(), x.clone());
            } else {
                // Case 4: Two children - find successor
                drop(z_borrow);
                y = self.tree_minimum(z.borrow().right.as_ref().unwrap());
                let y_val = y.borrow().value;
                y_original_color = y.borrow().color;

                steps.push(Step {
                    description: format!("Node {} has two children, finding successor {}", z_val, y_val),
                    highlight_indices: vec![self.find_node_index(&y)],
                    active_indices: vec![z_idx],
                    metadata: serde_json::json!({
                        "case": "two_children",
                        "node": z_val,
                        "successor": y_val
                    }),
                });

                x = y.borrow().right.clone();
                let y_parent = y.borrow().parent.clone();

                if let Some(y_parent_rc) = y_parent {
                    if Rc::ptr_eq(&y_parent_rc, &z) {
                        x_parent = Some(y.clone());
                    } else {
                        x_parent = Some(y_parent_rc.clone());
                        self.transplant(y.clone(), x.clone());
                        y.borrow_mut().right = z.borrow().right.clone();
                        if let Some(right) = &y.borrow().right {
                            right.borrow_mut().parent = Some(y.clone());
                        }
                    }
                } else {
                    x_parent = Some(y.clone());
                }

                self.transplant(z.clone(), Some(y.clone()));
                y.borrow_mut().left = z.borrow().left.clone();
                if let Some(left) = &y.borrow().left {
                    left.borrow_mut().parent = Some(y.clone());
                }
                y.borrow_mut().color = z.borrow().color;

                steps.push(Step {
                    description: format!("Replaced {} with successor {}", z_val, y_val),
                    highlight_indices: vec![],
                    active_indices: vec![self.find_node_index(&y)],
                    metadata: serde_json::json!({
                        "replaced": z_val,
                        "with": y_val
                    }),
                });
            }
        }

        // Fix RB violations if a black node was deleted
        if y_original_color == Color::Black {
            steps.push(Step {
                description: "A BLACK node was removed, fixing Red-Black properties".to_string(),
                highlight_indices: vec![],
                active_indices: vec![],
                metadata: serde_json::json!({
                    "fixup_needed": true,
                    "deleted_color": "black"
                }),
            });

            self.delete_fixup_with_steps(x, x_parent, steps)?;
        } else {
            steps.push(Step {
                description: "A RED node was removed, no fixup needed".to_string(),
                highlight_indices: vec![],
                active_indices: vec![],
                metadata: serde_json::json!({
                    "fixup_needed": false,
                    "deleted_color": "red"
                }),
            });
        }

        Ok(())
    }

    /// RB delete fixup with animation steps
    fn delete_fixup_with_steps(
        &mut self,
        mut x: Option<Rc<RefCell<Node>>>,
        mut x_parent: Option<Rc<RefCell<Node>>>,
        steps: &mut Vec<Step>,
    ) -> Result<()> {
        let mut iteration = 0;

        while x.as_ref().map_or(true, |node| self.root.as_ref().map_or(true, |root| !Rc::ptr_eq(node, root)))
              && x.as_ref().map_or(true, |node| node.borrow().color == Color::Black) {

            iteration += 1;

            let x_is_left = if let Some(parent) = &x_parent {
                parent.borrow().left.as_ref()
                    .map(|l| x.as_ref().map_or(false, |x_node| Rc::ptr_eq(l, x_node)))
                    .unwrap_or(true)
            } else {
                break;
            };

            if x_is_left {
                // x is left child
                let mut w = x_parent.as_ref().unwrap().borrow().right.clone();

                if let Some(w_node) = &w {
                    // Case 1: Sibling is red
                    if w_node.borrow().color == Color::Red {
                        let w_idx = self.find_node_index(w_node);
                        steps.push(Step {
                            description: format!("Case 1: Sibling {} is RED, recoloring and rotating", w_node.borrow().value),
                            highlight_indices: vec![w_idx],
                            active_indices: vec![],
                            metadata: serde_json::json!({
                                "case": "sibling_red",
                                "iteration": iteration
                            }),
                        });

                        w_node.borrow_mut().color = Color::Black;
                        x_parent.as_ref().unwrap().borrow_mut().color = Color::Red;
                        self.rotate_left(x_parent.clone().unwrap());
                        w = x_parent.as_ref().unwrap().borrow().right.clone();
                    }
                }

                if let Some(w_node) = &w {
                    let left_is_black = w_node.borrow().left.as_ref()
                        .map_or(true, |l| l.borrow().color == Color::Black);
                    let right_is_black = w_node.borrow().right.as_ref()
                        .map_or(true, |r| r.borrow().color == Color::Black);

                    if left_is_black && right_is_black {
                        // Case 2: Both children black
                        steps.push(Step {
                            description: "Case 2: Sibling's children are BLACK, recoloring sibling to RED".to_string(),
                            highlight_indices: vec![self.find_node_index(w_node)],
                            active_indices: vec![],
                            metadata: serde_json::json!({
                                "case": "both_children_black",
                                "iteration": iteration
                            }),
                        });

                        w_node.borrow_mut().color = Color::Red;
                        x = x_parent.clone();
                        x_parent = x.as_ref().and_then(|node| node.borrow().parent.clone());
                    } else {
                        if right_is_black {
                            // Case 3: Right child black, left child red
                            steps.push(Step {
                                description: "Case 3: Sibling's right child BLACK, left RED - rotating".to_string(),
                                highlight_indices: vec![self.find_node_index(w_node)],
                                active_indices: vec![],
                                metadata: serde_json::json!({
                                    "case": "triangle",
                                    "iteration": iteration
                                }),
                            });

                            if let Some(left) = &w_node.borrow().left {
                                left.borrow_mut().color = Color::Black;
                            }
                            w_node.borrow_mut().color = Color::Red;
                            self.rotate_right(w_node.clone());
                            w = x_parent.as_ref().unwrap().borrow().right.clone();
                        }

                        // Case 4: Right child red
                        steps.push(Step {
                            description: "Case 4: Sibling's right child is RED, final rotation".to_string(),
                            highlight_indices: vec![],
                            active_indices: vec![],
                            metadata: serde_json::json!({
                                "case": "line",
                                "iteration": iteration
                            }),
                        });

                        if let Some(w_node) = &w {
                            w_node.borrow_mut().color = x_parent.as_ref().unwrap().borrow().color;
                            x_parent.as_ref().unwrap().borrow_mut().color = Color::Black;
                            if let Some(right) = &w_node.borrow().right {
                                right.borrow_mut().color = Color::Black;
                            }
                            self.rotate_left(x_parent.clone().unwrap());
                        }
                        x = self.root.clone();
                        break;
                    }
                } else {
                    break;
                }
            } else {
                // x is right child (mirror cases)
                let mut w = x_parent.as_ref().unwrap().borrow().left.clone();

                if let Some(w_node) = &w {
                    if w_node.borrow().color == Color::Red {
                        let w_idx = self.find_node_index(w_node);
                        steps.push(Step {
                            description: format!("Case 1 (mirror): Sibling {} is RED, recoloring and rotating", w_node.borrow().value),
                            highlight_indices: vec![w_idx],
                            active_indices: vec![],
                            metadata: serde_json::json!({
                                "case": "sibling_red_mirror",
                                "iteration": iteration
                            }),
                        });

                        w_node.borrow_mut().color = Color::Black;
                        x_parent.as_ref().unwrap().borrow_mut().color = Color::Red;
                        self.rotate_right(x_parent.clone().unwrap());
                        w = x_parent.as_ref().unwrap().borrow().left.clone();
                    }
                }

                if let Some(w_node) = &w {
                    let left_is_black = w_node.borrow().left.as_ref()
                        .map_or(true, |l| l.borrow().color == Color::Black);
                    let right_is_black = w_node.borrow().right.as_ref()
                        .map_or(true, |r| r.borrow().color == Color::Black);

                    if left_is_black && right_is_black {
                        steps.push(Step {
                            description: "Case 2 (mirror): Sibling's children are BLACK, recoloring".to_string(),
                            highlight_indices: vec![self.find_node_index(w_node)],
                            active_indices: vec![],
                            metadata: serde_json::json!({
                                "case": "both_children_black_mirror",
                                "iteration": iteration
                            }),
                        });

                        w_node.borrow_mut().color = Color::Red;
                        x = x_parent.clone();
                        x_parent = x.as_ref().and_then(|node| node.borrow().parent.clone());
                    } else {
                        if left_is_black {
                            steps.push(Step {
                                description: "Case 3 (mirror): Sibling's left child BLACK, right RED - rotating".to_string(),
                                highlight_indices: vec![self.find_node_index(w_node)],
                                active_indices: vec![],
                                metadata: serde_json::json!({
                                    "case": "triangle_mirror",
                                    "iteration": iteration
                                }),
                            });

                            if let Some(right) = &w_node.borrow().right {
                                right.borrow_mut().color = Color::Black;
                            }
                            w_node.borrow_mut().color = Color::Red;
                            self.rotate_left(w_node.clone());
                            w = x_parent.as_ref().unwrap().borrow().left.clone();
                        }

                        steps.push(Step {
                            description: "Case 4 (mirror): Sibling's left child is RED, final rotation".to_string(),
                            highlight_indices: vec![],
                            active_indices: vec![],
                            metadata: serde_json::json!({
                                "case": "line_mirror",
                                "iteration": iteration
                            }),
                        });

                        if let Some(w_node) = &w {
                            w_node.borrow_mut().color = x_parent.as_ref().unwrap().borrow().color;
                            x_parent.as_ref().unwrap().borrow_mut().color = Color::Black;
                            if let Some(left) = &w_node.borrow().left {
                                left.borrow_mut().color = Color::Black;
                            }
                            self.rotate_right(x_parent.clone().unwrap());
                        }
                        x = self.root.clone();
                        break;
                    }
                } else {
                    break;
                }
            }
        }

        if let Some(x_node) = x {
            x_node.borrow_mut().color = Color::Black;
        }

        steps.push(Step {
            description: "Delete fixup complete, Red-Black properties restored".to_string(),
            highlight_indices: vec![],
            active_indices: vec![],
            metadata: serde_json::json!({
                "fixup_complete": true
            }),
        });

        Ok(())
    }

    /// Render state with NIL leaves shown
    pub fn render_state_with_nil_nodes(&self) -> RenderState {
        let mut elements = Vec::new();
        let mut connections = Vec::new();

        let array = self.tree_to_array();

        // First pass: add all real nodes
        for (idx, node_opt) in array.iter().enumerate() {
            if let Some((value, color)) = node_opt {
                while elements.len() <= idx {
                    elements.push(RenderElement::new(0).with_label("".to_string()));
                }

                let state = match color {
                    Color::Red => ElementState::Comparing,
                    Color::Black => ElementState::Normal,
                };

                elements[idx] = RenderElement::new(*value)
                    .with_label(value.to_string())
                    .with_sublabel(format!("{}", if *color == Color::Red { "R" } else { "B" }))
                    .with_state(state);

                // Add connections to children (including NIL nodes)
                let left_idx = idx * 2 + 1;
                let right_idx = idx * 2 + 2;

                // Always add connections for NIL visualization
                if left_idx < array.len() * 2 { // Allow space for NIL nodes
                    connections.push((idx, left_idx));
                    // If child doesn't exist, we'll add a NIL node
                    if array.get(left_idx).is_none() || !array[left_idx].is_some() {
                        while elements.len() <= left_idx {
                            elements.push(RenderElement::new(0).with_label("".to_string()));
                        }
                        elements[left_idx] = RenderElement::new(0)
                            .with_label("NIL".to_string())
                            .with_sublabel("B".to_string())
                            .with_state(ElementState::Normal);
                    }
                }

                if right_idx < array.len() * 2 {
                    connections.push((idx, right_idx));
                    // If child doesn't exist, we'll add a NIL node
                    if array.get(right_idx).is_none() || !array[right_idx].is_some() {
                        while elements.len() <= right_idx {
                            elements.push(RenderElement::new(0).with_label("".to_string()));
                        }
                        elements[right_idx] = RenderElement::new(0)
                            .with_label("NIL".to_string())
                            .with_sublabel("B".to_string())
                            .with_state(ElementState::Normal);
                    }
                }
            }
        }

        RenderState {
            elements,
            connections,
        }
    }

    /// RB insert fixup with animation steps
    fn insert_fixup_with_steps(
        &mut self,
        z: Rc<RefCell<Node>>,
        steps: &mut Vec<Step>,
    ) -> Result<()> {
        let mut iteration = 0;
        let mut current_z = z;

        loop {
            iteration += 1;

            // Check if parent is black or doesn't exist
            let parent_rc = {
                let z_borrow = current_z.borrow();
                match &z_borrow.parent {
                    Some(p) if p.borrow().color == Color::Red => p.clone(),
                    _ => {
                        steps.push(Step {
                            description: "Parent is BLACK or root reached - fixup complete".to_string(),
                            highlight_indices: vec![],
                            active_indices: vec![],
                            metadata: serde_json::json!({ "fixup_end": true }),
                        });
                        break;
                    }
                }
            };

            let grandparent_rc = {
                let parent_borrow = parent_rc.borrow();
                match &parent_borrow.parent {
                    Some(gp) => gp.clone(),
                    None => break,
                }
            };

            // Get indices for visualization
            let z_idx = self.find_node_index(&current_z);
            let parent_idx = self.find_node_index(&parent_rc);
            let grandparent_idx = self.find_node_index(&grandparent_rc);

            let parent_is_left = {
                let gp_borrow = grandparent_rc.borrow();
                gp_borrow.left.as_ref()
                    .map(|l| Rc::ptr_eq(l, &parent_rc))
                    .unwrap_or(false)
            };

            if parent_is_left {
                let uncle = grandparent_rc.borrow().right.clone();
                let uncle_idx = uncle.as_ref().map(|u| self.find_node_index(u));

                let (z_val, parent_val, gp_val, gp_color) = {
                    let z_borrow = current_z.borrow();
                    let parent_borrow = parent_rc.borrow();
                    let gp_borrow = grandparent_rc.borrow();
                    (z_borrow.value, parent_borrow.value, gp_borrow.value, gp_borrow.color)
                };

                steps.push(Step {
                    description: format!(
                        "Current node: {} (RED), Parent: {} (RED), Grandparent: {} ({}), Uncle: {} ({})",
                        z_val,
                        parent_val,
                        gp_val,
                        if gp_color == Color::Red { "RED" } else { "BLACK" },
                        uncle.as_ref().map(|u| u.borrow().value.to_string()).unwrap_or("NIL".to_string()),
                        if Node::is_red(&uncle) { "RED" } else { "BLACK" }
                    ),
                    highlight_indices: vec![z_idx, parent_idx, grandparent_idx]
                        .into_iter()
                        .chain(uncle_idx)
                        .collect(),
                    active_indices: vec![],
                    metadata: serde_json::json!({
                        "z": z_val,
                        "parent": parent_val,
                        "grandparent": gp_val,
                        "uncle_is_red": Node::is_red(&uncle)
                    }),
                });

                if Node::is_red(&uncle) {
                    // Case 1: Uncle is RED - recolor
                    steps.push(Step {
                        description: "Case 1: Uncle is RED - Recolor parent and uncle to BLACK, grandparent to RED".to_string(),
                        highlight_indices: vec![parent_idx, grandparent_idx]
                            .into_iter()
                            .chain(uncle_idx)
                            .collect(),
                        active_indices: vec![],
                        metadata: serde_json::json!({
                            "case": "uncle_red",
                            "recolor": ["parent", "uncle", "grandparent"]
                        }),
                    });

                    parent_rc.borrow_mut().color = Color::Black;
                    if let Some(u) = uncle {
                        u.borrow_mut().color = Color::Black;
                    }
                    grandparent_rc.borrow_mut().color = Color::Red;
                    current_z = grandparent_rc;
                } else {
                    // Uncle is BLACK
                    let z_is_right = {
                        let parent_borrow = parent_rc.borrow();
                        parent_borrow.right.as_ref()
                            .map(|r| Rc::ptr_eq(r, &current_z))
                            .unwrap_or(false)
                    };

                    if z_is_right {
                        // Case 2: Triangle - rotate left at parent
                        let parent_val = parent_rc.borrow().value;
                        steps.push(Step {
                            description: format!("Case 2: Triangle configuration - Left rotate at parent ({})", parent_val),
                            highlight_indices: vec![z_idx, parent_idx],
                            active_indices: vec![],
                            metadata: serde_json::json!({
                                "case": "triangle",
                                "rotation": "left",
                                "pivot": parent_val
                            }),
                        });

                        current_z = parent_rc.clone();
                        self.rotate_left(current_z.clone());
                    }

                    // Case 3: Line - recolor and rotate right at grandparent
                    let parent_rc = current_z.borrow().parent.clone().unwrap();
                    let grandparent_rc = parent_rc.borrow().parent.clone().unwrap();
                    let gp_val = grandparent_rc.borrow().value;

                    steps.push(Step {
                        description: format!(
                            "Case 3: Line configuration - Recolor parent to BLACK, grandparent to RED, then right rotate at grandparent ({})",
                            gp_val
                        ),
                        highlight_indices: vec![self.find_node_index(&parent_rc), self.find_node_index(&grandparent_rc)],
                        active_indices: vec![],
                        metadata: serde_json::json!({
                            "case": "line",
                            "rotation": "right",
                            "pivot": grandparent_rc.borrow().value
                        }),
                    });

                    parent_rc.borrow_mut().color = Color::Black;
                    grandparent_rc.borrow_mut().color = Color::Red;
                    self.rotate_right(grandparent_rc);
                    break;
                }
            } else {
                // Mirror cases (parent is right child)
                let uncle = grandparent_rc.borrow().left.clone();
                let uncle_idx = uncle.as_ref().map(|u| self.find_node_index(u));

                let (z_val, parent_val, gp_val, gp_color) = {
                    let z_borrow = current_z.borrow();
                    let parent_borrow = parent_rc.borrow();
                    let gp_borrow = grandparent_rc.borrow();
                    (z_borrow.value, parent_borrow.value, gp_borrow.value, gp_borrow.color)
                };

                steps.push(Step {
                    description: format!(
                        "Current node: {} (RED), Parent: {} (RED), Grandparent: {} ({}), Uncle: {} ({})",
                        z_val,
                        parent_val,
                        gp_val,
                        if gp_color == Color::Red { "RED" } else { "BLACK" },
                        uncle.as_ref().map(|u| u.borrow().value.to_string()).unwrap_or("NIL".to_string()),
                        if Node::is_red(&uncle) { "RED" } else { "BLACK" }
                    ),
                    highlight_indices: vec![z_idx, parent_idx, grandparent_idx]
                        .into_iter()
                        .chain(uncle_idx)
                        .collect(),
                    active_indices: vec![],
                    metadata: serde_json::json!({
                        "z": z_val,
                        "parent": parent_val,
                        "grandparent": gp_val,
                        "uncle_is_red": Node::is_red(&uncle)
                    }),
                });

                if Node::is_red(&uncle) {
                    // Case 1: Uncle is RED
                    steps.push(Step {
                        description: "Case 1 (Mirror): Uncle is RED - Recolor parent and uncle to BLACK, grandparent to RED".to_string(),
                        highlight_indices: vec![parent_idx, grandparent_idx]
                            .into_iter()
                            .chain(uncle_idx)
                            .collect(),
                        active_indices: vec![],
                        metadata: serde_json::json!({
                            "case": "uncle_red_mirror",
                            "recolor": ["parent", "uncle", "grandparent"]
                        }),
                    });

                    parent_rc.borrow_mut().color = Color::Black;
                    if let Some(u) = uncle {
                        u.borrow_mut().color = Color::Black;
                    }
                    grandparent_rc.borrow_mut().color = Color::Red;
                    current_z = grandparent_rc;
                } else {
                    // Uncle is BLACK
                    let z_is_left = {
                        let parent_borrow = parent_rc.borrow();
                        parent_borrow.left.as_ref()
                            .map(|l| Rc::ptr_eq(l, &current_z))
                            .unwrap_or(false)
                    };

                    if z_is_left {
                        // Case 2: Triangle - rotate right at parent
                        let parent_val = parent_rc.borrow().value;
                        steps.push(Step {
                            description: format!("Case 2 (Mirror): Triangle configuration - Right rotate at parent ({})", parent_val),
                            highlight_indices: vec![z_idx, parent_idx],
                            active_indices: vec![],
                            metadata: serde_json::json!({
                                "case": "triangle_mirror",
                                "rotation": "right",
                                "pivot": parent_val
                            }),
                        });

                        current_z = parent_rc.clone();
                        self.rotate_right(current_z.clone());
                    }

                    // Case 3: Line - recolor and rotate left at grandparent
                    let parent_rc = current_z.borrow().parent.clone().unwrap();
                    let grandparent_rc = parent_rc.borrow().parent.clone().unwrap();
                    let gp_val = grandparent_rc.borrow().value;

                    steps.push(Step {
                        description: format!(
                            "Case 3 (Mirror): Line configuration - Recolor parent to BLACK, grandparent to RED, then left rotate at grandparent ({})",
                            gp_val
                        ),
                        highlight_indices: vec![self.find_node_index(&parent_rc), self.find_node_index(&grandparent_rc)],
                        active_indices: vec![],
                        metadata: serde_json::json!({
                            "case": "line_mirror",
                            "rotation": "left",
                            "pivot": grandparent_rc.borrow().value
                        }),
                    });

                    parent_rc.borrow_mut().color = Color::Black;
                    grandparent_rc.borrow_mut().color = Color::Red;
                    self.rotate_left(grandparent_rc);
                    break;
                }
            }

            if iteration > 100 {
                return Err(DsavError::InvalidState {
                    reason: "Fixup loop exceeded maximum iterations".to_string(),
                });
            }
        }

        // Ensure root is black
        if let Some(root) = &self.root {
            if root.borrow().color == Color::Red {
                steps.push(Step {
                    description: "Forcing root to BLACK (RB property)".to_string(),
                    highlight_indices: vec![0],
                    active_indices: vec![],
                    metadata: serde_json::json!({ "root_recolor": true }),
                });
                root.borrow_mut().color = Color::Black;
            }
        }

        Ok(())
    }

    fn inorder_traverse_steps(
        node: &Option<Rc<RefCell<Node>>>,
        idx: usize,
        steps: &mut Vec<Step>,
    ) {
        if let Some(n) = node {
            let n = n.borrow();
            Self::inorder_traverse_steps(&n.left, idx * 2 + 1, steps);

            steps.push(Step {
                description: format!("Visiting {} node with value {}",
                    if n.color == Color::Red { "RED" } else { "BLACK" },
                    n.value),
                highlight_indices: vec![idx],
                active_indices: vec![],
                metadata: serde_json::json!({
                    "value": n.value,
                    "color": if n.color == Color::Red { "red" } else { "black" },
                    "index": idx
                }),
            });

            Self::inorder_traverse_steps(&n.right, idx * 2 + 2, steps);
        }
    }

    /// Helper to find node's array index for visualization
    fn find_node_index(&self, target: &Rc<RefCell<Node>>) -> usize {
        Self::find_node_index_helper(&self.root, target, 0).unwrap_or(0)
    }

    fn find_node_index_helper(
        node: &Option<Rc<RefCell<Node>>>,
        target: &Rc<RefCell<Node>>,
        idx: usize,
    ) -> Option<usize> {
        node.as_ref().and_then(|n| {
            if Rc::ptr_eq(n, target) {
                Some(idx)
            } else {
                Self::find_node_index_helper(&n.borrow().left, target, idx * 2 + 1)
                    .or_else(|| Self::find_node_index_helper(&n.borrow().right, target, idx * 2 + 2))
            }
        })
    }
}

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn test_rb_tree_insert() {
        let mut tree = VisualizableRBTree::new();
        tree.insert(50);
        tree.insert(30);
        tree.insert(70);

        assert_eq!(tree.size(), 3);
        assert!(tree.search(50));
        assert!(tree.search(30));
        assert!(tree.search(70));
    }

    #[test]
    fn test_rb_tree_root_is_black() {
        let mut tree = VisualizableRBTree::new();
        tree.insert(50);

        let root = tree.root.as_ref().unwrap();
        assert_eq!(root.borrow().color, Color::Black);
    }

    #[test]
    fn test_rb_tree_no_duplicates() {
        let mut tree = VisualizableRBTree::new();
        tree.insert(50);
        tree.insert(50);

        assert_eq!(tree.size(), 1);
    }

    #[test]
    fn test_rb_tree_empty() {
        let tree = VisualizableRBTree::new();
        assert!(tree.is_empty());
        assert_eq!(tree.size(), 0);
    }

    #[test]
    fn test_rb_tree_clear() {
        let mut tree = VisualizableRBTree::new();
        tree.insert(50);
        tree.insert(30);

        tree.clear();
        assert!(tree.is_empty());
        assert_eq!(tree.size(), 0);
    }

    /// Test RB tree invariants
    #[test]
    fn test_rb_invariants_simple() {
        let mut tree = VisualizableRBTree::new();

        // Insert sequence that triggers various fixup cases
        for val in [50, 25, 75, 10, 30, 60, 80, 5, 15] {
            tree.insert(val);
            assert!(verify_rb_properties(&tree.root), "RB properties violated after inserting {}", val);
        }
    }

    /// Verify Red-Black Tree properties
    fn verify_rb_properties(root: &Option<Rc<RefCell<Node>>>) -> bool {
        // Property 1: Root is black
        if let Some(r) = root {
            if r.borrow().color != Color::Black {
                return false;
            }
        }

        // Property 2: No red node has red child
        // Property 3: All paths have same black height
        let (_black_height, valid) = verify_rb_recursive(root);
        valid
    }

    fn verify_rb_recursive(node: &Option<Rc<RefCell<Node>>>) -> (usize, bool) {
        match node {
            None => (1, true), // NIL nodes are black
            Some(n) => {
                let n = n.borrow();

                // Check no red-red parent-child
                if n.color == Color::Red {
                    if Node::is_red(&n.left) || Node::is_red(&n.right) {
                        return (0, false); // Red node with red child
                    }
                }

                let (left_bh, left_valid) = verify_rb_recursive(&n.left);
                let (right_bh, right_valid) = verify_rb_recursive(&n.right);

                if !left_valid || !right_valid || left_bh != right_bh {
                    return (0, false);
                }

                let bh = left_bh + if n.color == Color::Black { 1 } else { 0 };
                (bh, true)
            }
        }
    }

    #[test]
    fn test_rb_fixup_case_uncle_red() {
        let mut tree = VisualizableRBTree::new();

        // Sequence: 50, 25, 75 creates uncle red case when inserting 10
        tree.insert(50); // Black root
        tree.insert(25); // Red left
        tree.insert(75); // Red right
        tree.insert(10); // Triggers uncle red case

        assert!(verify_rb_properties(&tree.root));
        assert_eq!(tree.size(), 4);
    }

    #[test]
    fn test_rb_fixup_case_triangle() {
        let mut tree = VisualizableRBTree::new();

        // Sequence creates triangle that needs rotation
        tree.insert(50);
        tree.insert(25);
        tree.insert(30); // Triangle: need left-right rotation

        assert!(verify_rb_properties(&tree.root));
    }

    #[test]
    fn test_rb_fixup_case_line() {
        let mut tree = VisualizableRBTree::new();

        // Sequence creates line that needs single rotation
        tree.insert(50);
        tree.insert(25);
        tree.insert(10); // Line: need right rotation

        assert!(verify_rb_properties(&tree.root));
    }

    #[test]
    fn test_rb_random_insertions() {
        use rand::Rng;
        let mut rng = rand::thread_rng();
        let mut tree = VisualizableRBTree::new();

        for _ in 0..100 {
            let val = rng.gen_range(1..1000);
            tree.insert(val);
            assert!(verify_rb_properties(&tree.root), "RB properties violated");
        }

        // Verify in-order traversal is sorted
        let nodes = tree.collect_nodes();
        for i in 1..nodes.len() {
            assert!(nodes[i] >= nodes[i - 1], "Tree not sorted");
        }
    }
}

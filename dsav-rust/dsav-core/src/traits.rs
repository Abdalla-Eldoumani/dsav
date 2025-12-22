//! Core traits for data structures and algorithms.

use crate::error::Result;
use crate::state::RenderState;
use serde::{Deserialize, Serialize};

#[derive(Debug, Clone, Serialize, Deserialize)]
pub struct Step {
    pub description: String,
    pub highlight_indices: Vec<usize>,
    pub active_indices: Vec<usize>,
    pub metadata: serde_json::Value,
}

#[derive(Debug, Clone, Copy)]
pub enum Operation {
    Insert(usize, i32),
    Delete(usize),
    Search(i32),
    BinarySearch(i32),
    Traverse,
    PreOrderTraverse,
    PostOrderTraverse,
    LevelOrderTraverse,
    Push(i32),
    Pop,
    Enqueue(i32),
    Dequeue,
    BubbleSort,
    InsertionSort,
    QuickSort,
}

pub trait Visualizable {
    fn execute_with_steps(&mut self, operation: Operation) -> Result<Vec<Step>>;
    fn render_state(&self) -> RenderState;
}

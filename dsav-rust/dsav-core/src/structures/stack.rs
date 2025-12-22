//! Educational stack implementation with visualization support.
//!
//! This implementation demonstrates stack operations (LIFO - Last In First Out)
//! with step-by-step visualization.

use crate::error::{DsavError, Result};
use crate::state::{ElementState, RenderElement, RenderState};
use crate::traits::{Operation, Step, Visualizable};

const DEFAULT_CAPACITY: usize = 16;

#[derive(Debug, Clone)]
pub struct VisualizableStack {
    data: Vec<i32>,
    capacity: usize,
}

impl VisualizableStack {
    pub fn new() -> Self {
        Self::with_capacity(DEFAULT_CAPACITY)
    }

    pub fn with_capacity(capacity: usize) -> Self {
        Self {
            data: Vec::with_capacity(capacity),
            capacity,
        }
    }

    pub fn push(&mut self, value: i32) -> Result<()> {
        if self.is_full() {
            return Err(DsavError::Full {
                capacity: self.capacity,
            });
        }

        self.data.push(value);
        Ok(())
    }

    pub fn pop(&mut self) -> Result<i32> {
        self.data.pop().ok_or(DsavError::EmptyStructure)
    }

    pub fn peek(&self) -> Result<i32> {
        self.data.last().copied().ok_or(DsavError::EmptyStructure)
    }

    pub fn is_empty(&self) -> bool {
        self.data.is_empty()
    }

    pub fn is_full(&self) -> bool {
        self.data.len() >= self.capacity
    }

    pub fn len(&self) -> usize {
        self.data.len()
    }

    pub fn size(&self) -> usize {
        self.data.len()
    }

    pub fn capacity(&self) -> usize {
        self.capacity
    }

    pub fn clear(&mut self) {
        self.data.clear();
    }
}

impl Default for VisualizableStack {
    fn default() -> Self {
        Self::new()
    }
}

impl Visualizable for VisualizableStack {
    fn execute_with_steps(&mut self, operation: Operation) -> Result<Vec<Step>> {
        match operation {
            Operation::Push(value) => {
                let mut steps = Vec::new();

                steps.push(Step {
                    description: format!("Pushing {} onto stack", value),
                    highlight_indices: vec![],
                    active_indices: vec![],
                    metadata: serde_json::json!({
                        "operation": "push",
                        "value": value
                    }),
                });

                self.push(value)?;

                let top_index = self.data.len() - 1;
                steps.push(Step {
                    description: format!("{} is now on top of stack", value),
                    highlight_indices: vec![],
                    active_indices: vec![top_index],
                    metadata: serde_json::json!({
                        "top_index": top_index
                    }),
                });

                Ok(steps)
            }

            Operation::Pop => {
                let mut steps = Vec::new();

                let value = self.peek()?;
                let top_index = self.data.len() - 1;

                steps.push(Step {
                    description: format!("Popping {} from stack", value),
                    highlight_indices: vec![top_index],
                    active_indices: vec![],
                    metadata: serde_json::json!({
                        "value": value
                    }),
                });

                self.pop()?;

                steps.push(Step {
                    description: format!("Removed {}, stack size now {}", value, self.size()),
                    highlight_indices: vec![],
                    active_indices: vec![],
                    metadata: serde_json::json!({}),
                });

                Ok(steps)
            }

            _ => Err(DsavError::InvalidState {
                reason: "Operation not supported for stacks".to_string(),
            }),
        }
    }

    fn render_state(&self) -> RenderState {
        RenderState {
            elements: self
                .data
                .iter()
                .enumerate()
                .map(|(i, &value)| {
                    let is_top = i == self.data.len() - 1;
                    RenderElement::new(value)
                        .with_label(value.to_string())
                        .with_sublabel(if is_top {
                            "TOP".to_string()
                        } else {
                            String::new()
                        })
                        .with_state(if is_top {
                            ElementState::Highlighted
                        } else {
                            ElementState::Normal
                        })
                })
                .collect(),
            connections: Vec::new(),
        }
    }
}

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn test_stack_push_pop() {
        let mut stack = VisualizableStack::new();
        assert!(stack.is_empty());

        stack.push(10).unwrap();
        stack.push(20).unwrap();
        stack.push(30).unwrap();

        assert_eq!(stack.size(), 3);
        assert_eq!(stack.pop().unwrap(), 30);
        assert_eq!(stack.pop().unwrap(), 20);
        assert_eq!(stack.pop().unwrap(), 10);
        assert!(stack.is_empty());
    }

    #[test]
    fn test_stack_peek() {
        let mut stack = VisualizableStack::new();
        stack.push(42).unwrap();
        assert_eq!(stack.peek().unwrap(), 42);
        assert_eq!(stack.size(), 1);
    }

    #[test]
    fn test_stack_overflow() {
        let mut stack = VisualizableStack::with_capacity(2);
        stack.push(1).unwrap();
        stack.push(2).unwrap();
        assert!(stack.push(3).is_err());
    }

    #[test]
    fn test_stack_underflow() {
        let mut stack = VisualizableStack::new();
        assert!(stack.pop().is_err());
    }
}

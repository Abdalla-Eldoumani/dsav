//! Educational queue implementation with visualization support.
//!
//! This implementation demonstrates queue operations (FIFO - First In First Out)
//! with step-by-step visualization.

use crate::error::{DsavError, Result};
use crate::state::{ElementState, RenderElement, RenderState};
use crate::traits::{Operation, Step, Visualizable};

const DEFAULT_CAPACITY: usize = 16;

#[derive(Debug, Clone)]
pub struct VisualizableQueue {
    data: Vec<i32>,
    capacity: usize,
}

impl VisualizableQueue {
    pub fn new() -> Self {
        Self::with_capacity(DEFAULT_CAPACITY)
    }

    pub fn with_capacity(capacity: usize) -> Self {
        Self {
            data: Vec::with_capacity(capacity),
            capacity,
        }
    }

    pub fn enqueue(&mut self, value: i32) -> Result<()> {
        if self.is_full() {
            return Err(DsavError::Full {
                capacity: self.capacity,
            });
        }

        self.data.push(value);
        Ok(())
    }

    pub fn dequeue(&mut self) -> Result<i32> {
        if self.is_empty() {
            return Err(DsavError::EmptyStructure);
        }

        Ok(self.data.remove(0))
    }

    pub fn peek(&self) -> Result<i32> {
        self.data.first().copied().ok_or(DsavError::EmptyStructure)
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

impl Default for VisualizableQueue {
    fn default() -> Self {
        Self::new()
    }
}

impl Visualizable for VisualizableQueue {
    fn execute_with_steps(&mut self, operation: Operation) -> Result<Vec<Step>> {
        match operation {
            Operation::Enqueue(value) => {
                let mut steps = Vec::new();

                steps.push(Step {
                    description: format!("Enqueuing {} to back of queue", value),
                    highlight_indices: vec![],
                    active_indices: vec![],
                    metadata: serde_json::json!({
                        "operation": "enqueue",
                        "value": value
                    }),
                });

                self.enqueue(value)?;

                let back_index = self.data.len() - 1;
                steps.push(Step {
                    description: format!("{} added to back, queue size now {}", value, self.len()),
                    highlight_indices: vec![],
                    active_indices: vec![back_index],
                    metadata: serde_json::json!({
                        "back_index": back_index
                    }),
                });

                Ok(steps)
            }

            Operation::Dequeue => {
                let mut steps = Vec::new();

                let value = self.peek()?;

                steps.push(Step {
                    description: format!("Dequeuing {} from front of queue", value),
                    highlight_indices: vec![0],
                    active_indices: vec![],
                    metadata: serde_json::json!({
                        "value": value
                    }),
                });

                self.dequeue()?;

                if !self.is_empty() {
                    steps.push(Step {
                        description: "Shifting remaining elements forward".to_string(),
                        highlight_indices: (0..self.len()).collect(),
                        active_indices: vec![],
                        metadata: serde_json::json!({}),
                    });
                }

                steps.push(Step {
                    description: format!("Removed {}, queue size now {}", value, self.size()),
                    highlight_indices: vec![],
                    active_indices: vec![],
                    metadata: serde_json::json!({}),
                });

                Ok(steps)
            }

            _ => Err(DsavError::InvalidState {
                reason: "Operation not supported for queues".to_string(),
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
                    let is_front = i == 0;
                    let is_back = i == self.data.len() - 1;

                    RenderElement::new(value)
                        .with_label(value.to_string())
                        .with_sublabel(if is_front {
                            "FRONT".to_string()
                        } else if is_back {
                            "BACK".to_string()
                        } else {
                            String::new()
                        })
                        .with_state(if is_front {
                            ElementState::Highlighted
                        } else if is_back {
                            ElementState::Active
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
    fn test_queue_enqueue_dequeue() {
        let mut queue = VisualizableQueue::new();
        assert!(queue.is_empty());

        queue.enqueue(10).unwrap();
        queue.enqueue(20).unwrap();
        queue.enqueue(30).unwrap();

        assert_eq!(queue.size(), 3);
        assert_eq!(queue.dequeue().unwrap(), 10);
        assert_eq!(queue.dequeue().unwrap(), 20);
        assert_eq!(queue.dequeue().unwrap(), 30);
        assert!(queue.is_empty());
    }

    #[test]
    fn test_queue_peek() {
        let mut queue = VisualizableQueue::new();
        queue.enqueue(42).unwrap();
        queue.enqueue(17).unwrap();

        assert_eq!(queue.peek().unwrap(), 42);
        assert_eq!(queue.size(), 2);
    }

    #[test]
    fn test_queue_overflow() {
        let mut queue = VisualizableQueue::with_capacity(2);
        queue.enqueue(1).unwrap();
        queue.enqueue(2).unwrap();
        assert!(queue.enqueue(3).is_err());
    }

    #[test]
    fn test_queue_underflow() {
        let mut queue = VisualizableQueue::new();
        assert!(queue.dequeue().is_err());
    }

    #[test]
    fn test_queue_fifo_order() {
        let mut queue = VisualizableQueue::new();

        for i in 1..=5 {
            queue.enqueue(i * 10).unwrap();
        }

        for i in 1..=5 {
            assert_eq!(queue.dequeue().unwrap(), i * 10);
        }
    }
}

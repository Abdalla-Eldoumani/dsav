//! Educational array implementation with visualization support.
//!
//! This implementation demonstrates array operations with step-by-step visualization.
//! For production use, prefer Vec<T> from the standard library.

use crate::error::{DsavError, Result};
use crate::state::{RenderElement, RenderState};
use crate::traits::{Operation, Step, Visualizable};

#[derive(Debug, Clone)]
pub struct VisualizableArray {
    elements: Vec<i32>,
    capacity: usize,
}

impl VisualizableArray {
    pub fn new(capacity: usize) -> Self {
        Self {
            elements: Vec::with_capacity(capacity),
            capacity,
        }
    }

    pub fn insert(&mut self, index: usize, value: i32) -> Result<()> {
        if self.elements.len() >= self.capacity {
            return Err(DsavError::Full {
                capacity: self.capacity,
            });
        }

        if index > self.elements.len() {
            return Err(DsavError::IndexOutOfBounds {
                index,
                size: self.elements.len(),
            });
        }

        self.elements.insert(index, value);
        Ok(())
    }

    pub fn delete(&mut self, index: usize) -> Result<i32> {
        if index >= self.elements.len() {
            return Err(DsavError::IndexOutOfBounds {
                index,
                size: self.elements.len(),
            });
        }

        Ok(self.elements.remove(index))
    }

    pub fn update(&mut self, index: usize, value: i32) -> Result<i32> {
        if index >= self.elements.len() {
            return Err(DsavError::IndexOutOfBounds {
                index,
                size: self.elements.len(),
            });
        }

        let old_value = self.elements[index];
        self.elements[index] = value;
        Ok(old_value)
    }

    pub fn get(&self, index: usize) -> Result<i32> {
        self.elements.get(index).copied().ok_or_else(|| {
            DsavError::IndexOutOfBounds {
                index,
                size: self.elements.len(),
            }
        })
    }

    pub fn search(&self, value: i32) -> Option<usize> {
        self.elements.iter().position(|&x| x == value)
    }

    pub fn len(&self) -> usize {
        self.elements.len()
    }

    pub fn is_empty(&self) -> bool {
        self.elements.is_empty()
    }

    pub fn capacity(&self) -> usize {
        self.capacity
    }
}

impl Visualizable for VisualizableArray {
    fn execute_with_steps(&mut self, operation: Operation) -> Result<Vec<Step>> {
        match operation {
            Operation::Insert(index, value) => {
                let mut steps = Vec::new();

                steps.push(Step {
                    description: format!("Inserting {} at index {}", value, index),
                    highlight_indices: vec![index],
                    active_indices: vec![],
                    metadata: serde_json::json!({
                        "operation": "insert",
                        "value": value,
                        "index": index
                    }),
                });

                if index < self.elements.len() {
                    steps.push(Step {
                        description: "Shifting elements to make room".to_string(),
                        highlight_indices: (index..self.elements.len()).collect(),
                        active_indices: vec![],
                        metadata: serde_json::json!({}),
                    });
                }

                self.insert(index, value)?;

                steps.push(Step {
                    description: "Insertion complete".to_string(),
                    highlight_indices: vec![],
                    active_indices: vec![index],
                    metadata: serde_json::json!({}),
                });

                Ok(steps)
            }

            Operation::Delete(index) => {
                let mut steps = Vec::new();

                let value = self.get(index)?;

                steps.push(Step {
                    description: format!("Deleting element {} at index {}", value, index),
                    highlight_indices: vec![index],
                    active_indices: vec![],
                    metadata: serde_json::json!({}),
                });

                self.delete(index)?;

                if index < self.elements.len() {
                    steps.push(Step {
                        description: "Shifting elements to fill gap".to_string(),
                        highlight_indices: (index..self.elements.len()).collect(),
                        active_indices: vec![],
                        metadata: serde_json::json!({}),
                    });
                }

                steps.push(Step {
                    description: "Deletion complete".to_string(),
                    highlight_indices: vec![],
                    active_indices: vec![],
                    metadata: serde_json::json!({}),
                });

                Ok(steps)
            }

            Operation::Search(target) => {
                let mut steps = Vec::new();

                for (i, &value) in self.elements.iter().enumerate() {
                    steps.push(Step {
                        description: format!("Checking index {}: {}", i, value),
                        highlight_indices: vec![i],
                        active_indices: vec![],
                        metadata: serde_json::json!({
                            "checking": value,
                            "target": target
                        }),
                    });

                    if value == target {
                        steps.push(Step {
                            description: format!("Found {} at index {}", target, i),
                            highlight_indices: vec![],
                            active_indices: vec![i],
                            metadata: serde_json::json!({}),
                        });
                        return Ok(steps);
                    }
                }

                steps.push(Step {
                    description: format!("Value {} not found", target),
                    highlight_indices: vec![],
                    active_indices: vec![],
                    metadata: serde_json::json!({}),
                });

                Ok(steps)
            }

            Operation::BubbleSort => {
                use crate::algorithms::sorting::bubble_sort_with_steps;
                bubble_sort_with_steps(&mut self.elements)
            }

            Operation::InsertionSort => {
                use crate::algorithms::sorting::insertion_sort_with_steps;
                insertion_sort_with_steps(&mut self.elements)
            }

            Operation::QuickSort => {
                use crate::algorithms::sorting::quick_sort_with_steps;
                quick_sort_with_steps(&mut self.elements)
            }

            Operation::BinarySearch(target) => {
                use crate::algorithms::sorting::binary_search_with_steps;
                binary_search_with_steps(&self.elements, target)
            }

            Operation::Update(index, value) => {
                let mut steps = Vec::new();

                let old_value = self.get(index)?;

                steps.push(Step {
                    description: format!("Updating index {} from {} to {}", index, old_value, value),
                    highlight_indices: vec![index],
                    active_indices: vec![],
                    metadata: serde_json::json!({
                        "operation": "update",
                        "index": index,
                        "old_value": old_value,
                        "new_value": value
                    }),
                });

                self.update(index, value)?;

                steps.push(Step {
                    description: format!("Updated index {} to {}", index, value),
                    highlight_indices: vec![],
                    active_indices: vec![index],
                    metadata: serde_json::json!({}),
                });

                Ok(steps)
            }

            Operation::SelectionSort => {
                use crate::algorithms::sorting::selection_sort_with_steps;
                selection_sort_with_steps(&mut self.elements)
            }

            Operation::MergeSort => {
                use crate::algorithms::sorting::merge_sort_with_steps;
                merge_sort_with_steps(&mut self.elements)
            }

            _ => Err(DsavError::InvalidState {
                reason: "Operation not supported for arrays".to_string(),
            }),
        }
    }

    fn render_state(&self) -> RenderState {
        RenderState {
            elements: self
                .elements
                .iter()
                .enumerate()
                .map(|(i, &value)| {
                    RenderElement::new(value)
                        .with_label(value.to_string())
                        .with_sublabel(format!("[{}]", i))
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
    fn test_array_insert() {
        let mut arr = VisualizableArray::new(10);
        assert!(arr.insert(0, 42).is_ok());
        assert_eq!(arr.get(0).unwrap(), 42);
        assert_eq!(arr.len(), 1);
    }

    #[test]
    fn test_array_insert_out_of_bounds() {
        let mut arr = VisualizableArray::new(10);
        let result = arr.insert(5, 42);
        assert!(result.is_err());
    }

    #[test]
    fn test_array_delete() {
        let mut arr = VisualizableArray::new(10);
        arr.insert(0, 42).unwrap();
        arr.insert(1, 17).unwrap();

        let deleted = arr.delete(0).unwrap();
        assert_eq!(deleted, 42);
        assert_eq!(arr.len(), 1);
        assert_eq!(arr.get(0).unwrap(), 17);
    }

    #[test]
    fn test_array_search() {
        let mut arr = VisualizableArray::new(10);
        arr.insert(0, 10).unwrap();
        arr.insert(1, 20).unwrap();
        arr.insert(2, 30).unwrap();

        assert_eq!(arr.search(20), Some(1));
        assert_eq!(arr.search(99), None);
    }
}

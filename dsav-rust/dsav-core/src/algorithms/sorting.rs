//! Sorting and searching algorithm implementations with step-by-step visualization.

use crate::error::Result;
use crate::traits::Step;

pub fn bubble_sort_with_steps(arr: &mut [i32]) -> Result<Vec<Step>> {
    let mut steps = Vec::new();
    let n = arr.len();

    if n <= 1 {
        return Ok(steps);
    }

    // Store initial state
    steps.push(Step {
        description: "Starting Bubble Sort".to_string(),
        highlight_indices: vec![],
        active_indices: vec![],
        metadata: serde_json::json!({
            "array_state": arr.to_vec()
        }),
    });

    for i in 0..n {
        let mut swapped = false;

        for j in 0..n - i - 1 {
            steps.push(Step {
                description: format!("Comparing {} and {}", arr[j], arr[j + 1]),
                highlight_indices: vec![j, j + 1],
                active_indices: vec![],
                metadata: serde_json::json!({
                    "operation": "compare",
                    "values": [arr[j], arr[j + 1]],
                    "array_state": arr.to_vec()
                }),
            });

            if arr[j] > arr[j + 1] {
                arr.swap(j, j + 1);

                steps.push(Step {
                    description: format!("Swapping {} and {}", arr[j + 1], arr[j]),
                    highlight_indices: vec![],
                    active_indices: vec![j, j + 1],
                    metadata: serde_json::json!({
                        "operation": "swap",
                        "values": [arr[j], arr[j + 1]],
                        "array_state": arr.to_vec()
                    }),
                });

                swapped = true;
            }
        }

        steps.push(Step {
            description: format!(
                "Element {} is now in final position",
                arr[n - i - 1]
            ),
            highlight_indices: vec![n - i - 1],
            active_indices: vec![],
            metadata: serde_json::json!({
                "operation": "sorted",
                "index": n - i - 1,
                "array_state": arr.to_vec()
            }),
        });

        if !swapped {
            steps.push(Step {
                description: "Array is sorted, no more swaps needed".to_string(),
                highlight_indices: vec![],
                active_indices: vec![],
                metadata: serde_json::json!({
                    "array_state": arr.to_vec()
                }),
            });
            break;
        }
    }

    steps.push(Step {
        description: "Sorting complete".to_string(),
        highlight_indices: vec![],
        active_indices: (0..n).collect(),
        metadata: serde_json::json!({
            "array_state": arr.to_vec()
        }),
    });

    Ok(steps)
}

pub fn insertion_sort_with_steps(arr: &mut [i32]) -> Result<Vec<Step>> {
    let mut steps = Vec::new();
    let n = arr.len();

    if n <= 1 {
        return Ok(steps);
    }

    // Store initial state
    steps.push(Step {
        description: "Starting Insertion Sort".to_string(),
        highlight_indices: vec![],
        active_indices: vec![],
        metadata: serde_json::json!({
            "array_state": arr.to_vec()
        }),
    });

    for i in 1..n {
        let key = arr[i];
        let mut j = i;

        steps.push(Step {
            description: format!("Selecting {} to insert into sorted portion", key),
            highlight_indices: vec![i],
            active_indices: vec![],
            metadata: serde_json::json!({
                "operation": "select",
                "value": key,
                "index": i,
                "array_state": arr.to_vec()
            }),
        });

        while j > 0 && arr[j - 1] > key {
            steps.push(Step {
                description: format!("Comparing {} with {}", arr[j - 1], key),
                highlight_indices: vec![j - 1, j],
                active_indices: vec![],
                metadata: serde_json::json!({
                    "operation": "compare",
                    "values": [arr[j - 1], key],
                    "array_state": arr.to_vec()
                }),
            });

            arr[j] = arr[j - 1];
            j -= 1;

            steps.push(Step {
                description: format!("Shifting element to the right"),
                highlight_indices: vec![],
                active_indices: vec![j, j + 1],
                metadata: serde_json::json!({
                    "operation": "shift",
                    "array_state": arr.to_vec()
                }),
            });
        }

        arr[j] = key;

        steps.push(Step {
            description: format!("Inserted {} at position {}", key, j),
            highlight_indices: vec![j],
            active_indices: vec![],
            metadata: serde_json::json!({
                "operation": "insert",
                "value": key,
                "index": j,
                "array_state": arr.to_vec()
            }),
        });

        steps.push(Step {
            description: format!("Elements 0..={} are now sorted", i),
            highlight_indices: (0..=i).collect(),
            active_indices: vec![],
            metadata: serde_json::json!({
                "array_state": arr.to_vec()
            }),
        });
    }

    steps.push(Step {
        description: "Insertion sort complete".to_string(),
        highlight_indices: vec![],
        active_indices: (0..n).collect(),
        metadata: serde_json::json!({
            "array_state": arr.to_vec()
        }),
    });

    Ok(steps)
}

pub fn quick_sort_with_steps(arr: &mut [i32]) -> Result<Vec<Step>> {
    let mut steps = Vec::new();
    let n = arr.len();

    if n <= 1 {
        return Ok(steps);
    }

    steps.push(Step {
        description: "Starting Quick Sort".to_string(),
        highlight_indices: vec![],
        active_indices: vec![],
        metadata: serde_json::json!({
            "array_state": arr.to_vec()
        }),
    });

    quick_sort_helper(arr, 0, n - 1, &mut steps)?;

    steps.push(Step {
        description: "Quick sort complete".to_string(),
        highlight_indices: vec![],
        active_indices: (0..n).collect(),
        metadata: serde_json::json!({
            "array_state": arr.to_vec()
        }),
    });

    Ok(steps)
}

fn quick_sort_helper(
    arr: &mut [i32],
    low: usize,
    high: usize,
    steps: &mut Vec<Step>,
) -> Result<()> {
    if low < high {
        let pivot_index = partition(arr, low, high, steps)?;

        if pivot_index > 0 {
            quick_sort_helper(arr, low, pivot_index - 1, steps)?;
        }

        if pivot_index < high {
            quick_sort_helper(arr, pivot_index + 1, high, steps)?;
        }
    }

    Ok(())
}

fn partition(arr: &mut [i32], low: usize, high: usize, steps: &mut Vec<Step>) -> Result<usize> {
    let pivot = arr[high];

    steps.push(Step {
        description: format!("Choosing {} as pivot (index {})", pivot, high),
        highlight_indices: vec![high],
        active_indices: vec![],
        metadata: serde_json::json!({
            "operation": "pivot",
            "value": pivot,
            "index": high,
            "array_state": arr.to_vec()
        }),
    });

    let mut i = low;

    for j in low..high {
        steps.push(Step {
            description: format!("Comparing {} with pivot {}", arr[j], pivot),
            highlight_indices: vec![j, high],
            active_indices: vec![],
            metadata: serde_json::json!({
                "operation": "compare",
                "values": [arr[j], pivot],
                "array_state": arr.to_vec()
            }),
        });

        if arr[j] < pivot {
            if i != j {
                arr.swap(i, j);

                steps.push(Step {
                    description: format!("Swapping {} and {}", arr[j], arr[i]),
                    highlight_indices: vec![],
                    active_indices: vec![i, j],
                    metadata: serde_json::json!({
                        "operation": "swap",
                        "values": [arr[i], arr[j]],
                        "array_state": arr.to_vec()
                    }),
                });
            }

            i += 1;
        }
    }

    arr.swap(i, high);

    steps.push(Step {
        description: format!("Placing pivot {} at final position {}", pivot, i),
        highlight_indices: vec![],
        active_indices: vec![i, high],
        metadata: serde_json::json!({
            "operation": "swap",
            "values": [arr[i], arr[high]],
            "array_state": arr.to_vec()
        }),
    });

    steps.push(Step {
        description: format!("Pivot {} is now in correct position", pivot),
        highlight_indices: vec![i],
        active_indices: vec![],
        metadata: serde_json::json!({
            "operation": "sorted",
            "index": i,
            "array_state": arr.to_vec()
        }),
    });

    Ok(i)
}

pub fn binary_search_with_steps(arr: &[i32], target: i32) -> Result<Vec<Step>> {
    let mut steps = Vec::new();
    let n = arr.len();

    if n == 0 {
        steps.push(Step {
            description: "Array is empty, cannot search".to_string(),
            highlight_indices: vec![],
            active_indices: vec![],
            metadata: serde_json::json!({
                "found": false
            }),
        });
        return Ok(steps);
    }

    steps.push(Step {
        description: format!("Starting binary search for {}", target),
        highlight_indices: vec![],
        active_indices: vec![],
        metadata: serde_json::json!({
            "operation": "binary_search",
            "target": target,
            "array_state": arr.to_vec()
        }),
    });

    let mut left = 0;
    let mut right = n - 1;

    while left <= right {
        let mid = left + (right - left) / 2;

        steps.push(Step {
            description: format!("Checking middle element at index {}", mid),
            highlight_indices: vec![left, mid, right],
            active_indices: vec![],
            metadata: serde_json::json!({
                "left": left,
                "mid": mid,
                "right": right,
                "mid_value": arr[mid],
                "array_state": arr.to_vec()
            }),
        });

        if arr[mid] == target {
            steps.push(Step {
                description: format!("Found {} at index {}", target, mid),
                highlight_indices: vec![],
                active_indices: vec![mid],
                metadata: serde_json::json!({
                    "found": true,
                    "index": mid,
                    "array_state": arr.to_vec()
                }),
            });
            return Ok(steps);
        }

        if arr[mid] < target {
            steps.push(Step {
                description: format!("{} < {}, searching right half", arr[mid], target),
                highlight_indices: vec![mid + 1, right],
                active_indices: vec![],
                metadata: serde_json::json!({
                    "array_state": arr.to_vec()
                }),
            });
            left = mid + 1;
        } else {
            if mid == 0 {
                break;
            }
            steps.push(Step {
                description: format!("{} > {}, searching left half", arr[mid], target),
                highlight_indices: vec![left, mid - 1],
                active_indices: vec![],
                metadata: serde_json::json!({
                    "array_state": arr.to_vec()
                }),
            });
            right = mid - 1;
        }
    }

    steps.push(Step {
        description: format!("Value {} not found in array", target),
        highlight_indices: vec![],
        active_indices: vec![],
        metadata: serde_json::json!({
            "found": false,
            "array_state": arr.to_vec()
        }),
    });

    Ok(steps)
}

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn test_bubble_sort_correctness() {
        let mut arr = vec![5, 2, 8, 1, 9];
        let _ = bubble_sort_with_steps(&mut arr).unwrap();
        assert_eq!(arr, vec![1, 2, 5, 8, 9]);
    }

    #[test]
    fn test_bubble_sort_already_sorted() {
        let mut arr = vec![1, 2, 3, 4, 5];
        let steps = bubble_sort_with_steps(&mut arr).unwrap();
        assert!(!steps.is_empty());
        assert_eq!(arr, vec![1, 2, 3, 4, 5]);
    }

    #[test]
    fn test_bubble_sort_single_element() {
        let mut arr = vec![42];
        let steps = bubble_sort_with_steps(&mut arr).unwrap();
        assert_eq!(steps.len(), 0);
        assert_eq!(arr, vec![42]);
    }

    #[test]
    fn test_insertion_sort_correctness() {
        let mut arr = vec![5, 2, 8, 1, 9];
        let _ = insertion_sort_with_steps(&mut arr).unwrap();
        assert_eq!(arr, vec![1, 2, 5, 8, 9]);
    }

    #[test]
    fn test_insertion_sort_already_sorted() {
        let mut arr = vec![1, 2, 3, 4, 5];
        let steps = insertion_sort_with_steps(&mut arr).unwrap();
        assert!(!steps.is_empty());
        assert_eq!(arr, vec![1, 2, 3, 4, 5]);
    }

    #[test]
    fn test_quick_sort_correctness() {
        let mut arr = vec![5, 2, 8, 1, 9, 3, 7];
        let _ = quick_sort_with_steps(&mut arr).unwrap();
        assert_eq!(arr, vec![1, 2, 3, 5, 7, 8, 9]);
    }

    #[test]
    fn test_quick_sort_with_duplicates() {
        let mut arr = vec![5, 2, 5, 1, 2];
        let _ = quick_sort_with_steps(&mut arr).unwrap();
        assert_eq!(arr, vec![1, 2, 2, 5, 5]);
    }
}

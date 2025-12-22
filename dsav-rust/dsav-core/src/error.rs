//! Error types for the DSAV core library.

use thiserror::Error;

pub type Result<T> = std::result::Result<T, DsavError>;

#[derive(Error, Debug)]
pub enum DsavError {
    #[error("Index {index} out of bounds (size: {size})")]
    IndexOutOfBounds { index: usize, size: usize },

    #[error("Operation failed: structure is empty")]
    EmptyStructure,

    #[error("Operation failed: structure is full (capacity: {capacity})")]
    Full { capacity: usize },

    #[error("Value {value} not found in structure")]
    NotFound { value: i32 },

    #[error("Invalid state: {reason}")]
    InvalidState { reason: String },

    #[error("Visualization error: {0}")]
    Visualization(String),

    #[error(transparent)]
    Other(#[from] anyhow::Error),
}

//! Core library for DSAV - Data Structures & Algorithms Visualizer.
//!
//! This library provides educational implementations of fundamental data structures
//! and algorithms with built-in support for step-by-step visualization. All implementations
//! prioritize clarity and educational value over performance.

pub mod error;
pub mod traits;
pub mod state;
pub mod structures;
pub mod algorithms;

pub use error::{DsavError, Result};
pub use traits::{Visualizable, Step, Operation};
pub use state::{RenderState, RenderElement, ElementState};

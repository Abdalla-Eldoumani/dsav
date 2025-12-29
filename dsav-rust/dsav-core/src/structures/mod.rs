//! Data structure implementations with visualization support.

pub mod array;
pub mod stack;
pub mod queue;
pub mod linked_list;
pub mod bst;
pub mod rb_tree;

pub use array::VisualizableArray;
pub use stack::VisualizableStack;
pub use queue::VisualizableQueue;
pub use linked_list::VisualizableLinkedList;
pub use bst::VisualizableBST;
pub use rb_tree::VisualizableRBTree;
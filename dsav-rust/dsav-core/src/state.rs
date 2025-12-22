//! Rendering state types for visualization.

#[derive(Debug, Clone)]
pub struct RenderState {
    pub elements: Vec<RenderElement>,
    pub connections: Vec<(usize, usize)>,
}

#[derive(Debug, Clone)]
pub struct RenderElement {
    pub value: i32,
    pub state: ElementState,
    pub label: String,
    pub sublabel: String,
}

#[derive(Debug, Clone, Copy, PartialEq, Eq)]
pub enum ElementState {
    Normal,
    Highlighted,
    Active,
    Sorted,
    Comparing,
    Swapping,
}

impl RenderElement {
    pub fn new(value: i32) -> Self {
        Self {
            value,
            state: ElementState::Normal,
            label: value.to_string(),
            sublabel: String::new(),
        }
    }

    pub fn with_label(mut self, label: String) -> Self {
        self.label = label;
        self
    }

    pub fn with_sublabel(mut self, sublabel: String) -> Self {
        self.sublabel = sublabel;
        self
    }

    pub fn with_state(mut self, state: ElementState) -> Self {
        self.state = state;
        self
    }
}

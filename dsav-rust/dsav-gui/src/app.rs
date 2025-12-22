//! Main application state and UI logic.

use dsav_core::{
    structures::VisualizableArray,
    structures::VisualizableStack,
    structures::VisualizableQueue,
    structures::VisualizableLinkedList,
    structures::VisualizableBST,
    Operation,
    Visualizable,
    Step
};
use crate::colors::{Colors, Theme, ColorPalette};

#[derive(Debug, Clone, Copy, PartialEq, Eq)]
enum DataStructure {
    Array,
    Stack,
    Queue,
    LinkedList,
    BST,
}

pub struct DsavApp {
    selected_structure: DataStructure,
    array: VisualizableArray,
    stack: VisualizableStack,
    queue: VisualizableQueue,
    linked_list: VisualizableLinkedList,
    bst: VisualizableBST,

    input_value: i32,
    input_index: usize,
    search_value: i32,
    randomize_size: usize,

    status_message: String,
    current_steps: Vec<Step>,
    current_step_index: usize,
    playing: bool,
    animation_speed: f32,
    time_since_last_step: f32,

    current_theme: Theme,
    show_settings: bool,
}

impl DsavApp {
    pub fn new() -> Self {
        let mut array = VisualizableArray::new(16);

        for i in 0..5 {
            let _ = array.insert(i, (i as i32 + 1) * 10);
        }

        let mut linked_list = VisualizableLinkedList::new();
        linked_list.insert_back(10);
        linked_list.insert_back(20);
        linked_list.insert_back(30);

        let mut bst = VisualizableBST::new();
        bst.insert(50);
        bst.insert(30);
        bst.insert(70);
        bst.insert(20);
        bst.insert(40);

        Self {
            selected_structure: DataStructure::Array,
            array,
            stack: VisualizableStack::with_capacity(16),
            queue: VisualizableQueue::with_capacity(16),
            linked_list,
            bst,
            input_value: 42,
            input_index: 0,
            search_value: 30,
            randomize_size: 8,
            status_message: "Ready. Select an operation to visualize.".to_string(),
            current_steps: Vec::new(),
            current_step_index: 0,
            playing: false,
            animation_speed: 1.0,
            time_since_last_step: 0.0,
            current_theme: Theme::Vibrant,
            show_settings: false,
        }
    }

    pub fn update(&mut self, delta_time: f32) {
        if self.playing && !self.current_steps.is_empty() {
            self.time_since_last_step += delta_time * self.animation_speed;

            let step_duration = 0.5;
            if self.time_since_last_step >= step_duration {
                self.time_since_last_step = 0.0;

                if self.current_step_index < self.current_steps.len() - 1 {
                    self.current_step_index += 1;
                    if let Some(step) = self.current_steps.get(self.current_step_index) {
                        self.status_message = step.description.clone();
                    }
                } else {
                    self.playing = false;
                    self.status_message = "Animation complete.".to_string();
                }
            }
        }
    }

    pub fn ui(&mut self, ctx: &egui::Context) {
        let palette = self.current_theme.colors();
        crate::colors::apply_theme(ctx, &palette);

        self.update(ctx.input(|i| i.stable_dt));

        if self.playing {
            ctx.request_repaint();
        }

        egui::TopBottomPanel::top("top_panel").show(ctx, |ui| {
            ui.add_space(8.0);
            ui.horizontal(|ui| {
                ui.heading("🦀 DSAV - Data Structures & Algorithms Visualizer");
                ui.with_layout(egui::Layout::right_to_left(egui::Align::Center), |ui| {
                    if ui.button("⚙ Settings").clicked() {
                        self.show_settings = !self.show_settings;
                    }
                    ui.label("Rust Edition");
                });
            });
            ui.add_space(4.0);
        });

        if self.show_settings {
            self.render_settings(ctx, &palette);
        }

        egui::SidePanel::left("control_panel").min_width(280.0).show(ctx, |ui| {
            ui.add_space(8.0);
            ui.heading("Controls");
            ui.separator();

            // Wrap the entire control panel in a scroll area
            egui::ScrollArea::vertical()
                .auto_shrink([false, false])
                .show(ui, |ui| {
                    ui.label("Data Structure:");
                    ui.horizontal(|ui| {
                        ui.selectable_value(&mut self.selected_structure, DataStructure::Array, "📊 Array");
                        ui.selectable_value(&mut self.selected_structure, DataStructure::Stack, "📚 Stack");
                    });
                    ui.horizontal(|ui| {
                        ui.selectable_value(&mut self.selected_structure, DataStructure::Queue, "🎯 Queue");
                        ui.selectable_value(&mut self.selected_structure, DataStructure::LinkedList, "🔗 List");
                    });
                    ui.horizontal(|ui| {
                        ui.selectable_value(&mut self.selected_structure, DataStructure::BST, "🌲 BST");
                    });

                    ui.add_space(16.0);

                    match self.selected_structure {
                        DataStructure::Array => self.array_controls(ui),
                        DataStructure::Stack => self.stack_controls(ui),
                        DataStructure::Queue => self.queue_controls(ui),
                        DataStructure::LinkedList => self.linked_list_controls(ui),
                        DataStructure::BST => self.bst_controls(ui),
                    }

                    ui.add_space(16.0);
                    ui.separator();

                    ui.label("Statistics:");
                    match self.selected_structure {
                        DataStructure::Array => {
                            ui.label(format!("Size: {} / {}", self.array.len(), self.array.capacity()));
                            ui.label(format!("Utilization: {:.1}%",
                                (self.array.len() as f32 / self.array.capacity() as f32) * 100.0));
                        }
                        DataStructure::Stack => {
                            ui.label(format!("Size: {} / {}", self.stack.len(), self.stack.capacity()));
                            ui.label(format!("Utilization: {:.1}%",
                                (self.stack.len() as f32 / self.stack.capacity() as f32) * 100.0));
                        }
                        DataStructure::Queue => {
                            ui.label(format!("Size: {} / {}", self.queue.len(), self.queue.capacity()));
                            ui.label(format!("Utilization: {:.1}%",
                                (self.queue.len() as f32 / self.queue.capacity() as f32) * 100.0));
                        }
                        DataStructure::LinkedList => {
                            ui.label(format!("Nodes: {}", self.linked_list.len()));
                            ui.label(if self.linked_list.is_empty() {
                                "Status: Empty".to_string()
                            } else {
                                format!("Head: {}", self.linked_list.get(0).unwrap_or(0))
                            });
                        }
                        DataStructure::BST => {
                            ui.label(format!("Nodes: {}", self.bst.size()));
                            ui.label(if self.bst.is_empty() {
                                "Status: Empty".to_string()
                            } else {
                                "Status: Has nodes".to_string()
                            });
                        }
                    }

                    if !self.current_steps.is_empty() {
                        ui.add_space(16.0);
                        ui.separator();
                        self.render_animation_controls(ui);
                    }
                });
        });

        egui::TopBottomPanel::bottom("status_panel").show(ctx, |ui| {
            ui.add_space(4.0);
            ui.horizontal(|ui| {
                ui.label("Status:");
                ui.label(&self.status_message);
            });
            ui.add_space(4.0);
        });

        egui::CentralPanel::default().show(ctx, |ui| {
            ui.add_space(16.0);

            ui.vertical_centered(|ui| {
                ui.heading(match self.selected_structure {
                    DataStructure::Array => "📊 Array Visualization",
                    DataStructure::Stack => "📚 Stack Visualization (LIFO)",
                    DataStructure::Queue => "🎯 Queue Visualization (FIFO)",
                    DataStructure::LinkedList => "🔗 Linked List Visualization",
                    DataStructure::BST => "🌲 Binary Search Tree Visualization",
                });
            });

            ui.add_space(24.0);

            let available_height = ui.available_height();
            let element_height = 80.0;
            let y_offset = (available_height - element_height) / 2.0;

            ui.allocate_ui_with_layout(
                egui::vec2(ui.available_width(), available_height),
                egui::Layout::top_down(egui::Align::Center),
                |ui| {
                    ui.add_space(y_offset.max(0.0));

                    match self.selected_structure {
                        DataStructure::Array => self.render_array(ui),
                        DataStructure::Stack => self.render_stack(ui),
                        DataStructure::Queue => self.render_queue(ui),
                        DataStructure::LinkedList => self.render_linked_list(ui),
                        DataStructure::BST => self.render_bst(ui),
                    }
                },
            );
        });
    }

    fn array_controls(&mut self, ui: &mut egui::Ui) {
        ui.group(|ui| {
            ui.label("Insert / Delete:");

            ui.horizontal(|ui| {
                ui.label("Value:");
                ui.add(egui::DragValue::new(&mut self.input_value).speed(1.0));
            });

            ui.horizontal(|ui| {
                ui.label("Index:");
                ui.add(egui::DragValue::new(&mut self.input_index).speed(0.1));
            });

            ui.horizontal(|ui| {
                if ui.button("📥 Insert").clicked() {
                    self.execute_array_operation(Operation::Insert(self.input_index, self.input_value));
                }

                if ui.button("🗑 Delete").clicked() {
                    self.execute_array_operation(Operation::Delete(self.input_index));
                }
            });
        });

        ui.add_space(8.0);

        ui.group(|ui| {
            ui.label("Search:");

            ui.horizontal(|ui| {
                ui.label("Value:");
                ui.add(egui::DragValue::new(&mut self.search_value).speed(1.0));
            });

            ui.horizontal(|ui| {
                if ui.button("🔍 Linear Search").clicked() {
                    self.execute_array_operation(Operation::Search(self.search_value));
                }

                if ui.button("⚡ Binary Search").clicked() {
                    // For binary search, sort the array instantly without animation
                    match self.array.execute_with_steps(Operation::QuickSort) {
                        Ok(_) => {
                            // Array is now sorted, clear steps to skip animation
                            self.current_steps.clear();
                            self.playing = false;

                            // Now execute the binary search
                            self.execute_array_operation(Operation::BinarySearch(self.search_value));
                        }
                        Err(e) => {
                            self.status_message = format!("Error sorting array: {}", e);
                        }
                    }
                }
            });
        });

        ui.add_space(8.0);

        ui.group(|ui| {
            ui.label("Sorting Algorithms:");

            if ui.button("🫧 Bubble Sort").clicked() {
                self.execute_array_operation(Operation::BubbleSort);
            }

            if ui.button("📌 Insertion Sort").clicked() {
                self.execute_array_operation(Operation::InsertionSort);
            }

            if ui.button("⚡ Quick Sort").clicked() {
                self.execute_array_operation(Operation::QuickSort);
            }
        });

        ui.add_space(8.0);

        ui.group(|ui| {
            ui.label("Randomize:");

            ui.horizontal(|ui| {
                ui.label("Size:");
                ui.add(egui::DragValue::new(&mut self.randomize_size).clamp_range(1..=16).speed(0.1));
            });

            if ui.button("🎲 Randomize").clicked() {
                use rand::Rng;
                let mut rng = rand::thread_rng();

                self.array = dsav_core::structures::VisualizableArray::new(16);
                for i in 0..self.randomize_size {
                    let random_value = rng.gen_range(1..=100);
                    let _ = self.array.insert(i, random_value);
                }

                self.current_steps.clear();
                self.status_message = format!("Generated {} random elements", self.randomize_size);
            }
        });

        ui.add_space(8.0);

        ui.group(|ui| {
            ui.label("Clear:");

            if ui.button("🗑 Clear Array").clicked() {
                self.array = dsav_core::structures::VisualizableArray::new(16);
                self.current_steps.clear();
                self.status_message = "Array cleared".to_string();
            }
        });
    }

    fn stack_controls(&mut self, ui: &mut egui::Ui) {
        ui.group(|ui| {
            ui.label("Stack Operations:");

            ui.horizontal(|ui| {
                ui.label("Value:");
                ui.add(egui::DragValue::new(&mut self.input_value).speed(1.0));
            });

            ui.horizontal(|ui| {
                if ui.button("⬇ Push").clicked() {
                    self.execute_stack_operation(Operation::Push(self.input_value));
                }

                if ui.button("⬆ Pop").clicked() {
                    self.execute_stack_operation(Operation::Pop);
                }
            });

            if ui.button("👁 Peek").clicked() {
                match self.stack.peek() {
                    Ok(value) => {
                        self.status_message = format!("Top element: {}", value);
                    }
                    Err(e) => {
                        self.status_message = format!("Error: {}", e);
                    }
                }
            }
        });

        ui.add_space(8.0);

        ui.group(|ui| {
            ui.label("Randomize:");

            ui.horizontal(|ui| {
                ui.label("Size:");
                ui.add(egui::DragValue::new(&mut self.randomize_size).clamp_range(1..=16).speed(0.1));
            });

            if ui.button("🎲 Randomize").clicked() {
                use rand::Rng;
                let mut rng = rand::thread_rng();

                self.stack = dsav_core::structures::VisualizableStack::with_capacity(16);
                for _ in 0..self.randomize_size {
                    let random_value = rng.gen_range(1..=100);
                    let _ = self.stack.push(random_value);
                }

                self.current_steps.clear();
                self.status_message = format!("Generated {} random elements", self.randomize_size);
            }
        });

        ui.add_space(8.0);

        ui.group(|ui| {
            ui.label("Clear:");

            if ui.button("🗑 Clear Stack").clicked() {
                self.stack = dsav_core::structures::VisualizableStack::with_capacity(16);
                self.current_steps.clear();
                self.status_message = "Stack cleared".to_string();
            }
        });
    }

    fn queue_controls(&mut self, ui: &mut egui::Ui) {
        ui.group(|ui| {
            ui.label("Queue Operations:");

            ui.horizontal(|ui| {
                ui.label("Value:");
                ui.add(egui::DragValue::new(&mut self.input_value).speed(1.0));
            });

            ui.horizontal(|ui| {
                if ui.button("➡ Enqueue").clicked() {
                    self.execute_queue_operation(Operation::Enqueue(self.input_value));
                }

                if ui.button("⬅ Dequeue").clicked() {
                    self.execute_queue_operation(Operation::Dequeue);
                }
            });

            if ui.button("👁 Peek Front").clicked() {
                match self.queue.peek() {
                    Ok(value) => {
                        self.status_message = format!("Front element: {}", value);
                    }
                    Err(e) => {
                        self.status_message = format!("Error: {}", e);
                    }
                }
            }
        });

        ui.add_space(8.0);

        ui.group(|ui| {
            ui.label("Randomize:");

            ui.horizontal(|ui| {
                ui.label("Size:");
                ui.add(egui::DragValue::new(&mut self.randomize_size).clamp_range(1..=16).speed(0.1));
            });

            if ui.button("🎲 Randomize").clicked() {
                use rand::Rng;
                let mut rng = rand::thread_rng();

                self.queue = dsav_core::structures::VisualizableQueue::with_capacity(16);
                for _ in 0..self.randomize_size {
                    let random_value = rng.gen_range(1..=100);
                    let _ = self.queue.enqueue(random_value);
                }

                self.current_steps.clear();
                self.status_message = format!("Generated {} random elements", self.randomize_size);
            }
        });

        ui.add_space(8.0);

        ui.group(|ui| {
            ui.label("Clear:");

            if ui.button("🗑 Clear Queue").clicked() {
                self.queue = dsav_core::structures::VisualizableQueue::with_capacity(16);
                self.current_steps.clear();
                self.status_message = "Queue cleared".to_string();
            }
        });
    }

    fn linked_list_controls(&mut self, ui: &mut egui::Ui) {
        ui.group(|ui| {
            ui.label("Insert / Delete:");

            ui.horizontal(|ui| {
                ui.label("Value:");
                ui.add(egui::DragValue::new(&mut self.input_value).speed(1.0));
            });

            ui.horizontal(|ui| {
                ui.label("Index:");
                ui.add(egui::DragValue::new(&mut self.input_index).speed(0.1));
            });

            ui.horizontal(|ui| {
                if ui.button("📥 Insert").clicked() {
                    self.execute_linked_list_operation(Operation::Insert(self.input_index, self.input_value));
                }

                if ui.button("🗑 Delete").clicked() {
                    self.execute_linked_list_operation(Operation::Delete(self.input_index));
                }
            });
        });

        ui.add_space(8.0);

        ui.group(|ui| {
            ui.label("Search / Traverse:");

            ui.horizontal(|ui| {
                ui.label("Value:");
                ui.add(egui::DragValue::new(&mut self.search_value).speed(1.0));
            });

            ui.horizontal(|ui| {
                if ui.button("🔍 Search").clicked() {
                    self.execute_linked_list_operation(Operation::Search(self.search_value));
                }

                if ui.button("🚶 Traverse").clicked() {
                    self.execute_linked_list_operation(Operation::Traverse);
                }
            });
        });

        ui.add_space(8.0);

        ui.group(|ui| {
            ui.label("Randomize:");

            ui.horizontal(|ui| {
                ui.label("Size:");
                ui.add(egui::DragValue::new(&mut self.randomize_size).clamp_range(1..=16).speed(0.1));
            });

            if ui.button("🎲 Randomize").clicked() {
                use rand::Rng;
                let mut rng = rand::thread_rng();

                self.linked_list = dsav_core::structures::VisualizableLinkedList::new();
                for _ in 0..self.randomize_size {
                    let random_value = rng.gen_range(1..=100);
                    self.linked_list.insert_back(random_value);
                }

                self.current_steps.clear();
                self.status_message = format!("Generated {} random elements", self.randomize_size);
            }
        });

        ui.add_space(8.0);

        ui.group(|ui| {
            ui.label("Clear:");

            if ui.button("🗑 Clear List").clicked() {
                self.linked_list = dsav_core::structures::VisualizableLinkedList::new();
                self.current_steps.clear();
                self.status_message = "Linked list cleared".to_string();
            }
        });
    }

    fn bst_controls(&mut self, ui: &mut egui::Ui) {
        ui.group(|ui| {
            ui.label("Insert / Search:");

            ui.horizontal(|ui| {
                ui.label("Value:");
                ui.add(egui::DragValue::new(&mut self.input_value).speed(1.0));
            });

            ui.horizontal(|ui| {
                if ui.button("📥 Insert").clicked() {
                    self.execute_bst_operation(Operation::Insert(0, self.input_value));
                }

                if ui.button("🔍 Search").clicked() {
                    self.execute_bst_operation(Operation::Search(self.input_value));
                }
            });
        });

        ui.add_space(8.0);

        ui.group(|ui| {
            ui.label("Traverse:");

            ui.horizontal(|ui| {
                if ui.button("🚶 In-Order").clicked() {
                    self.execute_bst_operation(Operation::Traverse);
                }

                if ui.button("📍 Pre-Order").clicked() {
                    self.execute_bst_operation(Operation::PreOrderTraverse);
                }
            });

            ui.horizontal(|ui| {
                if ui.button("📌 Post-Order").clicked() {
                    self.execute_bst_operation(Operation::PostOrderTraverse);
                }

                if ui.button("📊 Level-Order").clicked() {
                    self.execute_bst_operation(Operation::LevelOrderTraverse);
                }
            });
        });

        ui.add_space(8.0);

        ui.group(|ui| {
            ui.label("Randomize:");

            ui.horizontal(|ui| {
                ui.label("Size:");
                ui.add(egui::DragValue::new(&mut self.randomize_size).clamp_range(1..=16).speed(0.1));
            });

            if ui.button("🎲 Randomize").clicked() {
                use rand::Rng;
                let mut rng = rand::thread_rng();

                self.bst.clear();
                for _ in 0..self.randomize_size {
                    let random_value = rng.gen_range(1..=100);
                    self.bst.insert(random_value);
                }

                self.current_steps.clear();
                self.status_message = format!("Generated {} random elements", self.randomize_size);
            }
        });

        ui.add_space(8.0);

        ui.group(|ui| {
            ui.label("Clear:");

            if ui.button("🗑 Clear Tree").clicked() {
                self.bst.clear();
                self.current_steps.clear();
                self.status_message = "Binary Search Tree cleared".to_string();
            }
        });
    }

    fn execute_array_operation(&mut self, operation: Operation) {
        match self.array.execute_with_steps(operation) {
            Ok(steps) => {
                if !steps.is_empty() {
                    self.current_steps = steps;
                    self.current_step_index = 0;
                    self.playing = true;
                    self.time_since_last_step = 0.0;
                    if let Some(step) = self.current_steps.first() {
                        self.status_message = step.description.clone();
                    }
                }
            }
            Err(e) => {
                self.status_message = format!("Error: {}", e);
                self.current_steps.clear();
                self.playing = false;
            }
        }
    }

    fn execute_stack_operation(&mut self, operation: Operation) {
        match self.stack.execute_with_steps(operation) {
            Ok(steps) => {
                if !steps.is_empty() {
                    self.current_steps = steps;
                    self.current_step_index = 0;
                    self.playing = true;
                    self.time_since_last_step = 0.0;
                    if let Some(step) = self.current_steps.first() {
                        self.status_message = step.description.clone();
                    }
                }
            }
            Err(e) => {
                self.status_message = format!("Error: {}", e);
                self.current_steps.clear();
                self.playing = false;
            }
        }
    }

    fn execute_queue_operation(&mut self, operation: Operation) {
        match self.queue.execute_with_steps(operation) {
            Ok(steps) => {
                if !steps.is_empty() {
                    self.current_steps = steps;
                    self.current_step_index = 0;
                    self.playing = true;
                    self.time_since_last_step = 0.0;
                    if let Some(step) = self.current_steps.first() {
                        self.status_message = step.description.clone();
                    }
                }
            }
            Err(e) => {
                self.status_message = format!("Error: {}", e);
                self.current_steps.clear();
                self.playing = false;
            }
        }
    }

    fn execute_linked_list_operation(&mut self, operation: Operation) {
        match self.linked_list.execute_with_steps(operation) {
            Ok(steps) => {
                if !steps.is_empty() {
                    self.current_steps = steps;
                    self.current_step_index = 0;
                    self.playing = true;
                    self.time_since_last_step = 0.0;
                    if let Some(step) = self.current_steps.first() {
                        self.status_message = step.description.clone();
                    }
                }
            }
            Err(e) => {
                self.status_message = format!("Error: {}", e);
                self.current_steps.clear();
                self.playing = false;
            }
        }
    }

    fn execute_bst_operation(&mut self, operation: Operation) {
        match self.bst.execute_with_steps(operation) {
            Ok(steps) => {
                if !steps.is_empty() {
                    self.current_steps = steps;
                    self.current_step_index = 0;
                    self.playing = true;
                    self.time_since_last_step = 0.0;
                    if let Some(step) = self.current_steps.first() {
                        self.status_message = step.description.clone();
                    }
                }
            }
            Err(e) => {
                self.status_message = format!("Error: {}", e);
                self.current_steps.clear();
                self.playing = false;
            }
        }
    }

    fn render_array(&self, ui: &mut egui::Ui) {
        let palette = self.current_theme.colors();
        let mut state = self.array.render_state();

        // Check if we have array state from current step (for sorting animations)
        if !self.current_steps.is_empty() && self.current_step_index < self.current_steps.len() {
            let current_step = &self.current_steps[self.current_step_index];

            // If step contains array_state in metadata, use that instead
            if let Some(array_state) = current_step.metadata.get("array_state") {
                if let Some(arr) = array_state.as_array() {
                    state.elements.clear();
                    for (i, val) in arr.iter().enumerate() {
                        if let Some(num) = val.as_i64() {
                            state.elements.push(
                                dsav_core::state::RenderElement::new(num as i32)
                                    .with_label(num.to_string())
                                    .with_sublabel(format!("[{}]", i))
                            );
                        }
                    }
                }
            }

            // Apply highlights
            for &idx in &current_step.highlight_indices {
                if idx < state.elements.len() {
                    state.elements[idx].state = dsav_core::state::ElementState::Highlighted;
                }
            }

            for &idx in &current_step.active_indices {
                if idx < state.elements.len() {
                    state.elements[idx].state = dsav_core::state::ElementState::Active;
                }
            }
        }

        ui.horizontal(|ui| {
            ui.add_space(16.0);

            for (i, elem) in state.elements.iter().enumerate() {
                let (bg_color, border_color) = self.get_element_colors(elem.state);

                let size = egui::vec2(60.0, 60.0);
                let (rect, _response) = ui.allocate_exact_size(size, egui::Sense::hover());

                ui.painter().rect(
                    rect,
                    4.0,
                    bg_color,
                    egui::Stroke::new(2.0, border_color),
                );

                ui.painter().text(
                    rect.center(),
                    egui::Align2::CENTER_CENTER,
                    &elem.label,
                    egui::FontId::proportional(20.0),
                    palette.text,
                );

                ui.painter().text(
                    egui::pos2(rect.center().x, rect.bottom() + 8.0),
                    egui::Align2::CENTER_TOP,
                    format!("[{}]", i),
                    egui::FontId::proportional(14.0),
                    palette.subtext,
                );

                ui.add_space(8.0);
            }
        });
    }

    fn render_stack(&self, ui: &mut egui::Ui) {
        let palette = self.current_theme.colors();
        let mut state = self.stack.render_state();

        // Apply current step highlights
        if !self.current_steps.is_empty() && self.current_step_index < self.current_steps.len() {
            let current_step = &self.current_steps[self.current_step_index];

            for &idx in &current_step.highlight_indices {
                if idx < state.elements.len() {
                    state.elements[idx].state = dsav_core::state::ElementState::Highlighted;
                }
            }

            for &idx in &current_step.active_indices {
                if idx < state.elements.len() {
                    state.elements[idx].state = dsav_core::state::ElementState::Active;
                }
            }
        }

        // Add scrollable area with fixed height
        egui::ScrollArea::vertical()
            .max_height(500.0)
            .auto_shrink([false, false])
            .show(ui, |ui| {
                ui.vertical(|ui| {
                    ui.add_space(16.0);

                    if state.elements.is_empty() {
                        ui.vertical_centered(|ui| {
                            ui.label("Stack is empty");
                        });
                    } else {
                        for (i, elem) in state.elements.iter().enumerate().rev() {
                            ui.horizontal(|ui| {
                                ui.add_space(16.0);

                                let (bg_color, border_color) = self.get_element_colors(elem.state);

                                let size = egui::vec2(200.0, 50.0);
                                let (rect, _response) = ui.allocate_exact_size(size, egui::Sense::hover());

                                ui.painter().rect(
                                    rect,
                                    4.0,
                                    bg_color,
                                    egui::Stroke::new(2.0, border_color),
                                );

                                ui.painter().text(
                                    rect.center(),
                                    egui::Align2::CENTER_CENTER,
                                    &elem.label,
                                    egui::FontId::proportional(18.0),
                                    palette.text,
                                );

                                ui.label(if i == state.elements.len() - 1 {
                                    "← TOP"
                                } else {
                                    ""
                                });
                            });

                            ui.add_space(4.0);
                        }
                    }
                });
            });
    }

    fn render_queue(&self, ui: &mut egui::Ui) {
        let palette = self.current_theme.colors();
        let mut state = self.queue.render_state();

        // Apply current step highlights
        if !self.current_steps.is_empty() && self.current_step_index < self.current_steps.len() {
            let current_step = &self.current_steps[self.current_step_index];

            for &idx in &current_step.highlight_indices {
                if idx < state.elements.len() {
                    state.elements[idx].state = dsav_core::state::ElementState::Highlighted;
                }
            }

            for &idx in &current_step.active_indices {
                if idx < state.elements.len() {
                    state.elements[idx].state = dsav_core::state::ElementState::Active;
                }
            }
        }

        if state.elements.is_empty() {
            ui.vertical_centered(|ui| {
                ui.add_space(50.0);
                ui.label("Queue is empty. Click 'Enqueue' to add elements.");
                ui.add_space(50.0);
            });
            return;
        }

        ui.add_space(20.0);

        // Add horizontal scrolling for queue
        egui::ScrollArea::horizontal()
            .auto_shrink([false, false])
            .show(ui, |ui| {
                ui.horizontal(|ui| {
                    ui.add_space(20.0);

                    // FRONT label
                    ui.label("FRONT ↓");
                    ui.add_space(16.0);

                    for (i, elem) in state.elements.iter().enumerate() {
                        let (bg_color, border_color) = self.get_element_colors(elem.state);

                        ui.vertical(|ui| {
                            let size = egui::vec2(70.0, 70.0);
                            let (rect, _response) = ui.allocate_exact_size(size, egui::Sense::hover());

                            ui.painter().rect(
                                rect,
                                6.0,
                                bg_color,
                                egui::Stroke::new(3.0, border_color),
                            );

                            ui.painter().text(
                                rect.center(),
                                egui::Align2::CENTER_CENTER,
                                elem.value.to_string(),
                                egui::FontId::monospace(24.0),
                                palette.text,
                            );

                            ui.add_space(8.0);
                            ui.label(format!("Index {}", i));
                        });

                        if i < state.elements.len() - 1 {
                            ui.add_space(4.0);
                            ui.label("→");
                            ui.add_space(4.0);
                        }
                    }

                    ui.add_space(16.0);
                    ui.label("↑ BACK");
                });
            });
    }

    fn render_linked_list(&self, ui: &mut egui::Ui) {
        let palette = self.current_theme.colors();
        let mut state = self.linked_list.render_state();

        // Early return if empty
        if state.elements.is_empty() {
            ui.vertical_centered(|ui| {
                ui.add_space(50.0);
                ui.label("Linked list is empty. Click 'Insert Front' or 'Insert Back' to add nodes.");
                ui.add_space(50.0);
            });
            return;
        }

        // Apply current step highlights
        if !self.current_steps.is_empty() && self.current_step_index < self.current_steps.len() {
            let current_step = &self.current_steps[self.current_step_index];

            for &idx in &current_step.highlight_indices {
                if idx < state.elements.len() {
                    state.elements[idx].state = dsav_core::state::ElementState::Highlighted;
                }
            }

            for &idx in &current_step.active_indices {
                if idx < state.elements.len() {
                    state.elements[idx].state = dsav_core::state::ElementState::Active;
                }
            }
        }

        ui.add_space(20.0);

        // Add horizontal scrolling for linked list
        egui::ScrollArea::horizontal()
            .auto_shrink([false, false])
            .show(ui, |ui| {
                ui.horizontal(|ui| {
                    ui.add_space(20.0);

                    // HEAD label
                    ui.vertical(|ui| {
                        ui.add_space(30.0);
                        ui.label("HEAD ↓");
                    });

                    ui.add_space(16.0);

                    for (i, elem) in state.elements.iter().enumerate() {
                        let (bg_color, border_color) = self.get_element_colors(elem.state);

                        ui.vertical(|ui| {
                            let size = egui::vec2(80.0, 80.0);
                            let (rect, _response) = ui.allocate_exact_size(size, egui::Sense::hover());

                            // Draw node box
                            ui.painter().rect(
                                rect,
                                6.0,
                                bg_color,
                                egui::Stroke::new(3.0, border_color),
                            );

                            // Draw value using monospace font
                            ui.painter().text(
                                rect.center(),
                                egui::Align2::CENTER_CENTER,
                                elem.value.to_string(),
                                egui::FontId::monospace(26.0),
                                palette.text,
                            );

                            // Draw node index below
                            ui.add_space(8.0);
                            ui.label(format!("Node {}", i));
                        });

                        // Draw arrow to next node
                        if i < state.elements.len() - 1 {
                            ui.add_space(4.0);
                            ui.vertical(|ui| {
                                ui.add_space(30.0);
                                ui.label("→");
                            });
                            ui.add_space(4.0);
                        }
                    }

                    ui.add_space(16.0);

                    // NULL/TAIL label
                    ui.vertical(|ui| {
                        ui.add_space(30.0);
                        ui.label("↓ NULL");
                    });
                });
            });
    }

    fn render_bst(&self, ui: &mut egui::Ui) {
        let palette = self.current_theme.colors();
        let mut state = self.bst.render_state();

        // Early return if empty
        if state.elements.is_empty() {
            ui.vertical_centered(|ui| {
                ui.add_space(50.0);
                ui.label("Binary Search Tree is empty. Click 'Insert' to add nodes.");
                ui.add_space(50.0);
            });
            return;
        }

        // Apply current step highlights
        if !self.current_steps.is_empty() && self.current_step_index < self.current_steps.len() {
            let current_step = &self.current_steps[self.current_step_index];

            for &idx in &current_step.highlight_indices {
                if idx < state.elements.len() {
                    state.elements[idx].state = dsav_core::state::ElementState::Highlighted;
                }
            }

            for &idx in &current_step.active_indices {
                if idx < state.elements.len() {
                    state.elements[idx].state = dsav_core::state::ElementState::Active;
                }
            }
        }

        // Calculate tree layout positions
        let node_radius = 25.0;
        let level_height = 100.0;
        let min_horizontal_spacing = 80.0;

        // Find tree depth
        let max_depth = self.calculate_tree_depth(&state);

        // Calculate positions for each node
        let mut positions = std::collections::HashMap::new();
        self.calculate_node_positions(
            0,
            0,
            0.0,
            1000.0,
            level_height,
            &state,
            &mut positions,
        );

        // Calculate required canvas size
        let mut max_x = 0.0f32;
        let mut max_y = 0.0f32;
        for &(x, y) in positions.values() {
            max_x = max_x.max(x);
            max_y = max_y.max(y);
        }

        let canvas_width = (max_x + 100.0).max(600.0);
        let canvas_height = (max_y + 100.0).max(400.0);

        // Create scrollable area for the tree
        egui::ScrollArea::both()
            .auto_shrink([false, false])
            .show(ui, |ui| {
                let (response, painter) = ui.allocate_painter(
                    egui::vec2(canvas_width, canvas_height),
                    egui::Sense::hover(),
                );

                let to_screen = |pos: egui::Pos2| response.rect.min + pos.to_vec2();

                // Draw connections first (under nodes)
                for &(parent_idx, child_idx) in &state.connections {
                    if let (Some(&parent_pos), Some(&child_pos)) =
                        (positions.get(&parent_idx), positions.get(&child_idx)) {
                        let start = to_screen(egui::pos2(parent_pos.0, parent_pos.1 + node_radius));
                        let end = to_screen(egui::pos2(child_pos.0, child_pos.1 - node_radius));

                        painter.line_segment(
                            [start, end],
                            egui::Stroke::new(2.0, palette.overlay),
                        );
                    }
                }

                // Draw nodes on top
                for (i, elem) in state.elements.iter().enumerate() {
                    if elem.label.is_empty() {
                        continue; // Skip empty slots
                    }

                    if let Some(&(x, y)) = positions.get(&i) {
                        let center = to_screen(egui::pos2(x, y));
                        let (bg_color, border_color) = self.get_element_colors(elem.state);

                        // Draw node circle
                        painter.circle(
                            center,
                            node_radius,
                            bg_color,
                            egui::Stroke::new(3.0, border_color),
                        );

                        // Draw value
                        painter.text(
                            center,
                            egui::Align2::CENTER_CENTER,
                            elem.value.to_string(),
                            egui::FontId::monospace(18.0),
                            palette.text,
                        );
                    }
                }
            });
    }

    // Calculate maximum depth of the tree
    fn calculate_tree_depth(&self, state: &dsav_core::state::RenderState) -> usize {
        let mut max_depth = 0;
        for i in 0..state.elements.len() {
            if !state.elements[i].label.is_empty() {
                let depth = (i as f32 + 1.0).log2().floor() as usize;
                max_depth = max_depth.max(depth);
            }
        }
        max_depth
    }

    // Recursively calculate positions for nodes in the tree
    fn calculate_node_positions(
        &self,
        idx: usize,
        depth: usize,
        left_bound: f32,
        right_bound: f32,
        level_height: f32,
        state: &dsav_core::state::RenderState,
        positions: &mut std::collections::HashMap<usize, (f32, f32)>,
    ) {
        if idx >= state.elements.len() || state.elements[idx].label.is_empty() {
            return;
        }

        let x = (left_bound + right_bound) / 2.0;
        let y = 50.0 + depth as f32 * level_height;
        positions.insert(idx, (x, y));

        let mid = (left_bound + right_bound) / 2.0;

        // Calculate left child position
        let left_child_idx = idx * 2 + 1;
        if left_child_idx < state.elements.len() && !state.elements[left_child_idx].label.is_empty() {
            self.calculate_node_positions(
                left_child_idx,
                depth + 1,
                left_bound,
                mid,
                level_height,
                state,
                positions,
            );
        }

        // Calculate right child position
        let right_child_idx = idx * 2 + 2;
        if right_child_idx < state.elements.len() && !state.elements[right_child_idx].label.is_empty() {
            self.calculate_node_positions(
                right_child_idx,
                depth + 1,
                mid,
                right_bound,
                level_height,
                state,
                positions,
            );
        }
    }

    fn render_animation_controls(&mut self, ui: &mut egui::Ui) {
        ui.label("Animation Controls:");
        ui.add_space(4.0);

        ui.horizontal(|ui| {
            if ui.button("⏮").clicked() {
                self.current_step_index = 0;
                self.playing = false;
                self.time_since_last_step = 0.0;
                if let Some(step) = self.current_steps.first() {
                    self.status_message = step.description.clone();
                }
            }

            if self.playing {
                if ui.button("⏸").clicked() {
                    self.playing = false;
                }
            } else {
                if ui.button("▶").clicked() {
                    self.playing = true;
                }
            }

            if ui.button("⏭").clicked() {
                if self.current_step_index < self.current_steps.len() - 1 {
                    self.current_step_index += 1;
                    self.playing = false;
                    self.time_since_last_step = 0.0;
                    if let Some(step) = self.current_steps.get(self.current_step_index) {
                        self.status_message = step.description.clone();
                    }
                }
            }
        });

        ui.add_space(8.0);

        ui.horizontal(|ui| {
            if ui.button("⏪ Step Back").clicked() {
                if self.current_step_index > 0 {
                    self.current_step_index -= 1;
                    self.playing = false;
                    self.time_since_last_step = 0.0;
                    if let Some(step) = self.current_steps.get(self.current_step_index) {
                        self.status_message = step.description.clone();
                    }
                }
            }

            if ui.button("⏩ Step Forward").clicked() {
                if self.current_step_index < self.current_steps.len() - 1 {
                    self.current_step_index += 1;
                    self.playing = false;
                    self.time_since_last_step = 0.0;
                    if let Some(step) = self.current_steps.get(self.current_step_index) {
                        self.status_message = step.description.clone();
                    }
                }
            }
        });

        ui.add_space(8.0);

        ui.horizontal(|ui| {
            ui.label("Speed:");
            if ui.add(egui::Slider::new(&mut self.animation_speed, 0.25..=4.0)
                .text("x")
                .logarithmic(true)).changed() {
            }
        });

        ui.add_space(4.0);

        ui.label(format!("Step {} / {}",
            self.current_step_index + 1,
            self.current_steps.len()
        ));

        let progress = if self.current_steps.is_empty() {
            0.0
        } else {
            (self.current_step_index + 1) as f32 / self.current_steps.len() as f32
        };

        let progress_bar = egui::ProgressBar::new(progress)
            .show_percentage()
            .animate(self.playing);

        ui.add(progress_bar);
    }

    fn render_settings(&mut self, ctx: &egui::Context, palette: &ColorPalette) {
        egui::Window::new("⚙ Settings")
            .collapsible(false)
            .resizable(false)
            .default_width(300.0)
            .show(ctx, |ui| {
                ui.add_space(8.0);

                ui.heading("Appearance");
                ui.separator();
                ui.add_space(8.0);

                ui.label("Select Theme:");
                ui.add_space(4.0);

                for theme in Theme::all() {
                    let is_selected = *theme == self.current_theme;

                    if ui.selectable_label(is_selected, theme.name()).clicked() {
                        self.current_theme = *theme;
                    }
                }

                ui.add_space(16.0);

                ui.label("Preview:");
                ui.add_space(4.0);

                // Draw color preview boxes
                ui.horizontal(|ui| {
                    let box_size = egui::vec2(32.0, 32.0);

                    ui.vertical(|ui| {
                        let (rect, _) = ui.allocate_exact_size(box_size, egui::Sense::hover());
                        ui.painter().rect_filled(rect, 4.0, palette.blue);
                        ui.label("Primary");
                    });

                    ui.add_space(8.0);

                    ui.vertical(|ui| {
                        let (rect, _) = ui.allocate_exact_size(box_size, egui::Sense::hover());
                        ui.painter().rect_filled(rect, 4.0, palette.green);
                        ui.label("Success");
                    });

                    ui.add_space(8.0);

                    ui.vertical(|ui| {
                        let (rect, _) = ui.allocate_exact_size(box_size, egui::Sense::hover());
                        ui.painter().rect_filled(rect, 4.0, palette.yellow);
                        ui.label("Accent");
                    });
                });

                ui.add_space(16.0);

                if ui.button("Close").clicked() {
                    self.show_settings = false;
                }

                ui.add_space(8.0);
            });
    }

    fn get_element_colors(&self, state: dsav_core::state::ElementState) -> (egui::Color32, egui::Color32) {
        use dsav_core::state::ElementState;
        let palette = self.current_theme.colors();

        match state {
            ElementState::Normal => (palette.surface, palette.blue),
            ElementState::Highlighted => (palette.surface, palette.yellow),
            ElementState::Active => (palette.green.gamma_multiply(0.3), palette.green),  // Green for found/active
            ElementState::Sorted => (palette.green.gamma_multiply(0.3), palette.green),
            ElementState::Comparing => (palette.yellow.gamma_multiply(0.3), palette.yellow),
            ElementState::Swapping => (palette.peach.gamma_multiply(0.3), palette.peach),
        }
    }
}

impl Default for DsavApp {
    fn default() -> Self {
        Self::new()
    }
}

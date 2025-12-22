//! DSAV GUI Application - Data Structures & Algorithms Visualizer

use glutin::config::ConfigTemplateBuilder;
use glutin::context::{ContextApi, ContextAttributesBuilder, PossiblyCurrentContext};
use glutin::display::GetGlDisplay;
use glutin::prelude::*;
use glutin::surface::{Surface, SurfaceAttributesBuilder, WindowSurface};
use glutin_winit::DisplayBuilder;
use raw_window_handle::HasWindowHandle;
use std::num::NonZeroU32;
use winit::application::ApplicationHandler;
use winit::event::WindowEvent;
use winit::event_loop::{ActiveEventLoop, EventLoop};
use winit::window::{Window, WindowId};

mod app;
mod colors;
mod renderer;

use app::DsavApp;

struct DsavApplication {
    window: Option<Window>,
    gl_config: Option<glutin::config::Config>,
    gl_surface: Option<Surface<WindowSurface>>,
    gl_context: Option<PossiblyCurrentContext>,
    gl: Option<std::sync::Arc<glow::Context>>,
    egui_glow: Option<egui_glow::winit::EguiGlow>,
    app: DsavApp,
}

impl DsavApplication {
    fn new() -> Self {
        Self {
            window: None,
            gl_config: None,
            gl_surface: None,
            gl_context: None,
            gl: None,
            egui_glow: None,
            app: DsavApp::new(),
        }
    }
}

impl ApplicationHandler for DsavApplication {
    fn resumed(&mut self, event_loop: &ActiveEventLoop) {
        if self.window.is_some() {
            return;
        }

        let window_attributes = Window::default_attributes()
            .with_title("DSAV - Data Structures & Algorithms Visualizer")
            .with_inner_size(winit::dpi::PhysicalSize::new(1280u32, 720u32))
            .with_resizable(true);

        let template = ConfigTemplateBuilder::new()
            .with_alpha_size(8)
            .with_transparency(false);

        let display_builder = DisplayBuilder::new().with_window_attributes(Some(window_attributes));

        let (window, gl_config) = display_builder
            .build(event_loop, template, |configs| {
                configs
                    .reduce(|accum, config| {
                        if config.num_samples() > accum.num_samples() {
                            config
                        } else {
                            accum
                        }
                    })
                    .unwrap()
            })
            .expect("Failed to create window");

        let window = window.expect("Failed to get window");

        let gl_display = gl_config.display();

        let context_attributes = ContextAttributesBuilder::new()
            .with_context_api(ContextApi::OpenGl(Some(glutin::context::Version::new(3, 3))))
            .build(Some(
                window
                    .window_handle()
                    .expect("Failed to get window handle")
                    .as_raw(),
            ));

        let gl_context = unsafe {
            gl_display
                .create_context(&gl_config, &context_attributes)
                .expect("Failed to create GL context")
        };

        let (width, height) = {
            let size = window.inner_size();
            (size.width, size.height)
        };

        let surface_attributes = SurfaceAttributesBuilder::<WindowSurface>::new().build(
            window
                .window_handle()
                .expect("Failed to get window handle")
                .as_raw(),
            NonZeroU32::new(width).unwrap(),
            NonZeroU32::new(height).unwrap(),
        );

        let gl_surface = unsafe {
            gl_display
                .create_window_surface(&gl_config, &surface_attributes)
                .expect("Failed to create window surface")
        };

        let gl_context = gl_context
            .make_current(&gl_surface)
            .expect("Failed to make context current");

        let gl = unsafe {
            glow::Context::from_loader_function(|s| {
                gl_display.get_proc_address(&std::ffi::CString::new(s).unwrap())
            })
        };

        let gl = std::sync::Arc::new(gl);

        let egui_glow = egui_glow::winit::EguiGlow::new(event_loop, gl.clone(), None, None, true);

        self.window = Some(window);
        self.gl_config = Some(gl_config);
        self.gl_surface = Some(gl_surface);
        self.gl_context = Some(gl_context);
        self.gl = Some(gl);
        self.egui_glow = Some(egui_glow);
    }

    fn window_event(&mut self, event_loop: &ActiveEventLoop, _window_id: WindowId, event: WindowEvent) {
        let window = match &self.window {
            Some(w) => w,
            None => return,
        };

        if let Some(ref mut egui_glow) = self.egui_glow {
            if egui_glow.on_window_event(window, &event).consumed {
                return;
            }
        }

        match event {
            WindowEvent::CloseRequested => {
                self.gl_context.take();
                self.gl_surface.take();
                event_loop.exit();
            }
            WindowEvent::Resized(size) => {
                if size.width > 0 && size.height > 0 {
                    if let (Some(context), Some(surface)) =
                        (self.gl_context.as_ref(), self.gl_surface.as_ref())
                    {
                        surface.resize(
                            context,
                            NonZeroU32::new(size.width).unwrap(),
                            NonZeroU32::new(size.height).unwrap(),
                        );
                    }
                }
            }
            WindowEvent::RedrawRequested => {
                if let (Some(window), Some(gl), Some(egui_glow), Some(context), Some(surface)) = (
                    &self.window,
                    &self.gl,
                    &mut self.egui_glow,
                    &self.gl_context,
                    &self.gl_surface,
                ) {
                    egui_glow.run(window, |egui_ctx| {
                        self.app.ui(egui_ctx);
                    });

                    unsafe {
                        use glow::HasContext as _;
                        gl.clear_color(0.118, 0.118, 0.180, 1.0);
                        gl.clear(glow::COLOR_BUFFER_BIT);
                    }

                    egui_glow.paint(window);

                    surface.swap_buffers(context).unwrap();
                    window.request_redraw();
                }
            }
            _ => {}
        }
    }

    fn about_to_wait(&mut self, _event_loop: &ActiveEventLoop) {
        if let Some(window) = &self.window {
            window.request_redraw();
        }
    }
}

fn main() {
    tracing_subscriber::fmt::init();

    let event_loop = EventLoop::new().expect("Failed to create event loop");
    let mut app = DsavApplication::new();

    event_loop.run_app(&mut app).expect("Event loop error");
}

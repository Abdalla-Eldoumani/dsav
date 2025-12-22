//! Color palettes and theming for DSAV GUI.

use egui::Color32;

#[derive(Debug, Clone, Copy, PartialEq, Eq)]
pub enum Theme {
    CatppuccinMocha,
    CatppuccinLatte,
    HighContrast,
    Vibrant,
    Nord,
    Dracula,
    GruvboxDark,
    TokyoNight,
    SolarizedDark,
    OneDark,
}

impl Theme {
    pub fn all() -> &'static [Theme] {
        &[
            Theme::Vibrant,
            Theme::TokyoNight,
            Theme::Dracula,
            Theme::GruvboxDark,
            Theme::OneDark,
            Theme::Nord,
            Theme::SolarizedDark,
            Theme::CatppuccinMocha,
            Theme::CatppuccinLatte,
            Theme::HighContrast,
        ]
    }

    pub fn name(&self) -> &'static str {
        match self {
            Theme::CatppuccinMocha => "🌙 Mocha (Dark)",
            Theme::CatppuccinLatte => "☀️ Latte (Light)",
            Theme::HighContrast => "⚡ High Contrast",
            Theme::Vibrant => "🎨 Vibrant (Default)",
            Theme::Nord => "❄️ Nord (Cool)",
            Theme::Dracula => "🧛 Dracula (Purple)",
            Theme::GruvboxDark => "🍂 Gruvbox (Retro)",
            Theme::TokyoNight => "🌃 Tokyo Night (Modern)",
            Theme::SolarizedDark => "☯️ Solarized Dark (Classic)",
            Theme::OneDark => "🌑 One Dark (Atom)",
        }
    }

    pub fn colors(&self) -> ColorPalette {
        match self {
            Theme::CatppuccinMocha => ColorPalette::mocha(),
            Theme::CatppuccinLatte => ColorPalette::latte(),
            Theme::HighContrast => ColorPalette::high_contrast(),
            Theme::Vibrant => ColorPalette::vibrant(),
            Theme::Nord => ColorPalette::nord(),
            Theme::Dracula => ColorPalette::dracula(),
            Theme::GruvboxDark => ColorPalette::gruvbox_dark(),
            Theme::TokyoNight => ColorPalette::tokyo_night(),
            Theme::SolarizedDark => ColorPalette::solarized_dark(),
            Theme::OneDark => ColorPalette::one_dark(),
        }
    }
}

#[derive(Debug, Clone, Copy)]
pub struct ColorPalette {
    pub background: Color32,
    pub surface: Color32,
    pub overlay: Color32,
    pub blue: Color32,
    pub green: Color32,
    pub yellow: Color32,
    pub peach: Color32,
    pub red: Color32,
    pub mauve: Color32,
    pub teal: Color32,
    pub text: Color32,
    pub subtext: Color32,
}

impl ColorPalette {
    pub fn mocha() -> Self {
        Self {
            background: Color32::from_rgb(30, 30, 46),
            surface: Color32::from_rgb(49, 50, 68),
            overlay: Color32::from_rgb(69, 71, 90),
            blue: Color32::from_rgb(137, 180, 250),
            green: Color32::from_rgb(166, 227, 161),
            yellow: Color32::from_rgb(249, 226, 175),
            peach: Color32::from_rgb(250, 179, 135),
            red: Color32::from_rgb(243, 139, 168),
            mauve: Color32::from_rgb(203, 166, 247),
            teal: Color32::from_rgb(148, 226, 213),
            text: Color32::from_rgb(205, 214, 244),
            subtext: Color32::from_rgb(108, 112, 134),
        }
    }

    pub fn latte() -> Self {
        Self {
            background: Color32::from_rgb(239, 241, 245),
            surface: Color32::from_rgb(230, 233, 239),
            overlay: Color32::from_rgb(220, 224, 232),
            blue: Color32::from_rgb(30, 102, 245),
            green: Color32::from_rgb(64, 160, 43),
            yellow: Color32::from_rgb(223, 142, 29),
            peach: Color32::from_rgb(254, 100, 11),
            red: Color32::from_rgb(210, 15, 57),
            mauve: Color32::from_rgb(136, 57, 239),
            teal: Color32::from_rgb(23, 146, 153),
            text: Color32::from_rgb(76, 79, 105),
            subtext: Color32::from_rgb(108, 111, 133),
        }
    }

    pub fn high_contrast() -> Self {
        Self {
            background: Color32::from_rgb(0, 0, 0),
            surface: Color32::from_rgb(20, 20, 20),
            overlay: Color32::from_rgb(40, 40, 40),
            blue: Color32::from_rgb(100, 200, 255),
            green: Color32::from_rgb(100, 255, 100),
            yellow: Color32::from_rgb(255, 255, 100),
            peach: Color32::from_rgb(255, 180, 100),
            red: Color32::from_rgb(255, 100, 100),
            mauve: Color32::from_rgb(220, 150, 255),
            teal: Color32::from_rgb(100, 255, 255),
            text: Color32::from_rgb(255, 255, 255),
            subtext: Color32::from_rgb(180, 180, 180),
        }
    }

    pub fn vibrant() -> Self {
        Self {
            background: Color32::from_rgb(15, 15, 30),
            surface: Color32::from_rgb(30, 30, 60),
            overlay: Color32::from_rgb(45, 45, 75),
            blue: Color32::from_rgb(100, 150, 255),
            green: Color32::from_rgb(100, 255, 150),
            yellow: Color32::from_rgb(255, 230, 100),
            peach: Color32::from_rgb(255, 150, 100),
            red: Color32::from_rgb(255, 100, 150),
            mauve: Color32::from_rgb(200, 150, 255),
            teal: Color32::from_rgb(100, 230, 230),
            text: Color32::from_rgb(240, 240, 255),
            subtext: Color32::from_rgb(150, 150, 180),
        }
    }

    pub fn nord() -> Self {
        Self {
            background: Color32::from_rgb(46, 52, 64),      // Nord0
            surface: Color32::from_rgb(59, 66, 82),         // Nord1
            overlay: Color32::from_rgb(67, 76, 94),         // Nord2
            blue: Color32::from_rgb(136, 192, 208),         // Nord8 (Frost)
            green: Color32::from_rgb(163, 190, 140),        // Nord14 (Aurora)
            yellow: Color32::from_rgb(235, 203, 139),       // Nord13 (Aurora)
            peach: Color32::from_rgb(208, 135, 112),        // Nord12 (Aurora)
            red: Color32::from_rgb(191, 97, 106),           // Nord11 (Aurora)
            mauve: Color32::from_rgb(180, 142, 173),        // Nord15 (Aurora)
            teal: Color32::from_rgb(143, 188, 187),         // Nord7 (Frost)
            text: Color32::from_rgb(236, 239, 244),         // Nord6
            subtext: Color32::from_rgb(216, 222, 233),      // Nord4
        }
    }

    pub fn dracula() -> Self {
        Self {
            background: Color32::from_rgb(40, 42, 54),      // Dracula Background
            surface: Color32::from_rgb(68, 71, 90),         // Dracula Current Line
            overlay: Color32::from_rgb(98, 114, 164),       // Dracula Comment
            blue: Color32::from_rgb(139, 233, 253),         // Dracula Cyan
            green: Color32::from_rgb(80, 250, 123),         // Dracula Green
            yellow: Color32::from_rgb(241, 250, 140),       // Dracula Yellow
            peach: Color32::from_rgb(255, 184, 108),        // Dracula Orange
            red: Color32::from_rgb(255, 85, 85),            // Dracula Red
            mauve: Color32::from_rgb(189, 147, 249),        // Dracula Purple
            teal: Color32::from_rgb(139, 233, 253),         // Dracula Cyan (same as blue)
            text: Color32::from_rgb(248, 248, 242),         // Dracula Foreground
            subtext: Color32::from_rgb(98, 114, 164),       // Dracula Comment
        }
    }

    pub fn gruvbox_dark() -> Self {
        Self {
            background: Color32::from_rgb(40, 40, 40),      // Gruvbox dark bg
            surface: Color32::from_rgb(60, 56, 54),         // Gruvbox dark bg1
            overlay: Color32::from_rgb(80, 73, 69),         // Gruvbox dark bg2
            blue: Color32::from_rgb(131, 165, 152),         // Gruvbox aqua
            green: Color32::from_rgb(184, 187, 38),         // Gruvbox green
            yellow: Color32::from_rgb(250, 189, 47),        // Gruvbox yellow
            peach: Color32::from_rgb(254, 128, 25),         // Gruvbox orange
            red: Color32::from_rgb(251, 73, 52),            // Gruvbox red
            mauve: Color32::from_rgb(211, 134, 155),        // Gruvbox purple
            teal: Color32::from_rgb(142, 192, 124),         // Gruvbox aqua light
            text: Color32::from_rgb(235, 219, 178),         // Gruvbox fg
            subtext: Color32::from_rgb(168, 153, 132),      // Gruvbox fg4
        }
    }

    pub fn tokyo_night() -> Self {
        Self {
            background: Color32::from_rgb(26, 27, 38),      // Tokyo Night bg
            surface: Color32::from_rgb(36, 40, 59),         // Tokyo Night bg dark
            overlay: Color32::from_rgb(54, 61, 88),         // Tokyo Night bg highlight
            blue: Color32::from_rgb(125, 207, 255),         // Tokyo Night blue
            green: Color32::from_rgb(158, 206, 106),        // Tokyo Night green
            yellow: Color32::from_rgb(224, 175, 104),       // Tokyo Night yellow
            peach: Color32::from_rgb(255, 158, 100),        // Tokyo Night orange
            red: Color32::from_rgb(247, 118, 142),          // Tokyo Night red
            mauve: Color32::from_rgb(187, 154, 247),        // Tokyo Night purple
            teal: Color32::from_rgb(115, 218, 202),         // Tokyo Night teal
            text: Color32::from_rgb(192, 202, 245),         // Tokyo Night fg
            subtext: Color32::from_rgb(86, 95, 137),        // Tokyo Night comment
        }
    }

    pub fn solarized_dark() -> Self {
        Self {
            background: Color32::from_rgb(0, 43, 54),       // Solarized base03
            surface: Color32::from_rgb(7, 54, 66),          // Solarized base02
            overlay: Color32::from_rgb(88, 110, 117),       // Solarized base01
            blue: Color32::from_rgb(38, 139, 210),          // Solarized blue
            green: Color32::from_rgb(133, 153, 0),          // Solarized green
            yellow: Color32::from_rgb(181, 137, 0),         // Solarized yellow
            peach: Color32::from_rgb(203, 75, 22),          // Solarized orange
            red: Color32::from_rgb(220, 50, 47),            // Solarized red
            mauve: Color32::from_rgb(211, 54, 130),         // Solarized magenta
            teal: Color32::from_rgb(42, 161, 152),          // Solarized cyan
            text: Color32::from_rgb(131, 148, 150),         // Solarized base0
            subtext: Color32::from_rgb(88, 110, 117),       // Solarized base01
        }
    }

    pub fn one_dark() -> Self {
        Self {
            background: Color32::from_rgb(40, 44, 52),      // One Dark bg
            surface: Color32::from_rgb(53, 59, 69),         // One Dark gutter
            overlay: Color32::from_rgb(76, 82, 99),         // One Dark selection
            blue: Color32::from_rgb(97, 175, 239),          // One Dark blue
            green: Color32::from_rgb(152, 195, 121),        // One Dark green
            yellow: Color32::from_rgb(229, 192, 123),       // One Dark yellow
            peach: Color32::from_rgb(209, 154, 102),        // One Dark orange
            red: Color32::from_rgb(224, 108, 117),          // One Dark red
            mauve: Color32::from_rgb(198, 120, 221),        // One Dark purple
            teal: Color32::from_rgb(86, 182, 194),          // One Dark cyan
            text: Color32::from_rgb(171, 178, 191),         // One Dark fg
            subtext: Color32::from_rgb(92, 99, 112),        // One Dark comment
        }
    }
}

// Backwards compatibility - use mocha theme by default
pub struct Colors;

impl Colors {
    pub const BACKGROUND: Color32 = Color32::from_rgb(30, 30, 46);
    pub const SURFACE: Color32 = Color32::from_rgb(49, 50, 68);
    pub const BLUE: Color32 = Color32::from_rgb(137, 180, 250);
    pub const GREEN: Color32 = Color32::from_rgb(166, 227, 161);
    pub const YELLOW: Color32 = Color32::from_rgb(249, 226, 175);
    pub const PEACH: Color32 = Color32::from_rgb(250, 179, 135);
    pub const RED: Color32 = Color32::from_rgb(243, 139, 168);
    pub const MAUVE: Color32 = Color32::from_rgb(203, 166, 247);
    pub const TEAL: Color32 = Color32::from_rgb(148, 226, 213);
    pub const TEXT: Color32 = Color32::from_rgb(205, 214, 244);
    pub const SUBTEXT: Color32 = Color32::from_rgb(108, 112, 134);
}

pub fn apply_theme(ctx: &egui::Context, palette: &ColorPalette) {
    let mut style = (*ctx.style()).clone();

    let is_dark = palette.background.r() < 128;

    style.visuals.dark_mode = is_dark;
    style.visuals.override_text_color = Some(palette.text);
    style.visuals.hyperlink_color = palette.blue;
    style.visuals.faint_bg_color = palette.surface;
    style.visuals.extreme_bg_color = palette.background;
    style.visuals.code_bg_color = palette.overlay;
    style.visuals.window_fill = palette.background;
    style.visuals.panel_fill = palette.surface;

    style.visuals.widgets.noninteractive.bg_fill = palette.surface;
    style.visuals.widgets.noninteractive.fg_stroke.color = palette.text;

    style.visuals.widgets.inactive.bg_fill = palette.surface;
    style.visuals.widgets.inactive.fg_stroke.color = palette.text;
    style.visuals.widgets.inactive.weak_bg_fill = palette.surface;

    style.visuals.widgets.hovered.bg_fill = palette.overlay;
    style.visuals.widgets.hovered.fg_stroke.color = palette.blue;

    style.visuals.widgets.active.bg_fill = palette.blue;
    style.visuals.widgets.active.fg_stroke.color = palette.text;

    style.visuals.selection.bg_fill = palette.blue.gamma_multiply(0.3);
    style.visuals.selection.stroke.color = palette.blue;

    style.spacing.button_padding = egui::vec2(8.0, 4.0);
    style.spacing.item_spacing = egui::vec2(8.0, 6.0);
    style.spacing.window_margin = egui::Margin::same(12.0);

    style.visuals.window_rounding = egui::Rounding::same(8.0);
    style.visuals.menu_rounding = egui::Rounding::same(6.0);
    style.visuals.window_shadow.offset = egui::vec2(4.0, 8.0);
    style.visuals.window_shadow.blur = 16.0;
    style.visuals.window_shadow.color = Color32::from_black_alpha(100);
    style.visuals.popup_shadow.offset = egui::vec2(2.0, 4.0);
    style.visuals.popup_shadow.blur = 12.0;
    style.visuals.popup_shadow.color = Color32::from_black_alpha(80);

    ctx.set_style(style);
}

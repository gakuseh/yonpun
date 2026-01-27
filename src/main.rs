mod style;
mod topbar;

use freya::prelude::*;

fn main() {
    // *Start* your app with a window and its root component
    launch(LaunchConfig::new()
        .with_window(
        WindowConfig::new(app)
            .with_title("Yonpun")
            .with_decorations(true)  // Hide window decorations (title bar)
            .with_transparency(false)
    ))
}

fn app() -> impl IntoElement {
    // Declare the *UI*
    rect()
        .width(Size::fill())
        .height(Size::fill())
        .background(style::BACKGROUND)
        .color(Color::WHITE)
        .font_family(style::FONT_FAMILY)
        .child(topbar::Topbar)
}
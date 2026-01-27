use freya::prelude::*;
use freya::radio;

use crate::style;

#[derive(Default, Clone)]
struct CurrentTabState {
    current_tab: String
}

#[derive(PartialEq, Eq, Clone, Debug, Copy, Hash)]
enum CurrentTabChannel {
    CurrentTab
}

impl radio::RadioChannel<CurrentTabState> for CurrentTabChannel {}

#[derive(PartialEq)]
pub struct Topbar;
impl Component for Topbar {
    fn render(&self) -> impl IntoElement {
        radio::use_init_radio_station::<CurrentTabState, CurrentTabChannel>(|| CurrentTabState{current_tab: String::from("Dashboard")});

        rect()
            .width(Size::fill())
            .background(style::PRIMARY)
            .height(Size::px(50.0))
            .direction(Direction::Horizontal)
            .main_align(Alignment::SpaceAround)
            .cross_align(Alignment::Center)
            .child(label().text("Yonpun").font_size(30.).font_weight(FontWeight::SEMI_BOLD))
            .child(TopbarButton::new("Dashboard"))
            .child(TopbarButton::new("To-do"))
            .child(TopbarButton::new("Calendar"))
            .child(TopbarButton::new("Settings"))
    }
}

#[derive(PartialEq)]
pub struct TopbarButton {
    pub text: &'static str,
}
impl TopbarButton {
    pub fn new(text: &'static str) -> Self {
        TopbarButton { text: text }
    }
}
impl Component for TopbarButton {
    fn render(&self) -> impl IntoElement {
        let mut radio = radio::use_radio(CurrentTabChannel::CurrentTab);
        let mut is_hovered = use_state(| | false);
        let text_ref = self.text;

        let mut container: Rect = rect()
            .content(Content::Fit)
            .child(self.text)
            .padding(Gaps::new_all(12.))
            .rounded_lg()
            .on_press(move |_| radio.write().current_tab = text_ref.to_string())
            .on_pointer_enter(move |_| *is_hovered.write() = true)
            .on_pointer_leave(move |_| *is_hovered.write() = false);
            
        if radio.read().current_tab == self.text {
            container = container.child(Divider);
        }

        if *is_hovered.read() {
            container = container.background(Color::from_argb(30, 255, 255, 255));
        } else {
            container = container.background(Color::from_argb(0, 255, 255, 255));
        }
        
        container
    }
}

#[derive(PartialEq)]
pub struct Divider;
impl Component for Divider {
    fn render(&self) -> impl IntoElement {
        rect()
            .height(Size::px(5.0))
            .width(Size::fill_minimum())
            .background(style::ACCENT) // Blue
            .rounded()
    }
}
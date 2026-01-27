/*
Copyright 2026 baconerie

This file is part of Yonpun.

Yonpun is free software: you can redistribute it and/or modify it under the 
terms of the GNU General Public License as published by the Free Software 
Foundation, either version 3 of the License, or (at your option) any later
version.

Yonpun is distributed in the hope that it will be useful, but WITHOUT ANY 
WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A 
PARTICULAR PURPOSE. See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License along with 
Yonpun. If not, see <https://www.gnu.org/licenses/>. 
*/

use freya::prelude::*;
use freya::radio;

use crate::{style, components};

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
            container = container.child(components::Divider);
        }

        if *is_hovered.read() {
            container = container.background(Color::from_argb(30, 255, 255, 255));
        } else {
            container = container.background(Color::from_argb(0, 255, 255, 255));
        }
        
        container
    }
}
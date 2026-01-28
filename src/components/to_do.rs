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

use crate::{style, components, core};

#[derive(PartialEq)]
pub struct TaskDisplay {
    // task: &'static core::Task,
}
impl TaskDisplay {
    fn new(/*associated_task: &'static core::Task*/) -> Self {
        TaskDisplay { /*task: associated_task*/ }
    }
}
impl Component for TaskDisplay {
    fn render(&self) -> impl IntoElement {
        rect() // Wrapper
            .content(Content::Fit)
            .width(Size::Fill)
            .horizontal()
            .child( 
                // Color on the left side
                rect()
                    .background((255, 0, 0))
                    .width(Size::px(50.))
                    .height(Size::FillMinimum)
                    .corner_radius(CornerRadius{top_left: 8., top_right: 0., bottom_right: 0., bottom_left: 8., smoothing: 0.})
            )
            .child( 
                // Main Part
                rect()
                    .content(Content::Fit)
                    .padding(Gaps::new_all(15.))
                    .background(style::PRIMARY)
                    .horizontal()
                    .width(Size::Fill)
                    .main_align(Alignment::SpaceBetween)
                    .cross_align(Alignment::Center)
                    .corner_radius(CornerRadius{top_left: 0., top_right: 8., bottom_right: 8., bottom_left: 0., smoothing: 0.})
                    .child(
                        // Left side wrapper
                        rect()
                            .horizontal()
                            .cross_align(Alignment::Center)
                            .spacing(10.)
                            .child(
                                Button::new()
                                    .outline()
                                    .width(Size::px(40.))
                                    .height(Size::px(40.))
                                    .rounded_lg()
                                    .background(style::PRIMARY)
                                    .hover_background(style::HOVER_PRIMARY)
                            )
                            .child(
                                label()
                                    .font_size(30.)
                                    .text("Pickup supplies")
                            )
                    )

                    .child(
                        // Right side wrapper
                        rect()
                            .content(Content::Fit)
                            .horizontal()
                            .cross_align(Alignment::Center)
                            .spacing(20.)
                            .child(
                                label()
                                    .font_size(30.)
                                    .text("30 min")
                            )
                            .child(
                                rect()
                                    .width(Size::px(3.))
                                    .height(Size::px(50.))
                                    .background(Color::WHITE)
                            )
                            .child(
                                label()
                                    .font_size(30.)
                                    .text("Jan 2 2026")
                            )
                    )
            )
    }
}


#[derive(PartialEq)]
pub struct ToDo;
impl Component for ToDo {
    fn render(&self) -> impl IntoElement {
        rect()
            .content(Content::Fit)
            .width(Size::fill())
            .height(Size::fill_minimum())
            .background(style::BACKGROUND)
            .padding(Gaps::new_all(15.))
            .spacing(20.)
            .cross_align(Alignment::Center)

            // Main top bar 
            .child(
                rect()
                    .width(Size::fill())
                    .horizontal()
                    .main_align(Alignment::SpaceBetween)
                    .child(components::Heading::new("To-do"))
                    .child(
                        rect()
                            .horizontal()
                            .spacing(15.)
                            .child(
                                freya::components::Button::new()
                                    .background(style::ACCENT)
                                    .hover_background(style::HOVER_ACCENT)
                                    .filled()
                                    .padding(Gaps::new_all(12.))
                                    .rounded_lg()
                                    .child(
                                        label()
                                            .text("Add Task")
                                            .font_size(24.)
                                    )
                            )
                            .child(
                                freya::components::Button::new()
                                    .background(style::ALERT)
                                    .hover_background(style::HOVER_ALERT)
                                    .filled()
                                    .padding(Gaps::new_all(12.))
                                    .rounded_lg()
                                    .child(
                                        label()
                                            .text("Re-plan")
                                            .font_size(24.)
                                    )
                            )
                            .child(
                                freya::components::Button::new()
                                    .background(style::PRIMARY)
                                    .hover_background(style::HOVER_PRIMARY)
                                    .filled()
                                    .padding(Gaps::new_all(12.))
                                    .rounded_lg()
                                    .child(
                                        label()
                                            .text("Edit labels")
                                            .font_size(24.)
                                    )
                            )
                    )
            )

            .child(
                rect()
                    .spacing(10.)
                    .child(
                        TaskDisplay::new()
                    )
                    .child(
                        TaskDisplay::new()
                    )
                    .child(
                        TaskDisplay::new()
                    )
            )
            
    }
}
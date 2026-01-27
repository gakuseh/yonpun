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

use crate::{style, components};

#[derive(PartialEq)]
pub struct Dashboard;
impl Component for Dashboard {
    fn render(&self) -> impl IntoElement {
        let happening_now_rect = rect()
            .child(
                rect()
                    .background((147, 196, 125))
                    .padding(Gaps::new(30., 15., 30., 15.))
                    .child(
                        label()
                            .text("Happening now")
                            .font_size(45.)
                            .font_weight(FontWeight::BOLD)
                    )
                    .width(Size::fill())
                    .corner_radius(CornerRadius{top_left: 8., top_right: 8., bottom_left: 0., bottom_right: 0., smoothing: 0.})
            )
            .child(
                rect()
                    .background(style::PRIMARY)
                    .width(Size::fill())
                    .padding(Gaps::new_all(15.))
                    .direction(Direction::Horizontal)
                    .main_align(Alignment::SpaceBetween)
                    .child(
                        label()
                            .text("Pickup supplies")
                            .font_size(30.)
                    )
                    .child(
                        rect()
                            .content(Content::Fit)
                            .direction(Direction::Horizontal)
                            .spacing(10.)
                            .child(
                                label()
                                    .text("For 25 mins")
                                    .font_size(30.)
                            )
                            .child(
                                rect()
                                    .width(Size::px(2.))
                                    .height(Size::fill_minimum())
                                    .background(Color::WHITE)
                            )
                            .child(
                                label()
                                    .text("Due Jan 2 2026")
                                    .font_size(30.)
                            )
                    )
                    .corner_radius(CornerRadius{top_left: 0., top_right: 0., bottom_left: 8., bottom_right: 8., smoothing: 0.})
            );

        let next_up_rect = rect()
            .child(
                rect()
                    .background((66, 133, 244))
                    .padding(Gaps::new(30., 15., 30., 15.))
                    .child(
                        label()
                            .text("Next up")
                            .font_size(45.)
                            .font_weight(FontWeight::BOLD)
                    )
                    .width(Size::fill())
                    .corner_radius(CornerRadius{top_left: 8., top_right: 8., bottom_left: 0., bottom_right: 0., smoothing: 0.})
            )
            .child(
                rect()
                    .background(style::PRIMARY)
                    .width(Size::fill())
                    .padding(Gaps::new_all(15.))
                    .direction(Direction::Horizontal)
                    .main_align(Alignment::SpaceBetween)
                    .child(
                        label()
                            .text("Collect chestnuts")
                            .font_size(30.)
                    )
                    .child(
                        rect()
                            .content(Content::Fit)
                            .direction(Direction::Horizontal)
                            .spacing(10.)
                            .child(
                                label()
                                    .text("@ 5:30 PM")
                                    .font_size(30.)
                            )
                            .child(
                                rect()
                                    .width(Size::px(2.))
                                    .height(Size::fill_minimum())
                                    .background(Color::WHITE)
                            )
                            .child(
                                label()
                                    .text("For 25 mins")
                                    .font_size(30.)
                            )
                            .child(
                                rect()
                                    .width(Size::px(2.))
                                    .height(Size::fill_minimum())
                                    .background(Color::WHITE)
                            )
                            .child(
                                label()
                                    .text("Due Jan 2 2026")
                                    .font_size(30.)
                            )
                    )
                    .corner_radius(CornerRadius{top_left: 0., top_right: 0., bottom_left: 8., bottom_right: 8., smoothing: 0.})
            );


        rect()
            .content(Content::Fit)
            .width(Size::fill())
            .height(Size::fill_minimum())
            .background(style::BACKGROUND)
            .padding(Gaps::new_all(15.))
            .spacing(20.)
            .cross_align(Alignment::Center)
            .child(
                rect()
                    .width(Size::fill())
                    .child(components::Heading::new("Dashboard"))
            )
            .child(happening_now_rect)
            .child(next_up_rect)
    }
}
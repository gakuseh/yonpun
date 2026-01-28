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
use crate::components;

#[derive(PartialEq)]
pub struct Heading {
    text: &'static str,
}
impl Heading {
    pub fn new(text: &'static str) -> Self {
        Heading {text: text}
    }
}
impl Component for Heading {
    fn render(&self) -> impl IntoElement {
        rect()
            .content(Content::Fit)
            .child(
                label()
                    .text(self.text)
                    .font_size(40.)
                    .font_family("Merriweather")
            )
            .child(
                components::Divider
            )
    }
}
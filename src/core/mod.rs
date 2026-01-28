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

use chrono::prelude::*;
use chrono::{DateTime, Local};

#[derive(PartialEq)]
pub struct Label {
    name: String,
    color: (u8, u8, u8),
}

#[derive(PartialEq)]
pub struct Task {
    name: String,
    due_date: DateTime<Local>,
    schedule_after: Option<DateTime<Local>>,
    repeat: u8,
    repeat_until: Option<DateTime<Local>>,
    duration_quarters: u32,
    minimum_split_size: u32,
    label: &'static Label,
}
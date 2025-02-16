/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/. */

//! Generic types for CSS Easing Functions.
//! https://drafts.csswg.org/css-easing/#timing-functions

use crate::parser::ParserContext;
use crate::values::generics::Optional;

/// An entry for linear easing function.
#[derive(
    Clone,
    Copy,
    Debug,
    MallocSizeOf,
    PartialEq,
    SpecifiedValueInfo,
    ToComputedValue,
    ToCss,
    ToResolvedValue,
    ToShmem,
    Serialize,
    Deserialize,
)]
#[repr(C)]
pub struct LinearStop<Number, Percentage> {
    /// Output of the function at the given point.
    pub output: Number,
    /// Playback progress at which this output is given.
    #[css(skip_if = "Optional::is_none")]
    pub input: Optional<Percentage>,
}

/// A generic easing function.
#[derive(
    Clone,
    Debug,
    MallocSizeOf,
    PartialEq,
    SpecifiedValueInfo,
    ToComputedValue,
    ToCss,
    ToResolvedValue,
    ToShmem,
    Serialize,
    Deserialize,
)]
#[value_info(ty = "TIMING_FUNCTION")]
#[repr(u8, C)]
/// cbindgen:private-default-tagged-enum-constructor=false
pub enum TimingFunction<Integer, Number, Percentage> {
    /// `linear | ease | ease-in | ease-out | ease-in-out`
    Keyword(TimingKeyword),
    /// `cubic-bezier(<number>, <number>, <number>, <number>)`
    #[allow(missing_docs)]
    #[css(comma, function)]
    CubicBezier {
        x1: Number,
        y1: Number,
        x2: Number,
        y2: Number,
    },
    /// `step-start | step-end | steps(<integer>, [ <step-position> ]?)`
    /// `<step-position> = jump-start | jump-end | jump-none | jump-both | start | end`
    #[css(comma, function)]
    #[value_info(other_values = "step-start,step-end")]
    Steps(Integer, #[css(skip_if = "is_end")] StepPosition),
    /// linear([<linear-stop>]#)
    /// <linear-stop> = <output> && <linear-stop-length>?
    /// <linear-stop-length> = <percentage>{1, 2}
    #[css(comma, function = "linear")]
    LinearFunction(#[css(iterable)] crate::OwnedSlice<LinearStop<Number, Percentage>>),
}

#[allow(missing_docs)]
#[cfg_attr(feature = "servo", derive(Deserialize, Serialize))]
#[derive(
    Clone,
    Copy,
    Debug,
    Eq,
    MallocSizeOf,
    Parse,
    PartialEq,
    SpecifiedValueInfo,
    ToComputedValue,
    ToCss,
    ToResolvedValue,
    ToShmem,
    Serialize,
    Deserialize,
)]
#[repr(u8)]
pub enum TimingKeyword {
    Linear,
    Ease,
    EaseIn,
    EaseOut,
    EaseInOut,
}

#[cfg(feature = "gecko")]
fn step_position_jump_enabled(_context: &ParserContext) -> bool {
    static_prefs::pref!("layout.css.step-position-jump.enabled")
}

#[cfg(feature = "servo")]
fn step_position_jump_enabled(_context: &ParserContext) -> bool {
    false
}

#[allow(missing_docs)]
#[cfg_attr(feature = "servo", derive(Deserialize, Serialize))]
#[derive(
    Clone,
    Copy,
    Debug,
    Eq,
    MallocSizeOf,
    Parse,
    PartialEq,
    ToComputedValue,
    ToCss,
    ToResolvedValue,
    ToShmem,
    Serialize,
    Deserialize,
)]
#[repr(u8)]
pub enum StepPosition {
    #[parse(condition = "step_position_jump_enabled")]
    JumpStart,
    #[parse(condition = "step_position_jump_enabled")]
    JumpEnd,
    #[parse(condition = "step_position_jump_enabled")]
    JumpNone,
    #[parse(condition = "step_position_jump_enabled")]
    JumpBoth,
    Start,
    End,
}

#[inline]
fn is_end(position: &StepPosition) -> bool {
    *position == StepPosition::JumpEnd || *position == StepPosition::End
}

impl<Integer, Number, Percentage> TimingFunction<Integer, Number, Percentage> {
    /// `ease`
    #[inline]
    pub fn ease() -> Self {
        TimingFunction::Keyword(TimingKeyword::Ease)
    }
}

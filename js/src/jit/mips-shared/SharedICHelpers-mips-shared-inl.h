/* -*- Mode: C++; tab-width: 8; indent-tabs-mode: nil; c-basic-offset: 2 -*-
 * vim: set ts=8 sts=2 et sw=2 tw=80:
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef jit_mips_shared_SharedICHelpers_mips_shared_inl_h
#define jit_mips_shared_SharedICHelpers_mips_shared_inl_h

#include "jit/BaselineFrame.h"
#include "jit/SharedICHelpers.h"

#include "jit/MacroAssembler-inl.h"

namespace js {
namespace jit {

inline void EmitBaselineTailCallVM(TrampolinePtr target, MacroAssembler& masm,
                                   uint32_t argSize) {
#ifdef DEBUG
  Register scratch = R2.scratchReg();

  // Store frame size without VMFunction arguments for debug assertions.
  masm.movePtr(FramePointer, scratch);
  masm.addPtr(Imm32(BaselineFrame::FramePointerOffset), scratch);
  masm.subPtr(StackPointer, scratch);
  masm.subPtr(Imm32(argSize), scratch);
  Address frameSizeAddr(FramePointer,
                        BaselineFrame::reverseOffsetOfDebugFrameSize());
  masm.store32(scratch, frameSizeAddr);
#endif

  // Push frame descriptor and perform the tail call.
  // ICTailCallReg (ra) already contains the return address (as we
  // keep it there through the stub calls), but the VMWrapper code being
  // called expects the return address to also be pushed on the stack.
  MOZ_ASSERT(ICTailCallReg == ra);
  masm.subPtr(Imm32(sizeof(CommonFrameLayout)), StackPointer);
  masm.storePtr(ImmWord(MakeFrameDescriptor(FrameType::BaselineJS)),
                Address(StackPointer, CommonFrameLayout::offsetOfDescriptor()));
  masm.storePtr(
      ra, Address(StackPointer, CommonFrameLayout::offsetOfReturnAddress()));

  masm.jump(target);
}

inline void EmitBaselineCallVM(TrampolinePtr target, MacroAssembler& masm) {
  masm.pushFrameDescriptor(FrameType::BaselineStub);
  masm.call(target);
}

inline void EmitBaselineEnterStubFrame(MacroAssembler& masm, Register scratch) {
  MOZ_ASSERT(scratch != ICTailCallReg);

#ifdef DEBUG
  // Compute frame size.
  masm.movePtr(FramePointer, scratch);
  masm.addPtr(Imm32(BaselineFrame::FramePointerOffset), scratch);
  masm.subPtr(StackPointer, scratch);

  Address frameSizeAddr(FramePointer,
                        BaselineFrame::reverseOffsetOfDebugFrameSize());
  masm.store32(scratch, frameSizeAddr);
#endif

  // Note: when making changes here, don't forget to update
  // BaselineStubFrame if needed.

  // Push frame descriptor and return address.
  masm.subPtr(Imm32(STUB_FRAME_SIZE), StackPointer);
  masm.storePtr(ImmWord(MakeFrameDescriptor(FrameType::BaselineJS)),
                Address(StackPointer, offsetof(BaselineStubFrame, descriptor)));
  masm.storePtr(ICTailCallReg, Address(StackPointer, offsetof(BaselineStubFrame,
                                                              returnAddress)));

  // Save old frame pointer, stack pointer and stub reg.
  masm.storePtr(ICStubReg,
                Address(StackPointer, offsetof(BaselineStubFrame, savedStub)));
  masm.storePtr(FramePointer,
                Address(StackPointer, offsetof(BaselineStubFrame, savedFrame)));
  masm.movePtr(StackPointer, FramePointer);

  // Stack should remain aligned.
  masm.assertStackAlignment(sizeof(Value), 0);
}

}  // namespace jit
}  // namespace js

#endif /* jit_mips_shared_SharedICHelpers_mips_shared_inl_h */

/* -*- Mode: C++; tab-width: 8; indent-tabs-mode: nil; c-basic-offset: 2 -*-
 * vim: set ts=8 sts=2 et sw=2 tw=80:
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef jit_wasm32_Assembler_wasm32_h
#define jit_wasm32_Assembler_wasm32_h

#include "mozilla/Assertions.h"

#include <cstdint>

#include "jit/Registers.h"
#include "jit/RegisterSets.h"
#include "jit/shared/Assembler-shared.h"
#include "jit/wasm32/Architecture-wasm32.h"
#include "js/Value.h"

namespace js::jit {

struct ImmTag : public Imm32 {
  explicit ImmTag(JSValueTag mask) : Imm32(int32_t(mask)) {}
};

struct ImmType : public ImmTag {
  explicit ImmType(JSValueType type) : ImmTag(JSVAL_TYPE_TO_TAG(type)) {}
};

class MacroAssembler;

static constexpr Register StackPointer{Registers::StackPointer};
static constexpr Register FramePointer{Registers::FramePointer};

static constexpr Register ReturnReg{Registers::invalid_reg2};
static constexpr FloatRegister ReturnFloat32Reg = {FloatRegisters::invalid_reg};
static constexpr FloatRegister ReturnDoubleReg = {FloatRegisters::invalid_reg};
static constexpr FloatRegister ReturnSimd128Reg = {FloatRegisters::invalid_reg};
static constexpr FloatRegister ScratchSimd128Reg = {
    FloatRegisters::invalid_reg};
static constexpr FloatRegister InvalidFloatReg = {FloatRegisters::invalid_reg};

struct ScratchFloat32Scope : FloatRegister {
  explicit ScratchFloat32Scope(MacroAssembler& masm) {}
};

struct ScratchDoubleScope : FloatRegister {
  explicit ScratchDoubleScope(MacroAssembler& masm) {}
};

static constexpr Register OsrFrameReg{Registers::invalid_reg};
static constexpr Register PreBarrierReg{Registers::invalid_reg};
static constexpr Register InterpreterPCReg{Registers::invalid_reg};
static constexpr Register CallTempReg0{Registers::invalid_reg};
static constexpr Register CallTempReg1{Registers::invalid_reg};
static constexpr Register CallTempReg2{Registers::invalid_reg};
static constexpr Register CallTempReg3{Registers::invalid_reg};
static constexpr Register CallTempReg4{Registers::invalid_reg};
static constexpr Register CallTempReg5{Registers::invalid_reg};
static constexpr Register InvalidReg{Registers::invalid_reg};
static constexpr Register CallTempNonArgRegs[] = {InvalidReg, InvalidReg};
static const uint32_t NumCallTempNonArgRegs = std::size(CallTempNonArgRegs);

static constexpr Register IntArgReg0{Registers::invalid_reg};
static constexpr Register IntArgReg1{Registers::invalid_reg};
static constexpr Register IntArgReg2{Registers::invalid_reg};
static constexpr Register IntArgReg3{Registers::invalid_reg};
static constexpr Register HeapReg{Registers::invalid_reg};

static constexpr Register RegExpTesterRegExpReg{Registers::invalid_reg};
static constexpr Register RegExpTesterStringReg{Registers::invalid_reg};
static constexpr Register RegExpTesterLastIndexReg{Registers::invalid_reg};
static constexpr Register RegExpTesterStickyReg{Registers::invalid_reg};

static constexpr Register RegExpMatcherRegExpReg{Registers::invalid_reg};
static constexpr Register RegExpMatcherStringReg{Registers::invalid_reg};
static constexpr Register RegExpMatcherLastIndexReg{Registers::invalid_reg};
static constexpr Register RegExpMatcherStickyReg{Registers::invalid_reg};

// Uses |invalid_reg2| to avoid static_assert failures.
static constexpr Register JSReturnReg_Type{Registers::invalid_reg2};
static constexpr Register JSReturnReg_Data{Registers::invalid_reg2};
static constexpr Register JSReturnReg{Registers::invalid_reg2};

#if defined(JS_NUNBOX32)
static constexpr ValueOperand JSReturnOperand(Register{Registers::r2},
                                              Register{Registers::r3});
static constexpr Register64 ReturnReg64(InvalidReg, InvalidReg);
#elif defined(JS_PUNBOX64)
static constexpr ValueOperand JSReturnOperand(InvalidReg);
static constexpr Register64 ReturnReg64(InvalidReg);
#else
#  error "Bad architecture"
#endif

static constexpr Register ABINonArgReg0{Registers::invalid_reg};
static constexpr Register ABINonArgReg1{Registers::invalid_reg};
static constexpr Register ABINonArgReg2{Registers::invalid_reg};
static constexpr Register ABINonArgReg3{Registers::invalid_reg};
static constexpr Register ABINonArgReturnReg0{Registers::invalid_reg};
static constexpr Register ABINonArgReturnReg1{Registers::invalid_reg};
static constexpr Register ABINonVolatileReg{Registers::invalid_reg};
static constexpr Register ABINonArgReturnVolatileReg{Registers::invalid_reg};

static constexpr FloatRegister ABINonArgDoubleReg = {
    FloatRegisters::invalid_reg};

static constexpr Register WasmTableCallScratchReg0{Registers::invalid_reg};
static constexpr Register WasmTableCallScratchReg1{Registers::invalid_reg};
static constexpr Register WasmTableCallSigReg{Registers::invalid_reg};
static constexpr Register WasmTableCallIndexReg{Registers::invalid_reg};
static constexpr Register InstanceReg{Registers::invalid_reg};
static constexpr Register WasmJitEntryReturnScratch{Registers::invalid_reg};

static constexpr uint32_t ABIStackAlignment = 4;
static constexpr uint32_t CodeAlignment = 16;
static constexpr uint32_t JitStackAlignment = 8;
static constexpr uint32_t JitStackValueAlignment =
    JitStackAlignment / sizeof(Value);

static const Scale ScalePointer = TimesOne;

static constexpr uint32_t Int32SizeLog2 = 2;

struct MemoryArgument {
  uint32_t align;
  uint32_t offset;
};

class AssemblerWasm32 : public AssemblerShared {};

class Assembler : public AssemblerWasm32 {
 public:
  enum Condition {
    Equal,
    NotEqual,
    Above,
    AboveOrEqual,
    Below,
    BelowOrEqual,
    GreaterThan,
    GreaterThanOrEqual,
    LessThan,
    LessThanOrEqual,
    Overflow,
    CarrySet,
    CarryClear,
    Signed,
    NotSigned,
    Zero,
    NonZero,
    Always,
  };

  enum DoubleCondition {
    DoubleOrdered,
    DoubleEqual,
    DoubleNotEqual,
    DoubleGreaterThan,
    DoubleGreaterThanOrEqual,
    DoubleLessThan,
    DoubleLessThanOrEqual,
    DoubleUnordered,
    DoubleEqualOrUnordered,
    DoubleNotEqualOrUnordered,
    DoubleGreaterThanOrUnordered,
    DoubleGreaterThanOrEqualOrUnordered,
    DoubleLessThanOrUnordered,
    DoubleLessThanOrEqualOrUnordered
  };

  static Condition InvertCondition(Condition) { MOZ_CRASH(); }

  static DoubleCondition InvertCondition(DoubleCondition) { MOZ_CRASH(); }

  template <typename T, typename S>
  static void PatchDataWithValueCheck(CodeLocationLabel, T, S) {
    MOZ_CRASH();
  }
  static void PatchWrite_Imm32(CodeLocationLabel, Imm32) { MOZ_CRASH(); }

  static void PatchWrite_NearCall(CodeLocationLabel, CodeLocationLabel) {
    MOZ_CRASH();
  }
  static uint32_t PatchWrite_NearCallSize() { MOZ_CRASH(); }

  static void ToggleToJmp(CodeLocationLabel) { MOZ_CRASH(); }
  static void ToggleToCmp(CodeLocationLabel) { MOZ_CRASH(); }
  static void ToggleCall(CodeLocationLabel, bool) { MOZ_CRASH(); }

  static void Bind(uint8_t*, const CodeLabel&) { MOZ_CRASH(); }

  static uintptr_t GetPointer(uint8_t*) { MOZ_CRASH(); }

  static bool HasRoundInstruction(RoundingMode) { return false; }

  void verifyHeapAccessDisassembly(uint32_t begin, uint32_t end,
                                   const Disassembler::HeapAccess& heapAccess) {
    MOZ_CRASH();
  }

  void setUnlimitedBuffer() { MOZ_CRASH(); }
};

class Operand {
 public:
  explicit Operand(const Address&) { MOZ_CRASH(); }
  explicit Operand(const Register) { MOZ_CRASH(); }
  explicit Operand(const FloatRegister) { MOZ_CRASH(); }
  explicit Operand(Register, Imm32) { MOZ_CRASH(); }
  explicit Operand(Register, int32_t) { MOZ_CRASH(); }
};

class ABIArgGenerator {
 public:
  ABIArgGenerator() = default;
  ABIArg next(MIRType) { MOZ_CRASH(); }
  ABIArg& current() { MOZ_CRASH(); }
  uint32_t stackBytesConsumedSoFar() const { MOZ_CRASH(); }
  void increaseStackOffset(uint32_t) { MOZ_CRASH(); }
};

}  // namespace js::jit

#endif /* jit_wasm32_Assembler_wasm32_h */

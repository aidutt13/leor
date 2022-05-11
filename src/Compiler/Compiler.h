#pragma once

#ifndef LEOR_COMPILER_H
#define LEOR_COMPILER_H

#include "Compiler/Program.h"

namespace leor
{
  enum class ArgumentRegister
  {
    RDI, RSI, RDX, RCX, R8, R9, STACK
  };

  ArgumentRegister GetArgumentRegister(uint64_t index);

  struct Assembly
  {
    std::ostringstream code, data, bss;
    uint64_t argRegIndex = 0UL;
  };

  Assembly Compile(Program& program);

  void CompileNASM_Linux_x86_64(Program& program, Assembly& assembly);
  void CompileNodeNASM_Linux_x86_64(const AST& node, Program& program, Assembly& assembly);
} // namespace leor


#endif // LEOR_COMPILER_H
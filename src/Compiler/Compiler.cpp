#include "Compiler/Compiler.h"

namespace leor
{
  ArgumentRegister GetArgumentRegister(uint64_t index)
  {
    if (index > (uint64_t)ArgumentRegister::STACK)
      return ArgumentRegister::STACK;
    return (ArgumentRegister)index;
  }

  Assembly Compile(Program& program)
  {
    Assembly assembly;

    CompileNASM_Linux_x86_64(program, assembly);

    return assembly;
  };

  void CompileNASM_Linux_x86_64(Program& program, Assembly& assembly)
  {
    const AST& ast = program.ast;

    if (ast.type != AST::Type::PROG)
    {
      throw std::runtime_error("AST is not a program");
    }

    assembly.data << "section .data\n";
    assembly.code
      << "SECTION .text\n"
      << "global _start\n"
      << "_start:\n"
      << "  call main\n"
      << "  xor rdi, rdi\n"
      << "  mov rax, 60\n"
      << "  syscall\n";
    ;

    const auto& prog = std::get<std::vector<AST>>(ast.at("prog"));
    for (const auto& node : prog)
    {
      CompileNodeNASM_Linux_x86_64(node, program, assembly);
    }
  }

  void CompileNodeNASM_Linux_x86_64(const AST& node, Program& program, Assembly& assembly)
  {
    switch (node.type)
    {
    case AST::Type::NONE:
      return;

    case AST::Type::PROG:
    {
      const auto& prog = std::get<std::vector<AST>>(node.at("prog"));
      for (const auto& node : prog)
      {
        CompileNodeNASM_Linux_x86_64(node, program, assembly);
      }
    }

    case AST::Type::FUNCTION:
    {
      const auto& name = std::get<std::string>(node.at("name"));
      const auto& args = std::get<std::vector<AST>>(node.at("args"));
      const auto& body = std::get<AST>(node.at("body"));
      const auto& typeName = std::get<std::string>(node.at("type"));

      auto type = program.GetType(typeName);

      type == nullptr
        ? throw std::runtime_error("Type "s + typeName + " does not exist in the current scope")
        : 0;

      program.GetFunction(name) != nullptr
        ? throw std::runtime_error("Function of name "s + name + " already exists in the current scope")
        : 0;

      auto scope = program.ExpandCurrent(name);
      auto func = Function(name, type, {}, scope);

      for (const auto& arg : args)
      {
        const auto& argName = std::get<std::string>(arg.at("name"));
        program.DoesNameExist(argName, scope)
          ? throw std::runtime_error("Variable of name "s + argName + " already exists in the current scope")
          : 0;

        const auto& argTypeName = std::get<std::string>(arg.at("type"));
        auto argType = program.GetType(argTypeName);
        argType == nullptr
          ? throw std::runtime_error("Type "s + argTypeName + " does not exist in the current scope")
          : 0;

        func.parameters.push_back(argType);

        
      }

      Assembly funcAsm;

      assembly.code
        << name << ":\n"
        << "  push rbp\n"
        << "  mov rbp, rsp\n"
        << "  sub rsp, " << args.size() * 8 << "\n"
      ;
    }
    
    default:
      break;
    }
  }
} // namespace leor

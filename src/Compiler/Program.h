#pragma once

#ifndef LEOR_PROGRAM_H
#define LEOR_PROGRAM_H

#include "Parser/Parser.h"

#include <memory>

namespace leor
{
  using namespace std::string_literals;

  struct Type
  {
    std::string name;
    uint64_t size;

    Type(const std::string& name, uint64_t size)
      : name(name), size(size)
    { }
  };

  struct PointerType : public Type
  {
    uint64_t size;
    std::shared_ptr<Type> type;

    PointerType(const std::string& name, std::shared_ptr<Type> type)
      : Type(name, sizeof(void*)), type(type)
    { }
  };

  struct Variable
  {
    std::string name;
    uint64_t offset;
    std::shared_ptr<Type> type;
    bool is_const;

    Variable(const std::string& name, uint64_t offset, std::shared_ptr<Type> type, bool is_const)
      : name(name), offset(offset), type(type), is_const(is_const)
    { }
  };

  struct Function
  {
    std::string name;
    std::shared_ptr<Type> return_type;
    std::vector<std::shared_ptr<Type>> parameters;
    std::shared_ptr<Scope> scope;

    Function(const std::string& name, std::shared_ptr<Type> return_type, std::vector<std::shared_ptr<Type>> parameters, std::shared_ptr<Scope> scope)
      : name(name), return_type(return_type), parameters(parameters), scope(scope)
    { }
  };

  struct Scope
  {
    std::string name;

    std::shared_ptr<Scope> parent;
    std::vector<std::shared_ptr<Scope>> children;

    std::unordered_map<std::string, Function> functions;

    std::unordered_map<std::string, Variable> variables;
    std::unordered_map<std::string, Type> types;

    Scope(const std::string& name, std::shared_ptr<Scope> parent = nullptr)
      : name(name), parent(parent)
    { }
  };

  struct Program
  {
    AST ast;
    std::unordered_map<std::string, Scope> scopes;
    std::shared_ptr<Scope> current_scope;
    std::shared_ptr<Scope> global_scope;

    Program(const AST& ast);

    std::shared_ptr<Scope> ExpandCurrent(const std::string& name);

    void AddScope(const Scope& scope);
    void AddVariable(const Variable& variable);
    void AddType(const Type& type);
    void AddFunction(const Function& function);

    void SetCurrentScope(const std::string& name);
    
    std::shared_ptr<Scope> GetScope(const std::string& name);
    std::shared_ptr<Scope> GetCurrentScope();
    std::shared_ptr<Scope> GetGlobalScope();

    std::shared_ptr<Variable> GetVariable(const std::string& name, std::shared_ptr<Scope> scope = nullptr);
    std::shared_ptr<Type> GetType(const std::string& name, std::shared_ptr<Scope> scope = nullptr);
    std::shared_ptr<Function> GetFunction(const std::string& name, std::shared_ptr<Scope> scope = nullptr);

    bool DoesNameExist(const std::string& name, std::shared_ptr<Scope> scope = nullptr);

    bool IsCurrentScopeFunction();
  };
} // namespace leor


#endif // LEOR_PROGRAM_H
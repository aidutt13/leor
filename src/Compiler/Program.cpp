#include "Compiler/Program.h"

namespace leor
{
  Program::Program(const AST& ast)
    : ast(ast)
  {
    scopes[""] = Scope("");
    global_scope = std::shared_ptr<Scope>(&scopes.at(""));
    current_scope = global_scope;
  }

  std::shared_ptr<Scope> Program::ExpandCurrent(const std::string& name)
  {
    scopes[name] = Scope(name, current_scope);

    return std::shared_ptr<Scope>(&scopes.at(name));
  }

  void Program::AddScope(const Scope& scope)
  {
    scopes[scope.name] = scope;
  }

  void Program::AddVariable(const Variable& variable)
  {
    current_scope->variables[variable.name] = variable;
  }

  void Program::AddType(const Type& type)
  {
    current_scope->types[type.name] = type;
  }

  void Program::AddFunction(const Function& function)
  {
    current_scope->functions[function.name] = function;
  }

  void Program::SetCurrentScope(const std::string& name)
  {
    current_scope = GetScope(name);
  }

  std::shared_ptr<Scope> Program::GetScope(const std::string& name)
  {
    if (scopes.find(name) != scopes.end())
    {
      return std::shared_ptr<Scope>(&scopes.at(name));
    }
    return nullptr;
  }

  std::shared_ptr<Scope> Program::GetCurrentScope()
  {
    return current_scope;
  }

  std::shared_ptr<Scope> Program::GetGlobalScope()
  {
    return global_scope;
  }

  std::shared_ptr<Variable> Program::GetVariable(const std::string& name, std::shared_ptr<Scope> scope)
  {
    auto curr = scope ? scope : current_scope;

    if (current_scope->variables.find(name) != current_scope->variables.end())
    {
      return std::shared_ptr<Variable>(&current_scope->variables.at(name));
    }

    if (curr->parent)
    {
      return GetVariable(name, curr->parent);
    }

    return nullptr;
  }

  std::shared_ptr<Type> Program::GetType(const std::string& name, std::shared_ptr<Scope> scope)
  {
    auto curr = scope ? scope : current_scope;

    if (current_scope->types.find(name) != current_scope->types.end())
    {
      return std::shared_ptr<Type>(&current_scope->types.at(name));
    }

    if (curr->parent)
    {
      return GetType(name, curr->parent);
    }

    return nullptr;
  }

  std::shared_ptr<Function> Program::GetFunction(const std::string& name, std::shared_ptr<Scope> scope)
  {
    auto curr = scope ? scope : current_scope;

    if (current_scope->functions.find(name) != current_scope->functions.end())
    {
      return std::shared_ptr<Function>(&current_scope->functions.at(name));
    }

    if (curr->parent)
    {
      return GetFunction(name, curr->parent);
    }

    return nullptr;
  }

  bool Program::DoesNameExist(const std::string& name, std::shared_ptr<Scope> scope)
  {
    auto curr = scope ? scope : current_scope;

    if (current_scope->variables.find(name) != current_scope->variables.end())
    {
      return true;
    }

    if (current_scope->functions.find(name) != current_scope->functions.end())
    {
      return true;
    }

    if (current_scope->types.find(name) != current_scope->types.end())
    {
      return true;
    }

    if (curr->parent)
    {
      return DoesNameExist(name, curr->parent);
    }

    return false;
  }

  bool Program::IsCurrentScopeFunction()
  {
    return GetFunction(current_scope->name, current_scope->parent) != nullptr;
  }
  
} // namespace leor

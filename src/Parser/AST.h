#pragma once

#ifndef LEOR_AST_H
#define LEOR_AST_H

#include <memory>
#include <variant>

#include "Lexer/Lexer.h"

namespace leor_new
{
  struct AST
  {
  private:
    AST() = default;
  
  public:
    using Value = std::variant<
      bool,
      int64_t,
      double,
      std::string,
      char,
      AST,
      std::vector<AST> 
    >;

    enum class Type
    {
      NONE,
      BOOL, INT, FLOAT, STRING, CHAR, VAR,
      FUNCTION, VARIABLE, CALL,
      IF, WHILE, FOR,
      ASSIGN, BINARY,
      PROG,
    };

    Type type;
    std::unordered_map<std::string, Value> values;
    std::tuple<uint64_t, uint64_t> pos;

    Value& operator[](const std::string& key)
    {
      return values[key];
    }

    const Value& at(const std::string& key)
    {
      return values.at(key);
    }

  private:
    AST& set(Type type);
    AST& set(const std::tuple<uint64_t, uint64_t>& pos);
    AST& set(const std::string& key, const Value& value);
    AST& set(const std::string& key, const AST& value);

    // Constructors
  public:
    AST None();

    AST Bool(
      const bool& value,
      const std::tuple<uint64_t, uint64_t> pos = std::make_tuple(0UL, 0UL)
    );

    AST Int(
      const int64_t& value,
      const std::tuple<uint64_t, uint64_t> pos = std::make_tuple(0UL, 0UL)
    );

    AST Float(
      const double& value,
      const std::tuple<uint64_t, uint64_t> pos = std::make_tuple(0UL, 0UL)
    );

    AST String(
      const std::string& value,
      const std::tuple<uint64_t, uint64_t> pos = std::make_tuple(0UL, 0UL)
    );

    AST Char(
      const char& value,
      const std::tuple<uint64_t, uint64_t> pos = std::make_tuple(0UL, 0UL)
    );

    AST Var(
      const std::string& value,
      const std::tuple<uint64_t, uint64_t> pos = std::make_tuple(0UL, 0UL)
    );

    AST Function(
      const std::string& name,
      const std::vector<AST>& args,
      const AST& body,
      const std::tuple<uint64_t, uint64_t> pos = std::make_tuple(0UL, 0UL)
    );

    AST Variable(
      const std::string& name,
      const AST& value,
      const bool& is_constant,
      const std::tuple<uint64_t, uint64_t> pos = std::make_tuple(0UL, 0UL)
    );

    AST Call(
      AST function,
      const std::vector<AST>& args,
      const std::tuple<uint64_t, uint64_t> pos = std::make_tuple(0UL, 0UL)
    );

    // AST If(); // TODO

    // AST While(); // TODO

    // AST For(); // TODO

    AST Binary(
      const std::string& op,
      const AST& left,
      const AST& right,
      const std::tuple<uint64_t, uint64_t> pos = std::make_tuple(0UL, 0UL)
    );

    AST Assign(
      const std::string& op,
      const AST& left,
      const AST& right,
      const std::tuple<uint64_t, uint64_t> pos = std::make_tuple(0UL, 0UL)
    );

    AST Prog(
      const std::vector<AST>& prog,
      const std::tuple<uint64_t, uint64_t> pos = std::make_tuple(0UL, 0UL)
    );
  };
} // namespace leor_new


namespace leor_old
{
  // struct BasicAST - A basic AST node
  struct BasicAST
  {
    enum class Type
    {
      NONE,
      BOOL, INT, FLOAT, STRING, CHAR, VAR,
      FUNCTION, VARIABLE, CALL,
      IF, WHILE, FOR,
      ASSIGN, BINARY,
      PROG,
    };

    Type type;
    std::tuple<uint64_t, uint64_t> pos;

    BasicAST(
      Type type,
      const std::tuple<uint64_t, uint64_t>& pos = std::make_tuple(0, 0)
    );
  };

  struct BoolAST : BasicAST
  {
    bool value;

    BoolAST(
      bool value,
      const std::tuple<uint64_t, uint64_t>& pos = std::make_tuple(0, 0)
    );
  };

  struct IntAST : BasicAST
  {
    int64_t value;

    IntAST(
      int64_t value,
      const std::tuple<uint64_t, uint64_t>& pos = std::make_tuple(0, 0)
    );
  };

  struct FloatAST : BasicAST
  {
    double value;

    FloatAST(
      double value,
      const std::tuple<uint64_t, uint64_t>& pos = std::make_tuple(0, 0)
    );
  };

  struct StringAST : BasicAST
  {
    std::string value;

    StringAST(
      std::string value,
      const std::tuple<uint64_t, uint64_t>& pos = std::make_tuple(0, 0)
    );
  };

  struct CharAST : BasicAST
  {
    char value;

    CharAST(
      char value,
      const std::tuple<uint64_t, uint64_t>& pos = std::make_tuple(0, 0)
    );
  };

  struct VarAST : BasicAST
  {
    std::string name;

    VarAST(
      std::string name,
      const std::tuple<uint64_t, uint64_t>& pos = std::make_tuple(0, 0)
    );
  };

  struct FunctionAST : BasicAST
  {
    std::string name;
    std::vector<std::unique_ptr<BasicAST>> args;
    std::unique_ptr<BasicAST> body;

    FunctionAST(
      std::string name,
      std::vector<std::unique_ptr<BasicAST>>&& args,
      std::unique_ptr<BasicAST>&& body,
      const std::tuple<uint64_t, uint64_t>& pos = std::make_tuple(0, 0)
    );
  };

  struct VariableAST : BasicAST
  {
    std::string name;
    std::unique_ptr<BasicAST> value;
    bool isMutable;

    VariableAST(
      std::string name,
      const std::tuple<uint64_t, uint64_t>& pos = std::make_tuple(0, 0),
      bool isMutable = false
    );
  };

  struct BinaryAST : BasicAST
  {
    std::string op;
    std::unique_ptr<BasicAST> left;
    std::unique_ptr<BasicAST> right;

    BinaryAST(
      const std::string op,
      std::unique_ptr<BasicAST>&& left,
      std::unique_ptr<BasicAST>&& right,
      const std::tuple<uint64_t, uint64_t>& pos = std::make_tuple(0, 0)
    );
  };

  struct CallAST : BasicAST
  {
    std::unique_ptr<BasicAST> func;
    std::vector<std::unique_ptr<BasicAST>> args;

    CallAST(
      std::unique_ptr<BasicAST>&& func,
      std::vector<std::unique_ptr<BasicAST>>&& args,
      const std::tuple<uint64_t, uint64_t>& pos = std::make_tuple(0, 0)
    );
  };

  struct ProgAST : BasicAST
  {
    std::vector<std::unique_ptr<BasicAST>> prog;

    ProgAST(
      std::vector<std::unique_ptr<BasicAST>>&& prog,
      const std::tuple<uint64_t, uint64_t>& pos = std::make_tuple(0, 0)
    );
  };
  
  // Precedence of operators
  extern const hash_map<std::string, uint64_t> OP_PRECEDENCE;


} // namespace leor


namespace std
{
  // TODO: Implement to_string for BasicAST
  string to_string(leor::BasicAST* ast);
} // namespace std

#endif // LEOR_AST_H
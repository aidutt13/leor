#pragma once

#ifndef LEOR_AST_H
#define LEOR_AST_H

#include <memory>
#include <variant>

#include "Lexer/Lexer.h"

namespace leor
{
  template <typename T>
  struct Base {
    T value;

    Base(const T& value) : value(value) {}

    inline T& get() { return value; }
  };

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
      Base<AST>,
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
    std::map<std::string, Value> values;
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
    AST& set(const std::string& key, const AST& value);
    
    template <typename T>
    AST& set(const std::string& key, const T& value)
    {
      values.insert_or_assign(key, value);
      return *this;
    }


    // Constructors
  public:
    static AST None();

    static AST Bool(
      const bool& value,
      const std::tuple<uint64_t, uint64_t> pos = std::make_tuple(0UL, 0UL)
    );

    static AST Int(
      const int64_t& value,
      const std::tuple<uint64_t, uint64_t> pos = std::make_tuple(0UL, 0UL)
    );

    static AST Float(
      const double& value,
      const std::tuple<uint64_t, uint64_t> pos = std::make_tuple(0UL, 0UL)
    );

    static AST String(
      const std::string& value,
      const std::tuple<uint64_t, uint64_t> pos = std::make_tuple(0UL, 0UL)
    );

    static AST Char(
      const char& value,
      const std::tuple<uint64_t, uint64_t> pos = std::make_tuple(0UL, 0UL)
    );

    static AST Var(
      const std::string& value,
      const std::tuple<uint64_t, uint64_t> pos = std::make_tuple(0UL, 0UL)
    );

    static AST Function(
      const std::string& name,
      const std::vector<AST>& args,
      const AST& body,
      const std::tuple<uint64_t, uint64_t> pos = std::make_tuple(0UL, 0UL)
    );

    static AST Variable(
      const std::string& name,
      const AST& value,
      const bool& is_constant,
      const std::tuple<uint64_t, uint64_t> pos = std::make_tuple(0UL, 0UL)
    );

    static AST Call(
      AST function,
      const std::vector<AST>& args,
      const std::tuple<uint64_t, uint64_t> pos = std::make_tuple(0UL, 0UL)
    );

    // static AST If(); // TODO

    // static AST While(); // TODO

    // static AST For(); // TODO

    static AST Binary(
      const std::string& op,
      const AST& left,
      const AST& right,
      const std::tuple<uint64_t, uint64_t> pos = std::make_tuple(0UL, 0UL)
    );

    static AST Assign(
      const std::string& op,
      const AST& left,
      const AST& right,
      const std::tuple<uint64_t, uint64_t> pos = std::make_tuple(0UL, 0UL)
    );

    static AST Prog(
      const std::vector<AST>& prog,
      const std::tuple<uint64_t, uint64_t> pos = std::make_tuple(0UL, 0UL)
    );
  };

  extern const std::unordered_map<std::string, uint64_t> OP_PRECEDENCE;
} // namespace leor

#endif // LEOR_AST_H
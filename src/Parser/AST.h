#pragma once

#ifndef LEOR_AST_H
#define LEOR_AST_H

#include <memory>

#include "Lexer/Lexer.h"

namespace leor
{
  // struct BasicAST - A basic AST node
  struct BasicAST
  {
    enum class Type
    {
      NONE,
      INT, FLOAT, STRING, CHAR, VAR,
      FUNCTION, VARIABLE, IF, WHILE, FOR,
      ASSIGN, BINARY,
    };

    Type type;
    std::tuple<uint64_t, uint64_t> pos;

    BasicAST(
      Type type,
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
    std::vector<BasicAST> args;
    std::unique_ptr<BasicAST> body;

    FunctionAST(
      std::string name,
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
  
  // class Parser - A parser for a stream of tokens


} // namespace leor


#endif // LEOR_AST_H
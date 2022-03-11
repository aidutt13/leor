#pragma once

#include <variant>

#include "parser/ast.h"
#include "lexer/lexer.h"

class UnexpectedTokenException : Exception
{
public:
  static UnexpectedTokenException&& of(const Token&);
  static UnexpectedTokenException&& of(const Token&, const Token&);
};

class Parser
{
public:
  Parser(Lexer*);

  AST operator()();

  static const std::map<std::string_view, uint64> OpPrecedence;
  static const std::map<std::string_view, AST::Type> OpBinaryType;

  Token isOp();
  Token isOp(const std::string_view&);

  Token isKw();
  Token isKw(const std::string_view&);

  Token isPunc();
  Token isPunc(const std::string_view&);

  void skipOp  (const std::string_view&);
  void skipKw  (const std::string_view&);
  void skipPunc(const std::string_view&);

  std::string parseVarname();

  template<typename T>
  std::vector<T> delimited(char, char, char, std::function<T()>);

  AST parseToplevel();
  AST parseProg();
  AST parseVar();
  AST parseFunction();
  AST parseCall(AST);
  AST parseBool();
  AST parseExpression();

  AST maybeBinary(AST, uint64);
  AST maybeCall(std::function<AST()>);

  AST parseAtom();
private:
  Lexer* m_data;
};
#pragma once

#include <functional>

#include <map>
#include <memory>

#include "lexer/charstream.h"
#include "../chartest.h"
#include "myexception.h"

typedef std::function<bool(const std::string_view&)> CharPred;

struct Token {
  enum Type {
    NONE, EOB,
    INT,  FLOAT,
    CHAR, STRING,
    VAR,  KEYWORD,
    OP,   PUNC
  };

  static std::map<Type, std::string_view> TypeToString;

  Type type;
  std::string value;
  std::tuple<uint64, uint64> pos;

  bool operator!() const;
  bool operator~() const;
  friend std::ostream& operator<<(std::ostream&, const Token&);
};

class UnexpectedInputException : public Exception
{ };

class Lexer {
public:
  Lexer(CharStream*);

  std::string rdWhile(const CharPred&);
  std::string rdEscaped(const char&);

  Token rdNumber();
  Token rdID();

  template<Token::Type, char>
  Token rdQuoted();

  Token rdOp();
  Token rdPunc();

  Token rdNext();

  const Token& peek();
  const Token next();
  bool eof();

private:
  CharStream* m_data;
  Token m_current;
};
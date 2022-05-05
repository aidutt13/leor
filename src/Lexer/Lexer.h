#pragma once

#ifndef LEOR_LEXER_H
#define LEOR_LEXER_H

#include <functional>
#include <unordered_map>

#include "Input/CharStream.h"
#include "Lexer/RegExs.h"


namespace leor
{
  template<typename _Key, typename _Tp,
    typename _Hash = std::hash<_Key>,
    typename _Pred = std::equal_to<_Key>,
    typename _Alloc = std::allocator<std::pair<const _Key, _Tp>>>
  using hash_map = std::unordered_map<_Key, _Tp, _Hash, _Pred, _Alloc>;
  
  // Struct Token - A token with a type and value
  struct Token
  {
    using TokenPos = CharStream::StreamPos;
    enum class Type
    {
      NONE, EOB,
      INT, FLOAT, STRING, CHAR,
      VAR, KEYWORD,
      OP, PUNC
    };
    static const hash_map<Type, std::string> TypeToString;

    Type type;
    std::string value;
    TokenPos pos;

    Token();
    Token(Type type, const std::string& value);
    Token(Type type, const std::string& value, const TokenPos& pos);

    std::string toString() const;

    // Check if the token is NONE
    bool isNone() const;

    // Check if the token is EOB
    bool isEOB() const;

    // Operator '!' to check if the token is NONE or EOB
    bool operator!() const;
  };

  // Map of escape sequences
  extern const hash_map<char, char> ESCAPE_SEQUENCES;

  // Class Lexer - A lexer for a stream of characters
  class Lexer
  {
  private:
    CharStream m_stream;
    Token m_current;
  
  public:
    Lexer(const std::string& buffer);

    std::string rdWhile(const std::function<bool(int8_t)>& pred);
    std::string rdWhile(const std::function<bool(const std::string&)>& pred);
    std::string rdEsc(const char& end);

    void skipWhitespace();
    void skipComment();

    Token rdNumber();
    Token rdID();
    Token rdString();
    Token rdChar();
    Token rdNext();

    // Get the current token
    Token peek();
    // Get the next token
    Token get();
    //EOF check
    bool eof();
  };

} // namespace leor


#endif //LEOR_LEXER_H
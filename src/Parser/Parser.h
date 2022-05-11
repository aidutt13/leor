#pragma once

#ifndef LEOR_PARSER_H
#define LEOR_PARSER_H

#include "Parser/AST.h"
#include "Lexer/Lexer.h"

#include <functional>

namespace leor
{
  class Parser
  {
  private:
    Lexer m_lexer;

    Token IsPunc(const std::string& c = "");
    Token IsOp(const std::string& c = "");
    Token IsKeyword(const std::string& c = "");

    void SkipPunc(const std::string& c);
    void SkipOp(const std::string& c);
    void SkipKeyword(const std::string& c);

    AST MaybeBinary(AST lhs, uint64_t myPrec);
    AST MaybeCall(std::function<AST()> f);

    AST ParseCall(AST func);
    AST ParseFunction();
    AST ParseVariable();
    AST ParseBool();
    AST ParseProg();

    std::string ParseVarname();
    AST ParseExpression();
    AST ParseAtom();

    AST ParseToplevel();

  private:
    std::vector<AST> Delimited(
      const std::string& beg,
      const std::string& end,
      const std::string& sep,
      std::function<AST()> parser
    );
    
  public:
    Parser(const std::string& buffer);

    AST operator()();
  };
  
} // namespace leor


#endif // LEOR_PARSER_H
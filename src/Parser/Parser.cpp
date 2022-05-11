#include "Parser/Parser.h"

namespace leor
{

  Token Parser::IsPunc(const std::string& c)
  {
    auto tok = m_lexer.peek();
    if (tok.type == Token::Type::PUNC && (c.empty() || tok.value == c))
    {
      return tok;
    }
    return Token();
  }

  Token Parser::IsOp(const std::string& c)
  {
    auto tok = m_lexer.peek();
    if (tok.type == Token::Type::OP && (c.empty() || tok.value == c))
    {
      return tok;
    }
    return Token();
  }

  Token Parser::IsKeyword(const std::string& c)
  {
    auto tok = m_lexer.peek();
    if (tok.type == Token::Type::KEYWORD && (c.empty() || tok.value == c))
    {
      return tok;
    }
    return Token();
  }

  void Parser::SkipPunc(const std::string& c)
  {
    auto tok = IsPunc(c);
    if (tok.type == Token::Type::PUNC)
    {
      m_lexer.get();
    }
    else
    {
      auto [row, col] = tok.pos;
      std::stringstream ss;
      ss << "Error:" << row << ":" << col << ": Expected punctuation: " << c;
      throw std::runtime_error(ss.str());
    }
  }

  void Parser::SkipOp(const std::string& c)
  {
    auto tok = IsOp(c);
    if (tok.type == Token::Type::OP)
    {
      m_lexer.get();
    }
    else
    {
      auto [row, col] = tok.pos;
      std::stringstream ss;
      ss << "Error:" << row << ":" << col << ": Expected operator: " << c;
      throw std::runtime_error(ss.str());
    }
  }

  void Parser::SkipKeyword(const std::string& c)
  {
    auto tok = IsKeyword(c);
    if (tok.type == Token::Type::KEYWORD)
    {
      m_lexer.get();
    }
    else
    {
      auto [row, col] = tok.pos;
      std::stringstream ss;
      ss << "Error:" << row << ":" << col << ": Expected keyword: " << c;
      throw std::runtime_error(ss.str());
    }
  }

  AST Parser::MaybeBinary(AST lhs, uint64_t myPrec)
  {
    auto tok = IsOp();
    if (!!tok)
    {
      auto hisPrec = OP_PRECEDENCE.at(tok.value);
      if (hisPrec > myPrec)
      {
        auto pos = m_lexer.get().pos;
        return MaybeBinary(
          (
            AST::Binary(
              tok.value,
              std::move(lhs),
              MaybeBinary(ParseAtom(), hisPrec),
              pos
            )
          ), myPrec
        );
      }
    }
    return lhs;
  }

  AST Parser::MaybeCall(std::function<AST()> f)
  {
    auto expr = f();
    return !!IsPunc("(") ? ParseCall(std::move(expr)) : std::move(expr);
  }

  AST Parser::ParseCall(AST func)
  {
    auto pos = m_lexer.peek().pos;
    return (
      AST::Call(
        std::move(func),
        Delimited(
          "(",
          ")",
          ",",
          std::bind(&Parser::ParseExpression, this)
        ),
        pos
      )
    );
  }

  std::vector<AST> Parser::Delimited(
    const std::string& beg,
    const std::string& end,
    const std::string& sep,
    std::function<AST()> parser
  ) {

    std::vector<AST> res;
    bool first = true;

    SkipPunc(beg);
    while (!m_lexer.eof())
    {
      // If the first token is the end token, return
      if (!!IsPunc(end))
      {
        break;
      }

      // Skip separator if it's not the first element
      if (first)
      {
        first = false;
      }
      else
      {
        SkipPunc(sep);
      }

      // If the next token is the end token, return
      if (!!IsPunc(end))
      {
        break;
      }
      
      res.push_back(parser());
    }
    SkipPunc(end);
    return res;
  }

  AST Parser::ParseFunction()
  {
    auto pos = m_lexer.peek().pos;

    SkipKeyword("def");
    auto name = ParseVarname();
    auto args = Delimited(
      "(",
      ")",
      ",",
      std::bind(&Parser::ParseExpression, this) // TODO: ParseVar
    );
    SkipOp("->");
    auto type = ParseVarname();
    auto body = ParseExpression();

    return (
      AST::Function(
        name,
        std::move(args),
        std::move(body),
        type,
        pos
      )
    );
  }

  AST Parser::ParseVariable()
  {
    bool isConst = !!IsKeyword("const");
    m_lexer.get();

    auto name = ParseVarname();
    SkipOp(":");
    auto type = ParseVarname();

    auto value = AST::None();
    if (!!IsOp("="))
    {
      m_lexer.get();
      value = ParseExpression();
    }

    return AST::Variable(name, type, value, isConst);
  }

  AST Parser::ParseBool()
  {
    auto pos = m_lexer.peek().pos;
    return (
      AST::Bool(
        m_lexer.get().value == "true",
        pos
      )
    );
  }

  AST Parser::ParseProg()
  {
    auto pos = m_lexer.peek().pos;
    auto prog = Delimited("{", "}", ";", std::bind(&Parser::ParseExpression, this));
    return (
      AST::Prog(
        std::move(prog),
        pos
      )
    );
  }

  std::string Parser::ParseVarname()
  {
    auto name = m_lexer.get();
    if (name.type != Token::Type::VAR)
    {
      auto [row, col] = name.pos;
      std::stringstream ss;
      ss << "Error:" << row << ":" << col << ": Expected variable name";
      throw std::runtime_error(ss.str());
    }
    return name.value;
  }

  AST Parser::ParseAtom()
  {
    return MaybeCall([this]() -> AST {
      if (!!IsPunc("("))
      {
        m_lexer.get();
        auto expr = ParseExpression();
        SkipPunc(")");
        return expr;
      }

      if (!!IsPunc("{"))
      {
        return ParseProg();
      }

      if (!!IsKeyword("true") || !!IsKeyword("false"))
      {
        return ParseBool();
      }

      if (!!IsKeyword("def"))
      {
        return ParseFunction();
      }

      if (!!IsKeyword("const") || !!IsKeyword("mut"))
      {
        return ParseVariable();
      }

      auto tok = m_lexer.get();
      if (tok.type == Token::Type::INT)
      {
        return AST::Int(std::stoi(tok.value));
      }
      if (tok.type == Token::Type::FLOAT)
      {
        return AST::Float(std::stof(tok.value));
      }
      if (tok.type == Token::Type::STRING)
      {
        return AST::String(std::move(tok.value));
      }
      if (tok.type == Token::Type::CHAR)
      {
        return AST::Char(tok.value.at(0));
      }
      if (tok.type == Token::Type::VAR)
      {
        return AST::Var(tok.value);
      }

      throw std::runtime_error("Unexpected token");
    });
  }

  AST Parser::ParseExpression()
  {
    return MaybeCall([this]() -> AST {
      auto lhs = ParseAtom();
      return MaybeBinary(std::move(lhs), 0);
    });
  }

  AST Parser::ParseToplevel()
  {
    auto pos = m_lexer.peek().pos;
    std::vector<AST> prog;
    while (!m_lexer.eof())
    {
      prog.push_back(ParseExpression());
      SkipPunc(";");
    }
    return (AST::Prog(std::move(prog), pos));
  }

  Parser::Parser(const std::string& buffer)
    : m_lexer(buffer)
  { }

  AST Parser::operator()()
  {
    return ParseToplevel();
  }
  
} // namespace leor

#include "parser/parser.h"

#ifdef LEOR_DEBUG
  #include <iostream>
#endif

using namespace std::literals;

UnexpectedTokenException&& UnexpectedTokenException::of(const Token& found)
{
  UnexpectedTokenException ex;
  auto& [row, col] = found.pos;
  ex.stream() << "ERROR: "
    << row << ':' << col
    << ": Unexpected token TokenType::" << found.type << " '" << found.value << "'\n";

  return std::move(ex);
}

UnexpectedTokenException&& UnexpectedTokenException::of(const Token& found, const Token& expected)
{
  UnexpectedTokenException ex;
  auto& [row, col] = found.pos;
  ex.stream() << "ERROR: "
    << row << ':' << col
    << ": Unexpected token TokenType::" << found.type << " '" << found.value << "'\n"
    <<   "Expected TokenType::" << expected.type << " '" << expected.value << "'\n";

  return std::move(ex);
}

Parser::Parser(Lexer* data)
{
  m_data = data;
}

AST Parser::operator()()
{
  return parseToplevel();
}

const std::map<std::string_view, uint64> Parser::OpPrecedence
{{
  { "="sv,  1  },
  { "||"sv, 2  },
  { "&&"sv, 3  },
  { "<"sv,  7  },
  { ">"sv,  7  },
  { "<="sv, 7  },
  { ">="sv, 7  },
  { "=="sv, 7  },
  { "!="sv, 7  },
  { "+"sv,  10 },
  { "-"sv,  10 },
  { "*"sv,  20 },
  { "/"sv,  20 },
  { "%"sv,  20 }
}};

const std::map<std::string_view, AST::Type> Parser::OpBinaryType
{{
  { "="sv,  AST::ASSIGN  },
  { "||"sv, AST::BINARY  },
  { "&&"sv, AST::BINARY  },
  { "<"sv,  AST::BINARY  },
  { ">"sv,  AST::BINARY  },
  { "<="sv, AST::BINARY  },
  { ">="sv, AST::BINARY  },
  { "=="sv, AST::BINARY  },
  { "!="sv, AST::BINARY  },
  { "+"sv,  AST::BINARY  },
  { "-"sv,  AST::BINARY  },
  { "*"sv,  AST::BINARY  },
  { "/"sv,  AST::BINARY  },
  { "%"sv,  AST::BINARY  }
}};

Token Parser::isOp()
{
  Token& tok = m_data->peek();
  return true
    && !!tok
    && !~tok
    && tok.type == Token::OP
      ? tok
      : Token{ Token::NONE, "", tok.pos };
}

Token Parser::isOp(const std::string_view& x)
{
  Token& tok = m_data->peek();
  return true
    && !!tok
    && !~tok
    && tok.type == Token::OP
    && tok.value == x
      ? tok
      : Token{ Token::NONE, "", tok.pos };
}

Token Parser::isKw()
{
  Token& tok = m_data->peek();
  return true
    && !!tok
    && !~tok
    && tok.type == Token::KEYWORD
      ? tok
      : Token{ Token::NONE, "", tok.pos };
}

Token Parser::isKw(const std::string_view& x)
{
  Token& tok = m_data->peek();
  return true
    && !!tok
    && !~tok
    && tok.type == Token::KEYWORD
    && tok.value == x
      ? tok
      : Token{ Token::NONE };
}

Token Parser::isPunc()
{
  Token& tok = m_data->peek();
  return true
    && !!tok
    && !~tok
    && tok.type == Token::PUNC
      ? tok
      : Token{ Token::NONE };
}

Token Parser::isPunc(const std::string_view& x)
{
  Token& tok = m_data->peek();
  return true
    && !!tok
    && !~tok
    && tok.type == Token::PUNC
    && tok.value == x
      ? tok
      : Token{ Token::NONE };
}

void Parser::skipOp(const std::string_view& x)
{
  auto tok = m_data->peek();
  if (!isOp(x))
  {
    std::string msg{x};
    throw UnexpectedTokenException::of(tok, Token{Token::OP, msg});
  }
  m_data->next();
}

void Parser::skipKw(const std::string_view& x)
{
  auto tok = m_data->peek();
  if (!isKw(x))
  {
    std::string msg{x};
    throw UnexpectedTokenException::of(tok, Token{Token::KEYWORD, msg});
  }
  m_data->next();
}

void Parser::skipPunc(const std::string_view& x)
{
  auto tok = m_data->peek();
  if (!isPunc(x))
  {
    std::string msg{x};
    throw UnexpectedTokenException::of(tok, Token{Token::PUNC, msg});
  }
  m_data->next();
}

std::string Parser::parseVarname()
{
  auto tok = m_data->peek();
  if (tok.type != Token::VAR)
  {
    throw UnexpectedTokenException::of(tok, Token{Token::VAR, ""});
  }
  return tok.value;
}

template<typename T>
std::vector<T> Parser::delimited(char beg, char end, char sep, std::function<T()> parser)
{
  const std::string_view begsv{&beg, 1}, endsv{&end, 1}, sepsv{&sep, 1};

  std::vector<T> ret;
  bool first = true;
  skipPunc(begsv);

  while (!isPunc(endsv))
  {
    if (!first)
    {
      first = false;
      skipPunc(sepsv);
    }
    ret.push_back(parser());
  }
  return std::move(ret);
};

AST Parser::parseToplevel()
{
  std::vector<AST> prog;
  auto pos = m_data->peek().pos;

  while (!m_data->eof())
  {
    prog.push_back(parseExpression());
    skipPunc(";"sv);
  }
  return { AST::PROG, new ProgNode{ std::move(prog) }, pos };
}

AST Parser::parseProg()
{
  auto pos = m_data->peek().pos;
  auto prog = delimited<AST>(
    '{', '}', ';', std::bind(&Parser::parseExpression, this)
  );
  return
  {
    AST::PROG,
    new ProgNode{ std::move(prog) },
    pos
  };
}

AST Parser::parseVar()
{
  auto pos = m_data->peek().pos;
  bool isConst = !!isKw("const");

  m_data->next();

  auto name = parseVarname();
  skipOp(":");
  auto typeName = parseVarname();

  AST value{ AST::NONE };

  if (!!isOp("="))
  {
    skipOp("=");
    value = parseExpression();
  }
  
  return AST
  {
    AST::VARDEF,
    new VarDefNode
      { name, typeName, isConst, std::move(value) },
    pos
  };
}

AST Parser::parseFunction()
{
  auto pos = m_data->peek().pos;
  skipKw("def"sv);
  auto name = parseVarname();
  auto params = delimited<AST>('(', ')', ',', std::bind(&Parser::parseVar, this));
  skipOp("->"sv);
  auto returnType = parseVarname();
  auto body = parseProg();

  return
  {
    AST::FUNCTION,
    new FunctionNode{ name, returnType, std::move(params), std::move(body) },
    pos
  };
}

AST Parser::parseCall(AST func)
{
  auto pos = m_data->peek().pos;
  return
  {
    AST::CALL,
    new CallNode
    {
      func,
      delimited<AST>('(', ')', ',', std::bind(&Parser::parseExpression, this))
    },
    pos
  };
}

AST Parser::parseBool()
{
  auto tok = m_data->next();
  return
  {
    AST::BOOL,
    new BoolNode{ tok.value == "true" },
    tok.pos
  };
}

AST Parser::parseExpression()
{
  return maybeCall([this]() -> AST {
    return this->maybeBinary(parseAtom(), 0);
  });
}

AST Parser::maybeCall(std::function<AST()> expr)
{
  auto e = expr();
  return !!isPunc("("sv) ? parseCall(std::move(e)) : std::move(e);
}

AST Parser::maybeBinary(AST left, uint64 myPrec)
{
  auto tok = isOp();
  if (!tok)
    return left;

  uint64 hisPrec = OpPrecedence.at(tok.value);
  if (hisPrec <= myPrec)
    return left;
  
  m_data->next();
  return AST
  {
    OpBinaryType.at(tok.value),
    new BinaryNode
    {
      tok.value,
      std::move( left ),
      std::move( maybeBinary(parseAtom(), hisPrec) )
    },
    tok.pos
  };
}

AST Parser::parseAtom()
{
  return maybeCall([this]() -> AST {
    if ( !!this->isPunc("("sv) )
    {
      this->m_data->next();
      auto e = this->parseExpression();
      this->skipPunc(")"sv);
      return e;
    }
    if ( !!this->isPunc("{"sv) )
      return this->parseProg();
    if ( !!this->isKw("true"sv) || !!this->isKw("false"sv) )
      return this->parseBool();
    if ( !!this->isKw("def"sv) )
      return this->parseFunction();
    if (!!this->isKw("const") || !!this->isKw("mut"))
      return this->parseVar();
    
    auto tok = this->m_data->next();
    switch (tok.type)
    {
    case Token::CHAR:
      return {
        AST::CHAR,
        new CharNode{ tok.value.at(0) },
        tok.pos
      };
    case Token::INT:
      return {
        AST::INT,
        new IntNode{ std::stoll(tok.value) },
        tok.pos
      };
    case Token::STRING:
      return {
        AST::INT,
        new StringNode{ std::move(tok.value) },
        tok.pos
      };
    case Token::VAR:
      return {
        AST::VAR,
        new StringNode{ std::move(tok.value) },
        tok.pos
      };
    default:
      break;
    }
    throw UnexpectedTokenException::of(tok);
  });
}
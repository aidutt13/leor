#include "lexer/lexer.h"

using namespace std::literals;

std::map<Token::Type, std::string_view> Token::TypeToString
{{
  { Token::NONE,    "NONE"sv    },
  { Token::EOB,     "EOB"sv     },
  { Token::INT,     "INT"sv     },
  { Token::FLOAT,   "FLOAT"sv   },
  { Token::CHAR,    "CHAR"sv    },
  { Token::STRING,  "STRING"sv  },
  { Token::VAR,     "VAR"sv     },
  { Token::KEYWORD, "KEYWORD"sv },
  { Token::OP,      "OP"sv      },
  { Token::PUNC,    "PUNC"sv    },
}};

bool Token::operator!() const
{
  return type == Type::NONE;
}

bool Token::operator~() const
{
  return type == Type::EOB;
}

std::ostream& operator<<(std::ostream& os, const Token& tok)
{
  auto& [row, col] = tok.pos;

  // std::cout << "Token::Type = '" << tok.type << "'\n";

  os << "{"
     << Token::TypeToString.at(tok.type) << ", "
     << "'" << tok.value << "', "
     << "(" << row << ":" << col << ") }";
  
  return os;
}

Lexer::Lexer(CharStream* data)
  : m_data(data)
{ }

std::string Lexer::rdWhile(const CharPred& pred)
{
  std::string ret;
  char c = m_data->peek();

  while (!m_data->eof() && pred({&c, 1}))
  {
    ret += m_data->next();
    c = m_data->peek();
  }
  return ret;
}

std::string Lexer::rdEscaped(const char& end)
{
  bool esc = false;
  std::string ret;
  auto func = [end, &esc, &ret](const std::string_view& csv) -> bool
  {

    const auto& c = csv.at(0);
    if (esc)
      esc = false;

    else if (c == end)
      return false;

    else if (c == '\\')
    {
      esc = true;
      return true;
    }
    ret += c;
    return true;
  };
  rdWhile(func);
  m_data->next();
  return ret;
}

Token Lexer::rdNumber()
{
  auto pos{ m_data->pos() };
  bool dot{ false };

  auto func = [&dot](const std::string_view& csv) -> bool
  {
    if (csv == ".")
    {
      if (dot) return false;
      else
      {
        dot = true;
        return true;
      }
    }
    return ct::isDigit(csv);
  };

  return Token
  {
    dot ? Token::FLOAT : Token::INT,
    rdWhile(func).data(),
    pos
  };
}

Token Lexer::rdID()
{
  auto pos = m_data->pos();
  auto id = rdWhile(ct::isID);
  return Token
  {
    ct::isKeyword(id) ? Token::KEYWORD : Token::VAR,
    id,
    pos
  };
}

template<Token::Type TYPE, char QUOTE>
Token Lexer::rdQuoted()
{
  auto pos = m_data->pos();
  m_data->next();
  return Token
  {
    TYPE,
    rdEscaped(QUOTE),
    pos
  };
}

Token Lexer::rdOp()
{
  auto pos = m_data->pos();
  return Token
  {
    Token::OP,
    rdWhile(ct::isOp),
    pos
  };
}

Token Lexer::rdPunc()
{
  auto pos = m_data->pos();
  return Token
  {
    Token::PUNC,
    std::string{ 1, m_data->next() },
    pos
  };
}

Token Lexer::rdNext()
{
  if (m_data->eof())
    return Token{Token::EOB};

  rdWhile(ct::isWS);

  if (m_data->eof())
    return Token{Token::EOB};

  auto c = m_data->peek();
  auto csv = std::string_view{&c, 1};
  if (ct::isComment(csv))
  {
    rdWhile(ct::isNotNL);
    return rdNext();
  }
  
  if (ct::isDigit(csv))
    return rdNumber();

  if (ct::isID(csv))
    return rdID();

  if (ct::isCharQutoe(csv))
    return rdQuoted<Token::CHAR, '\''>();
  if (ct::isStringQuote(csv))
    return rdQuoted<Token::STRING, '\"'>();

  if (ct::isOp(csv))
    return rdOp();
  if (ct::isPunc(csv))
    return rdPunc();

  auto exc = UnexpectedInputException();
  auto [row, col] = m_data->pos();

  exc.stream()
    << "File:" << row << ":" << col
    << ": Unexpected character `" << c << "`\n";

  exc.flush();

  throw std::move(exc);
}

const Token& Lexer::peek()
{
  if (!m_current)
    m_current = rdNext();
  return m_current;
}

const Token Lexer::next()
{
  if (eof())
    return m_current;
  const auto tok = peek();
  m_current = Token{ Token::NONE };
  return tok; 
}

bool Lexer::eof()
{
  return ~peek();
}
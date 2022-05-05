#include "Lexer/Lexer.h"

namespace leor
{
  
  Token::Token()
    : type(Type::NONE), value("")
  { }

  Token::Token(Type type, const std::string& value)
    : type(type), value(value), pos(std::make_tuple(0, 0))
  { }

  Token::Token(Type type, const std::string& value, const TokenPos& pos)
    : type(type), value(value), pos(pos)
  { }

  const hash_map<Token::Type, std::string> Token::TypeToString =
  {
    { Type::NONE,    "NONE" },
    { Type::EOB,     "EOB" },
    { Type::INT,     "INT" },
    { Type::FLOAT,   "FLOAT" },
    { Type::STRING,  "STRING" },
    { Type::CHAR,    "CHAR" },
    { Type::VAR,     "VAR" },
    { Type::KEYWORD, "KEYWORD" },
    { Type::OP,      "OP" },
    { Type::PUNC,    "PUNC" }
  };

  std::string Token::toString() const
  {
    return std::string("(") + std::to_string(std::get<0>(pos)) + ", " + std::to_string(std::get<1>(pos)) + ")\t" + TypeToString.at(type) + "\t  " + value;
  }

  bool Token::isNone() const
  {
    return type == Type::NONE;
  }

  bool Token::isEOB() const
  {
    return type == Type::EOB;
  }

  bool Token::operator!() const
  {
    return isNone() || isEOB();
  }

  const hash_map<char, char> ESCAPE_SEQUENCES =
  {
    { 'n', '\n' },
    { 'r', '\r' },
    { 't', '\t' },
    { 'v', '\v' },
    { 'b', '\b' },
    { 'a', '\a' },
    { 'f', '\f' },
    { '\\', '\\' },
    { '\'', '\'' },
    { '\"', '\"' },
    { '0', '\0' }
  };

  Lexer::Lexer(const std::string& buffer)
    : m_stream(buffer), m_current(Token::Type::NONE, "")
  { }

  void Lexer::skipWhitespace()
  {
    rdWhile(isWHITESPACE);
  }

  void Lexer::skipComment()
  {
    rdWhile([](int8_t c) { return c != '\n'; });
  }

  std::string Lexer::rdWhile(const std::function<bool(int8_t)>& pred)
  {
    std::string result;
    while (!m_stream.eof() && pred(m_stream.peek()))
    {
      result += m_stream.get();
    }
    return result;
  }

  std::string Lexer::rdWhile(const std::function<bool(const std::string&)>& pred)
  {
    std::string result;
    while (!m_stream.eof() && pred(std::string(1, m_stream.peek())))
    {
      result += m_stream.get();
    }
    return result;
  }

  std::string Lexer::rdEsc(const char& end)
  {
    std::string result;
    m_stream.get();
    char c = m_stream.get();
    bool esc = false;
    while (!m_stream.eof() && c != end)
    {
      if (esc)
      {
        result += ESCAPE_SEQUENCES.at(c);
        esc = false;
      }
      else if (c == '\\')
      {
        esc = true;
      }
      else
      {
        result += c;
      }
      c = m_stream.get();
    }
    return result;
  }

  Token Lexer::rdNumber()
  {
    auto pos = m_stream.getPos();

    std::string result;
    bool dot = false;
    auto f = [&dot](int8_t c) -> bool
    {
      if (c == '.') {
        if (dot) {
          return false;
        }
        dot = true;
        return true;
      }
      return std::regex_match(std::string(1, c), RegExs::DIGIT);
    };
    std::string num = rdWhile(f);
    return Token
    (
      dot ? Token::Type::FLOAT : Token::Type::INT,
      num,
      pos
    );
  }

  Token Lexer::rdID()
  {
    auto pos = m_stream.getPos();

    std::string result = rdWhile(isID);

    return Token
    (
      std::regex_match(result, RegExs::KEYWORD) ? Token::Type::KEYWORD : Token::Type::VAR,
      result,
      pos
    );
  }

  Token Lexer::rdString()
  {
    auto pos = m_stream.getPos();

    std::string result = rdEsc('\"');

    return Token
    (
      Token::Type::STRING,
      result,
      pos
    );
  }

  Token Lexer::rdChar()
  {
    auto pos = m_stream.getPos();

    std::string result = rdEsc('\'');

    return Token
    (
      Token::Type::CHAR,
      result,
      pos
    );
  }

  Token Lexer::rdNext()
  {
    skipWhitespace();
    if (std::regex_match(std::string(1, m_stream.peek()), RegExs::COMMENT))
    {
      skipComment();
      return rdNext();
    }
    if (m_stream.eof())
    {
      return Token(Token::Type::EOB, "", m_stream.getPos());
    }
    auto c = m_stream.peek();
    auto sc = std::string(1, c);

    if (std::regex_match(sc, RegExs::DIGIT))
    {
      return rdNumber();
    }
    if (std::regex_match(sc, RegExs::ID))
    {
      return rdID();
    }
    if (c == '\"')
    {
      return rdString();
    }
    if (c == '\'')
    {
      return rdChar();
    }
    if (std::regex_match(sc, RegExs::OP))
    {
      auto pos = m_stream.getPos();
      return Token(Token::Type::OP, rdWhile(isOP), pos);
    }
    if (std::regex_match(sc, RegExs::PUNC))
    {
      auto pos = m_stream.getPos();
      m_stream.get();
      return Token(Token::Type::PUNC, sc, pos);
    }
    auto pos = m_stream.getPos();
    std::stringstream err;
    err << std::get<0>(pos) << ":" << std::get<1>(pos) << ":Error: Unexpected character '" << sc << "'";
    throw std::runtime_error(err.str());
  }

  Token Lexer::peek()
  {
    if (m_current.isNone())
    {
      m_current = rdNext();
    }
    return m_current;
  }

  Token Lexer::get()
  {
    auto result = peek();
    m_current = Token();
    return result;
  }

  bool Lexer::eof()
  {
    return peek().isEOB();
  }

} // namespace leor

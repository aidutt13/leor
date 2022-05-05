#include "Lexer/RegExs.h"

namespace leor
{
  
  std::regex operator "" _re(const char* str, size_t len)
  {
    return std::regex(str, std::regex::optimize);
  }
  
  const std::regex RegExs::WHITESPACE = "\\s"_re;
  const std::regex RegExs::COMMENT = "#"_re;
  const std::regex RegExs::ID = "[a-zA-Z_][a-zA-Z0-9_]*"_re;
  const std::regex RegExs::DIGIT = "[0-9]"_re;
  const std::regex RegExs::CHAR_QUOTE = "\'"_re;
  const std::regex RegExs::STRING_QUOTE = "\""_re;
  const std::regex RegExs::OP = "[+\\-*/%=<>!&|^]"_re;
  const std::regex RegExs::PUNC = "[\\(\\)\\[\\]\\{\\}\\;\\,]"_re;
  const std::regex RegExs::KEYWORD = "true|false|def|return"_re;

  ReMatchFunction::ReMatchFunction(const std::regex& re)
    : m_re(re)
  { }

  bool ReMatchFunction::operator()(const std::string& str)
  {
    return std::regex_match(str, m_re);
  }

  const ReMatchFunction isWHITESPACE = RegExs::WHITESPACE;
  const ReMatchFunction isCOMMENT = RegExs::COMMENT;
  const ReMatchFunction isID = RegExs::ID;
  const ReMatchFunction isDIGIT = RegExs::DIGIT;
  const ReMatchFunction isCHAR_QUOTE = RegExs::CHAR_QUOTE;
  const ReMatchFunction isSTRING_QUOTE = RegExs::STRING_QUOTE;
  const ReMatchFunction isOP = RegExs::OP;
  const ReMatchFunction isPUNC = RegExs::PUNC;
  const ReMatchFunction isKEYWORD = RegExs::KEYWORD;
  
} // namespace leor

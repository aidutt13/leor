#pragma once

#ifndef LEOR_REGEXS_H
#define LEOR_REGEXS_H

#include <regex>

namespace leor
{
  std::regex operator "" _re(const char* str, size_t len);

  // Class RegExs - A collection of regular expressions
  class RegExs
  {
  private:
    RegExs() { }
  public:
    static const std::regex WHITESPACE;
    static const std::regex COMMENT;
    static const std::regex ID;
    static const std::regex DIGIT;
    static const std::regex CHAR_QUOTE;
    static const std::regex STRING_QUOTE;
    static const std::regex OP;
    static const std::regex PUNC;
    static const std::regex KEYWORD;
  };

  class ReMatchFunction
  {
  private:
    const std::regex& m_re;
  public:
    ReMatchFunction(const std::regex& re);

    bool operator()(const std::string& str);
  };

  extern const ReMatchFunction isWHITESPACE;
  extern const ReMatchFunction isCOMMENT;
  extern const ReMatchFunction isID;
  extern const ReMatchFunction isDIGIT;
  extern const ReMatchFunction isCHAR_QUOTE;
  extern const ReMatchFunction isSTRING_QUOTE;
  extern const ReMatchFunction isOP;
  extern const ReMatchFunction isPUNC;
  extern const ReMatchFunction isKEYWORD;

} // namespace leor


#endif //LEOR_REGEXS_H
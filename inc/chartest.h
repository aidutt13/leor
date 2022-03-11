#pragma once

#include <regex>
#include <string_view>

#include "myutils.h"

namespace _ct {

struct BasicCharTest {
  std::regex rgx;
  bool res = true;

  inline bool operator()(const std::string_view& target) const
  {
    auto str = std::string(target);
    return std::regex_match(str, rgx) == res;
  }
};

};

class ct {
public:
  ct() = delete;

  static const _ct::BasicCharTest isWS;
  static const _ct::BasicCharTest isNotNL;
  static const _ct::BasicCharTest isComment;

  static const _ct::BasicCharTest isDigit;
  static const _ct::BasicCharTest isID;
  
  static const _ct::BasicCharTest isCharQutoe;
  static const _ct::BasicCharTest isStringQuote;

  static const _ct::BasicCharTest isOp;
  static const _ct::BasicCharTest isPunc;

  static const _ct::BasicCharTest isKeyword;
};

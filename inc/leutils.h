#pragma once

#include <regex>

namespace literals {
  inline namespace regex {
    std::regex operator"" _re(const char*, unsigned long);
  }
}
#include "leutils.h"

namespace literals{

  std::regex regex::operator"" _re(const char* str, unsigned long)
  {
    return std::regex(str);
  }
}
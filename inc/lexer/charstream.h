#pragma once

#include <string>
#include <string_view>
#include <tuple>

#include "mytypes.h"

class CharStream {
public:
  CharStream(const std::string &);

  char peek();
  char next();
  bool eof();
  std::tuple<uint64, uint64> pos();

  uint64 iterator() const;
  std::string_view data() const;

private:
  uint64 m_it{ 0 }, m_row{ 1 }, m_col{ 1 };
  std::string_view m_data;
};
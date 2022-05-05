#pragma once

#ifndef LEOR_CHARSTREAM_H
#define LEOR_CHARSTREAM_H

#include <string>
#include <tuple>

namespace leor
{

  // Class CharStream - A simple stream of characters with row and column tracking
  class CharStream
  {
  public:
    using StreamPos = std::tuple<uint64_t, uint64_t>;
  private:
    std::string m_buffer;
    uint64_t m_row, m_col;
    uint64_t m_it;

  public:
    CharStream(const std::string& buffer);

    // Peek the current character without advancing the stream
    int8_t peek();

    // Get the current character and advance the stream
    int8_t get();

    // EOF check
    bool eof();

    // Get the current row and column
    StreamPos getPos();
  };

} // namespace leor

#endif //LEOR_CHARSTREAM_H
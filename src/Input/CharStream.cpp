#include "Input/CharStream.h"

namespace leor
{

  CharStream::CharStream(const std::string& buffer)
    : m_buffer(buffer), m_col(0), m_row(0), m_it(0)
  { }

  int8_t CharStream::peek()
  {
    return m_buffer[m_it];
  }

  int8_t CharStream::get()
  {
    int8_t c = m_buffer[m_it];
    if (c == '\n') {
      m_col = 0;
      m_row++;
    } else {
      m_col++;
    }
    m_it++;
    return c;
  }

  bool CharStream::eof()
  {
    return m_it >= m_buffer.size();
  }

  CharStream::StreamPos CharStream::getPos()
  {
    return std::make_tuple(m_row, m_col);
  }

}
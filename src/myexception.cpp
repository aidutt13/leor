#include "myexception.h"

Exception::Exception()
{}

const char* Exception::what() const noexcept
{
  return m_str.c_str();
}

std::ostream& Exception::stream()
{
  return m_stream;
}

void Exception::flush()
{
  m_str = m_stream.str();
}
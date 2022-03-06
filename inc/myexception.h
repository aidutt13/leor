#pragma once

#include <exception>
#include <sstream>

class Exception : public std::exception
{
public:
  Exception();
  const char* what() const noexcept;

  std::ostream& stream();
  void flush();

private:
  std::ostringstream m_stream;
  std::string m_str;
};
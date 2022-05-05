#include <fstream>
#include <iostream>
#include "Lexer/Lexer.h"

int32_t main()
{
  std::ifstream file("tests/hello.leor");
  std::stringstream buffer;
  buffer << file.rdbuf();
  leor::Lexer lexer(buffer.str());
  while (!lexer.eof())
  {
    std::cout << lexer.get().toString() << std::endl;
  }
  return 0;
}
#include <iostream>
#include <fstream>
#include <sstream>

#include "lexer.h"

int main() {

  std::stringstream ss;
  {
    std::ifstream fs("./tests/hello.leor");
    ss << fs.rdbuf();
  }

  const std::string data = ss.str();

  CharStream cs(data);
  Lexer lx = Lexer(&cs);

  try
  {
    while(!lx.eof())
      std::cout << lx.next() << std::endl;
  }
  catch (Exception& exc)
  {
    std::cerr << exc.what() << std::endl;
  }
  return 0;
}
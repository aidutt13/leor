#include <fstream>
#include <iostream>
#include "Parser/Parser.h"

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

  leor::Parser parser(buffer.str());
  auto ast = parser();
  if (ast.type == leor::AST::Type::PROG)
    std::cout << "ProgAST found" << std::endl;

  auto prog = std::get<std::vector<leor::AST>>(ast.at("prog"));

  for (auto& a : prog)
  {
    std::cout << (int64_t)a.type << std::endl;
  }
  return 0;
}
#include "lexer.hpp"
#include <iostream>

int main() {
  std::string code = "Hello, world!";

  Lexer lexer{code};

  lexer.print();

  return 0;
}
#pragma once
#include <string>

class Lexer {
public:
  Lexer(std::string source_code);

  void print();

private:
  std::string m_source_code;
};
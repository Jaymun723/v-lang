#include "lexer.hpp"
#include <iostream>

Lexer::Lexer(std::string source_code) : m_source_code{source_code} {}

void Lexer::print() { std::cout << m_source_code << std::endl; }
#pragma once

#include <cctype>
#include <string>

class Scanner {
private:
  std::string input;
  int line;
  int totalTokens;

public:
  Scanner(const std::string &input) : input(input) {line = 1;totalTokens=1;}

  Token scanToken() {
		TokenType type = UNDEFINED;
    std::string value = "";
    int line = 0;
    while (input.at(0)==' ') {
      input = input.substr(1);
    }
    if (input[0] == ',') {
      type = COMMA;
      value = ",";
      line = this->line;
			input = input.substr(1);
    }
    return Token(type, value, line);
  }
};
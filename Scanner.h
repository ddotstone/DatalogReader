#pragma once

#include "Token.h"
#include <ctype.h>
#include <iostream>
#include <string>
#include <stdexcept>
#include <vector>

class Scanner {
private:
  std::string input;
  int totalTokens;
  int line;
  std::vector<Token> tokens;

public:
  Scanner(const std::string &input) : input(input) {
    line = 1;
    totalTokens = 0;
  }

  void scanToken() {
    while (input.compare("") != 0) {
			int extraLines = 0;
      TokenType type = UNDEFINED;
      std::string value = "";
			while (checkNext(' ',0) || checkNext('\0',0)|| checkNext('\t',0) || checkNext('\n', 0)) {
        if (checkNext('\n', 0)) {
          ++line;
        }
        input.erase(input.begin());
      }
			if (input.compare("") == 0){
				break;
			}
      else if (input[0] == ',') {
        type = COMMA;
        value = ",";
        input.erase(input.begin());
      }

      else if (input[0] == '.') {
        type = PERIOD;
        value = ".";
        input.erase(input.begin());
      }

      else if (input[0] == '?') {
        type = Q_MARK;
        value = "?";
        input.erase(input.begin());
      }

      else if (input[0] == '(') {
        type = LEFT_PAREN;
        value = "(";
        input.erase(input.begin());
      }

      else if (input[0] == ')') {
        type = RIGHT_PAREN;
        value = ")";
        input.erase(input.begin());
      }

      else if (input[0] == ':') {
        if (checkNext('-', 1)) {
          type = COLON_DASH;
          value = ":-";
          input.erase(0, 2);
        } else {
          type = COLON;
          value = ":";
          input.erase(input.begin());
        }
      }

      else if (input[0] == '*') {
        type = MULTIPLY;
        value = "*";
        input.erase(input.begin());
      }

      else if (input[0] == '+') {
        type = ADD;
        value = "+";
        input.erase(input.begin());
      }

      else if (input[0] == '\'') {
        int pos2 = 1;
        while (!checkEnd(pos2, '\'')) {
          if (input.at(pos2) == '\n') {
            ++extraLines;
          }
          ++pos2;
        }
        value = input.substr(0, pos2 + 1);
        if (value.at(value.size() - 1) != '\'') {
          type = UNDEFINED;
        } else {
          type = STRING;
        }
        input.erase(input.begin(), input.begin() + value.size());
      }

      else if (input[0] == '"') {
        int pos2 = 1;
        while (!checkEnd(pos2, '"')) {
          if (input.at(pos2) == '\n') {
            ++extraLines;
          }

          ++pos2;
        }

        value = input.substr(0, pos2 + 1);

        if (value.at(value.size() - 1) != '"') {

          type = UNDEFINED;
        } else {

          type = STRING;
        }
        input.erase(input.begin(), input.begin() + value.size());
      }

      else if (isalpha(input[0])) {
        int pos2 = 1;
        while (alphanumCheck(pos2)) {
          ++pos2;
        }
        value = input.substr(0, pos2);

        if (value.compare("Facts") == 0) {
          type = FACTS;
        }

        else if (value.compare("Rules") == 0) {
          type = RULES;
        }

        else if (value.compare("Schemes") == 0) {
          type = SCHEMES;
        }

        else if (value.compare("Queries") == 0) {
          type = QUERIES;
        } else {
          type = ID;
        }

        input.erase(input.begin(), input.begin() + value.size());
      }

      else if (input[0] == '#') {

        if (checkNext('|', 1)) {

          int slen = input.length();
          int pos2 = 2;
          while (pos2 < slen - 1) {
            if (input.at(pos2) == '\n') {
              ++extraLines;
            }

            if (input.at(pos2) == '|') {
              if (checkNext('#', pos2 + 1)) {
                ++pos2;
                break;
              }
            }
            ++pos2;
          }
          value = input.substr(0, pos2 + 1);
          if (value.substr(value.size() - 2, 2).compare("|#")==0) {
            type = COMMENT;
          } else {
            type = UNDEFINED;
            ;
          }
        }

        else {
          int pos2 = 1;

          while (!checkEnd(pos2, '\n')) {
            ++pos2;
          }

          value = input.substr(0, pos2);
          type = COMMENT;
        }
        input.erase(input.begin(), input.begin() + value.size());
      }

      else {
        type = UNDEFINED;
        value = input[0];
        input.erase(input.begin());
      }

      if (value.compare("") == 0){
				break;
			}
			tokens.push_back(Token(type, value, line));
      line += extraLines;
      ++totalTokens;
    }
		
    tokens.push_back(Token(EOF_, "", line));
    ++totalTokens;
		return;
  }

  bool checkEnd(int place, char test) {
    try {
      if (test == input.at(place)) {
        return true;
      } else {
        return false;
      }
    } catch (const std::out_of_range &e) {
      return true;
    }
  }

  bool checkNext(char test, int place) {
		try {
			if (test == input.at(place)) {
				return true;
      } else {
         return false;
      }

    } catch (const std::out_of_range &e) {
			return false;
    }
		
  }

  void printTokens() {
    for (auto &token : this->tokens) {
      std::cout << token.toString() << std::endl;
    }

    std::cout << "Total Tokens = " << totalTokens << std::endl;
  }

  bool alphanumCheck(int place) {
    try {
      if (isalpha(input.at(place)) || isdigit(input.at(place))) {
        return true;
      } else {
        return false;
      }
    } catch (const std::out_of_range &e) {
      return false;
    }
  }

	std::vector<Token> getVector(){
		return tokens;
	}
};
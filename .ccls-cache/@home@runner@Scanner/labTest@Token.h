#pragma once

#include <string>
#include <sstream>
enum TokenType {
  COMMA, PERIOD,Q_MARK,LEFT_PAREN,RIGHT_PAREN,COLON,COLON_DASH,MULTIPLY,ADD,SCHEMES,FACTS,RULES,QUERIES,ID,STRING,COMMENT,UNDEFINED,EOF_
};

class Token {
 private:
  TokenType type;
  std::string value;
  int line;
	
 public:
  Token(TokenType type,std::string value,int line): type(type),value(value),line(line){}


  std::string toString() const{
		std::stringstream out;
    out << "(" << typeName(type) << "," << "\"" << value << "\"" << "," << line << ")";
    return out.str();
	}

  std::string typeName(TokenType type) const{
		std::string values[] = {"COMMA", "PERIOD","Q_MARK","LEFT_PAREN","RIGHT_PAREN","COLON","COLON_DASH","MULTIPLY","ADD","SCHEMES","FACTS","RULES","QUERIES","ID","STRING","COMMENT","UNDEFINED","EOF"};
		return values[type];
		}
		
};
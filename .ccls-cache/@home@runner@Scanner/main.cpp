#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

#include "Parameter.h"
#include "Parser.h"
#include "Predicate.h"
#include "Scanner.h"
#include "Token.h"
#include "Relation.h"

/*int main(int argc, char *argv[]) {

  std::ifstream outputFile(argv[1]);

  if (outputFile.is_open()) {
    std::stringstream text;
    std::string line;
    while (outputFile) {
      if (outputFile.eof())
        break;
      std::getline(outputFile, line);
      text << line << '\n';
    }

    outputFile.close();
    std::string stext = text.str();
    stext = stext.substr(0, stext.size() - 1);
    Scanner s = Scanner(stext);

    s.scanToken();
    //s.printTokens();

    Parser myParser = Parser(s.getVector());
		myParser.datalogProgram();
		std::cout<<myParser.toString();

  } else {
    std::cout << "Could not open file" << std::endl;
  }
}*/
int main() {

  std::vector<std::string> names = { "ID", "Name", "Major" };

  Scheme scheme(names);

  Relation relation("student", scheme);

  std::vector<std::string> values[] = {
    {"'42'", "'Ann'", "'CS'"},
    {"'32'", "'Bob'", "'CS'"},
    {"'64'", "'Ned'", "'EE'"},
    {"'16'", "'Jim'", "'EE'"},
  };

  for (auto& value : values) {
    Tuple tuple(value);
    std::cout << tuple.toString(scheme) << std::endl;
    relation.addTuple(tuple);
  }

  std::cout << "relation:" << std::endl;
  std::cout << relation.toString();

}


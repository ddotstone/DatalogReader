
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

#include "Parser.h"
#include "Scanner.h"
#include "Database.h"

int main(int argc, char *argv[]) {

  /*std::ifstream outputFile(argv[1]);

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
		//std::cout<<myParser.toString();
        Database database;
        database.processDatalog(myParser.getDatalogProgram());
        //std::cout << database.toString();

  } else {
    std::cout << "Could not open file" << std::endl;
  }*/
	 Relation studentRelation("students", Scheme( {"ID", "Name", "Major"} ));

  std::vector<std::string> studentValues[] = {
    {"'42'", "'Ann'", "'CS'"},
    {"'64'", "'Ned'", "'EE'"},
  };

  for (auto& value : studentValues)
    studentRelation.addTuple(Tuple(value));

	Relation courseRelation("courses", Scheme( {"ID", "Course"} ));

  std::vector<std::string> courseValues[] = {
    {"'42'", "'CS 100'"},
    {"'32'", "'CS 232'"},
  };

  for (auto& value : courseValues)
    courseRelation.addTuple(Tuple(value));

  studentRelation.join(courseRelation);
}




#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

#include "Parser.h"
#include "Scanner.h"
#include "Database.h"

/*int main(int argc, char* argv[]) {

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
        //std::cout<<myParser.toString();
        Database database;
        database.processDatalog(myParser.getDatalogProgram());
        //std::cout << database.toString();

    }
    else {
        std::cout << "Could not open file" << std::endl;
    }
}
*/
int main() {
    // predicate names for fake rules
  // first is name for head predicate
  // second is names for body predicates
    std::pair<std::string, std::vector<std::string>> ruleNames[] = {
  { "A", { "B", "C" } },
    { "B", { "A", "D" } },
    { "B", { "B" } },
    { "E", { "F", "G" } },
    { "E", { "E", "F" } },
    };

    std::vector<Rule> rules;

    for (auto& rulePair : ruleNames) {
        std::string headName = rulePair.first;
        Rule rule = Rule(Predicate(headName));
        std::vector<std::string> bodyNames = rulePair.second;
        for (auto& bodyName : bodyNames)
            rule.addPredicate(Predicate(bodyName));
        rules.push_back(rule);
    }

    Graph graph = Database::makeGraph(rules);
    std::cout << graph.toString();
}



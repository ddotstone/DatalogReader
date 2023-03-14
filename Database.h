#pragma once

#include <map>

#include "Relation.h"
#include "DatalogProgram.h"

class Database {
private:
	std::map<std::string,Relation> relations;
	std::vector<Predicate*> queries;
public:
	std::string toString() {
		std::stringstream s;
		for (auto& i : relations) {
			s << i.second.toString() << "\n";
		}
		return s.str();
	}
	void addRelation(Relation temp) {
		relations.emplace(temp.getName(),temp);
	}
	void processDatalog(DatalogProgram datalog) {
		for (auto& i : datalog.getSchemes()) {
			
			Relation temp = Relation(i.getName(),Scheme(conToString(i.getParameters())));
			relations.emplace(i.getName(),temp);
		}
		for (auto& i : datalog.getFacts()) {
			relations[i.getName()].addTuple(conToString(i.getParameters()));
		}
		
		processQueries(datalog);
		
	}
	
	void processQueries(DatalogProgram& datalog) {
		std::vector<int> indexes = {0};
		std::vector<std::string> values = {"'b'","X"};
		Relation temp = this->relations["SK"].select(indexes,values);
		std::cout << temp.toString() << "\n";

	}
	std::vector<std::string> conToString(std::vector<Parameter> values) const{
		std::vector<std::string> temp;
		for (auto& i : values) {
			temp.push_back(i.getValue());
		}
		return temp;
	}
};
#pragma once

#include <map>

#include "Relation.h"
#include "DatalogProgram.h"

class Database {
private:
	std::map<std::string,Relation> relations;
	std::vector<Relation> queries;
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
	
	void processQueries(DatalogProgram datalog) {
		std::vector<std::string> values;
		std::vector<int> indexes;
		std::vector<int> var_indexes;
		std::set<std::string> vars;
		unsigned int k;
		Relation temp;
		Relation temp2;
		for (auto& query : datalog.getQueries()) {
			values = conToString(query.getParameters());
			indexes.clear();
			for (unsigned int i = 0; i < values.size(); i++){
				if (values.at(i)[0] == '\'') {
					indexes.push_back(i);
				}
			}
			temp = relations[query.getName()].select(indexes, values);
			k = 0;
			var_indexes.clear();
			vars.clear();
			for (unsigned int i = 0; i < values.size(); i++) {
				if (k < indexes.size() && (int)i == indexes.at(k)) {
					k++;
					continue;
				}
				if (vars.find(values.at(i)) == vars.end()) {
					var_indexes.push_back(i);
					vars.insert(values.at(i));
				}
			}
			temp2 = temp.project(var_indexes);
			std::vector<std::string> variables;
			for (auto& val : var_indexes) {
				variables.push_back(values.at(val));
				
			}
			var_indexes.clear();
			for (unsigned int i=0; i < variables.size();i++) {
				var_indexes.push_back(i);
			}
			temp2 = temp2.rename(var_indexes, variables);
			std::cout << query.toString() << "? ";
			if (temp.numTuples() > 0) {
				std::cout << "Yes(" << temp.numTuples() << ")" << "\n";
				if (temp2.getScheme().size() != 0) {
					std::cout << temp2.toString() << "\n";
					}
			}
			else {
				std::cout << "No" << "\n";
			}
			this->queries.push_back(temp);
		}
}
	std::vector<std::string> conToString(std::vector<Parameter> values) const{
		std::vector<std::string> temp;
		for (auto& i : values) {
			temp.push_back(i.getValue());
		}
		return temp;
	}
};
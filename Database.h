#pragma once

#include <map>
#include<bits/stdc++.h>
#include <algorithm>
#include <cctype>

#include "Relation.h"
#include "DatalogProgram.h"
#include "Graph.h"

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
		std::vector<Rule> rules = datalog.getRules();
		Graph graph(rules.size());
		graph = this->makeGraph(rules);
		std::cout << "Dependency Graph\n" << graph.toString() << "\n\n";
		std::vector<std::vector<int>> sccs = graph.findSCC();


		processRules(datalog,sccs,graph);
		processQueries(datalog);
		
	}

	
	void processRules(DatalogProgram datalog, std::vector<std::vector<int>> sccs, Graph graph) {
		std::cout << "Rule Evaluation" << "\n";
		std::vector<std::string> values;
		std::vector<int> indexes;
		std::vector<int> var_indexes;
		std::vector<Relation> rules;
		std::set<std::string> vars;
		
		std::vector<Rule> curr_rules;

		std::unordered_map<std::string, int> scheme_location;
		unsigned int k;
		Relation temp;
		Relation temp2;
		bool any_added = true;
		bool continue_loop = true;

		bool override = false;
		int passes = 0;
		for (auto& scc : sccs) {
			std::cout << "SCC: R" << scc.at(0) << "\n";
			override = false;
			passes = 0;
			any_added = true;
			continue_loop = true;
			curr_rules = datalog.getRulesAt(scc);
			int curr_size = curr_rules.size();
			while (((continue_loop && curr_rules.size() > 0) || passes == 0) && !override) {
				if (curr_size == 1 && !graph.isDependent(scc.at(0),scc.at(0))) {
					override = true;
				}
				passes += 1;
				continue_loop = false;
				for (auto& rule : curr_rules) {
					std::cout << rule.toString() << "\n";
					rules.clear();
					for (auto& predicate : rule.getPredicates()) {
						values = conToString(predicate.getParameters());
						indexes.clear();
						for (unsigned int i = 0; i < values.size(); i++) {
							if (values.at(i)[0] == '\'') {
								indexes.push_back(i);
							}
						}
						temp = relations[predicate.getName()].select(indexes, values);
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
						for (unsigned int i = 0; i < variables.size();i++) {
							var_indexes.push_back(i);
						}
						temp2 = temp2.rename(var_indexes, variables);
						rules.push_back(temp2);
					}

					temp2 = rules.at(0);
					if (rules.size() > 0) {
						for (unsigned int i = 1; i < rules.size();i++) {
							temp2 = temp2.join(rules.at(i));
						}
					}

					values.clear();
					values = temp2.getScheme();
					if (temp2.getTuple().size() != 0) {
					}
					for (unsigned int i = 0; i < values.size(); i++) {
						std::string value = values.at(i);
						scheme_location[value] = i;
					}
					indexes.clear();
					for (auto& val : conToString(rule.getPredicate().getParameters())) {
						indexes.push_back(scheme_location[val]);
					}
					var_indexes.clear();
					for (unsigned int i = 0;i < relations[rule.getName()].getScheme().size();i++) {
						var_indexes.push_back(i);
					}
					temp2 = temp2.project(indexes);
					std::vector<std::string> new_ref = relations[rule.getName()].getScheme();
					temp2 = temp2.rename(var_indexes, new_ref);

					any_added = relations[rule.getName()].unions(temp2);
					if (any_added == true) {
						continue_loop = true;
					}
					new_ref.clear();
				}
			}
			std::cout << passes << " passes: R" << scc.at(0) << "\n";
		}
		std::cout << "\n";
	}

	
	void processQueries(DatalogProgram datalog) {
		std::cout << "Query Evaluation\n";
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

	static Graph makeGraph(const std::vector<Rule>& rules) {

		Graph graph(rules.size());
		for (unsigned fromID = 0; fromID < rules.size(); fromID++) {
			Rule fromRule = rules.at(fromID);
			std::vector<Predicate> bodyPreds = fromRule.getPredicates();
			for (auto& bodyPred : bodyPreds) {
				
				for (unsigned toID = 0; toID < rules.size(); toID++) {
					Rule toRule = rules.at(toID);
					if (toRule.getHeadPredicate().isEqual(bodyPred)) {
						graph.addEdge(fromID, toID);
					}
				}
			}
		}
		return graph;

	}
	

};
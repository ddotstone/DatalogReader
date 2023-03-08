#pragma once

#include "Predicate.h"
#include "Rule.h"
#include "Parameter.h"
#include <sstream>
#include <string>
#include <vector>
#include <set>

class DatalogProgram {
private:
  std::vector<Predicate> schemes;
  std::vector<Predicate> facts;
  std::vector<Rule> rules;
  std::vector<Predicate> queries;
	std::set<std::string> domains;

public:
  DatalogProgram() {
    this->schemes = {};
    this->facts = {};
    this->rules = {};
    this->queries = {};
		this->domains = {};
  }

  std::string toString() {
    std::stringstream s;
    
		
		s << "Success!\nSchemes(" << schemes.size() << "):\n";
    for (auto &scheme : this->schemes) {
      s << "  " << scheme.toString() << "\n";
    }
    
		s << "Facts(" << facts.size() << "):\n";
    for (auto &fact : this->facts) {
      s << "  " << fact.toString() << ".\n";
    }
    
		s << "Rules(" << rules.size() << "):\n";
    for (auto &rule : this->rules) {
      s << "  " << rule.toString() << "\n";
    }
		
		s << "Queries(" << queries.size() << "):\n";
    for (auto &querie : this->queries) {
      s << "  " << querie.toString() << "?\n";
    }
		s << "Domain(" << domains.size() << "):\n";
    for (auto domain : domains) {
      s << "  " << domain << "\n";	
    }
  s.seekp(-1,s.cur);
	s<<"";
	return s.str();
	}
	void addToSchemes(Predicate temp){
		schemes.push_back(temp);
		return;
	}
	void addToFacts(Predicate temp){
		facts.push_back(temp);
		return;
	}
	void addToRules(Rule temp){
		rules.push_back(temp);
		return;
	}
	void addToQueries(Predicate temp){
		queries.push_back(temp);
		return;
	}
	void addToParameters(std::string temp){
		domains.insert(temp);
		return;
	}
};
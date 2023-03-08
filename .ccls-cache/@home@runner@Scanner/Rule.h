#pragma once

#include "Predicate.h"
#include <sstream>
#include <string>
#include <vector>

class Rule {
private:
  Predicate name;
  std::vector<Predicate> predicates;

public:
  Rule(Predicate name = Predicate(), std::vector<Predicate> predicates = {})
      : name(name), predicates(predicates) {}

  void addPredicate(Predicate P) {
    predicates.push_back(P);
    return;
  }

  void setName(Predicate name) {
    this->name = name;
    return;
  }

  std::string toString() {
    std::stringstream s;
    s << name.toString() << " :- ";
		for (auto& predicate:this->predicates){
			s << predicate.toString() << ",";
		}
		s.seekp(-1,s.cur);
		s<<".";
		return s.str();
  }
};
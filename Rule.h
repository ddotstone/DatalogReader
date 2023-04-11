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

  bool isEqual(Rule to_compare) {
      bool is_equal = true;
      if (!this->name.isEqual(to_compare.name)) {
          is_equal = false;
      }
      for (unsigned int i = 0; i < this->predicates.size(); i++) {
          if (!this->predicates.at(i).isEqual(to_compare.predicates.at(i))) {
              is_equal = false;
          }
      }
      return is_equal;
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

  std::vector<Predicate> getPredicates() {
      return this->predicates;
  }
Predicate getPredicate(){
	return this->name;
}
  std::string getName() {
      return this->name.getName();
  }
  Predicate getHeadPredicate() {
      return this->name;
  }
};
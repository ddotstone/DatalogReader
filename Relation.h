#pragma once

#include <set>

#include "Tuple.h"

class Relation {
private:
  std::string name;
  Scheme scheme;
  long unsigned int scheme_len;
  std::set<Tuple> tuples;

public:
  Relation(const std::string &name, const Scheme &scheme)
      : name(name), scheme(scheme), scheme_len(scheme.size()) {}
  bool addTuple(const Tuple &tuple) {
    if (tuple.size() == scheme_len) {
      tuples.insert(tuple);
      return true;
    }
    return false;
  }
std::string toString() const {
	std::stringstream s;
	for (auto& tuple:tuples){
		s<<tuple.toString(scheme)<<"\n";
	}
	return s.str();
}
Relation select(int index, const std::string& value) const {
	Relation result(name,scheme);
	for (auto&tuple:tuples){
		if (tuple.at(index) == value){
			result.addTuple(tuple);
		}
	}
	return result;
}
};
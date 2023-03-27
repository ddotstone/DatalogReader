#pragma once

#include <vector>
#include <string>
#include <sstream>

#include "Scheme.h"


class Tuple:public std::vector<std::string>{
private:
public:
	Tuple() {}
	Tuple(std::vector<std::string> names):std::vector<std::string>(names){};
	std::string toString(const Scheme& scheme) const{
		if (scheme.size() ==0){
			return "";
		}
		const Tuple& tuple = *this;
		std::stringstream s;
		for (unsigned int i = 0;i<tuple.size();++i){
			s<<scheme.at(i);
			s<<"=";
			s<<tuple.at(i);
			s<<", ";
			}
		
		return s.str().substr(0,s.str().size()-2);
		}
	std::string toString() const {
		std::stringstream s;
		for (auto& val : *this) {
			s << val << " ";
		}
		return s.str();
	}

};
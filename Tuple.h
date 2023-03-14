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
		s.seekp(-2,s.cur);
		s<<'\0'<<'\0';
		return s.str();
		}

};
#pragma once

#include <string>
#include <vector>
#include <sstream>
#include "Parameter.h"

class Predicate{
	private:
		std::string name;
		std::vector<Parameter> parameters;
	public:
		Predicate(std::string name ="", std::vector<Parameter> parameters = {}): name(name), parameters(parameters){}

		void addParameter(Parameter t){
				parameters.push_back(t);
				return;
		}

		void setName(std::string name){
			this->name = name;
			return;
		}

		std::string toString(){
			std::stringstream s;
			s << name << "(";
			for(auto &parameter: this->parameters){
				s << parameter.toString() << ",";
			}
			s.seekp(-1,s.cur);
			s<<")";
			return s.str();
		}
		
		
};
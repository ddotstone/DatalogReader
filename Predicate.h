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
		std::string getName() {
			return this->name;
		}

		bool isEqual(Predicate toCompare) {
			bool is_equal = true;
			if ((this->name != toCompare.getName()) || (this->parameters.size() != toCompare.parameters.size())) {
				is_equal = false;
			}
			for (unsigned int i; i < this->parameters.size(); i++) {
				if (this->parameters.at(i).getValue() != toCompare.parameters.at(i).getValue()) {
					is_equal = false;
				}
			}
			return is_equal;
		}

		std::vector<Parameter> getParameters() {
			return this->parameters;
		}

		std::string toString(){
			std::stringstream s;
			s << name << "(";
			if (!parameters.empty()) {
				for (auto& parameter : this->parameters) {
					s << parameter.toString() << ",";
				}
				s.seekp(-1, s.cur);
			}
			s<<")";
			return s.str();
		}
		
		
};
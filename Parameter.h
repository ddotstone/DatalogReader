#pragma once

#include <string>

class Parameter{
	private:
		std::string value;
	public:
		Parameter(const std::string value): value(value){}

		std::string toString(){
			return value;
		}
};
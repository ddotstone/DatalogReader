#pragma once

#include <vector>
#include<string>

class Scheme:public std::vector<std::string> {
private:
public:
	Scheme(std::vector<std::string> names): std::vector<std::string>(names){}
};
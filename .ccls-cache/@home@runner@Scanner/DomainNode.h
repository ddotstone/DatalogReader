#pragma once

#include <string>

class DomainNode {
private:
  std::string value;
  DomainNode *next;

public:
  DomainNode(std::string value = "", DomainNode *next = NULL)
      : value(value), next(next) {}
  DomainNode *getNext() { return next; }
  std::string getValue() { return value; }
  
	void changeValue(std::string value) { this->value = value; }
  
	void changePointer(DomainNode *next) { this->next = next; }
};

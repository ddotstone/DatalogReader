#pragma once

#include "DomainNode.h"
#include <string>
#include <vector>
class DomainList {
private:
  DomainNode *root;
	int size;

public:
  DomainList() {
		size = 0;
		root = NULL;
	}
	  ~DomainList() {
			DomainNode *current = root;
	    DomainNode *ptr = root;
	    while (current->getNext() != NULL) {
	      current = ptr->getNext();
	      delete ptr;
	      ptr = current;
	    }
	    root = {};
	  }
	
  void addValue(std::string value) {
    size++;
		if (root == NULL) {
      DomainNode* new_node = new DomainNode(value);
			root = new_node;
    } else {
      DomainNode *curr = root;
      while (curr->getNext() != NULL) {
        int a = value.size();
        int b = curr->getNext()->getValue().size();
        bool doswitch = true;
        if (a < b) {
          for (int i=0; i < a; i++) {
            if (value.at(i) > curr->getNext()->getValue().at(i)) {
              continue;
            } else {
              doswitch = false;
            }
          }
        } else {
          for (int i=0; i < b; i++) {
            if (value.at(i) < curr->getNext()->getValue().at(i)) {
              if (i == b - 1)
                doswitch = false;
              continue;
            } else {
              doswitch = false;
            }
          }
        }
        if (doswitch) {
          break;
        }
				curr = curr->getNext();
      }
      DomainNode *new_node = new DomainNode(value);
      new_node->changePointer(curr->getNext());
      curr->changePointer(new_node);
    }
  }


	int getSize(){
		return size;
	}

std::vector<std::string> returnVector(){
	std::vector<std::string> values = {};
	DomainNode* curr = root;
	while(curr != NULL){
		values.push_back(curr->getValue());
		curr = curr->getNext();
	}
	return values;
}
};
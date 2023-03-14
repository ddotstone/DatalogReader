#pragma once

#include <set>
#include<unordered_map>

#include "Tuple.h"

class Relation {
private:
  std::string name;
  Scheme scheme;
  long unsigned int scheme_len;
  std::set<Tuple> tuples;
  int num_tuples;

public:
	
	
	Relation() {
		num_tuples = 0;
	};
  
	
	Relation(const std::string& name, const Scheme& scheme)
		: name(name), scheme(scheme), scheme_len(scheme.size()) {
		num_tuples = 0;
	}
  
  
	bool addTuple(const Tuple &tuple) {
		if (tuple.size() == scheme_len) {
		  tuples.insert(tuple);
		  num_tuples++;
		  return true;
		}
		return false;
	  }

	int numTuples() {
		return this->num_tuples;
	}


	std::string toString() const {
		std::stringstream s;
		for (auto& tuple:tuples){
			s<<"  "<<tuple.toString(scheme) << "\n";
		}
		s.seekp(-1, s.cur); 
		s << '\0';
		return s.str();
	}


	Relation select(const std::vector <int>& indexes, const std::vector<std::string>& values) const {
		Relation result(this->name,this->scheme);
		bool is_match;
		unsigned int j = 0;
		unsigned int k = 0;
		unsigned int l = 0;
		std::unordered_map <int, std::vector<int>> ref_;
		std::set<std::string> found;
		for (unsigned int i = 0; i < values.size(); ++i) {
			if (l < indexes.size()) {
				if ((int)i == (int)indexes.at(l)) {
					l++;
					continue;
				}
			}
			k = 0;
			if (found.find(values.at(i)) != found.end()) {
				found.insert(values.at(i));
				k = i+1;
				while (k < values.size()) {
					if (values.at(i) == values.at(k)) {
						ref_[i].push_back(k);
					}
					k++;
				}
			}
		}
		for (auto&tuple:tuples){
			is_match = true;
			j = 0;
			for (unsigned int i = 0; i < values.size(); i++){
				if (j < indexes.size()) {
					if ((int)i == (int)indexes.at(j)) {
						if (tuple.at(i) != values.at(i)) {
							is_match = false;
								break;
						}
						j++;
					}
				}
				else {
					if (ref_.find(i)!= ref_.end() && !ref_[i].empty())
						for (auto& pos : ref_[i]) {
							if (tuple.at(pos) != tuple.at(i)) {
								is_match = false;
								break;
							}
						}
				}
			}
			if (is_match) {
				result.addTuple(tuple);
			}
		}
		return result;
	}


	Relation project(const std::vector<int>& indexes) const{
		Scheme temp1;
		Tuple temp2;
		std::map<int, bool> ref;
		for (auto& index:indexes) {
			temp1.push_back(this->scheme.at(index));
		}
		Relation result(this->name,temp1);
		for (auto& tuple: tuples) {
			temp2 = {};
			for (auto& val : indexes) {
				temp2.push_back(tuple.at(val));
				result.addTuple(temp2);
			}
		}
		return result;
	}


	Relation rename(std::vector<int>& indexes, std::vector<std::string>& values) {
		Scheme scheme = this->scheme;
		for (auto& val : indexes) {
			scheme.at(val) = values.at(val);
		}
		Relation result = *this;
		result.changeScheme(scheme);
		return result;
	}


	void changeScheme(Scheme scheme) {
		this->scheme = scheme;
	}
	
	Scheme getScheme() {
		return this->scheme;
	}


	void setName(std::string name) {
		this->name = name;
	}


	std::string getName() const{
		return this->name;
	}
	};
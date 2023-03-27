#pragma once

#include <set>
#include <unordered_map>
#include <iostream>

#include "Tuple.h"

class Relation {
private:
    std::string name;
    Scheme scheme;
    long unsigned int scheme_len;
    std::set<Tuple> tuples;

public:
    Relation() {};

    Relation(const std::string& name, const Scheme& scheme)
        : name(name), scheme(scheme), scheme_len(scheme.size()) {}
    int numTuples() { return this->tuples.size(); }

    bool addTuple(const Tuple& tuple) {
        bool is_added = false;
        if (this->tuples.find(tuple) == this->tuples.end()) {
            is_added = true;
        }
        
        this->tuples.insert(tuple);
        return is_added;
    }

    std::string toString() const {
        std::stringstream s;
        for (auto& tuple : tuples) {
            s << "  " << tuple.toString(scheme) << "\n";
        }
        return s.str().substr(0, s.str().size() - 1);
    }


    Relation select(const std::vector<int>& indexes,
        const std::vector<std::string>& values) const {
        Relation result(this->name, this->scheme);
        bool is_match;
        unsigned int j = 0;
        unsigned int k = 0;
        unsigned int l = 0;
        std::unordered_map<int, std::vector<int>> ref_;
        std::set<std::string> found;
        for (unsigned int i = 0; i < values.size(); ++i) {
            if (l < indexes.size()) {
                if ((int)i == (int)indexes.at(l)) {
                    l++;
                    continue;
                }
            }

            if (found.find(values.at(i)) == found.end()) {
                found.insert(values.at(i));
                k = i + 1;
                while (k < values.size()) {
                    if (values.at(i).compare(values.at(k)) == 0) {
                        ref_[i].push_back(k);
                    }
                    k++;
                }
            }
        }

        for (auto& tuple : tuples) {
            is_match = true;
            j = 0;

            for (unsigned int i = 0; i < values.size(); i++) {

                if ((j < indexes.size()) && ((int)i == (int)indexes.at(j))) {
                    if (tuple.at(i) != values.at(i)) {
                        is_match = false;
                        break;
                    }

                    j++;
                }
                else {
                    if (ref_.count(i)) {
                        for (auto& pos : ref_[i]) {
                            if (tuple.at(pos) != tuple.at(i)) {
                                is_match = false;
                                break;
                            }
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

    Relation project(const std::vector<int>& indexes) const {
        Scheme temp1;
        Tuple temp2;
        for (auto& index : indexes) {
            temp1.push_back(this->scheme.at(index));
        }
        Relation result(this->name, temp1);
        for (auto& tuple : tuples) {
            temp2 = {};
            for (auto& val : indexes) {
                temp2.push_back(tuple.at(val));
            }
            result.addTuple(temp2);
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

  bool unions(const Relation &right) {
      bool is_added;
      bool any_added = false;
      
      for (auto& tuple : right.tuples) {
          
          is_added = this->addTuple(tuple);
          if (is_added) {
              std::cout << "  " << tuple.toString(this->scheme) << "\n";
              any_added = true;
          }
      }
      return any_added;
  }

  bool joinable(const std::vector<int> right_additions,std::vector<std::vector<int>> &to_compare, const Tuple &leftTuple, const Tuple &rightTuple) {
    bool joinable = true;
    for (std::vector<int>& val : to_compare) {
      if (leftTuple.at(val.at(0)) != rightTuple.at(val.at(1))) {
        joinable = false;
      }
    }
    return joinable;
  }

  Relation join(const Relation &right) {
    const Relation &left = *this;
    std::vector<int> right_additions;
    std::vector<std::vector<int>> comparisons;
    Scheme scheme = createScheme(right_additions, comparisons,this->scheme, right.getScheme());
    bool related = true;

    if (comparisons.size() == 0) {
      related = false;
    }
		std::string new_name= this->name + right.name;
    Relation result(new_name,scheme);

    if (!related) {
      cartesianProduct(result, left, right);
    } else {

      Tuple temp;

      for (const Tuple &leftTuple : left.tuples) {

        for (const Tuple &rightTuple : right.tuples) {
          if (joinable(right_additions,comparisons, leftTuple, rightTuple)) {
              temp.clear();
              for (const std::string& i:leftTuple){
							temp.push_back(i);
						}
            for (int &i : right_additions) {
                temp.push_back(rightTuple.at(i));
            }
            result.addTuple(temp);
          }
        }
      }
    }
    return result;
  }

  void changeScheme(Scheme scheme) { this->scheme = scheme; }
  void cartesianProduct(Relation &relation, const Relation &left,const Relation &right) {
    Tuple temp;
    for (const Tuple &left : left.tuples) {
      for (const Tuple &right : right.tuples) {
        temp = left;
        for (std::string val : right) {
          temp.push_back(val);
        }
        relation.addTuple(temp);
      }
    }
  }

  Scheme createScheme(std::vector<int>& right_additions,
                      std::vector<std::vector<int>> &to_compare,
                      const Scheme &left, const Scheme &right) {
    Scheme result;
    std::unordered_map<std::string, int> scheme_left;
    for (unsigned int i = 0; i < left.size(); ++i) {
      result.push_back(left.at(i));
      scheme_left[left.at(i)] = i;
    }

    std::vector<int> pair;

    for (unsigned int i = 0; i < right.size(); ++i) {
      if (scheme_left.find(right.at(i)) != scheme_left.end()) {
        pair = {(int)scheme_left[right.at(i)], (int)i};
        to_compare.push_back(pair);
      } else {
        result.push_back(right.at(i));
        right_additions.push_back(i);
      }
    }
    return result;
  }

  Scheme getScheme() const { return this->scheme; }

  void setName(std::string name) { this->name = name; }

  std::string getName() const { return this->name; }

  std::set<Tuple> getTuple() const { return this->tuples; }
};
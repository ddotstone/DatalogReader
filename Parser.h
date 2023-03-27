#pragma once
#include "DatalogProgram.h"
#include "Parameter.h"
#include "Rule.h"
#include "Token.h"
#include <iostream>
#include <string>
#include <vector>

class Parser {
private:
  std::vector<Token> tokens;
  DatalogProgram dataStructure;
	bool containsFailure = false;
	std::string errorMessage;

public:
  Parser(const std::vector<Token> &tokens) : tokens(tokens) {}

  void advanceToken() {
    tokens.erase(tokens.begin());
    return;
  }

  DatalogProgram getDatalogProgram() {
      return dataStructure;
  }

  TokenType tokenType() { return tokens.at(0).getType(); }

  std::string tokenValue() { return tokens.at(0).getValue(); }

  void throwError() {
    if (!containsFailure){
			errorMessage = "Failure!\n  " + tokens.at(0).toString() + "\n";
		}
		containsFailure = true;
		
    return;
  }

  std::string toString() { 
		if (!containsFailure){
		return dataStructure.toString(); }
		else{
			return errorMessage;
		}
		}
  void printTokens() {
    for (auto &token : this->tokens) {
      std::cout << token.toString() << std::endl;
    }
  }

  void match(TokenType t) {
    while (tokenType() == COMMENT){
			advanceToken();
		}
		if (tokenType() == t) {
      advanceToken();
    } else {
      throwError();
    }
  }
  // datalogProgram	->	SCHEMES COLON scheme schemeList
  // 		        FACTS COLON factList
  // 		        RULES COLON ruleList
  // 		        QUERIES COLON query queryList
  // 						EOF
  void datalogProgram() {
    match(SCHEMES);
    match(COLON);
    std::vector<Predicate> schemes_tot = schemeList();
    for (auto &scheme : schemes_tot) {
      dataStructure.addToSchemes(scheme);
    }
		match(FACTS);
		match(COLON);
		std::vector<Predicate> facts_tot = factList();
		for (auto& fact:facts_tot){
			dataStructure.addToFacts(fact);
		}
    match(RULES);
    match(COLON);
    std::vector<Rule> rules_tot = ruleList();
    for (auto &rule : rules_tot) {
      dataStructure.addToRules(rule);
    }
    match(QUERIES);
    match(COLON);
    std::vector<Predicate> queries_tot = queryList();
    for (auto &query : queries_tot) {
      dataStructure.addToQueries(query);
    }
    match(EOF_);
  }

  //   schemeList	->	scheme schemeList | lambda
  std::vector<Predicate> schemeList() {
    std::vector<Predicate> temp = {};
    Predicate first = scheme();
    temp.push_back(first);
    while (tokenType() == ID) {
      Predicate next = scheme();
      temp.push_back(next);
    }
    return temp;
  }
  // factList	->	fact factList | lambda
  std::vector<Predicate> factList() {
    std::vector<Predicate> temp = {};
    while (tokenType() == ID) {
      Predicate next = fact();
      temp.push_back(next);
    }
    return temp;
  }
  // ruleList	->	rule ruleList | lambda
  std::vector<Rule> ruleList() {
    std::vector<Rule> temp = {};
	
    while (tokenType() == ID) {
      Rule next = rule();
      temp.push_back(next);
			while (tokenType() == COMMENT){
			advanceToken();
			}
    }
    return temp;
  }
  // queryList	->	query queryList | lambda
  std::vector<Predicate> queryList() {
    std::vector<Predicate> temp = {};
    Predicate first = query();
    temp.push_back(first);
    while (tokenType() == ID) {
      Predicate next = query();
      temp.push_back(next);
    }
    return temp;
  }

  // scheme -> ID LEFT_PAREN ID idList RIGHT_PAREN

  Predicate scheme() {
    std::string name = tokenValue();
    Predicate temp = Predicate();
    match(ID);
    temp.setName(name);
    match(LEFT_PAREN);
    Parameter first = Parameter(tokenValue());
    temp.addParameter(first);
    match(ID);
    std::vector<Parameter> id_s = idList();
    for (auto &curr_id : id_s) {
      temp.addParameter(curr_id);
    }
    match(RIGHT_PAREN);
    return temp;
  }
  // fact    	->	ID LEFT_PAREN STRING stringList RIGHT_PAREN PERIOD
  Predicate fact() {
    std::string name = tokenValue();
    match(ID);
    Predicate temp = Predicate();
    temp.setName(name);
    match(LEFT_PAREN);
    Parameter first = Parameter(tokenValue());
		dataStructure.addToParameters(first.toString());
    temp.addParameter(first);
    match(STRING);
    std::vector<std::string> stringVec = stringList();
    for (auto &currString : stringVec) {
      Parameter temp2 = Parameter(currString);
      dataStructure.addToParameters(temp2.toString());
      temp.addParameter(temp2);
    }
    match(RIGHT_PAREN);
    match(PERIOD);
    return temp;
  }
  // rule    	->	headPredicate COLON_DASH predicate predicateList PERIOD
  Rule rule() {
    Predicate name = headPredicate();
    Rule temp = Rule();
    temp.setName(name);
    match(COLON_DASH);
    Predicate first = predicate();
    temp.addPredicate(first);
    std::vector<Predicate> allPredicates = predicateList();
    for (auto &predicate : allPredicates) {
      temp.addPredicate(predicate);
    }

    match(PERIOD);
    return temp;
  }
  // query	        ->      predicate Q_MARK
  Predicate query() {
    Predicate temp = predicate();

    match(Q_MARK);
    return temp;
  }

  // headPredicate	->	ID LEFT_PAREN ID idList RIGHT_PAREN
  Predicate headPredicate() {
    Predicate temp = Predicate(tokenValue());
    match(ID);
    match(LEFT_PAREN);
    Parameter first = Parameter(tokenValue());
    temp.addParameter(first);
    match(ID);
    std::vector<Parameter> parameters = idList();
    for (auto &parameter : parameters) {
      temp.addParameter(parameter);
    }
    match(RIGHT_PAREN);
		return temp;
  }
  // predicate	->	ID LEFT_PAREN parameter parameterList RIGHT_PAREN
  Predicate predicate() {
    Predicate newPredicate = Predicate();
    newPredicate.setName(tokenValue());
    match(ID);
    match(LEFT_PAREN);
    Parameter firstParameter = parameter();
    newPredicate.addParameter(firstParameter);
    std::vector<Parameter> additionalParameters = parameterList();
    for (auto &parameter : additionalParameters) {
      newPredicate.addParameter(parameter);
    }
    match(RIGHT_PAREN);
    return newPredicate;
  }

  // predicateList	->	COMMA predicate predicateList | lambda
  std::vector<Predicate> predicateList() {
    std::vector<Predicate> predicatesList;
    predicateListCreator(predicatesList);
    return predicatesList;
  }

  void predicateListCreator(std::vector<Predicate> &predicates) {
    if (tokenType() == COMMA) {
      match(COMMA);
      Predicate newPredicate = predicate();
      predicates.push_back(newPredicate);
      predicateListCreator(predicates);
    }
  }

  // parameterList	-> 	COMMA parameter parameterList | lambda
  std::vector<Parameter> parameterList() {
    std::vector<Parameter> parameters = {};
    parameterListCreator(parameters);
    return parameters;
  }
  void parameterListCreator(std::vector<Parameter> &parameters) {
    if (tokenType() == COMMA) {
      match(COMMA);
      Parameter temp = parameter();
      parameters.push_back(temp);
      parameterListCreator(parameters);
    }
  }

  // stringList	-> 	COMMA STRING stringList | lambda
  std::vector<std::string> stringList() {
    std::vector<std::string> stringsList = {};
    stringListCreator(stringsList);
    return stringsList;
  }

  void stringListCreator(std::vector<std::string> &stringsList) {
    if (tokenType() == COMMA) {
      match(COMMA);
      stringsList.push_back(tokenValue());
      match(STRING);
      stringListCreator(stringsList);
    }
  }

  // idList -> COMMA ID idList | lambda
  std::vector<Parameter> idList() {
    std::vector<Parameter> parameters = {};
    idListCreator(parameters);
    return parameters;
  }

  void idListCreator(std::vector<Parameter> &parameters) {
    if (tokenType() == COMMA) {
      match(COMMA);
      Parameter temp = Parameter(tokenValue());
      parameters.push_back(temp);
      match(ID);
      idListCreator(parameters);
    }
  }

  // parameter	->	STRING | ID
  Parameter parameter() {
    if (tokenType() == STRING) {
      Parameter temp = Parameter(tokenValue());
      match(STRING);
      return temp;
    } else {
      Parameter temp = Parameter(tokenValue());
      match(ID);
      return temp;
    }
  }
};
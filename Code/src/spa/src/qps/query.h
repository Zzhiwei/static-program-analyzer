//
// Created by Vanessa Khor on 5/2/23.
//
#pragma once
#include <list>
#include <string>
#include "declaration.h"
#include "Clause.h"
#include <vector>
#include <iostream>

//#include "../../../../../../../CLionProjects/untitled/qps/clause.h"

class Query {
private:
    std::vector<Declaration> declarations;
    std::vector<std::shared_ptr<Clause>> clauses;
    std::string target;

public:
    explicit Query(std::vector<Declaration> d, std::vector<std::shared_ptr<Clause>> c, std::string target);

    void printQuery(); // output query to console

    // get functions for query evaluator
    std::string getTarget();
    std::vector<std::shared_ptr<Clause>> getClauseList();
    std::vector<Declaration> getDeclarationList();

    // set functions for query parser
    void addClause(std::shared_ptr<Clause> c);
    void addDeclaration(Declaration d);
    void setTarget(std::string t);
    bool isDeclarationPresent(std::string type, std::string syn);
    bool isSynonymPresent(std::string syn);

    bool operator==(const Query& other) const {
    return target == other.target && declarations == other.declarations && clauses == other.clauses;
    }
};
//
// Created by Vanessa Khor on 5/2/23.
//
#include "query.h"

Query::Query(std::vector<Declaration> d, std::vector<std::shared_ptr<Clause>> c, std::string target) : declarations(d), clauses(c), target(target) {};

std::vector<std::shared_ptr<Clause>> Query::getClauseList() {
	return clauses;
}

std::string Query::getTarget() {
    return target;
}

std::vector<Declaration> Query::getDeclarationList() {
    return declarations;
}

void Query::printQuery() {
    std::cout << "target:" << target << std::endl;
    std::cout << "clauses: ";
    for(std::shared_ptr<Clause> c : clauses) {
        c->printClause();
    }
    std::cout << "declarations: ";
    for(Declaration d : declarations) {
        d.printDeclaration();
    }
}

void Query::addClause(std::shared_ptr<Clause> c) {
    clauses.push_back(c);
}

void Query::addDeclaration(Declaration d) {
    declarations.push_back(d);
}

void Query::setTarget(std::string t) {
    target = t;
}

bool Query::isDeclarationPresent(std::string type, std::string syn) {
    for(Declaration d : declarations) {
        std::vector<std::string> curr = d.getSynonymList();
        if(d.getType() == type && (std::find(curr.begin(), curr.end(), syn) != curr.end())) {
            return true;
        }
    }
    return false;
}

bool Query::isSynonymPresent(std::string syn) {
    for(Declaration d : declarations) {
        std::vector<std::string> curr = d.getSynonymList();
        if(std::find(curr.begin(), curr.end(), syn) != curr.end()) {
            return true;
        }
    }
    return false;
}
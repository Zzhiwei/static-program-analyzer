#include<stdio.h>
#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>

#include "SPFacade.h"

SPFacade::SPFacade(std::shared_ptr <PatternManager> pm, std::shared_ptr <EntityManager> em, std::shared_ptr <RelationshipManager> rm) :
	entityManager(em), 
	relationshipManager(rm),
	patternManager(pm)
{}

void SPFacade::addFollows(int stmt1, int stmt2) {
	relationshipManager->addFollows(stmt1, stmt2);
}

void SPFacade::addParentChild(int parent, int child) {
	relationshipManager->addParentChild(parent, child);
}

void SPFacade::addUses(int stmtNo, std::string var) {
	relationshipManager->addUses(stmtNo, var);
}

void SPFacade::addModifies(int stmtNo, std::string var) {
	relationshipManager->addModifies(stmtNo, var);
}

void SPFacade::addStatement(int stmt, StatementType st) {
	entityManager->addStatement(stmt, st);
}

void SPFacade::addVariable(std::string var) {
	entityManager->addVariable(var);
}

void SPFacade::addConstant(std::string value) {
	entityManager->addConstant(value);
}

void SPFacade::addPattern(int stmtRef, AssignNode* assign) {
	patternManager->addPattern(stmtRef, std::make_shared<AssignNode>(*assign));
}


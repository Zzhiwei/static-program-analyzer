#pragma once

#include<stdio.h>
#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>
#include <memory>

#include "EntityManager.h"
#include "RelationshipManager.h"
#include "PatternManager.h"
#include "StatementType.h"

class SPFacade {
public:
	SPFacade(std::shared_ptr <PatternManager> pm, std::shared_ptr <EntityManager> em, std::shared_ptr <RelationshipManager> rm);
	
	// Relationships

	void addFollows(int stmt1, int stmt2);
	void addParentChild(int parent, int child);
	void addUses(int stmtNo, std::string var);
	void addModifies(int stmtNo, std::string var);

	// Entities

	void addStatement(int stmtNo, StatementType st);
	void addVariable(std::string var);
	void addConstant(std::string const);

	// Patterns

	void addPattern(int stmtRef, AssignNode* assign);
	
private:
	std::shared_ptr <EntityManager> entityManager;
	std::shared_ptr <RelationshipManager> relationshipManager;
	std::shared_ptr <PatternManager> patternManager;
};
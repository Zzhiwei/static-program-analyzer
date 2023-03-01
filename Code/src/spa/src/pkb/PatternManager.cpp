#include<stdio.h>
#include <iostream>
#include <string>
#include <vector>

#include "PatternManager.h"

void PatternManager::addPattern(int stmtRef, std::shared_ptr<AssignNode> assignNode) {
	LhsRhsTable[assignNode->getLhs()->toString()] = assignNode->getRhs();
	LhsStmtRefTable[assignNode->getLhs()->toString()].push_back(stmtRef);
	StmtRefAssignTable[stmtRef] = assignNode->getRhs();
}

std::vector<int> PatternManager::getStmtRefLhsVarRhsPartial(std::string lhs, std::string rhs) {
	std::vector<int> res;
	std::vector<int> StmtRefLhsMatch = LhsStmtRefTable[lhs];
	for (auto& i : StmtRefLhsMatch) {
		Expression node = StmtRefAssignTable[i];
		//search the expression tree for rhs
		if (dfsNodeTree(node, rhs)) {
			res.push_back(i);
		}
	}
	return res;
}

std::vector<int> PatternManager::getStmtRefRhs(std::string rhs) {
	std::vector<int> res;
	for (auto& [key, value] : StmtRefAssignTable) {
		//if rhs exists in the rhs of the assign
		if (dfsNodeTree(value, rhs)) {
			res.push_back(key);
		}
	}
	return res;
}

std::vector<int> PatternManager::getStmtRefLhs(std::string lhs) {
	//return LHSValTable[RHS];
	return LhsStmtRefTable[lhs];
	
}

std::vector<int> PatternManager::getStmtRefBothWild() {
	std::vector<int> res;
	for (auto& [key, value] : StmtRefAssignTable) {
		res.push_back(key);
	}
	return res;
}

bool PatternManager::dfsNodeTree(Expression refNode, std::string target) {
	std::shared_ptr<VariableNode> castVar = std::dynamic_pointer_cast<VariableNode>(refNode);
	if (castVar!=nullptr) {
		return refNode->toString() == target;
	}
	std::stack<Expression> stack;
	stack.push(refNode);
	while (!stack.empty()) {
		Expression node = stack.top();
		stack.pop();
		std::shared_ptr<BinaryOpNode> castBin = std::dynamic_pointer_cast<BinaryOpNode>(node);
		if (castBin == nullptr) {
			if (node->toString() == target) {
				return true;
			}
		} else {
			stack.push(castBin->getLhs());
			stack.push(castBin->getRhs());
		}
	}
	return false;
}
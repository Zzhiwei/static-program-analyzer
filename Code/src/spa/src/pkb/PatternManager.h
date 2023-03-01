#pragma once

#include <stdio.h>
#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>
#include <utility>
#include <algorithm>
#include <stack>

#include "ast/ExpressionNode.h"
#include "ast/StmtNode.h"

class PatternManager {
private:
	std::unordered_map<std::string, Expression> LhsRhsTable;
	std::unordered_map<std::string, std::vector<int>> LhsStmtRefTable;
	std::unordered_map<int, Expression> StmtRefAssignTable;
	//std::map<std::string, std::string> RHSValTable;
public:
	void addPattern(int stmtRef, std::shared_ptr<AssignNode> assignNode);
	std::vector<int> getStmtRefLhsVarRhsPartial(std::string lhs, std::string rhs);
	std::vector<int> getStmtRefRhs(std::string rhs);
	std::vector<int> getStmtRefLhs(std::string lhs);
	std::vector<int> getStmtRefBothWild();
	bool dfsNodeTree(Expression rhsNode, std::string target);
};
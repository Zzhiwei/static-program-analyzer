#pragma once

#include "Visitor.h"
#include "ast/TNode.h"
#include "ast/StmtNode.h"
#include "ast/ExpressionNode.h"
#include "pkb/SPFacade.h"

// adds basic entities (stmt, var, constant), modifies and  uses
class BasicExtractionVisitor : public Visitor {
private:
    std::shared_ptr<SPFacade> spFacade;
    void addAncestorModifies(TNode *node, std::string varName);
    void addAncestorUses(TNode *node, std::string varName);
public:
    BasicExtractionVisitor(std::shared_ptr<SPFacade> spf);

    void visit(ReadNode *node);
    void visit(PrintNode *node);
    void visit(AssignNode *node);
    void visit(IfNode *node);
    void visit(WhileNode *node);
    void visit(VariableNode *node);
    void visit(ConstantNode *node);
};

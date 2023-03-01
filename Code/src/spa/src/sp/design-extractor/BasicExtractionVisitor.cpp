#include <vector>

#include "BasicExtractionVisitor.h"

BasicExtractionVisitor::BasicExtractionVisitor(std::shared_ptr<SPFacade> spf)
    : spFacade(spf)
{}

void BasicExtractionVisitor::visit(ReadNode *node) {
    spFacade->addStatement(node->getStmtNo(), StatementType::StatementRead);

    std::string varName = node->getVar()->getVarName();
    spFacade->addVariable(varName);
    spFacade->addModifies(node->getStmtNo(), varName);
    addAncestorModifies(node, varName);
}

void BasicExtractionVisitor::visit(PrintNode *node) {
    spFacade->addStatement(node->getStmtNo(), StatementType::StatementPrint);
    node->getVar()->accept(this);
}

void BasicExtractionVisitor::visit(AssignNode *node) {
    spFacade->addStatement(node->getStmtNo(), StatementType::StatementAssign);
    spFacade->addPattern(node->getStmtNo(), node);

    std::string varName = node->getLhs()->getVarName();
    spFacade->addVariable(varName);
    spFacade->addModifies(node->getStmtNo(), varName);
    addAncestorModifies(node, varName);

    node->getRhs()->accept(this);
}

void BasicExtractionVisitor::visit(IfNode *node) {
    spFacade->addStatement(node->getStmtNo(), StatementType::StatementIf);
    Visitor::visit(node);
}

void BasicExtractionVisitor::visit(WhileNode *node) {
    spFacade->addStatement(node->getStmtNo(), StatementType::StatementWhile);
    Visitor::visit(node);
}


void BasicExtractionVisitor::visit(VariableNode *node) {
    // all modifies relationship are added while visiting assign and read nodes
    // all uses relationship are added while visiting leaf node (i.e. variable node)
    std::string varName = node->getVarName();
    spFacade->addVariable(varName);
    spFacade->addUses(node->getStmtNo(), varName);
    addAncestorUses(node, varName);
}

void BasicExtractionVisitor::visit(ConstantNode *node) {
    spFacade->addConstant(node->getConstant());
}

void BasicExtractionVisitor::addAncestorModifies(TNode *node, std::string varName) {
    Node parent = node->getParent();
    while (parent != nullptr) {
        if (const WhileNode* ancestor = dynamic_cast<const WhileNode*>(parent.get())) {
            spFacade->addModifies(ancestor->getStmtNo(), varName);
        } else if (const IfNode* ancestor = dynamic_cast<const IfNode*>(parent.get())) {
            spFacade->addModifies(ancestor->getStmtNo(), varName);
        }
        parent = parent->getParent();
    }
}

void BasicExtractionVisitor::addAncestorUses(TNode *node, std::string varName) {
    Node parent = node->getParent();
    while (parent != nullptr) {
        if (const WhileNode* ancestor = dynamic_cast<const WhileNode*>(parent.get())) {
            spFacade->addUses(ancestor->getStmtNo(), varName);
        } else if (const IfNode* ancestor = dynamic_cast<const IfNode*>(parent.get())) {
            spFacade->addUses(ancestor->getStmtNo(), varName);
        }
        parent = parent->getParent();
    }
}

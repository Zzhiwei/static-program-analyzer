#include"QPSFacade.h"

QPSFacade::QPSFacade(std::shared_ptr <PatternManager> pm, std::shared_ptr <EntityManager> em, std::shared_ptr <RelationshipManager> rm) {
	patternManager = pm;
	entityManager = em;
	relationshipManager = rm;
}

bool QPSFacade::isFollows(int stmt1, int stmt2) {
	return relationshipManager->isFollows(stmt1, stmt2);
}

int QPSFacade::getFollowing(int s1) {
	return relationshipManager->getFollows(s1);
}

int QPSFacade::getPreceding(int s2) {
	return relationshipManager->getPreceding(s2);
}

std::vector<std::pair<int, int>> QPSFacade::getAllFollows() {
	return relationshipManager->getAllFollows();
}

bool QPSFacade::isFollowsStar(int stmt1, int stmt2) {
	return relationshipManager->isFollowsStar(stmt1, stmt2);
}

std::vector<int> QPSFacade::getFollowsStarForward(int stmt1) {
	return relationshipManager->getFollowsStarForward(stmt1);
}

std::vector<int> QPSFacade::getFollowsStarBackward(int stmt2) {
	return relationshipManager->getFollowsStarBackward(stmt2);
}

std::vector<std::pair<int, int>> QPSFacade::getAllFollowsStar() {
	return relationshipManager->getAllFollowsStar();
}

bool QPSFacade::isParent(int parent, int child) {
	return relationshipManager->isParent(parent, child);
}

int QPSFacade::getParent(int child) {
	return relationshipManager->getParent(child);
}

std::vector<int> QPSFacade::getChildren(int parent) {
	return relationshipManager->getChildren(parent);
}

std::vector<std::pair<int, int>> QPSFacade::getAllParent() {
	return relationshipManager->getAllParent();
}

bool QPSFacade::isParentStar(int parent, int child) {
	return relationshipManager->isParentStar(parent, child);
}

std::vector<int> QPSFacade::getParentStar(int child) {
	return relationshipManager->getParentStar(child);
}

std::vector<int> QPSFacade::getChildStar(int parent) {
	return relationshipManager->getChildStar(parent);
}

std::vector<std::pair<int, int>> QPSFacade::getAllParentStar() {
	return relationshipManager->getAllParentStar();
}

bool QPSFacade::isUses(int stmtNo, std::string var) {
	return relationshipManager->isUses(stmtNo, var);
}

std::vector<std::string> QPSFacade::getUses(int stmtNo) {
	return relationshipManager->getUses(stmtNo);
}

std::vector<int> QPSFacade::getUses(std::string var) {
	return relationshipManager->getUses(var);
}

std::vector<std::pair<int, std::string>> QPSFacade::getUses() {
	return relationshipManager->getUses();
}

bool QPSFacade::isModifies(int stmtNo, std::string var) {
	return relationshipManager->isModifies(stmtNo, var);
}

std::vector<std::string> QPSFacade::getModifies(int stmtNo) {
	return relationshipManager->getModifies(stmtNo);
}

std::vector<int> QPSFacade::getModifies(std::string var) {
	return relationshipManager->getModifies(var);
}

std::vector<std::pair<int, std::string>> QPSFacade::getModifies() {
	return relationshipManager->getModifies();
}

std::vector<int> QPSFacade::getStatements() {
	return entityManager->getStatements();
}

std::vector<int> QPSFacade::getPrintStatements() {
	return entityManager->getPrintStatements();
}

std::vector<int> QPSFacade::getAssignStatements() {
	return entityManager->getAssignStatements();
}

std::vector<int> QPSFacade::getWhileStatements() {
	return entityManager->getWhileStatements();
}

std::vector<int> QPSFacade::getIfStatements() {
	return entityManager->getIfStatements();
}

std::vector<int> QPSFacade::getCallStatements() {
	return entityManager->getCallStatements();
}

std::vector<int> QPSFacade::getReadStatements() {
	return entityManager->getReadStatements();
}

std::vector<std::string> QPSFacade::getVariables() {
	return entityManager->getVariables();
}

std::vector<std::string> QPSFacade::getConstants() {
	return entityManager->getConstants();
}

std::vector<int> QPSFacade::getStmtRefLhsVarRhsPartial(std::string lhs, std::string rhs) {
	return patternManager->getStmtRefLhsVarRhsPartial(lhs, rhs);
}

std::vector<int> QPSFacade::getStmtRefRhs(std::string rhs) {
	return patternManager->getStmtRefRhs(rhs);
}

std::vector<int> QPSFacade::getStmtRefLhs(std::string lhs) {
	return patternManager->getStmtRefLhs(lhs);
}

std::vector<int> QPSFacade::getStmtRefBothWild() {
	return patternManager->getStmtRefBothWild();
}
#include<stdio.h>
#include <iostream>
#include <string>
#include <vector>
#include <queue>

#include "RelationshipManager.h"



void RelationshipManager::addFollows(int stmt1, int stmt2) {
	followsForwardTable[stmt1] = stmt2;
	followsBackwardTable[stmt2] = stmt1;
}

void RelationshipManager::addParentChild(int parent, int child) {
	parentChildTable[parent].push_back(child);
	childParentTable[child] = parent;
}

void RelationshipManager::addUses(int stmtNo, std::string var) {
	std::vector<std::string> vec = usesStmtEntTable[stmtNo];
	if (std::find(vec.begin(), vec.end(), var) == vec.end()) {
		usesStmtEntTable[stmtNo].push_back(var);
		usesEntStmtTable[var].push_back(stmtNo);
	}
}

void RelationshipManager::addModifies(int stmtNo, std::string var) {
	std::vector<std::string> vec = modifiesStmtEntTable[stmtNo];
	if (std::find(vec.begin(), vec.end(), var) == vec.end()) {
		modifiesStmtEntTable[stmtNo].push_back(var);
		modifiesEntStmtTable[var].push_back(stmtNo);
	}
}

bool RelationshipManager::isFollows(int stmt1, int stmt2) {
	if (followsForwardTable.count(stmt1) == 0) {
		return false;
	}
	else {
		return stmt2 == followsForwardTable[stmt1];
	}
	
}

int RelationshipManager::getFollows(int stmt1) {
	if (followsForwardTable.count(stmt1) == 0) {
		return 0;
	}
	else {
		return followsForwardTable[stmt1];
	}
	
}

int RelationshipManager::getPreceding(int stmt2) {
	if (followsBackwardTable.count(stmt2) == 0) {
		return 0;
	}
	else {
		return followsBackwardTable[stmt2];
	}
	
}

std::vector<std::pair<int, int>> RelationshipManager::getAllFollows() {
	std::vector<std::pair<int, int>> vec;
	for (auto& [key, value] : followsForwardTable) {
		vec.push_back(std::pair(key, value));
	}
	return vec;
}

bool RelationshipManager::isFollowsStar(int stmt1, int stmt2) {
	int s1 = stmt1;
	while (followsForwardTable.count(s1) == 1) {
		s1 = followsForwardTable[s1];
		if (s1 == stmt2) {
			return true;
		}
	}
	return false;
}

std::vector<int> RelationshipManager::getFollowsStarForward(int stmt1) {
	std::vector<int> vector;
	int s1 = stmt1;
	while (followsForwardTable.count(s1) == 1) {
		s1 = followsForwardTable[s1];
		vector.push_back(s1);
	}
	return vector;
}

std::vector<int> RelationshipManager::getFollowsStarBackward(int stmt2) {
	std::vector<int> vector;
	int s2 = stmt2;
	while (followsBackwardTable.count(s2) == 1) {
		s2 = followsBackwardTable[s2];
		vector.push_back(s2);
	}
	return vector;
}

std::vector<std::pair<int, int>> RelationshipManager::getAllFollowsStar() {
	std::vector<std::pair<int, int>> vec;
	for (auto& [key, value] : followsForwardTable) {
		std::vector<int> followingElem = getFollowsStarForward(key);
		for (auto& elem : followingElem) {
			vec.push_back(std::pair(key, elem));
		}
	}
	return vec;
}

bool RelationshipManager::isParent(int parent, int child) {
	if (childParentTable.count(child) == 0) {
		return false;
	}
	else {
		return parent == childParentTable[child];
	}
	
}

std::vector<int> RelationshipManager::getChildren(int parent) {
	if (parentChildTable.count(parent) == 0) {
		return std::vector<int>{};
	}
	else {
		return parentChildTable[parent];
	}
	
}

int RelationshipManager::getParent(int child) {
	if (childParentTable.count(child) == 1) {
		return childParentTable[child];
	}
	else {
		return 0;
	}
	
}

bool RelationshipManager::isParentStar(int ancestor, int child) {
	int parent = getParent(child);
	if (parent == 0) {
		return false;
	}
	while (parent != 0) {
		if (ancestor == parent) {
			return true;
		}
		parent = getParent(parent);
	}
	return false;
}

std::vector<int> RelationshipManager::getParentStar(int child) {
	std::vector<int> vector;
	int s2 = child;
	while (childParentTable.count(s2) == 1) {
		s2 = childParentTable[s2];
		vector.push_back(s2);
	}
	return vector;
}

std::vector<std::pair<int, int>> RelationshipManager::getAllParent() {
	std::vector<std::pair<int, int>> vec;
	for (auto& [key, valueVec] : parentChildTable) {
		for (const auto& elem : valueVec) {
			vec.push_back(std::pair(key, elem));
		}
	}
	return vec;
}

std::vector<int> RelationshipManager::getChildStar(int parent) {
	std::vector<int> descendents = RelationshipManager::getChildren(parent);
	std::queue<int> yetToVisit;
	for (const auto& elem : descendents) {
		yetToVisit.push(elem);
	}
	while (!yetToVisit.empty()) {
		std::vector<int> children = RelationshipManager::getChildren(yetToVisit.front());
		yetToVisit.pop();
		descendents.insert(descendents.end(), children.begin(), children.end());
		for (const auto& elem : children) {
			yetToVisit.push(elem);
		}
	}
	return descendents;
}

std::vector<std::pair<int, int>> RelationshipManager::getAllParentStar() {
	std::vector<std::pair<int, int>> vec;
	for (auto& [parent, children] : parentChildTable) {
		std::vector<int> descendents = getChildStar(parent);
		for (auto& elem : descendents) {
			vec.push_back(std::pair(parent, elem));
		}
	}
	return vec;
}

bool RelationshipManager::isUses(int stmtNo, std::string var) {
	if (usesStmtEntTable.count(stmtNo) == 0) {
		return false;
	}
	else {
		std::vector<std::string> vec = usesStmtEntTable[stmtNo];
		return std::find(vec.begin(), vec.end(), var) != vec.end();
	}
}

std::vector<std::string> RelationshipManager::getUses(int stmtNo) {
	if (usesStmtEntTable.count(stmtNo) == 0) {
		return std::vector<std::string>{};
	}
	else {
		return usesStmtEntTable[stmtNo];
	}
	
}

std::vector<int> RelationshipManager::getUses(std::string var) {
	if (usesEntStmtTable.count(var) == 0) {
		return std::vector<int>{};
	}
	else {
		return usesEntStmtTable[var];
	}
	
}

std::vector<std::pair<int, std::string>> RelationshipManager::getUses() {
	std::vector<std::pair<int, std::string>> vec;
	for (auto& [keyTable, stringVec] : usesStmtEntTable) {
		for (auto& elem : stringVec) {
			vec.push_back(std::pair(keyTable, elem));
		}
	}
	return vec;
}

bool RelationshipManager::isModifies(int stmtNo, std::string var) {
	if (modifiesStmtEntTable.count(stmtNo) == 0) {
		return false;
	}
	else {
		std::vector<std::string> vec = modifiesStmtEntTable[stmtNo];
		if (std::find(vec.begin(), vec.end(), var) != vec.end()) {
			return true;
		}
		return false;
	}
}

std::vector<std::string> RelationshipManager::getModifies(int stmtNo) {
	if (modifiesStmtEntTable.count(stmtNo) == 0) {
		return std::vector<std::string>{};
	}
	else {
		return modifiesStmtEntTable[stmtNo];
	}
	
}

std::vector<int> RelationshipManager::getModifies(std::string var) {
	if (modifiesEntStmtTable.count(var) == 0) {
		return std::vector<int>{};
	}
	else {
		return modifiesEntStmtTable[var];
	}
	
}

std::vector<std::pair<int, std::string>> RelationshipManager::getModifies() {
	std::vector<std::pair<int, std::string>> vec;
	for (auto& [key, valueVec] : modifiesStmtEntTable) {
		for (auto& value : valueVec) {
			vec.push_back(std::pair(key, value));
		}
		
	}
	return vec;
}
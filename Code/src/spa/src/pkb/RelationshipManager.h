#pragma once

#include<stdio.h>
#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>
#include <algorithm>
#include <utility>

class RelationshipManager {
private:
	//Follows(s1,s2): s1=key, s2=value
	std::unordered_map<int, int> followsForwardTable;
	//Follows(s1,s2): s2=key, s1=value
	std::unordered_map<int, int> followsBackwardTable;
	//Parent(s1,s2): s1(parent)=key, s2(child)=value
	std::unordered_map<int, std::vector<int>> parentChildTable;
	// Parent(s1, s2) : s2(child)=key, s1(parent)=value
	std::unordered_map<int, int> childParentTable;
	std::unordered_map<int, std::vector<std::string>> usesStmtEntTable;
	std::unordered_map<std::string, std::vector<int>> usesEntStmtTable;
	std::unordered_map<int, std::vector<std::string>> modifiesStmtEntTable;
	std::unordered_map<std::string, std::vector<int>> modifiesEntStmtTable;
public:
	void addFollows(int stmt1, int stmt2);
	void addParentChild(int parent, int child);
	void addUses(int stmtNo, std::string var);
	void addModifies(int stmtNo, std::string var);
	bool isFollows(int stmt1, int stmt2);
	int getFollows(int stmt1);
	int getPreceding(int stmt2);
	std::vector<std::pair<int, int>> getAllFollows();

	bool isFollowsStar(int stmt1, int stmt2);

	/**
	 * Gets a vector of all stmtRef that follows the argument stmtRef in a Follows* r/s.
	 *
	 * @param the int stmtRef of the line that precedes the stmtRef(s) in the returned vector in a Follows* r/s.
	 * @return vector<int> of all stmtRefs that follows the param in a Follows* r/s.
	 */
	std::vector<int> getFollowsStarForward(int stmt1);

	/**
	 * Gets a vector of all stmtRef that precedes the argument stmtRef in a Follows* r/s.
	 * 
	 * @param the int stmtRef of the line that follows the stmtRef(s) in the returned vector in a Follows* r/s.
	 * @return vector<int> of all stmtRefs that precedes the param in a Follows* r/s.
	 */
	std::vector<int> getFollowsStarBackward(int stmt2);
	std::vector<std::pair<int, int>> getAllFollowsStar();

	/**
	 * checks if Parent(s1, s2) r/s is valid.
	 * 
	 * @param int stmtRef of parent.
	 * @param int stmtRef of child.
	 * @return boolean value of whether Parent r/s is valid.
	 */
	bool isParent(int parent, int child);
 	std::vector<int> getChildren(int parent);
	int getParent(int child);
	std::vector<std::pair<int, int>>getAllParent(); 

	bool isParentStar(int parent, int child);

	/**
	 * Gets a vector of all parent, grandparent, xgreatgrandparent stmtRef of input stmtRef in a Parent* r/s.
	 * 
	 * @param stmtRef of child whose ancestors are to be found.
	 * @return vector<int> of stmtRefs of param's ancestors.
	 */
	std::vector<int> getParentStar(int child);

	/**
	 * Gets a vector of all descendent's stmtRef of input stmtRef in a Parent* r/s.
	 * 
	 * @param stmtRef of parent.
	 * @return vector<int> of stmtRefs of all the inputs descendents.
	 */
	std::vector<int> getChildStar(int parent);
	std::vector<std::pair<int, int>> getAllParentStar();
	bool isUses(int stmtNo, std::string var);
	std::vector<std::string> getUses(int stmtNo);
	std::vector<int> getUses(std::string var);
	std::vector<std::pair<int, std::string>> getUses();
	bool isModifies(int stmtNo, std::string var);
	std::vector<std::string> getModifies(int stmtNo);
	std::vector<int> getModifies(std::string var);
	std::vector<std::pair<int, std::string>> getModifies();
};
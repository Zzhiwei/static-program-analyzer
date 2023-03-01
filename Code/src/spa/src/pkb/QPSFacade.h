#pragma once

#include<stdio.h>
#include <iostream>
#include <string>
#include <vector>
#include "PatternManager.h"
#include "EntityManager.h"
#include "RelationshipManager.h"

class QPSFacade {
private:
	std::shared_ptr<PatternManager> patternManager;
	std::shared_ptr <EntityManager> entityManager;
	std::shared_ptr <RelationshipManager> relationshipManager;
public:
	QPSFacade(std::shared_ptr<PatternManager> pm, std::shared_ptr <EntityManager> em, std::shared_ptr <RelationshipManager> rm);

	// Relationships

	/**
	 * Checks if Follows(s1,s2) is valid(0 synonym).
	 * 
	 * @param int s1 of Follows(s1,s2) r/s.
	 * @param int s2 of Follows(s1,s2) r/s.
	 * @return boolean value of whether Follows(s1,s2) r/s is valid.
	 */
	bool isFollows(int stmt1, int stmt2);
	/**
	 * Get s2 in a Follows(s1,s2) r/s with s1 as param.
	 * 
	 * @param int s1 stmtRef in a Follows(s1,s2) r/s.
	 * @return int s2 stmtRef in a Follows(s1,s2) r/s.
	 */
	int getFollowing(int s1);

	/**
	 * Get s1 in a Follows(s1,s2) r/s with s2 as param.
	 *
	 * @param int s2 stmtRef in a Follows(s1,s2) r/s.
	 * @return int s1 stmtRef in a Follows(s1,s2) r/s.
	 */
	int getPreceding(int s2);

	/**
	 * Gets all pair of (s1,s2) in a Follows(s1,s2) r/s.
	 * 
	 * @return a vector of pair of (s1,s2) in a Follows(s1,s2) r/s.
	 */
	std::vector<std::pair<int, int>> getAllFollows();

	/**
	 * Checks that Follows*(s1,s2) r/s is valid.
	 * 
	 * @param int stmtRef of s1 in Follows*(s1,s2).
	 * @param int stmtRef of s2 in Follows*(s1,s2).
	 * @return boolean value of whether Follows*(s1,s2) is valid.
	 */
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
	 * @return vector<int> of all stmtRefs that precedes the param in a Follows* r/s, stmtRef order will be backwards.
	 */
	std::vector<int> getFollowsStarBackward(int stmt2);

	/**
	 * Gets a vector of pairs<int,int> of stmtRef and stmtRef that satisfies a Follow*(s1,s2) r/s.
	 * 
	 * @return a vector of pairs<int,int> of stmtRef and stmtRef that satisfies a Follow*(s1,s2) r/s.
	 */
	std::vector<std::pair<int, int>> getAllFollowsStar();

	/**
	 * Checks if Parent(s1, s2) r/s is valid.
	 *
	 * @param int stmtRef of parent.
	 * @param int stmtRef of child.
	 * @return boolean value of whether Parent r/s is valid.
	 */
	bool isParent(int parent, int child);

	/**
	 * Gets the stmtRef of the parent of the given child's stmtRef in a Parent r/s.
	 * 
	 * @param int stmtRef of the child.
	 * @return int stmtRef of the parent of the given child.
	 */
	int getParent(int child);

	/**
	 * Gets all pair of (s1,s2) in Parent(s1,s2) r/s.
	 * 
	 * @retuns a vector of pair of (s1,s2) in Parent(s1,s2) r/s.
	 */
	std::vector<std::pair<int, int>> getAllParent();
	/**
	 * Checks if Parent*(s1,s2) is valid.
	 * 
	 * @param int stmtRef of parent(can be direct parent or an ancestor).
	 * @param int stmtRef of child(can be direct child or descendent).
	 * @return boolean value of whether the Parent* r/s is valid.
	 */
	bool isParentStar(int parent, int child);
	/**
	 * Gets a vector<int> of the children stmtRef of the given stmtRef in a Parent r/s.
	 * 
	 * @param int stmtRef of the parent.
	 * @return vector<int> of stmtRef of the children of the given parent.
	 */
	std::vector<int> getChildren(int parent);

	/**
	 * Gets a vector of all parent, grandparent, xgreatgrandparent stmtRef of input stmtRef in a Parent* r/s.
	 *
	 * @param int stmtRef of child whose ancestors are to be found.
	 * @return vector<int> of stmtRefs of param's ancestors.
	 */
	std::vector<int> getParentStar(int child);

	/**
	 * Gets a vector of all descendent's stmtRef of input stmtRef in a Parent* r/s.
	 * In order of nesting level.
	 *
	 * @param int stmtRef of parent.
	 * @return vector<int> of stmtRefs of all the inputs descendents.
	 */
	std::vector<int> getChildStar(int parent);

	/**
	 * Gets all pair of stmtRefs that satisfifes the Parent*(s1,s2) r/s.
	 * 
	 * @return a vector of pair<int,int> of stmtRefs that satisfifes the Parent*(s1,s2) r/s.
	 */
	std::vector<std::pair<int, int>> getAllParentStar();

	/**
	 * Checks if Uses(s1, v1)r/s is valid.
	 * 
	 * @param int stmtRef.
	 * @param string entRef.
	 * @return boolean value of whether the Uses r/s is valid.
	 */
	bool isUses(int stmtNo, std::string var);

	/**
	 * Gets a vector<string> of entRef given a stmtRef.
	 * 
	 * @param stmtRef.
	 * @return vector<string> of entRef in the given stmtRef which follows the Uses r/s.
	 */
	std::vector<std::string> getUses(int stmtNo);

	/**
	 * Gets a vector<int> of stmtRefs that uses the given entRef.
	 * 
	 * @param entRef.
	 * @return vector<int> of stmtRefs that uses the given entRef.
	 */
	std::vector<int> getUses(std::string var);

	/**
	 * Gets all pair of stmtRef and entRef that satisfies the Use r/s.
	 * 
	 * @return a vector of pair of stmtRef and entRef that satisfies the Use r/s.
	 */
	std::vector<std::pair<int, std::string>> getUses();

	/**
	 * Checks if Modifies(s1,v1) r/s is valid. 
	 * 
	 * @param int stmtRef.
	 * @param string entRef.
	 * @return boolean value of whether the Modifies r/s is valid.
	 */
	bool isModifies(int stmtNo, std::string var);

	/**
	 * Gets a string of the entRef being Modified in the given stmtRef.
	 * 
	 * @param stmtRef.
	 * @return string of the entRef being Modified in the given stmtRef.
	 */
	std::vector<std::string> getModifies(int stmtNo);

	/**
	 * Gets a vector<int> of all stmtRefs that modifies the given entRef.
	 * 
	 * @param entRef.
	 * @return vector<int> of all stmtRefs that modifies the given entRef.
	 */
	std::vector<int> getModifies(std::string var);

	/**
	 * Gets all pairs(stmtRef, entRef) that satisfies Modifies r/s.
	 * 
	 * @return a vector of pair(int, string) of stmtRef and entRef that satisfies Modifies r/s.
	 */
	std::vector<std::pair<int, std::string>> getModifies();

	// Entities

	/**
	 * Gets all stmtRefs.
	 * 
	 * @return vector<int> of all stmtRefs.
	 */
	std::vector<int> getStatements();
	std::vector<int> getPrintStatements();
	std::vector<int> getAssignStatements();
	std::vector<int> getWhileStatements();
	std::vector<int> getIfStatements();
	std::vector<int> getCallStatements();
	std::vector<int> getReadStatements();

	/**
	 * Gets all variable names.
	 *
	 * @return vector<string> of all variables.
	 */
	std::vector<std::string> getVariables();

	/**
	 * Gets all constants.
	 *
	 * @return vector<int> of all constants.
	 */
	std::vector<std::string> getConstants();

	//Pattern

	/**
	 * Gets a vector<int> of stmtRefs where lhs is the first param and the second param is a variable/constant in rhs.
	 * 
	 * @param string of lhs in an assign stmt.
	 * @param string a constant/variable which is part of the rhs.
	 */
	std::vector<int> getStmtRefLhsVarRhsPartial(std::string lhs, std::string rhs);

	/**
	 * Gets a vector<int> of stmtRefs where the provided param at the RHS is a variable/constant in the RHS while LHS is wildcard.
	 * 
	 * @param the rhs variable of an assign stmt.
	 * @return a vector<int> of stmtRefs where the param is a variable in the rhs.
	 */
	std::vector<int> getStmtRefRhs(std::string rhs);

	/**
	 * Gets a vector<int> of stmtRefs where the provided param at the LHS is a variable while LHS is wildcard.
	 * 
	 * @param the lhs variable of an assign stmt.
	 * @return a vector<int> of stmtRefs where the param is the variable in the lhs.
	 */
	std::vector<int> getStmtRefLhs(std::string lhs);

	/**
	 * Gets a vector<int> of stmtRefs where both lhs and rhs are wildcards.
	 * 
	 * @return a vector<int> of all assign stmtRefs.
	 */
	std::vector<int> getStmtRefBothWild();
};
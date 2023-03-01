#include <stack>

#include "QueryEvaluator.h"

ClauseResult QueryEvaluator::computeFinalResult(Query &query, QPSFacade& qpsFacade) {

	std::stack<ClauseResult> stack;
	std::vector<std::shared_ptr<Clause>> clauseList = query.getClauseList();

	// Create list of synonyms and their type
	std::unordered_map<std::string, std::string> synonymTypes;
	for (auto declaration : query.getDeclarationList()) {
		for (auto synonym : declaration.getSynonymList()) {
			synonymTypes[synonym] = declaration.getType();
		}
	}

	//select a pattern a
	std::string assignSynonym;
	for (auto clause : clauseList) {
		//eval pattern -> clause result list
		if (clause->getAssignSynonym().size() > 0) {
			ClauseResult patternRes = ClauseResultBuilder::createBooleanResult(true);
			std::vector<std::string> patternList = clause->evaluateSpecial(qpsFacade, query.getTarget(), synonymTypes);
			assignSynonym = clause->getAssignSynonym();
			bool hasSynonym = false;
			for (auto clause : clauseList) {
				if (clause->getAssignSynonym().size() == 0) {
					if (clause->getRHS() == assignSynonym || clause->getLHS() == assignSynonym) {
						hasSynonym = true;
						break;
					}
				}	
			}
			if (query.getTarget() == assignSynonym) {
				patternRes = ClauseResultBuilder::createListResult(assignSynonym, patternList);
			}
			else {
				patternRes = ClauseResultBuilder::createBooleanResult(!patternList.empty());
			}
			if (hasSynonym) {
				patternRes = ClauseResultBuilder::createListResult(assignSynonym, patternList);
			}
			stack.push(patternRes);
		}
		else {
			stack.push(clause->evaluate(qpsFacade, query.getTarget(), synonymTypes));
		}

	}

	/*for (auto clause : clauseList)
	{
		stack.push(clause->evaluate(qpsFacade, query.getTarget(), synonymTypes));
	}*/

	while (stack.size() > 1) {
		ClauseResult first = stack.top();
		stack.pop();
		ClauseResult second = stack.top();
		stack.pop();
		stack.push(ClauseResultEvaluator::evaluate(first, second));
	}

	if (stack.size() == 0) {
		//return select
		return ClauseResultBuilder::createBooleanResult(true);
	}

	return stack.top();
}
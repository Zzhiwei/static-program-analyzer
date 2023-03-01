#include <vector>
#include <string>
#include <unordered_map>

#include "ClauseResultEvaluator.h"

ClauseResult ClauseResultEvaluator::evaluate(ClauseResult& c1, ClauseResult& c2) {
	if (c1.isBoolean())			return ClauseResultEvaluator::evaluateBool(c1, c2);			// Boolean-Boolean and Boolean-Table pairs
	else if (c2.isBoolean())	return ClauseResultEvaluator::evaluateBool(c2, c1);			// Table-Boolean pairs
	else						return ClauseResultEvaluator::evaluateTableTable(c1, c2);	// Table-Table pairs
}

ClauseResult ClauseResultEvaluator::evaluateBool(ClauseResult& boolClause, ClauseResult& c2) {
	return boolClause.status == StatusType::ValidBooleanResult ? c2 : boolClause;
}

ClauseResult ClauseResultEvaluator::evaluateTableTable(ClauseResult& c1, ClauseResult& c2) {

	ClauseResult smallerClause = c1.synonymList.size() < c2.synonymList.size() ? c1 : c2;
	ClauseResult largerClause = c1.synonymList.size() < c2.synonymList.size() ? c2 : c1;

	std::vector<std::string> synonyms1 = smallerClause.synonymList;
	std::vector<std::string> synonyms2 = largerClause.synonymList;
	std::vector<std::vector<std::string>> table1 = smallerClause.table;
	std::vector<std::vector<std::string>> table2 = largerClause.table;

	// Find for all common synonyms and note their indexes in both clauses
	std::unordered_map < std::string, std::vector<int> > commonSynonyms;
	for (int i = 0; i < synonyms1.size(); i++) {
		std::string curr_i = synonyms1[i];

		for (int j = 0; j < synonyms2.size(); j++) {
			std::string curr_j = synonyms2[j];
			if (curr_i != curr_j) continue;

			std::vector<int> arr;
			arr.push_back(i);
			arr.push_back(j);
			commonSynonyms.insert(std::make_pair(curr_i, arr));
		}
	}

	if (commonSynonyms.size() == 0) return ClauseResultBuilder::createBooleanResult(false);

	// New size of table n = total_synonyms in both tables - common_synonyms
	int newSize = synonyms1.size() + synonyms2.size() - commonSynonyms.size();

	// Get synonymList
	// Find list of non-common row indexes to use for appending the elements
	std::vector<std::string> newSynonymList;
	std::vector<int> nonCommonCols1;
	std::vector<int> nonCommonCols2;

	for (auto i = 0; i < synonyms1.size(); i++) {
		std::string synonym = synonyms1[i];
		if (commonSynonyms.find(synonym) == commonSynonyms.end()) {
			newSynonymList.push_back(synonym);
			nonCommonCols1.push_back(i);
		}
	}
	for (auto i = 0; i < synonyms2.size(); i++) {
		std::string synonym = synonyms2[i];
		if (commonSynonyms.find(synonym) == commonSynonyms.end()) {
			newSynonymList.push_back(synonym);
			nonCommonCols2.push_back(i);
		}
	}
	for (auto synonym : commonSynonyms) newSynonymList.push_back(synonym.first);

	// For each row in the one with lesser synonyms, find if both synonyms contain the the common two
	// If yes, Initialise vector tuple, append all unique elements in that row to the vector and append to vector result, following synonym order above
	std::vector<std::vector<std::string>> newTable;

	for (auto row1 : table1) {
		for (auto row2 : table2) {
			bool addRow = true;
			for (auto commonSynonym : commonSynonyms) {
				std::vector<int> indexes = commonSynonym.second;
				addRow = addRow && (row1[indexes[0]] == row2[indexes[1]]);
			}
			if (!addRow) continue;

			std::vector<std::string> newRow;
			for (auto index : nonCommonCols1) newRow.push_back(row1[index]);
			for (auto index : nonCommonCols2) newRow.push_back(row2[index]);
			for (auto commonSynonym : commonSynonyms) newRow.push_back(row1[commonSynonym.second[0]]);
			newTable.push_back(newRow);
		}
	}

	// Init new clause result with the vector result
	return newTable.size() == 0
		? ClauseResultBuilder::createBooleanResult(false)
		: ClauseResultBuilder::createTableResult(newSynonymList, newTable, newSize);
}
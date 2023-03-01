#include <cassert>
#include <iostream>
#include <unordered_map>

#include "ClauseResult.h"

ClauseResult::ClauseResult(bool status) 
	: synonymList(),
	table()
{
	if (status)
		this->status = StatusType::ValidBooleanResult;
	else
		this->status = StatusType::InvalidBooleanResult;
}

ClauseResult::ClauseResult(std::vector< std::string > &synonymList, std::vector<std::vector<std::string>> &table) 
	: status(StatusType::ValidTableResult),
	synonymList(synonymList),
	table(table)
{
	for (auto row : table) assert(synonymList.size() == row.size());
}

int ClauseResult::size() {
	return this->synonymList.size();
}

bool ClauseResult::isBoolean() {
	return this->status == StatusType::ValidBooleanResult || this->status == StatusType::InvalidBooleanResult;
}

std::vector< std::vector < std::string> > ClauseResult::getTable() {
	return this->table;
}

bool ClauseResult::operator==(const ClauseResult& other) const {
	if (const ClauseResult* o = dynamic_cast<const ClauseResult*>(&other)) {
		// Early termination
		if (o->status != this->status) return false;
		if (o->status != StatusType::ValidTableResult) return true;
		if (o->synonymList.size() != this->synonymList.size()) return false;
		if (o->table.size() != this->table.size()) return false;

		// Map each synonym to the other's index, if present
		std::vector<std::vector<int>> synonymIndexes;
		if (!this->mapSynonyms(synonymIndexes, o)) return false;

		// Compare each row based on the mapped synonym indexes
		for (auto pair : synonymIndexes) {
			std::unordered_map<std::string, int> values;
			// Save to map
			for (auto currRow : this->table) {
				auto currString = currRow[pair[0]];
				if (values.find(currString) == values.end()) {
					values[currString] = 0;
				}
						
				values[currString] += 1;
					
			}
			// Remove from map
			for (auto currRow : o->table) {
				auto currString = currRow[pair[1]];
				if (values.find(currString) != values.end()) {
					values[currString] -= 1;
				}
				else {
					return false;
				}
			}
			// Check if elements remaining
			for (auto valuePair : values) {
				if (valuePair.second != 0) return false;
			}
		}

		return true;
	}
	return false;
}

std::vector<std::string> ClauseResult::extractQueryData(QPSFacade qpsf, std::string synonym, std::string synonymType) {
	switch (this->status) {
	case StatusType::InvalidBooleanResult:
		return { };
	case StatusType::ValidBooleanResult:
		return this->getEntitiesFromQps(qpsf, synonymType);
	case StatusType::ValidTableResult:
		return extractFromTable(synonym, synonymType, qpsf);
	}
	throw QpsSyntaxException(synonymType + " is not a synonym type");// TODO
}

std::vector<std::string> ClauseResult::extractFromTable(std::string target, std::string synonymType, QPSFacade qpsf) {
	// Check index of synonym
	int index = -1;
	for (int i = 0; i < this->synonymList.size(); i++) {
		//std::cout << "synonym " + this->synonymList[i] << std::endl;
		//std::cout << "target " + target << std::endl;
		if (!target.compare(this->synonymList[i])) {
			index = i;
			break;
		}
	}
	//std::cout << "INDEX " + std::to_string(index) << std::endl;
	// Early termination
	//if (index < 0) return { };
	if (index < 0) {
		if (this->table.empty()) {
			return { };
		}
		return this->getEntitiesFromQps(qpsf, synonymType);
	}

	std::vector<std::string> res;
	for (auto tuple : this->table) {
		//std::cout << "tuple val " + tuple[index] << std::endl;
		res.push_back(tuple[index]);
	}

	//remove duplicates
	sort(res.begin(), res.end());
	res.erase(unique(res.begin(), res.end()), res.end());

	//inner join with select
	std::vector<std::string> selectAll = this->getEntitiesFromQps(qpsf, synonymType);
	std::vector<std::string> finalRes;
	for (std::string i : res) {
		//if i is in selectAll
		if (std::find(selectAll.begin(), selectAll.end(), i) != selectAll.end()) {
			finalRes.push_back(i);
		}
	}
	return finalRes;
}

std::vector<std::string> ClauseResult::getEntitiesFromQps(QPSFacade &qpsf, std::string type) {
	if (type == "variable") {
		return qpsf.getVariables();
	}
	else if (type == "stmt") {
		return this->intArrToStringArr(qpsf.getStatements());
	}
	else if (type == "constant") {
		return qpsf.getConstants();
	}
	else if (type == "read") {
		return this->intArrToStringArr(qpsf.getReadStatements());
	}
	else if (type == "print") {
		return this->intArrToStringArr(qpsf.getPrintStatements());
	}
	else if (type == "while") {
		return this->intArrToStringArr(qpsf.getWhileStatements());
	}
	else if (type == "if") {
		return this->intArrToStringArr(qpsf.getIfStatements());
	}
	else if (type == "assign") {
		return this->intArrToStringArr(qpsf.getAssignStatements());
	}

	throw QpsSyntaxException("Synonym type does not exist");
}

std::vector< std::string > ClauseResult::intArrToStringArr(std::vector<int> arr) {
	std::vector < std::string > res;
	for (const auto& integer :arr) {
		res.push_back(std::to_string(integer));
	}
	return res;
}

// Populates synonymIndexes with the mapped indexes
// Returns false if there is any synonym in this that does not exist in o, or vice versa
bool ClauseResult::mapSynonyms(std::vector<std::vector<int>>& synonymIndexes, const ClauseResult* o) const {
	for (int i = 0; i < this->synonymList.size(); i++) {
		bool oHasSynonym = false;
		for (int j = 0; j < o->synonymList.size(); j++) {
			if (this->synonymList[i] != o->synonymList[j]) continue;
			synonymIndexes.push_back({ i, j });
			oHasSynonym = true;
			break;
		}

		if (!oHasSynonym) return false;
	}
	return true;
}
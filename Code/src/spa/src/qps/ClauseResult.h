#pragma once

#include <string>
#include <vector>
#include <memory>
#include <unordered_set>

#include "ClauseResultBuilder.h"
#include "ClauseResultEvaluator.h"
#include "pkb/QPSFacade.h"
#include "QpsException.h"

enum class StatusType {
	ValidTableResult,
	ValidBooleanResult,
	InvalidBooleanResult
};

class ClauseResult {
private:
	StatusType status;
	std::vector< std::string > synonymList;
	std::vector< std::vector < std::string> > table;

	// boolean Result
	ClauseResult(bool status);

	// n-table result
	ClauseResult(std::vector< std::string > &synonymList, std::vector< std::vector < std::string> > &table);

	std::vector< std::string > extractFromTable(std::string synonym, std::string synonymType, QPSFacade qpsf);
	std::vector< std::string > getEntitiesFromQps(QPSFacade &qpsf, std::string type);
	std::vector< std::string > intArrToStringArr(std::vector<int> arr);
	bool mapSynonyms(std::vector<std::vector<int>>& synonymIndexes, const ClauseResult* o) const;

	friend class ClauseResultBuilder;
	friend class ClauseResultEvaluator;
public:
	bool isBoolean();
	bool operator==(const ClauseResult& other) const;
	int size();
	std::vector< std::vector < std::string> > getTable();
	std::vector< std::string > extractQueryData(QPSFacade qpsf, std::string synonym, std::string synonymType);
};
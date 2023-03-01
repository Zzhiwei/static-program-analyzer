#include <string>
#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include "catch.hpp"

#include "pkb/PKB.h"
#include "pkb/QPSFacade.h"
#include "pkb/SPFacade.h"
#include "qps/ClauseResultBuilder.h"
#include "qps/ClauseResult.h"

void populatePkb(std::shared_ptr<SPFacade> spf) {
	spf->addConstant("5");
	spf->addConstant("10");
	spf->addConstant("15");
	spf->addConstant("124");
	spf->addVariable("x");
	spf->addVariable("y");
	spf->addVariable("z");
	spf->addVariable("abs");
	spf->addStatement(1, StatementType::StatementAssign);
	spf->addStatement(2, StatementType::StatementCall);
	spf->addStatement(3, StatementType::StatementAssign);
	spf->addStatement(4, StatementType::StatementWhile);
	spf->addStatement(5, StatementType::StatementAssign);
	spf->addStatement(6, StatementType::StatementRead);
	spf->addStatement(7, StatementType::StatementIf);
}

TEST_CASE("Boolean ClauseResult methods") {
	
	std::vector<std::vector<std::string>> emptyTupleList;
	std::vector<std::string> emptyStringList;
	PKB pkb;
	QPSFacade qpsf = pkb.getQPSFacade();

	SECTION("False") {
		ClauseResult falseResult = ClauseResultBuilder::createBooleanResult(false);
		REQUIRE(falseResult.getTable() == emptyTupleList);
		REQUIRE(falseResult.isBoolean());
		REQUIRE(falseResult.size() == 0);
		REQUIRE(falseResult.extractQueryData(qpsf, "test", "testType") == emptyStringList);
	}
	
	SECTION("True") {
		ClauseResult trueResult = ClauseResultBuilder::createBooleanResult(true);
		REQUIRE(trueResult.getTable() == emptyTupleList);
		REQUIRE(trueResult.isBoolean());
		REQUIRE(trueResult.size() == 0);
		REQUIRE(trueResult.extractQueryData(qpsf, "test", "variable") == emptyStringList);
		REQUIRE_THROWS(trueResult.extractQueryData(qpsf, "test", "nonvalid-type") == emptyStringList);

        std::shared_ptr<SPFacade> spf = pkb.getSPFacade();
		populatePkb(spf);
    
		std::vector<std::string> varList = { "abs", "x", "y", "z" };
		auto resultList1 = trueResult.extractQueryData(qpsf, "test", "variable");
		std::sort(resultList1.begin(), resultList1.end());
		REQUIRE(resultList1 == varList);

		std::vector<std::string> stmtList = { "1", "2", "3", "4", "5", "6" , "7"};
		auto resultList2 = trueResult.extractQueryData(qpsf, "anySynonymName", "stmt");
		std::sort(resultList2.begin(), resultList2.end());
		REQUIRE(resultList2 == stmtList);

		std::vector<std::string> constantList = { "10", "124", "15", "5" };
		auto resultList3 = trueResult.extractQueryData(qpsf, "", "constant");
		std::sort(resultList3.begin(), resultList3.end());
		REQUIRE(resultList3 == constantList);
	}
}

TEST_CASE("Table ClauseResult methods") {
	PKB pkb;
	QPSFacade qpsf = pkb.getQPSFacade();
	std::vector<std::vector<std::string>> emptyTupleList;
	std::vector<std::string> emptyStringList;

	SECTION("List") {
		std::vector<std::string> list = { "x", "y", "z" };
		ClauseResult listResult = ClauseResultBuilder::createListResult("v", list);
		REQUIRE_FALSE(listResult.isBoolean());
		REQUIRE(listResult.size() == 1);
		REQUIRE(listResult.extractQueryData(qpsf, "v", "variable") == list);
		REQUIRE(listResult.extractQueryData(qpsf, "c", "constant") == emptyStringList);
	}

	SECTION("Tuple") {
		std::vector<std::string> list1 = { "x", "1" };
		std::vector<std::string> list2 = { "y", "2" };
		std::vector<std::string> list3 = { "z", "3" };
		std::vector<std::vector<std::string>> variableList = { list1, list2, list3 };
		std::vector<std::string> synonymList = { "synonym1", "synonym2"};
		ClauseResult tupleResult = ClauseResultBuilder::createTupleResult(synonymList, variableList);
		REQUIRE_FALSE(tupleResult.isBoolean());
		REQUIRE(tupleResult.size() == 2);
		std::vector<std::string> synonym1List = { "x", "y", "z" };
		std::vector<std::string> synonym2List = { "1", "2", "3" };
		REQUIRE(tupleResult.extractQueryData(qpsf, "synonym1", "variable") == synonym1List);
		REQUIRE(tupleResult.extractQueryData(qpsf, "synonym2", "constant") == synonym2List);
		REQUIRE(tupleResult.extractQueryData(qpsf, "invalidSynonym", "stmt") == emptyStringList);
	}
}
#include "qps/Clause.h"
#include "pkb/PKB.h"
#include <string>
#include <iostream>
#include "catch.hpp"

TEST_CASE("General Clause") {
	SECTION("Getters") {
		std::string noSynLHS = "1";
		std::string noSynRHS = "\"x\"";
		Clause clause = Clause(noSynLHS, noSynRHS);
		REQUIRE(clause.getLHS() == "1");
		REQUIRE(clause.getRHS() == "\"x\"");
	}
	SECTION("isSynonym") {
		std::string noSynLHS = "s";
		std::string noSynRHS = "\"x\"";
		Clause clause = Clause(noSynLHS, noSynRHS);
		REQUIRE(clause.isSynonym(clause.getLHS()) == true);
		REQUIRE(clause.isSynonym(clause.getRHS()) == false);
	}
}

TEST_CASE("FollowsClause Evaluate") {
	std::vector<int> ifs = { 1,2, 3 };
	ifs.erase(ifs.begin() + 2);
	REQUIRE(ifs[0] == 0);
	REQUIRE((!(std::find(ifs.begin(), ifs.end(), 3) != ifs.end())));

	PKB pkb;
	std::shared_ptr<SPFacade> spFacade = pkb.getSPFacade();
	QPSFacade qpsFacade = pkb.getQPSFacade();
	std::string target = "dummy";
	SECTION("0 synonym") {
		spFacade->addFollows(1, 2);
		std::string noSynLHS = "1";
		std::string noSynRHS = "2";
		FollowsClause followsClause = FollowsClause(noSynLHS, noSynRHS);
		ClauseResult clauseResult = ClauseResultBuilder::createBooleanResult(true);
		//REQUIRE(followsClause.evaluate(qpsFacade, target) == clauseResult);
	}
	SECTION("LHS synonym") {
		spFacade->addFollows(1, 2);
		spFacade->addFollows(2, 3);
		std::string synLHS = "s";
		std::string noSynRHS = "2";
		FollowsClause followsClause = FollowsClause(synLHS, noSynRHS);
		std::vector<std::string> list;
		list.push_back("1");
		ClauseResult clauseResult = ClauseResultBuilder::createListResult(synLHS, list);
		//REQUIRE(followsClause.evaluate(qpsFacade, target) == clauseResult);
	}
	SECTION("RHS synonym") {
		spFacade->addFollows(1, 2);
		spFacade->addFollows(2, 3);
		std::string noSynLHS = "2";
		std::string synRHS = "s";
		FollowsClause followsClause = FollowsClause(noSynLHS, synRHS);
		std::vector<std::string> list;
		list.push_back("3");
		ClauseResult clauseResult = ClauseResultBuilder::createListResult(synRHS, list);
		//REQUIRE(followsClause.evaluate(qpsFacade, target) == clauseResult);
	}
	SECTION("2 synonyms") {
		// to be implemented
		//REQUIRE(0 == 0);
	}
}

TEST_CASE("FollowsTClause Evaluate") {
	PKB pkb;
	std::shared_ptr<SPFacade> spFacade = pkb.getSPFacade();
	QPSFacade qpsFacade = pkb.getQPSFacade();
	std::string target = "dummy";
	SECTION("0 synonym") {
		spFacade->addFollows(1, 2);
		spFacade->addFollows(2, 3);
		std::string noSynLHS = "1";
		std::string noSynRHS = "3";
		FollowsTClause followsTClause = FollowsTClause(noSynLHS, noSynRHS);
		ClauseResult clauseResult = ClauseResultBuilder::createBooleanResult(true);
		//REQUIRE(followsTClause.evaluate(qpsFacade, target) == clauseResult);
	}
	SECTION("LHS synonym") {
		spFacade->addFollows(1, 2);
		spFacade->addFollows(2, 3);
		std::string synLHS = "s";
		std::string noSynRHS = "3";
		FollowsTClause followsTClause = FollowsTClause(synLHS, noSynRHS);
		std::vector<std::string> list;
		list.push_back("1");
		list.push_back("2");
		ClauseResult clauseResult = ClauseResultBuilder::createListResult(synLHS, list);
		//REQUIRE(followsTClause.evaluate(qpsFacade, target) == clauseResult);
	}
	SECTION("RHS synonym") {
		spFacade->addFollows(1, 2);
		spFacade->addFollows(2, 3);
		spFacade->addFollows(3, 4);
		std::string noSynLHS = "2";
		std::string synRHS = "s";
		FollowsTClause followsTClause = FollowsTClause(noSynLHS, synRHS);
		std::vector<std::string> list;
		list.push_back("3");
		list.push_back("4");
		ClauseResult clauseResult = ClauseResultBuilder::createListResult(synRHS, list);
		//REQUIRE(followsTClause.evaluate(qpsFacade, target) == clauseResult);
	}
	SECTION("2 synonyms") {
		spFacade->addFollows(1, 2);
		spFacade->addFollows(2, 3);
		std::string synLHS = "s1";
		std::string synRHS = "s2";
		FollowsTClause followsTClause = FollowsTClause(synLHS, synRHS);
		std::vector<std::string> synList;
		synList.push_back(synLHS);
		synList.push_back(synRHS);
		std::vector<std::string> list1;
		list1.push_back("1");
		list1.push_back("2");
		std::vector<std::string> list2;
		list2.push_back("2");
		list2.push_back("3");
		std::vector<std::string> list3;
		list3.push_back("1");
		list3.push_back("3");
		std::vector<std::vector<std::string>> tupleList;
		tupleList.push_back(list1);
		tupleList.push_back(list2);
		tupleList.push_back(list3);
		ClauseResult clauseResult = ClauseResultBuilder::createTupleResult(synList, tupleList);
		//REQUIRE(followsTClause.evaluate(qpsFacade, target) == clauseResult);
	}
}

TEST_CASE("UsesClause Evaluate") {
	PKB pkb;
    std::shared_ptr<SPFacade> spFacade = pkb.getSPFacade();
	QPSFacade qpsFacade = pkb.getQPSFacade();
	std::string target = "dummy";
	SECTION("0 synonym") {
		spFacade->addUses(1, "\"x\"");
		std::string noSynLHS = "1";
		std::string noSynRHS = "\"x\"";
		UsesClause usesClause = UsesClause(noSynLHS, noSynRHS);
		ClauseResult clauseResult = ClauseResultBuilder::createBooleanResult(true);
		//REQUIRE(usesClause.evaluate(qpsFacade, target) == clauseResult);
	}
	SECTION("LHS synonym") {
		spFacade->addUses(1, "\"x\"");
		spFacade->addUses(3, "\"x\"");
		std::string synLHS = "s";
		std::string noSynRHS = "\"x\"";
		UsesClause usesClause = UsesClause(synLHS, noSynRHS);
		std::vector<std::string> list;
		list.push_back("1");
		list.push_back("3");
		ClauseResult clauseResult = ClauseResultBuilder::createListResult(synLHS, list);
		//REQUIRE(usesClause.evaluate(qpsFacade, target) == clauseResult);
	}
	SECTION("RHS synonym") {
		spFacade->addUses(1, "\"x\"");
		spFacade->addUses(1, "\"y\"");
		std::string noSynLHS = "1";
		std::string synRHS = "v";
		UsesClause usesClause = UsesClause(noSynLHS, synRHS);
		std::vector<std::string> list;
		list.push_back("\"x\"");
		list.push_back("\"y\"");
		ClauseResult clauseResult = ClauseResultBuilder::createListResult(synRHS, list);
		//REQUIRE(usesClause.evaluate(qpsFacade, target) == clauseResult);
	}
	SECTION("2 synonyms") {
		spFacade->addUses(1, "\"x\"");
		spFacade->addUses(3, "\"y\"");
		std::string synLHS = "s";
		std::string synRHS = "v";
		UsesClause usesClause = UsesClause(synLHS, synRHS);
		std::vector<std::string> synList;
		synList.push_back(synLHS);
		synList.push_back(synRHS);
		std::vector<std::string> list1;
		list1.push_back("1");
		list1.push_back("\"x\"");
		std::vector<std::string> list2;
		list2.push_back("3");
		list2.push_back("\"y\"");
		std::vector<std::vector<std::string>> tupleList;
		tupleList.push_back(list1);
		tupleList.push_back(list2);
		ClauseResult clauseResult = ClauseResultBuilder::createTupleResult(synList, tupleList);
		//REQUIRE(usesClause.evaluate(qpsFacade, target) == clauseResult);
	}
}

TEST_CASE("ModifiesClause Evaluate") {
	PKB pkb;
	std::shared_ptr<SPFacade> spFacade = pkb.getSPFacade();
	QPSFacade qpsFacade = pkb.getQPSFacade();
	std::string target = "dummy";
	SECTION("0 synonym") {
		spFacade->addModifies(1, "\"x\"");
		std::string noSynLHS = "1";
		std::string noSynRHS = "\"x\"";
		ModifiesClause modifiesClause = ModifiesClause(noSynLHS, noSynRHS);
		ClauseResult clauseResult = ClauseResultBuilder::createBooleanResult(true);
		//REQUIRE(modifiesClause.evaluate(qpsFacade, target) == clauseResult);
	}
	SECTION("LHS synonym") {
		spFacade->addModifies(1, "\"x\"");
		spFacade->addModifies(3, "\"x\"");
		std::string synLHS = "s";
		std::string noSynRHS = "\"x\"";
		ModifiesClause modifiesClause = ModifiesClause(synLHS, noSynRHS);
		std::vector<std::string> list;
		list.push_back("1");
		list.push_back("3");
		ClauseResult clauseResult = ClauseResultBuilder::createListResult(synLHS, list);
		//REQUIRE(modifiesClause.evaluate(qpsFacade, target) == clauseResult);
	}
	SECTION("RHS synonym") {
		spFacade->addModifies(1, "\"x\"");
		spFacade->addModifies(2, "\"y\"");
		std::string noSynLHS = "1";
		std::string synRHS = "v";
		ModifiesClause modifiesClause = ModifiesClause(noSynLHS, synRHS);
		std::vector<std::string> list;
		list.push_back("\"x\"");
		ClauseResult clauseResult = ClauseResultBuilder::createListResult(synRHS, list);
		//REQUIRE(modifiesClause.evaluate(qpsFacade, target) == clauseResult);
	}
	SECTION("2 synonyms") {
		spFacade->addModifies(1, "\"x\"");
		spFacade->addModifies(3, "\"y\"");
		spFacade->addModifies(5, "\"x\"");
		std::string synLHS = "s";
		std::string synRHS = "v";
		ModifiesClause modifiesClause = ModifiesClause(synLHS, synRHS);
		std::vector<std::string> synList;
		synList.push_back(synLHS);
		synList.push_back(synRHS);
		std::vector<std::string> list1;
		list1.push_back("1");
		list1.push_back("\"x\"");
		std::vector<std::string> list2;
		list2.push_back("3");
		list2.push_back("\"y\"");
		std::vector<std::string> list3;
		list3.push_back("5");
		list3.push_back("\"x\"");
		std::vector<std::vector<std::string>> tupleList;
		tupleList.push_back(list1);
		tupleList.push_back(list2);
		tupleList.push_back(list3);
		ClauseResult clauseResult = ClauseResultBuilder::createTupleResult(synList, tupleList);
		//REQUIRE(modifiesClause.evaluate(qpsFacade, target) == clauseResult);
	}
}
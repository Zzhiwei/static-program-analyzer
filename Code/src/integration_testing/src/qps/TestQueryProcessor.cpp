#include "qps/queryProcessor.h"
#include "pkb/PKB.h"
#include <string>
#include <iostream>
#include "catch.hpp"

TEST_CASE("Test QueryProcessor") {
	PKB pkb;
	std::shared_ptr<SPFacade> spFacade = pkb.getSPFacade();
	QPSFacade qpsFacade = pkb.getQPSFacade();
	QueryProcessor qps = QueryProcessor(qpsFacade);
	spFacade->addUses(1, "a");
	spFacade->addUses(1, "c");
	spFacade->addUses(2, "e");
	//spFacade->addStatement(1, StatementType::StatementAssign);
	spFacade->addStatement(2, StatementType::StatementAssign);
	spFacade->addVariable("a");
	spFacade->addVariable("ab");
	spFacade->addVariable("abc");

	SECTION("Invalid Query") {
		std::vector<std::string> expected = { "g" };
		//REQUIRE(qps.process("invalid") == expected);
	}
	SECTION("Empty Result") {
		std::vector<std::string> expected = { "dummyVal" };
		REQUIRE(qps.process("stmt s; variable v; Select s such that Uses(s, v)") == expected);
		//REQUIRE(1 == 1);
	}
	SECTION("No Clause") {
		std::vector<std::string> expected = { "a", "c", "e"};
		//REQUIRE(qps.process("stmt s; Select s such that Uses (s, \"a\")") == expected);
	}
	SECTION("Different Select") {
		std::vector<std::string> expected = { "patternTest" };
		//REQUIRE(qps.process("variable v; stmt s; Select s such that Uses(1, v)") == expected);
		REQUIRE(qps.process("assign a; variable v; Select a pattern a (\"varX\", _\"123\"_)") == expected);
	}
}
//
// Created by Vanessa Khor on 9/2/23.
//
#include "qps/queryProcessor.h"
#include "qps/QueryLexer.h"
#include "qps/queryParser.h"
#include <string>
#include <sstream>
#include "catch.hpp"

using namespace std;

// valid queries
TEST_CASE("Parse simple select variable query") {
    std::string input = "variable v; Select v";
    shared_ptr<istringstream> iss = make_shared<istringstream>(input);
    QueryLexer l1 = QueryLexer(iss);
    QueryParser p1 = QueryParser(l1);
    Query testQuery = p1.parse();

    // comparison
    REQUIRE(testQuery.getTarget() == "v");
    REQUIRE(testQuery.getClauseList().size() == 0);
    std::vector<std::string> synonyms;
    synonyms.push_back("v");
    for(Declaration d : testQuery.getDeclarationList()) {
        REQUIRE(d.getSynonymList() == synonyms);
        REQUIRE(d.getType() == "variable");
    }
}

TEST_CASE("Parse simple select stmt query") {
    std::string input = "stmt ss; Select ss";
    shared_ptr<istringstream> iss = make_shared<istringstream>(input);
    QueryLexer l1 = QueryLexer(iss);
    QueryParser p1 = QueryParser(l1);
    Query testQuery = p1.parse();

    // comparison
    REQUIRE(testQuery.getTarget() == "ss");
    REQUIRE(testQuery.getClauseList().size() == 0);
    std::vector<std::string> synonyms;
    synonyms.push_back("ss");
    for(Declaration d : testQuery.getDeclarationList()) {
        REQUIRE(d.getSynonymList() == synonyms);
        REQUIRE(d.getType() == "stmt");
    }
}

TEST_CASE("Parse simple uses query") {
    string input = "variable v; Select v such that Uses(1, v)";
    shared_ptr<istringstream> iss = make_shared<istringstream>(input);
    QueryLexer l1 = QueryLexer(iss);
    QueryParser p1 = QueryParser(l1);
    Query testQuery = p1.parse();

    // comparison
    REQUIRE(testQuery.getTarget() == "v");

    for(std::shared_ptr<Clause> c : testQuery.getClauseList()) {
        REQUIRE(c->getLHS() == "1");
        REQUIRE(c->getRHS() == "v");
    }
    std::vector<std::string> synonyms;
    synonyms.push_back("v");
    for(Declaration d : testQuery.getDeclarationList()) {
        REQUIRE(d.getSynonymList() == synonyms);
        REQUIRE(d.getType() == "variable");
    }
}

TEST_CASE("Parse simple uses query with IDENT and wildcard") {
    string input = "variable v; Select v such that Uses(2, \"x\")";
    shared_ptr<istringstream> iss = make_shared<istringstream>(input);
    QueryLexer l1 = QueryLexer(iss);
    QueryParser p1 = QueryParser(l1);
    Query testQuery = p1.parse();

    // comparison
    REQUIRE(testQuery.getTarget() == "v");

    for(std::shared_ptr<Clause> c : testQuery.getClauseList()) {
        REQUIRE(c->getLHS() == "2");
        REQUIRE(c->getRHS() == "\"x\"");
    }
    std::vector<std::string> synonyms;
    synonyms.push_back("v");
    for(Declaration d : testQuery.getDeclarationList()) {
        REQUIRE(d.getSynonymList() == synonyms);
        REQUIRE(d.getType() == "variable");
    }
}

TEST_CASE("Parse simple modifies query with ifs") {
    string input = "if ifs; Select ifs such that Modifies(ifs, _)";
    shared_ptr<istringstream> iss = make_shared<istringstream>(input);
    QueryLexer l1 = QueryLexer(iss);
    QueryParser p1 = QueryParser(l1);
    Query testQuery = p1.parse();

    // comparison
    REQUIRE(testQuery.getTarget() == "ifs");

    for(std::shared_ptr<Clause> c : testQuery.getClauseList()) {
        REQUIRE(c->getLHS() == "ifs");
        REQUIRE(c->getRHS() == "_");
    }
    std::vector<std::string> synonyms;
    synonyms.push_back("ifs");
    for(Declaration d : testQuery.getDeclarationList()) {
        REQUIRE(d.getSynonymList() == synonyms);
        REQUIRE(d.getType() == "if");
    }
}

TEST_CASE("Parse simple modifies query") {
    string input = "stmt s; variable v; Select s such that Modifies(s, v)";
    shared_ptr<istringstream> iss = make_shared<istringstream>(input);
    QueryLexer l1 = QueryLexer(iss);
    QueryParser p1 = QueryParser(l1);
    Query testQuery = p1.parse();

    // comparison
    REQUIRE(testQuery.getTarget() == "s");

    for(std::shared_ptr<Clause> c : testQuery.getClauseList()) {
        REQUIRE(c->getLHS() == "s");
        REQUIRE(c->getRHS() == "v");
    }

    std::vector<std::string> synonyms1;
    synonyms1.push_back("s");

    std::vector<std::string> synonyms2;
    synonyms2.push_back("v");

    std::vector<Declaration> dec = testQuery.getDeclarationList();

    for(int i = 0; i < dec.size(); i++) {
        if(i == 0) {
            REQUIRE(dec[i].getSynonymList() == synonyms1);
            REQUIRE(dec[i].getType() == "stmt");
        } else {
            REQUIRE(dec[i].getSynonymList() == synonyms2);
            REQUIRE(dec[i].getType() == "variable");
        }
    }
}

TEST_CASE("Parse Follows* query") {
    string input = "stmt s1, s2; Select s1 such that Follows*(s1, s2)";
    shared_ptr<istringstream> iss = make_shared<istringstream>(input);
    QueryLexer l1 = QueryLexer(iss);
    QueryParser p1 = QueryParser(l1);
    Query testQuery = p1.parse();

    // comparison
    REQUIRE(testQuery.getTarget() == "s1");

    for(std::shared_ptr<Clause> c : testQuery.getClauseList()) {
        REQUIRE(c->getLHS() == "s1");
        REQUIRE(c->getRHS() == "s2");
    }
    std::vector<std::string> synonyms;
    synonyms.push_back("s1");
    synonyms.push_back("s2");

    for(Declaration d : testQuery.getDeclarationList()) {
        REQUIRE(d.getSynonymList() == synonyms);
        REQUIRE(d.getType() == "stmt");
    }
}

TEST_CASE("Parse Follows query") {
    string input = "while w1, w2; Select w1 such that Follows(w1, w2)";
    shared_ptr<istringstream> iss = make_shared<istringstream>(input);
    QueryLexer l1 = QueryLexer(iss);
    QueryParser p1 = QueryParser(l1);
    Query testQuery = p1.parse();

    // comparison
    REQUIRE(testQuery.getTarget() == "w1");

    for(std::shared_ptr<Clause> c : testQuery.getClauseList()) {
        REQUIRE(c->getLHS() == "w1");
        REQUIRE(c->getRHS() == "w2");
    }
    std::vector<std::string> synonyms;
    synonyms.push_back("w1");
    synonyms.push_back("w2");

    for(Declaration d : testQuery.getDeclarationList()) {
        REQUIRE(d.getSynonymList() == synonyms);
        REQUIRE(d.getType() == "while");
    }
}

TEST_CASE("Parse Parent query") {
    string input = "stmt s1, s2; Select s1 such that Parent(s1, s2)";
    shared_ptr<istringstream> iss = make_shared<istringstream>(input);
    QueryLexer l1 = QueryLexer(iss);
    QueryParser p1 = QueryParser(l1);
    Query testQuery = p1.parse();

    // comparison
    REQUIRE(testQuery.getTarget() == "s1");

    for(std::shared_ptr<Clause> c : testQuery.getClauseList()) {
        REQUIRE(c->getLHS() == "s1");
        REQUIRE(c->getRHS() == "s2");
    }
    std::vector<std::string> synonyms;
    synonyms.push_back("s1");
    synonyms.push_back("s2");

    for(Declaration d : testQuery.getDeclarationList()) {
        REQUIRE(d.getSynonymList() == synonyms);
        REQUIRE(d.getType() == "stmt");
    }
}

TEST_CASE("Parse Parent* query with wildcard") {
    string input = "stmt s1; Select s1 such that Parent*(_, 1)";
    shared_ptr<istringstream> iss = make_shared<istringstream>(input);
    QueryLexer l1 = QueryLexer(iss);
    QueryParser p1 = QueryParser(l1);
    Query testQuery = p1.parse();

    // comparison
    REQUIRE(testQuery.getTarget() == "s1");

    for(std::shared_ptr<Clause> c : testQuery.getClauseList()) {
        REQUIRE(c->getLHS() == "_");
        REQUIRE(c->getRHS() == "1");
    }
    std::vector<std::string> synonyms;
    synonyms.push_back("s1");

    for(Declaration d : testQuery.getDeclarationList()) {
        REQUIRE(d.getSynonymList() == synonyms);
        REQUIRE(d.getType() == "stmt");
    }
}

TEST_CASE("Parse simple pattern query") {
    string input = "assign a; Select a pattern a(_, _\"x\"_)";
    shared_ptr<istringstream> iss = make_shared<istringstream>(input);
    QueryLexer l1 = QueryLexer(iss);
    QueryParser p1 = QueryParser(l1);
    Query testQuery = p1.parse();

    // comparison
    REQUIRE(testQuery.getTarget() == "a");

    for(std::shared_ptr<Clause> c : testQuery.getClauseList()) {
        REQUIRE(c->getLHS() == "_");
        REQUIRE(c->getRHS() == "_\"x\"_");
    }
    std::vector<std::string> synonyms;
    synonyms.push_back("a");

    for(Declaration d : testQuery.getDeclarationList()) {
        REQUIRE(d.getSynonymList() == synonyms);
        REQUIRE(d.getType() == "assign");
    }
}

TEST_CASE("Parse pattern query with IDENT") {
    string input = "assign a; Select a pattern a(\"x\", _\"3\"_)";
    shared_ptr<istringstream> iss = make_shared<istringstream>(input);
    QueryLexer l1 = QueryLexer(iss);
    QueryParser p1 = QueryParser(l1);
    Query testQuery = p1.parse();

    // comparison
    REQUIRE(testQuery.getTarget() == "a");

    for(std::shared_ptr<Clause> c : testQuery.getClauseList()) {
        REQUIRE(c->getLHS() == "\"x\"");
        REQUIRE(c->getRHS() == "_\"3\"_");
    }
    std::vector<std::string> synonyms;
    synonyms.push_back("a");

    for(Declaration d : testQuery.getDeclarationList()) {
        REQUIRE(d.getSynonymList() == synonyms);
        REQUIRE(d.getType() == "assign");
    }
}

TEST_CASE("Parse double wildcard pattern query") {
    string input = "assign a; Select a pattern a(_, _)";
    shared_ptr<istringstream> iss = make_shared<istringstream>(input);
    QueryLexer l1 = QueryLexer(iss);
    QueryParser p1 = QueryParser(l1);
    Query testQuery = p1.parse();

    // comparison
    REQUIRE(testQuery.getTarget() == "a");

    for(std::shared_ptr<Clause> c : testQuery.getClauseList()) {
        REQUIRE(c->getLHS() == "_");
        REQUIRE(c->getRHS() == "_");
    }
    std::vector<std::string> synonyms;
    synonyms.push_back("a");

    for(Declaration d : testQuery.getDeclarationList()) {
        REQUIRE(d.getSynonymList() == synonyms);
        REQUIRE(d.getType() == "assign");
    }
}

TEST_CASE("Parse PQL query with multiple clauses") {
    string input = "assign a; while w; variable v; Select a pattern a (v, _) such that Parent* (w, a)";
    shared_ptr<istringstream> iss = make_shared<istringstream>(input);
    QueryLexer l1 = QueryLexer(iss);
    QueryParser p1 = QueryParser(l1);
    Query testQuery = p1.parse();

    REQUIRE(testQuery.isSynonymPresent("a") == true);
    REQUIRE(testQuery.isSynonymPresent("w") == true);
    REQUIRE(testQuery.isSynonymPresent("x") == false);

    REQUIRE(testQuery.getTarget() == "a");

    std::vector<std::shared_ptr<Clause>> clauses = testQuery.getClauseList();
    for(int i = 0; i < clauses.size(); i++) {
        if(i == 0) {
            REQUIRE(clauses[i]->getLHS() == "v");
            REQUIRE(clauses[i]->getRHS() == "_");
        } else {
            REQUIRE(clauses[i]->getLHS() == "w");
            REQUIRE(clauses[i]->getRHS() == "a");
        }
    }

    std::vector<std::string> synonyms1;
    synonyms1.push_back("a");

    std::vector<std::string> synonyms2;
    synonyms2.push_back("w");

    std::vector<std::string> synonyms3;
    synonyms3.push_back("v");

    std::vector<Declaration> dec = testQuery.getDeclarationList();

    for(int i = 0; i < dec.size(); i++) {
        if(i == 0) {
            REQUIRE(dec[i].getSynonymList() == synonyms1);
            REQUIRE(dec[i].getType() == "assign");
        } else if (i == 1) {
            REQUIRE(dec[i].getSynonymList() == synonyms2);
            REQUIRE(dec[i].getType() == "while");
        } else {
            REQUIRE(dec[i].getSynonymList() == synonyms3);
            REQUIRE(dec[i].getType() == "variable");
        }
    }
}

TEST_CASE("Parse PQL query with multiple clauses 2") {

    string input = "assign a; while w; Select a such that Parent(w, a) pattern a (_,_)";
    shared_ptr<istringstream> iss = make_shared<istringstream>(input);
    QueryLexer l1 = QueryLexer(iss);
    QueryParser p1 = QueryParser(l1);
    Query testQuery = p1.parse();

    REQUIRE(testQuery.getTarget() == "a");

    std::vector<std::shared_ptr<Clause>> clauses = testQuery.getClauseList();
    for(int i = 0; i < clauses.size(); i++) {
        if(i == 0) {
            REQUIRE(clauses[i]->getLHS() == "w");
            REQUIRE(clauses[i]->getRHS() == "a");
        } else {
            REQUIRE(clauses[i]->getLHS() == "_");
            REQUIRE(clauses[i]->getRHS() == "_");
        }
    }

    std::vector<std::string> synonyms1;
    synonyms1.push_back("a");

    std::vector<std::string> synonyms2;
    synonyms2.push_back("w");

    std::vector<Declaration> dec = testQuery.getDeclarationList();

    for(int i = 0; i < dec.size(); i++) {
        if(i == 0) {
            REQUIRE(dec[i].getSynonymList() == synonyms1);
            REQUIRE(dec[i].getType() == "assign");
        } else {
            REQUIRE(dec[i].getSynonymList() == synonyms2);
            REQUIRE(dec[i].getType() == "while");
        }
    }
}

TEST_CASE("Parse PQL query with IDENT") {

    string input = "assign a; while w; Select a such that Parent(w, a) pattern a (_,_\"x\"_)";
    shared_ptr<istringstream> iss = make_shared<istringstream>(input);
    QueryLexer l1 = QueryLexer(iss);
    QueryParser p1 = QueryParser(l1);
    Query testQuery = p1.parse();

    REQUIRE(testQuery.getTarget() == "a");

    std::vector<std::shared_ptr<Clause>> clauses = testQuery.getClauseList();
    for(int i = 0; i < clauses.size(); i++) {
        if(i == 0) {
            REQUIRE(clauses[i]->getLHS() == "w");
            REQUIRE(clauses[i]->getRHS() == "a");
        } else {
            REQUIRE(clauses[i]->getLHS() == "_");
            REQUIRE(clauses[i]->getRHS() == "_\"x\"_");
        }
    }

    std::vector<std::string> synonyms1;
    synonyms1.push_back("a");

    std::vector<std::string> synonyms2;
    synonyms2.push_back("w");

    std::vector<Declaration> dec = testQuery.getDeclarationList();

    for(int i = 0; i < dec.size(); i++) {
        if(i == 0) {
            REQUIRE(dec[i].getSynonymList() == synonyms1);
            REQUIRE(dec[i].getType() == "assign");
        } else {
            REQUIRE(dec[i].getSynonymList() == synonyms2);
            REQUIRE(dec[i].getType() == "while");
        }
    }
}

TEST_CASE("Parse parent query") {

    string input = "procedure p; read rd; Select p such that Parent(rd, _)";
    shared_ptr<istringstream> iss = make_shared<istringstream>(input);
    QueryLexer l1 = QueryLexer(iss);
    QueryParser p1 = QueryParser(l1);
    Query testQuery = p1.parse();

    REQUIRE(testQuery.getTarget() == "p");

    std::vector<std::shared_ptr<Clause>> clauses = testQuery.getClauseList();
    for(int i = 0; i < clauses.size(); i++) {
            REQUIRE(clauses[i]->getLHS() == "rd");
            REQUIRE(clauses[i]->getRHS() == "_");
    }

    std::vector<std::string> synonyms1;
    synonyms1.push_back("p");

    std::vector<std::string> synonyms2;
    synonyms2.push_back("rd");

    std::vector<Declaration> dec = testQuery.getDeclarationList();

    for(int i = 0; i < dec.size(); i++) {
        if(i == 0) {
            REQUIRE(dec[i].getSynonymList() == synonyms1);
            REQUIRE(dec[i].getType() == "procedure");
        } else {
            REQUIRE(dec[i].getSynonymList() == synonyms2);
            REQUIRE(dec[i].getType() == "read");
        }
    }
}
#include "ast/TNode.h"
#include "sp/design-extractor/BasicExtractionVisitor.h"
#include "pkb/SPFacade.h"
#include "pkb/QPSFacade.h"
#include "pkb/PKB.h"
#include "sp/Lexer.h"
#include "sp/Parser.h"
#include <memory>
#include <fstream>

#include "catch.hpp"

using namespace std;

std::string testBevPath = "../../../../Tests09/integration_test/sp/parser/design-extraction/";

TEST_CASE("test on full AST 1: uses and modifies") {
    std::shared_ptr<std::ifstream> src = make_shared<std::ifstream>(testBevPath + "basic_visitor1.txt");
    std::shared_ptr<Lexer> l = make_shared<Lexer>(src);
    Parser p = Parser(l);
    AstRoot root = std::make_shared<RootNode>(nullptr);
    p.parse(root);

    // temp data structure for testing
    vector<int> lineNos;
    vector<string> vars;

    PKB pkb;
    std::shared_ptr<SPFacade> spf = pkb.getSPFacade();
    QPSFacade qpsf = pkb.getQPSFacade();

    auto visitor = std::make_shared<BasicExtractionVisitor>(spf);
    root->accept(visitor.get());

    // stmt modifies var
    vector<string> c;
    c =  pkb.getQPSFacade().getModifies(1);
    REQUIRE(c == vector<string> {"lo"});
    c =  pkb.getQPSFacade().getModifies(2);
    REQUIRE(c == vector<string> {"hi"});
    c =  pkb.getQPSFacade().getModifies(3);
    REQUIRE(c == vector<string> { "arrMid", "lo", "hi" });
    c =  pkb.getQPSFacade().getModifies(4);
    REQUIRE(c == vector<string> { "arrMid"});
    c =  pkb.getQPSFacade().getModifies(5);
    REQUIRE(c == vector<string> { "lo", "hi"});

    // var uses stmt
    vector<int> d;
    d =  pkb.getQPSFacade().getModifies("lo");
    REQUIRE(d == vector<int> { 1, 6, 5, 3 });
    d =  pkb.getQPSFacade().getModifies("hi");
    REQUIRE(d == vector<int> { 2, 7, 5, 3 });
    d =  pkb.getQPSFacade().getModifies("arrMid");
    REQUIRE(d == vector<int> { 4, 3 });
    d =  pkb.getQPSFacade().getModifies("x");
    REQUIRE(d == vector<int> { 9, 8 });

    // stmt uses var
    c = pkb.getQPSFacade().getUses(1);
    REQUIRE(c == vector<string> { });
    c = pkb.getQPSFacade().getUses(2);
    REQUIRE(c == vector<string> { "n" });
    c = pkb.getQPSFacade().getUses(3);
    REQUIRE(c == vector<string> { "lo", "hi", "target", "arrMid", "mid" });
    c = pkb.getQPSFacade().getUses(4);
    REQUIRE(c == vector<string> { "lo", "hi" });
    c = pkb.getQPSFacade().getUses(5);
    REQUIRE(c == vector<string> { "target", "arrMid", "mid" });
    c = pkb.getQPSFacade().getUses(8);
    REQUIRE(c == vector<string> { "lo", "y"});
}

TEST_CASE("test on full AST 2") {
    std::shared_ptr<std::ifstream> src = make_shared<std::ifstream>(testBevPath + "basic_visitor2.txt");
    std::shared_ptr<Lexer> l = make_shared<Lexer>(src);
    Parser p = Parser(l);
    AstRoot root = std::make_shared<RootNode>(nullptr);
    p.parse(root);

    // temp data structure for testing
    vector<int> lineNos;
    vector<string> vars;

    PKB pkb;
    std::shared_ptr<SPFacade> spf = pkb.getSPFacade();
    QPSFacade qpsf = pkb.getQPSFacade();
    REQUIRE_NOTHROW(BasicExtractionVisitor(spf));

    auto visitor = std::make_shared<BasicExtractionVisitor>(spf);
    root->accept(visitor.get());

    auto a =  pkb.getQPSFacade().getStatements();
    REQUIRE(a == vector<int>  { 10, 9, 8, 7, 6, 5, 4, 3, 2, 1 });
    auto b =  pkb.getQPSFacade().getConstants();
    REQUIRE(b == vector<string> { "12", "11", "2", "9", "6", "7", "1", "5", "10", "8", "4", "3" });

    // modifies
    auto c =  pkb.getQPSFacade().getModifies();
    for (auto p: c) {
        lineNos.push_back(p.first);
        vars.push_back(p.second);
    }
    CHECK(lineNos == vector<int> { 10, 8, 8, 9, 6, 7, 5, 3, 3, 4, 2, 1 });
    CHECK(vars == vector<string> { "a", "happiness", "a", "happiness", "h", "h", "e", "d", "e", "d", "b", "a" });
    lineNos.clear();
    vars.clear();

    // uses
    auto d =  pkb.getQPSFacade().getUses();
    for (auto p: d) {
        lineNos.push_back(p.first);
        vars.push_back(p.second);
    }
    CHECK(lineNos == vector<int> { 10, 10, 9, 9, 9, 9, 9, 6, 6, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 3 });
    CHECK(vars == vector<string> { "b", "c", "pEmo", "engage", "rel", "achieve", "meaning", "f", "g", "h", "i",
                                   "j", "k", "l", "m", "pEmo", "engage", "rel", "achieve", "meaning", "b", "c", "c" });
}

TEST_CASE("successful BEV initialisation") {
    PKB pkb;
    std::shared_ptr<SPFacade> spf = pkb.getSPFacade();
    QPSFacade qpsf = pkb.getQPSFacade();
    REQUIRE_NOTHROW(BasicExtractionVisitor(spf));

    auto visitor = std::make_shared<BasicExtractionVisitor>(spf);

    SECTION("BEV visits empty AstRoot") {
        AstRoot r = std::make_shared<RootNode>(nullptr);
        REQUIRE_NOTHROW(r->accept(visitor.get()));
    }

    SECTION("BEV visits empty Procedure") {
        // Setup Procedure node
        Procedure p = std::make_shared<ProcedureNode>(nullptr, "procedure");
        REQUIRE_NOTHROW(p->accept(visitor.get()));
    }

    SECTION("BEV visits Variable node") {
        Variable v1 = std::make_shared<VariableNode>(nullptr, 100, "variable1");
        Variable v2 = std::make_shared<VariableNode>(nullptr, 3, "v");
        Variable v3 = std::make_shared<VariableNode>(nullptr, 1, "variable2");
        //Variable v4 = std::make_shared<VariableNode>(nullptr, -1, "v");
        //Variable v5 = std::make_shared<VariableNode>(nullptr, 0, "variable2");
        // TODO: Since stmt number cannot be < 0, probably should add in these as well

        REQUIRE_NOTHROW(v1->accept(visitor.get()));
        REQUIRE_NOTHROW(v2->accept(visitor.get()));
        REQUIRE_NOTHROW(v3->accept(visitor.get()));

        auto variables = qpsf.getVariables();
        REQUIRE(variables.size() == 3);
    }

    SECTION("BEV visits Constant node") {
        Constant c1 = std::make_shared<ConstantNode>(nullptr, 1, "100");
        Constant c2 = std::make_shared<ConstantNode>(nullptr, 3, "-5");
        Constant c3 = std::make_shared<ConstantNode>(nullptr, 1, "0");

        REQUIRE_NOTHROW(c1->accept(visitor.get()));
        REQUIRE_NOTHROW(c2->accept(visitor.get()));
        REQUIRE_NOTHROW(c3->accept(visitor.get()));

        auto constants = qpsf.getConstants();
        REQUIRE(constants.size() == 3);
    }

    SECTION("BEV visits Read node") {
        Read r = std::make_shared<ReadNode>(nullptr, 4);
        Variable v = std::make_shared<VariableNode>(r, 4, "x");
        r->setVar(v);

        REQUIRE_NOTHROW(r->accept(visitor.get()));

        auto statements = qpsf.getStatements();
        auto modifies = qpsf.getModifies(4);
        REQUIRE(statements.size() == 1);
        REQUIRE(statements[0] == 4);
        REQUIRE(modifies == std::vector<std::string>{ "x"});
    }

    SECTION("BEV visits Print node") {
        Print p = std::make_shared<PrintNode>(nullptr, 4);
        Variable v = std::make_shared<VariableNode>(p, 4, "x");
        p->setVar(v);

        REQUIRE_NOTHROW(p->accept(visitor.get()));

        auto statements = qpsf.getStatements();
        auto uses = qpsf.getUses(4);
        REQUIRE(statements.size() == 1);
        REQUIRE(statements[0] == 4);
        REQUIRE(uses.size() == 1);
        REQUIRE(uses[0] == "x");
    }

    SECTION("BEV visits Assign node") {
        SECTION("ASSIGN (var = constant)") {
            Assign a1 = std::make_shared<AssignNode>(nullptr, 1);
            Variable lhs = std::make_shared<VariableNode>(a1, 1, "x");
            Constant rhs = std::make_shared<ConstantNode>(a1, 1, "100");
            a1->setAssign(lhs, rhs);

            // Adds Uses, Modifies relationships, statements, variables and constants
            a1->accept(visitor.get());

            // Check for variables
            auto variables = qpsf.getVariables();
            REQUIRE(variables.size() == 1);
            CHECK(variables[0] == "x");

            // Check for statements
            auto statements = qpsf.getStatements();
            REQUIRE(statements.size() == 1);
            CHECK(statements[0] == 1);

            // Check for constants
            auto constants = qpsf.getConstants();
            REQUIRE(constants.size() == 1);
            CHECK(constants[0] == "100");

            // Check for Uses/Modifies
            auto uses = qpsf.getUses(1);
            auto modifies = qpsf.getModifies(1);
            REQUIRE(uses.size() == 0);
            CHECK(modifies == std::vector<std::string>{ "x" });
        }

        SECTION("ASSIGN (var = var)") {
            Assign a1 = std::make_shared<AssignNode>(nullptr, 1);
            Variable lhs = std::make_shared<VariableNode>(a1, 1, "x");
            Variable rhs = std::make_shared<VariableNode>(a1, 1, "y");
            a1->setAssign(lhs, rhs);

            // Adds Uses, Modifies relationships, statements, variables and constants
            a1->accept(visitor.get());

            // Check for variables
            auto variables = qpsf.getVariables();
            REQUIRE(variables.size() == 2);
            CHECK(variables[0] == "y");
            CHECK(variables[1] == "x");

            // Check for statements
            auto statements = qpsf.getStatements();
            REQUIRE(statements.size() == 1);
            CHECK(statements[0] == 1);

            // Check for constants
            auto constants = qpsf.getConstants();
            REQUIRE(constants.size() == 0);

            // Check for Uses/Modifies
            auto uses = qpsf.getUses(1);
            auto modifies = qpsf.getModifies(1);
            REQUIRE(uses.size() == 1);
            CHECK(uses[0] == "y");
            CHECK(modifies == std::vector<std::string>{ "x"});
        }
    }
}

TEST_CASE("1 assign statement nested in 1 procedure") {
    // Setup BEV
    PKB pkb;
    std::shared_ptr<SPFacade> spf = pkb.getSPFacade();
    QPSFacade qpsf = pkb.getQPSFacade();
    auto visitor = std::make_shared<BasicExtractionVisitor>(spf);

    //// Setup nodes
    // Setup root node
    std::vector<Procedure> procs;
    AstRoot r = std::make_shared<RootNode>(nullptr);

    // Setup Procedure node
    Procedure p = std::make_shared<ProcedureNode>(r, "procedure");
    std::vector<Stmt> stmtList;
    procs.push_back(p);
    r->setProcedures(procs);

    SECTION("ASSIGN (var = constant)") {
        Assign a1 = std::make_shared<AssignNode>(p, 1);
        Variable lhs = std::make_shared<VariableNode>(a1, 1, "x");
        Constant rhs = std::make_shared<ConstantNode>(a1, 1, "100");
        a1->setAssign(lhs, rhs);
        stmtList.push_back(a1);
        p->setStmts(stmtList);

        // Adds Uses, Modifies relationships, statements, variables and constants
        r->accept(visitor.get());

        // Check for variables
        auto variables = qpsf.getVariables();
        REQUIRE(variables.size() == 1);
        CHECK(variables[0] == "x");

        // Check for statements
        auto statements = qpsf.getStatements();
        REQUIRE(statements.size() == 1);
        CHECK(statements[0] == 1);

        // Check for constants
        auto constants = qpsf.getConstants();
        REQUIRE(constants.size() == 1);
        CHECK(constants[0] == "100");

        // Check for Uses/Modifies
        auto uses = qpsf.getUses(1);
        auto modifies = qpsf.getModifies(1);
        REQUIRE(uses.size() == 0);
        CHECK(modifies == std::vector<std::string>{ "x" });
    }

    SECTION("ASSIGN (var = var)") {
        Assign a1 = std::make_shared<AssignNode>(p, 1);
        Variable lhs = std::make_shared<VariableNode>(a1, 1, "x");
        Variable rhs = std::make_shared<VariableNode>(a1, 1, "y");
        a1->setAssign(lhs, rhs);
        stmtList.push_back(a1);
        p->setStmts(stmtList);

        // Adds Uses, Modifies relationships, statements, variables and constants
        r->accept(visitor.get());

        // Check for variables
        auto variables = qpsf.getVariables();
        REQUIRE(variables.size() == 2);
        CHECK(variables[0] == "y");
        CHECK(variables[1] == "x");

        // Check for statements
        auto statements = qpsf.getStatements();
        REQUIRE(statements.size() == 1);
        CHECK(statements[0] == 1);

        // Check for constants
        auto constants = qpsf.getConstants();
        REQUIRE(constants.size() == 0);

        // Check for Uses/Modifies
        auto uses = qpsf.getUses(1);
        auto modifies = qpsf.getModifies(1);
        REQUIRE(uses.size() == 1);
        CHECK(uses[0] == "y");
        CHECK(modifies == std::vector<std::string>{"x"});
    }
}
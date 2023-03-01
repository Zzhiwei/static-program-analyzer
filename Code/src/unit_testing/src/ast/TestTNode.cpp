#include "ast/RootNode.h"
#include "ast/ExpressionNode.h"
#include "ast/StmtNode.h"
#include "ast/ProcedureNode.h"

#include "catch.hpp"
using namespace std;

TEST_CASE("can create Procedure Node") {
    ProcedureNode p = ProcedureNode(nullptr, "test");
    REQUIRE(p.toString() == string("======================== PROCEDURE test ========================\n")
        + "#### end procedure ####\n");
}

TEST_CASE("can create Root Node with procedures") {
    RootNode r = RootNode(nullptr);
    REQUIRE(r.toString() == "[empty_root]");
    std::vector<Procedure> procs;
    Procedure p = make_shared<ProcedureNode>(nullptr, "test1");
    Procedure p2 = make_shared<ProcedureNode>(nullptr, "test2");
    procs.push_back(p);
    procs.push_back(p2);
    r.setProcedures(procs);
    std::string expected = string("======================== PROCEDURE test1 ========================\n")
            + "#### end procedure ####\n"
            + "======================== PROCEDURE test2 ========================\n"
            + "#### end procedure ####\n";
    REQUIRE(r.toString() == expected);
}

TEST_CASE("can create Constant and Variable Node") {
    Constant c = make_shared<ConstantNode>(nullptr, 3, "33");
    REQUIRE(c->toString() == "33");
    Variable v = make_shared<VariableNode>(nullptr, 4, "var1");
    REQUIRE(v->toString() == "var1");
}

TEST_CASE("can create Statement Nodes") {
    SECTION("READ") {

        Variable v1 = make_shared<VariableNode>(nullptr, 5, "var1");
        Read r = make_shared<ReadNode>(nullptr, 5);
        r->setVar(v1);
        REQUIRE(r->toString() == "read var1\n");
    }

    SECTION("PRINT") {
        Variable v1 = make_shared<VariableNode>(nullptr, 5, "var1");
        Print p = make_shared<PrintNode>(nullptr, 5);
        p->setVar(v1);
        REQUIRE(p->toString() == "print var1\n");
    }

    SECTION("ASSIGN (var = var)") {
        Variable lhs = make_shared<VariableNode>(nullptr, 5, "x22");
        Variable rhsVar = make_shared<VariableNode>(nullptr, 5, "y33");
        Assign a1 = make_shared<AssignNode>(nullptr, 5);
        a1->setAssign(lhs, rhsVar);
        REQUIRE(a1->toString() == "x22:=y33\n");
    }

    SECTION("ASSIGN (var = constant)") {
        Variable lhs = make_shared<VariableNode>(nullptr, 5, "x22");
        Constant c = make_shared<ConstantNode>(nullptr, 5,"123");
        Assign a1 = make_shared<AssignNode>(nullptr, 5);
        a1->setAssign(lhs, c);
        REQUIRE(a1->toString() == "x22:=123\n");
    }
}

TEST_CASE("if Node") {
    Variable cv1 = make_shared<VariableNode>(nullptr, 5, "cv1");
    Variable cv2 = make_shared<VariableNode>(nullptr, 5, "cv2");
    CondExpr condExpr = make_shared<CondExprNode>(nullptr, 2, CondExprType::REL_EXPR);
    RelExpr expr = make_shared<RelExprNode>(nullptr, 2);
    expr->setRelExpr(cv1, cv2, "==");
    condExpr->setRelExpr(expr);

    Variable v1 = make_shared<VariableNode>(nullptr, 5, "var1");
    Read r1 = make_shared<ReadNode>(nullptr, 5);
    r1->setVar(v1);

    Variable v2 = make_shared<VariableNode>(nullptr, 5, "var2");
    Read r2 = make_shared<ReadNode>(nullptr, 5);
    r2->setVar(v2);

    std::vector<Stmt> lst;
    lst.push_back(r1);
    lst.push_back(r2);


    Variable v3 = make_shared<VariableNode>(nullptr, 5, "var3");
    Print p = make_shared<PrintNode>(nullptr, 5);
    p->setVar(v3);

    Variable v4 = make_shared<VariableNode>(nullptr, 5, "var4");
    Print p2 = make_shared<PrintNode>(nullptr, 5);
    p2->setVar(v4);

    std::vector<Stmt> lst2;
    lst2.push_back(p);
    lst2.push_back(p2);

    If ifNode = make_shared<IfNode>(nullptr, 5);
    ifNode->setIf(condExpr, lst, lst2);

    string expected = string("if: cv1 == cv2\n")
                      + "read var1\n"
                      + "read var2\n"
                      + "else:\n"
                      + "print var3\n"
                      + "print var4\n"
                      + + "#### end if ####\n";

    REQUIRE(ifNode->toString() == expected);
}



TEST_CASE("equality check") {
    SECTION("CONSTANT") {
        Constant c = make_shared<ConstantNode>(nullptr, 2, "3");
        Constant c2 = make_shared<ConstantNode>(nullptr, 2, "3");
        REQUIRE(*c == *c2);
    }

    SECTION("VARIABLE") {
        Variable v1 = make_shared<VariableNode>(nullptr, 2, "v1");
        Variable v2 = make_shared<VariableNode>(nullptr, 2, "v1");
        REQUIRE(*v1 == *v2);
    }

    SECTION("ASSIGN") {
        Variable v1 = make_shared<VariableNode>(nullptr, 2, "v1");
        Variable v2 = make_shared<VariableNode>(nullptr, 2, "v2");
        Assign a1 = make_shared<AssignNode>(nullptr, 5);
        a1->setAssign(v1, v2);
        Assign a2 = make_shared<AssignNode>(nullptr, 5);
        a2->setAssign(v1, v2);
        REQUIRE(*a1 == *a2);
    }

    SECTION("READ") {
        Variable v1 = make_shared<VariableNode>(nullptr, 2, "v1");
        Variable v2 = make_shared<VariableNode>(nullptr, 2, "v2");
        Read r1 = make_shared<ReadNode>(nullptr, 4);
        Read r2 = make_shared<ReadNode>(nullptr, 4);
        r1->setVar(v1);
        r2->setVar(v1);
        REQUIRE(*r1 == *r2);
        r2->setVar(v2);
        REQUIRE_FALSE(*r1 == *r2);
    }

    SECTION("PRINT") {
        Variable v1 = make_shared<VariableNode>(nullptr, 2, "v1");
        Print p1 = make_shared<PrintNode>(nullptr, 4);
        Print p2 = make_shared<PrintNode>(nullptr, 4);
        REQUIRE(*p1 == *p2);
    }

    SECTION("PROCEDURE") {
        Procedure proc = make_shared<ProcedureNode>(nullptr, "func");
        Print p = make_shared<PrintNode>(proc, 4);
        Read r = make_shared<ReadNode>(proc, 6);
        Variable v1 = make_shared<VariableNode>(p, 2, "v1");
        p->setVar(v1);
        r->setVar(v1);
        std::vector<Stmt> lst;
        lst.push_back(p);
        lst.push_back(r);
        proc->setStmts(lst);

        Procedure proc2 = make_shared<ProcedureNode>(nullptr, "func");
        proc2->setStmts(lst);
        REQUIRE(*proc == *proc2);
    }
}

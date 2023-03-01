#include "catch.hpp"
#include "sp/ExprParser.h"
#include <sstream>
#include <iostream>

using namespace std;

// friend class to test private methods
class TestExprParser {
public:
    static vector<string> getPostfix(ExprParser& ep) {
        return ep.postfix;
    }

    static void convertToPostfix(ExprParser& ep) {
        ep.infixToPostfix();
    }

    static void extractNextExpression(ExprParser& ep) {
        ep.extractExprTokens();
    }

    static string getCurrentToken(ExprParser& ep) {
        return ep.lexer->getCurrentToken();
    }
};


TEST_CASE("infix to postfix") {
    SECTION("1") {
        string input = string("(5 * 4 + p2 * 2) - var3;");
        shared_ptr<istringstream> iss = make_shared<istringstream>(input);
        shared_ptr<Lexer> lex = make_shared<Lexer>(iss);
        int stmtNo = 1;
        ExprParser p(lex, stmtNo);
        TestExprParser::extractNextExpression(p);
        std::vector<string> expected = {"5", "4", "*", "p2", "2", "*", "+", "var3", "-"};
        TestExprParser::convertToPostfix(p);
        REQUIRE(TestExprParser::getPostfix(p) == expected);
    }

    SECTION("2") {
        string input = string("5 * 4 + (3 * 2 - 1);");
        shared_ptr<istringstream> iss = make_shared<istringstream>(input);
        shared_ptr<Lexer> lex = make_shared<Lexer>(iss);
        int stmtNo = 1;
        ExprParser p(lex, stmtNo);
        TestExprParser::extractNextExpression(p);
        std::vector<string> expected = {"5", "4", "*", "3", "2", "*", "1", "-", "+"};
        TestExprParser::convertToPostfix(p);
        REQUIRE(TestExprParser::getPostfix(p) == expected);
    }

    SECTION("3") {
        string input = string("(2-kob+4)*(5+akldja*7)");
        shared_ptr<istringstream> iss = make_shared<istringstream>(input);
        shared_ptr<Lexer> lex = make_shared<Lexer>(iss);
        int stmtNo = 1;
        ExprParser p(lex, stmtNo);
        TestExprParser::extractNextExpression(p);
        std::vector<string> expected = {"2", "kob", "-", "4", "+", "5", "akldja", "7", "*", "+", "*"};
        TestExprParser::convertToPostfix(p);
        REQUIRE(TestExprParser::getPostfix(p) == expected);
    }

    SECTION("4") {
        string input = string("2-3+4-5*6");
        shared_ptr<istringstream> iss = make_shared<istringstream>(input);
        shared_ptr<Lexer> lex = make_shared<Lexer>(iss);
        int stmtNo = 1;
        ExprParser p(lex, stmtNo);
        TestExprParser::extractNextExpression(p);
        std::vector<string> expected = {"2", "3", "-", "4", "+", "5", "6",  "*",  "-"};
        TestExprParser::convertToPostfix(p);
        REQUIRE(TestExprParser::getPostfix(p) == expected);
    }

    SECTION("5") {
        string input = string("(x + y) / 2 / ((z + w) % 5)");
        shared_ptr<istringstream> iss = make_shared<istringstream>(input);
        shared_ptr<Lexer> lex = make_shared<Lexer>(iss);
        int stmtNo = 1;
        ExprParser p(lex, stmtNo);
        TestExprParser::extractNextExpression(p);
        std::vector<string> expected = {"x", "y", "+", "2", "/", "z", "w", "+", "5", "%", "/"};
        TestExprParser::convertToPostfix(p);
        REQUIRE(TestExprParser::getPostfix(p) == expected);
    }

    SECTION("5") {
        string input = string("l + (l - r) / 2");
        shared_ptr<istringstream> iss = make_shared<istringstream>(input);
        shared_ptr<Lexer> lex = make_shared<Lexer>(iss);
        int stmtNo = 1;
        ExprParser p(lex, stmtNo);
        TestExprParser::extractNextExpression(p);
        std::vector<string> expected = { "l", "l", "r", "-", "2", "/", "+" };
        TestExprParser::convertToPostfix(p);
        REQUIRE(TestExprParser::getPostfix(p) == expected);
    }



    SECTION("multiplication divide modulo has equal precedence") {
        string input = string("3 * 4 / 5 % 6");
        shared_ptr<istringstream> iss = make_shared<istringstream>(input);
        shared_ptr<Lexer> lex = make_shared<Lexer>(iss);
        int stmtNo = 1;
        ExprParser p(lex, stmtNo);
        TestExprParser::extractNextExpression(p);
        std::vector<string> expected = {"3", "4", "*", "5", "/", "6", "%"};
        TestExprParser::convertToPostfix(p);
        REQUIRE(TestExprParser::getPostfix(p) == expected);
    }
}

TEST_CASE("more test cases on longer ") {
    SECTION("1") {
        string input = string("a + b / 2 - c % 3 * d");
        shared_ptr<istringstream> iss = make_shared<istringstream>(input);
        shared_ptr<Lexer> lex = make_shared<Lexer>(iss);
        int stmtNo = 1;
        ExprParser p(lex, stmtNo);
        TestExprParser::extractNextExpression(p);
        std::vector<string> expected = { "a", "b", "2", "/", "+", "c", "3", "%", "d", "*", "-" };
        TestExprParser::convertToPostfix(p);
        REQUIRE(TestExprParser::getPostfix(p) == expected);
    }

    SECTION("2") {
        string input = string("2 * a / (b - c) % d + e");
        shared_ptr<istringstream> iss = make_shared<istringstream>(input);
        shared_ptr<Lexer> lex = make_shared<Lexer>(iss);
        int stmtNo = 1;
        ExprParser p(lex, stmtNo);
        TestExprParser::extractNextExpression(p);
        std::vector<string> expected = { { "2", "a", "*", "b", "c", "-", "/", "d", "%", "e", "+" } };
        TestExprParser::convertToPostfix(p);
        REQUIRE(TestExprParser::getPostfix(p) == expected);
    }

    SECTION("3") {
        string input = string("f % g + (h - i) / j * k");
        shared_ptr<istringstream> iss = make_shared<istringstream>(input);
        shared_ptr<Lexer> lex = make_shared<Lexer>(iss);
        int stmtNo = 1;
        ExprParser p(lex, stmtNo);
        TestExprParser::extractNextExpression(p);
        std::vector<string> expected = { { "f", "g", "%", "h", "i", "-", "j", "/", "k", "*", "+" } };
        TestExprParser::convertToPostfix(p);
        REQUIRE(TestExprParser::getPostfix(p) == expected);
    }
}

TEST_CASE("it should recognize end of expr when extracting tokens") {
    SECTION("closing bracket end") {
        string input = string("(3 + 4 - 5 + 6))");
        shared_ptr<istringstream> iss = make_shared<istringstream>(input);
        shared_ptr<Lexer> lex = make_shared<Lexer>(iss);
        int stmtNo = 1;
        ExprParser p(lex, stmtNo);
        TestExprParser::extractNextExpression(p);
        std::vector<string> expected = {"3", "4", "+", "5", "-", "6", "+"};
        TestExprParser::convertToPostfix(p);
        REQUIRE(TestExprParser::getPostfix(p) == expected);
        REQUIRE(TestExprParser::getCurrentToken(p) == ")");
    }

    SECTION("semicolon end") {
        string input = string("(3 + 4 - 5 + 6);");
        shared_ptr<istringstream> iss = make_shared<istringstream>(input);
        shared_ptr<Lexer> lex = make_shared<Lexer>(iss);
        int stmtNo = 1;
        ExprParser p(lex, stmtNo);
        TestExprParser::extractNextExpression(p);
        std::vector<string> expected = {"3", "4", "+", "5", "-", "6", "+"};
        TestExprParser::convertToPostfix(p);
        REQUIRE(TestExprParser::getPostfix(p) == expected);
        REQUIRE(TestExprParser::getCurrentToken(p) == ";");
    }
}


TEST_CASE("it should parse and return an AST") {
    string input = string("(3 + xyz - 5 + helloworld);");
    shared_ptr<istringstream> iss = make_shared<istringstream>(input);
    shared_ptr<Lexer> lex = make_shared<Lexer>(iss);
    int stmtNo = 1;
    ExprParser p(lex, stmtNo);
    Expression e = p.parseExpression(nullptr);
    auto root = static_cast<BinaryOpNode*>(e.get());
    REQUIRE( root->toString() == "3 + xyz - 5 + helloworld");
    auto lhs = root->getLhs();
    REQUIRE( lhs->toString() == "3 + xyz - 5");
    //leaf
    auto rhs = root->getRhs();
    REQUIRE(static_cast<VariableNode*>(rhs.get())->getVarName() == "helloworld");
    auto lhs2 = static_cast<BinaryOpNode*>(lhs.get())->getLhs();
    REQUIRE( lhs2->toString() == "3 + xyz");
    auto five = static_cast<BinaryOpNode*>(lhs.get())->getRhs();
    //leaf
    REQUIRE(static_cast<ConstantNode*>(five.get())->getConstant() == "5");
}

TEST_CASE("it should parse and return an AST 2") {
    string input = string("lo + (hi - lo) / 2");
    shared_ptr<istringstream> iss = make_shared<istringstream>(input);
    shared_ptr<Lexer> lex = make_shared<Lexer>(iss);
    int stmtNo = 1;
    ExprParser p(lex, stmtNo);
    Expression e = p.parseExpression(nullptr);

    auto plus = static_cast<BinaryOpNode*>(e.get());
    auto lo1 = static_cast<VariableNode*>(plus->getLhs().get());
    REQUIRE(lo1->getVarName() == "lo");

    auto divide = static_cast<BinaryOpNode*>(plus->getRhs().get());
    auto four = static_cast<ConstantNode*>(divide->getRhs().get());
    REQUIRE(four->getConstant() == "2");

    auto minus = static_cast<BinaryOpNode*>(divide->getLhs().get());
    auto hi = static_cast<VariableNode*>(minus->getLhs().get());
    REQUIRE(hi->getVarName() == "hi");
    auto lo2 = static_cast<VariableNode*>(minus->getRhs().get());
    REQUIRE(lo2->getVarName() == "lo");
}
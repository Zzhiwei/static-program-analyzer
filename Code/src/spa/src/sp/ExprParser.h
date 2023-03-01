#pragma once

#include <vector>
#include <string>
#include <stack>
#include <unordered_map>
#include <unordered_set>

#include "ast/ExpressionNode.h"
#include "Lexer.h"
#include "SpException.h"

class ExprParser {
private:
    std::shared_ptr<Lexer> lexer;

    // a reference to parser's stmtNo
    int &stmtNo;

    int unmatchedBrackets = 0;

    std::vector<std::string> tokens;

    std::vector<TokenType> types;

    std::vector<std::string> postfix;

    std::vector<TokenType> postfixTypes;

    void reset();

    bool isEndOfExpr();

    void infixToPostfix();

    void extractExprTokens();

    friend class TestExprParser;


public:
    explicit ExprParser(std::shared_ptr<Lexer> lexer, int &stmtNo);

    Expression parseExpression(Node parent);
};

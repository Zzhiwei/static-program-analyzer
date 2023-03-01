#include "ExprParser.h"

std::unordered_map<std::string, int> precedence = { {"+", 1}, {"-", 1}, {"*", 2},
                                                    {"/", 2}, {"%", 2}, {"(", 0}};

std::unordered_set<std::string> operatorSet = {"+", "-", "*", "/", "%"};

ExprParser::ExprParser(std::shared_ptr<Lexer> lexer, int &stmtNo) : lexer(lexer), stmtNo(stmtNo) {}

bool ExprParser::isEndOfExpr() {
    if ((unmatchedBrackets == 0 && lexer->match(")")) || lexer->match(";") || lexer->matchRelOperator()) {
        return true;
    }
    return false;
}

// guarantees tokens to be valid expression token
void ExprParser::extractExprTokens() {
    // must check lexer is not at the end because isEndOfExpr assumes there is a valid end to expression
    while (lexer->getTokenType() != TokenType::END && !isEndOfExpr()) {
        if (lexer->matchIdentifier()) {
            types.push_back(lexer->getTokenType());
            tokens.push_back(lexer->eatIdentifier());
        } else if (lexer->matchConstant()) {
            types.push_back(lexer->getTokenType());
            tokens.push_back(lexer->eatConstant());
        } else if (lexer->matchExprChar()) {
            if (lexer->match("(")) {
                unmatchedBrackets += 1;
            } else if (lexer->match(")")) {
                unmatchedBrackets -= 1;
            }
            types.push_back(lexer->getTokenType());
            tokens.push_back(lexer->eatExprChar());
        } else {
            throw ParserException("illegal token found in expression", lexer->getLineNo(),
                                  lexer->getCurrentToken());
        }
    }

}

void ExprParser::infixToPostfix() {
    std::stack<std::string> operatorStack;
    // vector representing reverse polish notation (RPN)

    for (int i = 0; i < tokens.size(); i++) {
        std::string currToken = tokens.at(i);
        TokenType currType = types.at(i);
        if (currType  == TokenType::NUMBER || currType  == TokenType::IDENT) {
            postfix.push_back(currToken);
            postfixTypes.push_back(currType);
        } else if (currToken == "(") {
            operatorStack.push(currToken);
        } else if (currToken == ")") {
            while (!operatorStack.empty() && operatorStack.top() != "(") {
                postfix.push_back(operatorStack.top());
                postfixTypes.push_back(TokenType::OTHERS);
                operatorStack.pop();
            }
            // stack has become empty OR top is '(' (mutually exclusive)
            if (operatorStack.empty()) {
                throw ParserException("invalid expression", lexer->getLineNo());
            } else {
                operatorStack.pop();
            }
        } else {
            // token is one of the binary operators
            int currPrecedence = precedence.at(currToken);
            while (!operatorStack.empty() && precedence.at(operatorStack.top()) >= currPrecedence ) {
                postfix.push_back(operatorStack.top());
                postfixTypes.push_back(TokenType::OTHERS);
                operatorStack.pop();
            }
            operatorStack.push(currToken);
        }
    }

    while (!operatorStack.empty()) {
        if (operatorStack.top() == "(") {
            throw ParserException("unclosed opening bracket found", lexer->getLineNo());
        }
        postfix.push_back(operatorStack.top());
        postfixTypes.push_back(TokenType::OTHERS);
        operatorStack.pop();
    }

}

Expression ExprParser::parseExpression(Node parent) {
    extractExprTokens();
    infixToPostfix();

    std::stack<Expression> outStack;
    for (int i = 0; i < postfix.size(); i++) {
        std::string currToken = postfix.at(i);
        TokenType currType = postfixTypes.at(i);
        if (operatorSet.find(currToken) == operatorSet.end()) {
            // curr token is not an operator
            if (currType == TokenType::IDENT) {
                // initialize with parent in the parameter first, will be replaced if expression is not a simple ref
                outStack.push(std::make_shared<VariableNode>(parent, stmtNo, currToken));
            } else if (currType == TokenType::NUMBER) {
                outStack.push(std::make_shared<ConstantNode>(parent, stmtNo, currToken));
            }
        } else {
            Expression rightNode = outStack.top();
            outStack.pop();

            Expression leftNode = outStack.top();
            outStack.pop();

            BinaryOp node = std::make_shared<BinaryOpNode>(parent, stmtNo, leftNode, rightNode, currToken);
            rightNode->setParent(node);
            leftNode->setParent(node);
            outStack.push(node);
        }
    }

    Expression res = outStack.top();
    reset();
    return res;
}

void ExprParser::reset() {
    tokens.clear();
    types.clear();
    postfix.clear();
    postfixTypes.clear();
}

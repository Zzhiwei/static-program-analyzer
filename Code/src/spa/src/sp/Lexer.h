#pragma once

#include <string>
#include <list>
#include <istream>

enum class TokenType {
    IDENT,
    NUMBER,
    OTHERS,
    END
};

class Lexer {
private:
    int lineNo = 1;

    std::string currentToken;

    TokenType currentTokenType;

    std::shared_ptr<std::istream> source;

    void eatSpaceAndNewLine();

public:
    void nextToken();

    explicit Lexer(std::shared_ptr<std::istream> source);

    bool match(std::string);

    bool matchIdentifier();

    bool matchConstant();

    bool matchRelOperator();

    bool matchCondOperator();

    bool matchExprChar();

    void eat(std::string);

    std::string eatIdentifier();

    std::string eatConstant();

    std::string eatRelOperator();

    std::string eatCondOperator();

    std::string eatExprChar();

    std::string getCurrentToken();

    int getLineNo();

    TokenType getTokenType();

    char peekChar();

    bool isEnd();
};
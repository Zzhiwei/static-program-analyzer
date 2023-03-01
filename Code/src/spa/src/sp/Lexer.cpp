#include <unordered_set>
#include <cctype>
#include "Lexer.h"
#include "SpException.h"

static const std::unordered_set<char> operatorAndDelimiters({'{', '}', '(', ')', '+', '-', '*', '/', '%',
                                                ';', '=', '>', '<', '&', '|', '!'});

static const std::unordered_set<std::string> relOperators({">", "<", ">=", "<=", "==", "!="});

// "!" is deliberately omitted here because it is matched and eaten directly using Lexer::match
static const std::unordered_set<std::string> condOperators({"&&", "||"});

std::unordered_set<std::string> exprCharSet = {"(", ")", "+", "-", "*", "/", "%"};

void Lexer::eatSpaceAndNewLine() {
    while (isspace(source->peek()) || source->peek() == '\n') {
        if (source->peek() == '\n') {
            lineNo++;
        }
        source->get();
    }
}

void Lexer::nextToken() {
    eatSpaceAndNewLine();
    if (source->eof()) {
        currentToken = "";
        currentTokenType = TokenType::END;
        return;
    }

    char nextChar = source->peek();
    std::string newToken = "";
    if (isalpha(nextChar)) {  // first char is a digit, found identifier token
        currentTokenType = TokenType::IDENT;
        while (isalnum(source->peek())) {
            newToken += std::string(1, source->get());
        }
    } else if (isdigit(nextChar)) { // first char is a digit, found number token
        currentTokenType = TokenType::NUMBER;
        while (isdigit(source->peek())) {
            newToken += std::string(1, source->get());
        }
    } else if (operatorAndDelimiters.find(nextChar) != operatorAndDelimiters.end()) {
        currentTokenType = TokenType::OTHERS;
        char c = source->get();
        if (c == '&' && source->peek() == '&') {
            //there is no need to check for a single & or |, parser will discover this error
            newToken = "&&";
            source->get();
        } else if (c == '|' && source->peek() == '|') {
            newToken = "||";
            source->get();
        } else if (c == '=' && source->peek() == '=') {
            newToken = "==";
            source->get();
        } else if (c == '!' && source->peek() == '=') {
            newToken = "!=";
            source->get();
        } else if (c == '>' && source->peek() == '=') {
            newToken = ">=";
            source->get();
        } else if (c == '<' && source->peek() == '=') {
            newToken = "<=";
            source->get();
        } else {
            newToken = c;
        }
    } else {
        throw LexerException("Unknown character found", lineNo, nextChar);
    }
    currentToken = newToken;
}


bool Lexer::match(std::string s) {
    return currentToken == s;
}

bool Lexer::matchIdentifier() {
    return currentTokenType == TokenType::IDENT;
}

bool Lexer::matchConstant() {
    return currentTokenType == TokenType::NUMBER;
}

bool Lexer::matchExprChar() {
    return exprCharSet.find(currentToken) != exprCharSet.end();
}

bool Lexer::matchRelOperator() {
    return relOperators.find(currentToken) != relOperators.end();
}

bool Lexer::matchCondOperator() {
    return condOperators.find(currentToken) != condOperators.end();
}

void Lexer::eat(std::string str) {
    if (!match(str)) {
        throw LexerException("current token does not match given string", lineNo, str, currentToken);
    }
    nextToken();
}

std::string Lexer::eatIdentifier() {
    if (!matchIdentifier()) {
        throw LexerException("current token is not a valid identifier", lineNo, currentToken);
    }
    std::string curr = currentToken;
    nextToken();
    return curr;
}

std::string Lexer::eatConstant() {
    if (!matchConstant()) {
        throw LexerException("current token is not a valid constant", lineNo, currentToken);
    }
    std::string curr = currentToken;
    nextToken();
    return curr;
}

std::string Lexer::eatExprChar() {
    if (!matchExprChar()) {
        throw LexerException("current token is not a valid expression character",
                             lineNo, currentToken);
    }
    std::string op = currentToken;
    nextToken();
    return op;
}

std::string Lexer::eatRelOperator() {
    if (!matchRelOperator()) {
        throw LexerException("current token is not a valid relational operator",
                             lineNo, currentToken);
    }
    std::string op = currentToken;
    nextToken();
    return op;
}

std::string Lexer::eatCondOperator() {
    if (!matchCondOperator()) {
        throw LexerException("current token is not '&&' or '||' ",
                             lineNo, currentToken);
    }
    std::string op = currentToken;
    nextToken();
    return op;
}

std::string Lexer::getCurrentToken() {
    return currentToken;
}

int Lexer::getLineNo() {
    return lineNo;
}

TokenType Lexer::getTokenType() {
    return currentTokenType;
}

char Lexer::peekChar() {
    eatSpaceAndNewLine();
    return source->peek();
}

bool Lexer::isEnd() {
    return source->eof();
}

Lexer::Lexer(std::shared_ptr<std::istream> source) : source(source){
    nextToken();
}

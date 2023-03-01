//
// Created by Vanessa Khor on 5/2/23.
//
#pragma once
#include <string>
#include <list>
#include <unordered_set>
#include "QueryLexer.h"
#include "declaration.h"
#include "query.h"
#include "Clause.h"
#include "query.h"
#include <iostream>
#include <vector>
#include <set>
#include "QpsException.h"

class QueryParser {
private:
    std::vector<Declaration> declarationsList;
    std::vector<std::shared_ptr<Clause>> clausesList;
    Query query =  Query(declarationsList, clausesList, "");

    std::string assignClause(std::string clauseType, std::string side);

    std::string assignPatternClause(std::string side);

public:
    QueryLexer lexer;

    explicit QueryParser(QueryLexer lexer);

    void isTokenValid(std::string input);

    Query parse();

    void parseDeclarations(std::string input);

    void parseClauses(std::string input);

    enum QueryState {
        Initialization,
        DeclarationStage,
        SelectStage,
        ClauseStage,
    };

    QueryState currentState = QueryState::Initialization;

    std::string getCurrentState();

    void handleSelectClause();

    void advanceState(int nxt);

    void matchBrackets();

    std::string getPatternToken(std::string side);

    std::string getClauseToken(std::string side);

    // to validate clause tokens
    bool matchValidEntRef(std::string token);
    bool matchValidExpressionSpec(std::string token);
    bool matchValidStmtRef(std::string token);
    bool matchValidStrictStmtRef(std::string token);
    bool matchValidNonWildcardStmtRef(std::string token, std::string clauseType);
    bool matchValidLPattern(std::string token);
};
//
// Created by Vanessa Khor on 5/2/23.
//
#include "queryParser.h"

QueryParser::QueryParser(QueryLexer lexer) : lexer(lexer) {}

std::unordered_set<std::string> declaration_keywords_set({ "stmt" , "read" , "print" , "while" , "if" , "assign" ,
                                           "variable" , "constant" , "procedure" });

std::unordered_set<std::string> clause_keywords_set({ "Uses", "Modifies", "Follows", "FollowsT", "Parent", "ParentT", "pattern" });
std::unordered_set<std::string> sub_clause_keywords_set({ "Uses", "Modifies" });
std::unordered_set<std::string> stmt_clause_keywords_set({ "Follows", "FollowsT", "Parent", "ParentT" });
std::unordered_set<std::string> valid_connectors({"such", "pattern"});

void QueryParser::advanceState(int nxt) {
    if(nxt > 3 || nxt < 0) {
        // invalid
    } else {
        currentState = static_cast<QueryState>(nxt);
    }
}

std::string QueryParser::getCurrentState() {
    return std::to_string(currentState);
}

void QueryParser::isTokenValid(std::string token) {
    std::string selectStmt = "Select";

    if(currentState == 0) { // initialization stage
        if(declaration_keywords_set.find(token) != declaration_keywords_set.end()) {
            advanceState(1);
        } else if (token == selectStmt) {  // case sensitive comparison
            advanceState(2);
        } else { // invalid query
            throw QpsSyntaxException("Expected a declaration or Select statement", lexer.getLineNo(), token);
        }

    } else if (currentState == 1) { // declaration stage

        if(clause_keywords_set.find(token) != clause_keywords_set.end()) {
            throw QpsSyntaxException("Expected a select statement", lexer.getLineNo(), token);
        } else if (token == selectStmt) {  // case sensitive comparison
            advanceState(2); // proceed to parse one select clause
//        } else if (strcasecmp(token.c_str(), selectStmt.c_str()) == 0) { // case insensitive comparison
//            throw QpsSyntaxException("Syntax error", lexer.getLineNo(), selectStmt, token);
        } else if (declaration_keywords_set.find(token) != declaration_keywords_set.end()) {
//            return true;
        } else {
            throw QpsSyntaxException("Invalid query declaration token", lexer.getLineNo(), token);
        }

    } else if (currentState == 2) { // select statement stage
        if(token == selectStmt) {
            throw QpsSyntaxException("Select clause can only be declared once", lexer.getLineNo(), token);
        } else if (clause_keywords_set.find(token) != clause_keywords_set.end()) { // valid clause
            advanceState(3);
        } else {
            throw QpsSyntaxException("Invalid query token", lexer.getLineNo(),token);
        }

    } else if (currentState == 3) { // clause stage
        if(token == selectStmt) {
            throw QpsSyntaxException("Select clause can only be declared once", lexer.getLineNo(), token);
        } else if (clause_keywords_set.find(token) != clause_keywords_set.end()) { // valid clause
//            return true;
        } else {
            throw QpsSyntaxException("Invalid clause query token", lexer.getLineNo(),token);
        }

    } else {
        // should not reach here
        throw QpsSemanticException("Parsing error, query state should be within 0-3");
    }
//    return true;
};

void QueryParser::handleSelectClause() {
    lexer.eat("Select");
    if(lexer.matchIdentifier()) {
        std::string selected = lexer.eatIdentifier();

        // check if declared
        if(!query.isSynonymPresent(selected)) {
            throw QpsSyntaxException("Synonym not declared!", lexer.getLineNo(), selected);
        }

        query.setTarget(selected);

        std::string curr = lexer.getCurrentToken();
        if(!lexer.isEnd()) {
            if(valid_connectors.find(curr) != valid_connectors.end()) {
                if(curr == "such") {
                    lexer.eat(curr);
                    if(lexer.match("that") && !lexer.isEnd()) {
                        lexer.eat("that");
                        if(lexer.match("pattern")) {
                            throw QpsSyntaxException("such that cannot be followed by pattern clause");
                        }
                    } else {
                        throw QpsSyntaxException("Syntax error", lexer.getLineNo(), "that", lexer.getCurrentToken());
                    }
                } else { // mt1: only "pattern"
                    // continue
                }
            } else {
                throw QpsSyntaxException("Invalid query selection", lexer.getLineNo(),curr);
            };
        }
    }
}

Query QueryParser::parse() {

    while(!lexer.isEnd()) { // [x, eof)]
        std::string token1 = lexer.getCurrentToken();
        isTokenValid(token1);
        std::cout << "CURRENT TOKEN: " + token1 << std::endl;
        if (declaration_keywords_set.find(token1) != declaration_keywords_set.end()) { // if token1 is a design-entity
            parseDeclarations(token1);
        } else if (clause_keywords_set.find(token1) != clause_keywords_set.end()) { // if token1 is a clause
            parseClauses(token1);
        } else if (token1 == "Select") {
            handleSelectClause();
        } else {
            // cut away valid connectors
            lexer.eat(token1);
        }
    }

    if(lexer.isEnd()) {
        if(lexer.getCurrentToken() != "") {
            throw QpsSyntaxException("Incomplete query!");
        }
        if(currentState != 2 && currentState != 3) {
            throw QpsSyntaxException("Incomplete query!");
        }
    }

//    std::cout << "final query:" << std::endl;
//    query.printQuery();

    // return populated query object
    return query;
}

/*note: current lexer::eatIdentifier does not eat(identifier)*/
void QueryParser::parseDeclarations(std::string token1) {
    std::string designEntity = token1;
    std::vector<std::string> synonymLst;

        lexer.eat(designEntity); // advance lexer
        if(lexer.matchIdentifier()) {
            while (!lexer.match(";")) {
                    std::string id = lexer.eatIdentifier(); // remove id and advance lexer

                    // check for duplicates
                    if(query.isSynonymPresent(id)) {
                        throw QpsSyntaxException("Synonym already declared!", lexer.getLineNo(), id);
                    }

                    synonymLst.push_back(id);

                    if (lexer.getCurrentToken() == ";") {
                        // end
                    } else if (lexer.getCurrentToken() == ",") {
                        lexer.eat(","); // continue
                    } else {
                        throw QpsSyntaxException("Invalid declaration syntax", lexer.getLineNo(), lexer.getCurrentToken());
                    }
            }
        } else {
            throw QpsSyntaxException("Invalid identifier", lexer.getLineNo(), lexer.getCurrentToken());
        }

        lexer.eat(";");

    // populate declaration list
    Declaration currDeclaration = Declaration(designEntity, synonymLst);
    query.addDeclaration(currDeclaration);
}

// LETTER ( LETTER | DIGIT )*
bool isSynonym(std::string token) {
    char firstChar = token[0];
    if (!isalpha(firstChar)) {
        return false;
    }
    for (int i = 1; i < token.length(); i++) {
        if (isalnum(token.at(i)) == 0) {
            return false;
        }
    }
    return true;
}

// LETTER ( LETTER | DIGIT )*
bool isStringIdent(std::string token) {
    int len = token.length();

    char firstChar = token[0];
    if (firstChar != '\"') {
        return false;
    }

    // check if contains Synonym
    if(!isSynonym(token.substr(1, len - 2))) {
        return false;
    }

    char lastChar = token[len -1];
    if (lastChar != '\"') {
        return false;
    }

    return true;
}

bool isConstant(std::string token) {
    char firstChar = token[0];
    if (firstChar == '0') {
        return false;
    }

    for (int i = 1; i < token.length(); i++) {
        if (!isdigit(token[i])) {
            return false;
        }
    }
    return true;
}

/**
 * Used to validate RHS of Modifies and Uses clause
 * @param token
 * @return is token valid
 */
bool QueryParser::matchValidEntRef(std::string token) {
    // mt1 valid pattern: "_", synonym, "IDENT"

    // verify if token is valid
    if (token == "_") {
        return true;
    } else if (isSynonym(token)) {
        if (!query.isSynonymPresent(token)) {
            throw QpsSyntaxException("Synonym not declared!", lexer.getLineNo(), token);
        }

        // verify if token is a declared variable
        if (!query.isDeclarationPresent("variable", token)) {
            throw QpsSemanticException("Invalid synonym used", lexer.getLineNo(), token);
        }
        return true;
    } else {
        return isStringIdent(token);
    }
}

/**
 * Used to validate LHS of Modifies and Uses Clause
 * stmtRef grammar with no wildcard allowed
 * @param token
 * @return is token valid
 */
bool QueryParser::matchValidNonWildcardStmtRef(std::string token, std::string clauseType) {
    // mt1 valid pattern: synonym, Integer

    // verify if token is valid
    if(token == "_") {
        throw QpsSemanticException("Semantic Error, invalid use of \"_\"", lexer.getLineNo());
    } else if (isSynonym(token)) {
        if(!query.isSynonymPresent(token)) {
            throw QpsSyntaxException("Synonym not declared!", lexer.getLineNo(), token);
        }

        // verify if token is a valid declared stmt type synonym
        if(query.isDeclarationPresent("constant", token) || query.isDeclarationPresent("variable", token)) {
            throw QpsSemanticException("Invalid synonym used", lexer.getLineNo(), token);
        } else if (clauseType == "Uses" && query.isDeclarationPresent("read", token)) {
            throw QpsSemanticException("Invalid synonym used", lexer.getLineNo(), token);
        } else if (clauseType == "Modifies" && query.isDeclarationPresent("print", token)) {
            throw QpsSemanticException("Invalid synonym used", lexer.getLineNo(), token);
        } else {
            return true;
        }

    } else {
        return isConstant(token);
    }
}

/**
 * Used to validate <LHS, RHS> of Follows, FollowsT, Parent, ParentT Clause
 * @param token
 * @return is token valid
 */
bool QueryParser::matchValidStmtRef(std::string token) {
    // mt1 valid pattern: "_", synonym, Integer

    // verify if token is valid
    if(token == "_") {
        return true;
    } else if (isSynonym(token)) {
        if(!query.isSynonymPresent(token)) {
            throw QpsSyntaxException("Synonym not declared!", lexer.getLineNo(), token);
        }
        return  true;
    } else {
        return isConstant(token);
    }
    // verify if token is a declared variable
    return query.isDeclarationPresent("variable", token);
}

/**
 * Used to validate <LHS, RHS> of Follows, FollowsT, Parent, ParentT Clause
 * @param token
 * @return is token valid
 */
bool QueryParser::matchValidStrictStmtRef(std::string token) {
    // mt1 valid pattern: "_", synonym (except variable and procedure) , Integer
    // Semantic error: IDENT

    // verify if token is valid
    if(isStringIdent(token)) {
        throw QpsSemanticException("IDENT is not allowed", lexer.getLineNo(), token);
    }

    if(token == "_") {
        return true;
    } else if (isSynonym(token)) {
        if(!query.isSynonymPresent(token)) {
            throw QpsSyntaxException("Synonym not declared!", lexer.getLineNo(), token);
        }

        // verify if token is a valid declared stmt type synonym
        if(query.isDeclarationPresent("variable", token) || query.isDeclarationPresent("procedure", token) || query.isDeclarationPresent("constant", token)) {
            throw QpsSemanticException("Invalid synonym used", lexer.getLineNo(), token);
        } else {
            return true;
        }

    } else {
        return isConstant(token);
    }
}

/**
 * Used to validate RHS of pattern clause
 * @param token
 * @return is token valid
 */
bool QueryParser::matchValidExpressionSpec(std::string token) {
    // mt1 valid pattern: _, "_"x"_"
    char wildcard = '_';

    // "_"
    if(token == "_") {
        return true;
    }

    // empty token
    if(token.length() == 0) {
        return false;
    }

    char firstChar = token[0];
    if (firstChar != wildcard) {
        return false;
    }
    char secondChar = token[1]; // expect "
    if (secondChar != '\"') {
        return false;
    } else {
        int terminate = token.length() - 2;
        for (int i = 2; i < terminate; i++) {
            if (isalnum(token.at(i)) == 0) {
                return false;
            }
        }
        if (token[terminate] != '\"' || (token[terminate + 1] != '_')) {
            return false;
        }
    }
    return true;
}

/**
 * Used to validate LHS of pattern clause
 * @param token
 * @return is token valid
 */
bool QueryParser::matchValidLPattern(std::string token) {
    // mt1 valid pattern: "_", variable synonym, ""x""

    if(!matchValidEntRef(token)) {
        return false;
    }

    // further filter entRef
    if(isSynonym(token)) {
        if(query.isDeclarationPresent("variable", token)) {
            return true;
        } else {
            throw QpsSemanticException("Invalid synonym for pattern clause", lexer.getLineNo(), token);
        }
    }
    return true;
}

std::string QueryParser::getPatternToken(std::string side) {
    std::string terminator = side == "LHS" ? "," : ")";
    std::string res = "";

    while(!lexer.match(terminator) && !lexer.isEnd()) {
        std::string curr = lexer.getCurrentToken();
        res = res + curr;
        lexer.eat(curr);
    }
    if(res == "") {
        throw QpsSyntaxException("Clause cannot be empty");
    }
    return res;
}

std::string QueryParser::getClauseToken(std::string side) {
    std::string terminator = side == "LHS" ? "," : ")";
    std::string res = "";

    while(!lexer.match(terminator) && !lexer.isEnd()) {
        std::string curr = lexer.getCurrentToken();
        res = res + curr;
        lexer.eat(curr);
    }
    if(res == "") {
        throw QpsSyntaxException("Clause cannot be empty");
    }
    return res;
}

std::string QueryParser::assignClause(std::string clauseType, std::string side) {
    std::string currToken = getClauseToken(side);

    // grammar: <stmtRef, entRef>
    if(sub_clause_keywords_set.find(clauseType) != sub_clause_keywords_set.end()) {
        if(side == "LHS") {
            if(matchValidNonWildcardStmtRef(currToken, clauseType)) {
                return currToken;
            } else {
                throw QpsSyntaxException("invalid grammar", lexer.getLineNo(), currToken);
            }
        } else { // RHS
            if(matchValidEntRef(currToken)) {
                return currToken;
            } else {
                throw QpsSyntaxException("invalid grammar", lexer.getLineNo(),currToken);
            }
        }

    } else { // grammar: <stmtRef, stmtRef>
        if(matchValidStrictStmtRef(currToken)) {
            return currToken;
        } else {
            throw QpsSyntaxException("invalid grammar", lexer.getLineNo(),currToken);
        }
    }
}

std::string QueryParser::assignPatternClause(std::string side) {
    std::string terminator = side == "LHS" ? "," : ")";

    if(side == "LHS") {
        std::string lPattern = getPatternToken("LHS");
        if(matchValidLPattern(lPattern)) {
            return lPattern;
        } else {
            throw QpsSyntaxException("invalid pattern", lexer.getLineNo(), lPattern);
        }
    } else { // RHS
        std::string rPattern = getPatternToken("RHS");
        if(matchValidExpressionSpec(rPattern)) {
            return rPattern;
        } else {
            throw QpsSyntaxException("invalid pattern", lexer.getLineNo(), rPattern);
        }
    }
}

void QueryParser::parseClauses(std::string token) {
    std::string clauseType = token;
    std::string LHS;
    std::string RHS;

    if(clauseType == "pattern") {
        std::string patternId;
        lexer.eat(clauseType);
        if(lexer.matchIdentifier()) {
            patternId = lexer.eatIdentifier(); // "a" of a (LHS, RHS)

            if(!query.isDeclarationPresent("assign", patternId)) {
                throw QpsSyntaxException("Undeclared pattern assign synonym", lexer.getLineNo(), patternId);
            }

            if(lexer.match("(")) {
                lexer.eat("(");
                LHS = this->assignPatternClause("LHS");
                if(lexer.match(",")) {
                    lexer.eat(",");
                    RHS = this->assignPatternClause("RHS");
                    if(lexer.match(")")) {
                        lexer.eat(")");
                    } else {
                        throw QpsSyntaxException("Syntax error", lexer.getLineNo(), ")", lexer.getCurrentToken());
                    }
                } else {
                    throw QpsSyntaxException("Syntax error", lexer.getLineNo(), ",", lexer.getCurrentToken());
                }
            } else {
                throw QpsSyntaxException("Syntax error", lexer.getLineNo(), "(", lexer.getCurrentToken());
            }

        } else {
            throw QpsSyntaxException("Expected an assign synonym", lexer.getLineNo(), lexer.getCurrentToken());
        }

        if(lexer.match("such")) {
            lexer.eat("such");
            if(lexer.match("that")) {
                lexer.eat("that");
            } else {
                throw QpsSyntaxException("Expected \"that\"", lexer.getLineNo(), lexer.getCurrentToken());
            }
        }

        // populate clause list
        std::shared_ptr<Clause> currClause = std::make_shared<PatternClause>(LHS, RHS);
        currClause->setAssignSynonym(patternId);
        query.addClause(currClause);

    } else {
        lexer.eat(clauseType);

        // identify clauses of type Clause*
        if(lexer.getCurrentToken() == "*") {
            lexer.eat("*");
            clauseType = clauseType + "T";
        }

        if(lexer.match("(")) {
            lexer.eat("(");
            LHS = this->assignClause(clauseType , "LHS");
            if(lexer.match(",")) {
                lexer.eat(",");
                RHS = this->assignClause(clauseType, "RHS");
                if(lexer.match(")")) {
                    lexer.eat(")");
                } else {
                    throw QpsSyntaxException("Syntax error", lexer.getLineNo(), ")", lexer.getCurrentToken());
                }
            } else {
                throw QpsSyntaxException("Syntax error", lexer.getLineNo(), ",", lexer.getCurrentToken());
            }
        } else {
            throw QpsSyntaxException("Syntax error", lexer.getLineNo(), "(", lexer.getCurrentToken());
        }

        // populate clause list
        if(clauseType == "Uses") {
            std::shared_ptr<Clause> currClause = std::make_shared<UsesClause>(LHS, RHS);
            query.addClause(currClause);
        } else if (clauseType == "Modifies") {
            std::shared_ptr<Clause> currClause = std::make_shared<ModifiesClause>(LHS, RHS);
            query.addClause(currClause);

        } else if (clauseType == "Follows") {
            std::shared_ptr<Clause> currClause = std::make_shared<FollowsClause>(LHS, RHS);
            query.addClause(currClause);

        } else if (clauseType == "FollowsT") {
            std::shared_ptr<Clause> currClause = std::make_shared<FollowsTClause>(LHS, RHS);
            query.addClause(currClause);

        } else if (clauseType == "Parent") {
            std::shared_ptr<Clause> currClause = std::make_shared<ParentClause>(LHS, RHS);
            query.addClause(currClause);

        } else if (clauseType == "ParentT") {
            std::shared_ptr<Clause> currClause = std::make_shared<ParentTClause>(LHS, RHS);
            query.addClause(currClause);
        } else {
            // unable to evaluate yet (should not reach here)
            throw QpsSemanticException("Unable to evaluate current clause type");
        }
    }
};



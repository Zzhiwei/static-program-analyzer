//
// Created by Vanessa Khor on 5/2/23.
//

#include <istream>
#include <sstream>
#include "Clause.h"
#include "declaration.h"
#include "queryProcessor.h"
#include "QueryLexer.h"
#include "queryParser.h"
#include "QueryEvaluator.h"
#include "pkb/QPSFacade.h"
#include "pkb/PKB.h"
#include "ClauseResult.h"
#include "ClauseResultEvaluator.h"
#include "responseBuilder.h"
#include <vector>

QueryProcessor::QueryProcessor(QPSFacade& qpsFacade) : qpsFacade(qpsFacade) {}

// takes in PQL from user input
std::vector<std::string> QueryProcessor::process(std::string input) {
    try {
        std::shared_ptr<std::istringstream> iss = std::make_shared<std::istringstream>(input);
        QueryLexer queryLexer = QueryLexer(iss);
        QueryParser queryParser = QueryParser(queryLexer);
        Query query = queryParser.parse();
        //QPSFacade qpsFacade = pkb.getQPSFacade();				// TODO Need to get the PKB instance, take in as param instead?
        QueryEvaluator queryEvaluator = QueryEvaluator();
        ClauseResult finalResult = queryEvaluator.computeFinalResult(query, qpsFacade);
        ResponseBuilder responseBuilder = ResponseBuilder(qpsFacade, query);
        std::vector<std::string> queryResponse = responseBuilder.formatOutput(finalResult);
        //std::cout << "final query" + queryResultString << std::endl;
        return queryResponse;

        // Evaluate the clauses in the query
        //ClauseResult result = queryEvaluator.computeFinalResult(query, qpsFacade);

        // To response builder
        //ResponseBuilder responseBuilder(qpsFacade, query);
        //auto resultStringList = responseBuilder.formatOutput(result);
    } catch (QpsSemanticException e) {
        std::vector<std::string> errMessage;
        errMessage.push_back(e.msg);
        return errMessage;
    } catch (QpsSyntaxException e) {
        std::vector<std::string> errMessage;
        errMessage.push_back(e.msg);
        return errMessage;
    }
}


#include <string>
#include <vector>
#include "query.h"
#include "Clause.h"
#include "ClauseResult.h"
#include "pkb/QPSFacade.h"
#include "ClauseResultEvaluator.h"

class QueryEvaluator {
public:
    // Get results from PKB for each clause and combines them
    // to obtain the final result to pass to ResponseBuilder
    ClauseResult computeFinalResult(Query &query, QPSFacade& qpsFacade);

};
#pragma once

#include "ClauseResult.h"
#include "ClauseResultBuilder.h"

// Forward declarations
class ClauseResult;

class ClauseResultEvaluator {
private:
	// Evaluate any combination of clause results where one of them is a boolean clause result
	static ClauseResult evaluateBool(ClauseResult& boolClause, ClauseResult& c2);
	// Evaluates any combination of two table clause results. Joining of the tables are non-destructive.
	static ClauseResult evaluateTableTable(ClauseResult& c1, ClauseResult& c2);
public:
	static ClauseResult evaluate(ClauseResult& c1, ClauseResult& c2);
};
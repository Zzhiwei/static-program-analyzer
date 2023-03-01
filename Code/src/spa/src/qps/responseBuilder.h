//
// Created by Vanessa Khor on 5/2/23.
//

#include <string>
#include <vector>
#include "pkb/QPSFacade.h"
#include "ClauseResult.h"
#include "query.h"
#include "QpsException.h"

class ResponseBuilder {
private:
	QPSFacade qpsf;
	Query query;

	std::string findTypeOf(std::string name);
public:
	ResponseBuilder(QPSFacade& qpsf, Query& query);
	std::vector<std::string> formatOutput(ClauseResult &res);
	//std::string formatString(std::vector<std::string> list);
};
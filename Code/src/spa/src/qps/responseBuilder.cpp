//
// Created by Vanessa Khor on 5/2/23.
//
#include "responseBuilder.h"

ResponseBuilder::ResponseBuilder(QPSFacade& qpsf, Query& query) : qpsf(qpsf), query(query) {}

std::vector<std::string> ResponseBuilder::formatOutput(ClauseResult &res) {
    std::string name = query.getTarget();
    std::string type = this->findTypeOf(name);
    //std::string formattedOutput = this->formatString(res.extractQueryData(this->qpsf, name, type));
    return res.extractQueryData(this->qpsf, name, type);
}


std::string ResponseBuilder::findTypeOf(std::string name) {
    for (auto declaration : query.getDeclarationList()) {
        for (auto synonym : declaration.getSynonymList()) {
            if (synonym == name) {
                return declaration.getType();
            }
        }
    }

    throw QpsSyntaxException("Synonym " + name + " is not included in any declarations");
}

/*
std::string ResponseBuilder::formatString(std::vector<std::string> list) {
    std::string str = list[0];
    for (auto& i : list) {
        if (i == list[0]) {
            continue;
        }
        str += ", " + i;
    }
    return str;
}
*/
//
// Created by Vanessa Khor on 5/2/23.
//
#pragma once
#include <iostream>
#include <string>
#include <vector>
#include "pkb/QPSFacade.h"
#include "pkb/PKB.h"

class QueryProcessor {
public:
    explicit QueryProcessor(QPSFacade& qpsFacade);
    QPSFacade& qpsFacade;
    std::vector<std::string> process(std::string input);
};


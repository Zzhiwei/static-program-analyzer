#pragma once

#include<stdio.h>
#include <iostream>
#include <string>
#include <vector>
#include <memory>
#include "PatternManager.h"
#include "EntityManager.h"
#include "RelationshipManager.h"
#include "SPFacade.h"
#include "QPSFacade.h"



class PKB {
private:
	std::shared_ptr<PatternManager> patternManager = std::make_shared<PatternManager>();
	std::shared_ptr<EntityManager> entityManager = std::make_shared<EntityManager>();
	std::shared_ptr<RelationshipManager> relationshipManager = std::make_shared<RelationshipManager>();


public:
    std::shared_ptr<SPFacade> getSPFacade();
	QPSFacade getQPSFacade();

};
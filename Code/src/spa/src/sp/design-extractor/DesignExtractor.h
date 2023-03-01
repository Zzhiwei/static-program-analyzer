#pragma once

#include <memory>

#include "ast/RootNode.h"
#include "BasicExtractionVisitor.h"
#include "FollowVisitor.h"
#include "ParentVisitor.h"
#include "pkb/SPFacade.h"

class DesignExtractor {
private:
    std::unique_ptr<BasicExtractionVisitor> basicVisitor;
    std::unique_ptr<ParentVisitor> parentVisitor;
    std::unique_ptr<FollowVisitor> followVisitor;
    AstRoot root;
public:
    DesignExtractor(std::shared_ptr<SPFacade> spFacade, AstRoot root);
    void extract();
};

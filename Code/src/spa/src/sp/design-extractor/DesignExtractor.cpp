#include "DesignExtractor.h"

DesignExtractor::DesignExtractor(std::shared_ptr<SPFacade> spFacade, AstRoot root) {
    this->basicVisitor = std::make_unique<BasicExtractionVisitor>(spFacade);
    this->parentVisitor = std::make_unique<ParentVisitor>(spFacade);
    this->followVisitor = std::make_unique<FollowVisitor>(spFacade);
    this->root = root;
}

void DesignExtractor::extract() {
    this->root->accept(basicVisitor.get());
    this->root->accept(parentVisitor.get());
    this->root->accept(followVisitor.get());
}

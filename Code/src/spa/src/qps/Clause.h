#pragma once
#include <string>
#include <vector>
#include <iostream>
#include "../pkb/QPSFacade.h"
#include "ClauseResult.h"

class Clause {
private:
    /*
     * constant x represented as '"x"'
     * synonym x represented as "x"
     * */
    std::string LHS;
    std::string RHS;
    std::string clauseType;
    std::string assignSynonym;

public:
    explicit Clause(std::string lhs, std::string rhs);
    void printClause();
    std::string getLHS();
    std::string getRHS();
    std::string getAssignSynonym();
    bool isSynonym(std::string input);
    bool isSubstring(std::string input);
    bool isWildcard(std::string input);
    std::vector< std::string > intArrToStringArr(std::vector<int> arr);
    virtual ClauseResult evaluate(QPSFacade& qpsFacade, std::string target, std::unordered_map<std::string, std::string> synonymTypes);
    virtual std::vector<std::string> evaluateSpecial(QPSFacade& qpsFacade, std::string target, std::unordered_map<std::string, std::string> synonymTypes);
    virtual void setAssignSynonym(std::string a);

    bool operator==(const Clause& other) const {
        return LHS == other.LHS && RHS == other.RHS && clauseType == other.clauseType;
    }

};

class FollowsClause : public Clause {
public:
    FollowsClause(std::string lhs, std::string rhs);
    ClauseResult evaluate(QPSFacade& qpsFacade, std::string target, std::unordered_map<std::string, std::string> synonymTypes);
    std::vector<std::string> evaluateSpecial(QPSFacade& qpsFacade, std::string target, std::unordered_map<std::string, std::string> synonymTypes);
};

class FollowsTClause : public Clause {
public:
    FollowsTClause(std::string lhs, std::string rhs);
    ClauseResult evaluate(QPSFacade& qpsFacade, std::string target, std::unordered_map<std::string, std::string> synonymTypes);
    std::vector<std::string> evaluateSpecial(QPSFacade& qpsFacade, std::string target, std::unordered_map<std::string, std::string> synonymTypes);
};

class ParentClause : public Clause {
public:
    ParentClause(std::string lhs, std::string rhs);
    ClauseResult evaluate(QPSFacade& qpsFacade, std::string target, std::unordered_map<std::string, std::string> synonymTypes);
    std::vector<std::string> evaluateSpecial(QPSFacade& qpsFacade, std::string target, std::unordered_map<std::string, std::string> synonymTypes);
};

class ParentTClause : public Clause {
public:
    ParentTClause(std::string lhs, std::string rhs);
    ClauseResult evaluate(QPSFacade& qpsFacade, std::string target, std::unordered_map<std::string, std::string> synonymTypes);
    std::vector<std::string> evaluateSpecial(QPSFacade& qpsFacade, std::string target, std::unordered_map<std::string, std::string> synonymTypes);
};

class UsesClause : public Clause {
public:
    UsesClause(std::string lhs, std::string rhs);
    ClauseResult evaluate(QPSFacade& qpsFacade, std::string target, std::unordered_map<std::string, std::string> synonymTypes);
    std::vector<std::string> evaluateSpecial(QPSFacade& qpsFacade, std::string target, std::unordered_map<std::string, std::string> synonymTypes);
};

class ModifiesClause : public Clause {
public:
    ModifiesClause(std::string lhs, std::string rhs);
    ClauseResult evaluate(QPSFacade& qpsFacade, std::string target, std::unordered_map<std::string, std::string> synonymTypes);
    std::vector<std::string> evaluateSpecial(QPSFacade& qpsFacade, std::string target, std::unordered_map<std::string, std::string> synonymTypes);
};

class PatternClause : public Clause {
public:

    PatternClause(std::string lhs, std::string rhs);
    ClauseResult evaluate(QPSFacade& qpsFacade, std::string target, std::unordered_map<std::string, std::string> synonymTypes);
    std::vector<std::string> evaluateSpecial(QPSFacade& qpsFacade, std::string target, std::unordered_map<std::string, std::string> synonymTypes);
};
#include "Clause.h"

Clause::Clause(std::string lhs, std::string rhs) : LHS(lhs), RHS(rhs) {
    //to be implemented
};
FollowsClause::FollowsClause(std::string lhs, std::string rhs) : Clause(lhs, rhs) {}

FollowsTClause::FollowsTClause(std::string lhs, std::string rhs) : Clause(lhs, rhs) {}

ParentClause::ParentClause(std::string lhs, std::string rhs) : Clause(lhs, rhs) {}

ParentTClause::ParentTClause(std::string lhs, std::string rhs) : Clause(lhs, rhs) {}

UsesClause::UsesClause(std::string lhs, std::string rhs) : Clause(lhs, rhs) {}

ModifiesClause::ModifiesClause(std::string lhs, std::string rhs) : Clause(lhs, rhs) {}

PatternClause::PatternClause(std::string lhs, std::string rhs) : Clause(lhs, rhs) {}

void Clause::printClause() {
    std::cout << "(" + LHS + "," + RHS + ")" << std::endl;
}

std::string Clause::getLHS() {
    return LHS;
}

std::string Clause::getRHS() {
    return RHS;
}

std::string Clause::getAssignSynonym() {
    return assignSynonym;
}

bool Clause::isSynonym(std::string input) {//TODO implement assingTypeToArg
    //return input[0] != '"' && !std::isdigit(input[0]);
    return isalpha(input[0]) || isWildcard(input);
}

bool Clause::isSubstring(std::string input) {//TODO implement assingTypeToArg
    return input[0] == '_' && input.length() > 1;
}

bool Clause::isWildcard(std::string input) {//TODO implement assingTypeToArg
    return input[0] == '_' && input.length() == 1;
}

std::vector<std::string> Clause::intArrToStringArr(std::vector<int> arr) {
    std::vector < std::string > res;
    for (const auto& integer : arr) {
        res.push_back(std::to_string(integer));
    }
    return res;
}

void Clause::setAssignSynonym(std::string a) {
    assignSynonym = a;
}

ClauseResult Clause::evaluate(QPSFacade& qpsFacade, std::string target, std::unordered_map<std::string, std::string> synonymTypes) {
    ClauseResult defaultResult = ClauseResultBuilder::createBooleanResult(true);
    return defaultResult;
}

std::vector<std::string> Clause::evaluateSpecial(QPSFacade& qpsFacade, std::string target, std::unordered_map<std::string, std::string> synonymTypes) {
    return {};
}

ClauseResult FollowsClause::evaluate(QPSFacade& qpsFacade, std::string target, std::unordered_map<std::string, std::string> synonymTypes) {
    std::string LHS = getLHS();
    std::string RHS = getRHS();
    std::vector<int> ifs = qpsFacade.getIfStatements();
    std::vector<int> whiles = qpsFacade.getWhileStatements();
    std::vector<int> prints = qpsFacade.getPrintStatements();
    std::vector<int> reads = qpsFacade.getReadStatements();
    std::vector<int> assigns = qpsFacade.getAssignStatements();

    ClauseResult defaultResult = ClauseResultBuilder::createBooleanResult(true);
    if (isSynonym(LHS) && isSynonym(RHS)) {
        //if 2 synonyms (check if have)
        std::vector<std::vector<std::string>> list;
        std::vector<std::pair<int, int>> tupleList = qpsFacade.getAllFollows();
        if (synonymTypes[LHS] == "if") {
            for (int i = 0; i < tupleList.size(); i++) {
                if (!(std::find(ifs.begin(), ifs.end(), tupleList[i].first) != ifs.end())) {
                    tupleList.erase(tupleList.begin() + i);
                    i--;
                }
            }
        }
        else if (synonymTypes[LHS] == "while") {
            for (int i = 0; i < tupleList.size(); i++) {
                if (!(std::find(whiles.begin(), whiles.end(), tupleList[i].first) != whiles.end())) {
                    tupleList.erase(tupleList.begin() + i);
                    i--;
                }
            }
        }
        else if (synonymTypes[LHS] == "read") {
            for (int i = 0; i < tupleList.size(); i++) {
                if (!(std::find(reads.begin(), reads.end(), tupleList[i].first) != reads.end())) {
                    tupleList.erase(tupleList.begin() + i);
                    i--;
                }
            }
        }
        else if (synonymTypes[LHS] == "print") {
            for (int i = 0; i < tupleList.size(); i++) {
                if (!(std::find(prints.begin(), prints.end(), tupleList[i].first) != prints.end())) {
                    tupleList.erase(tupleList.begin() + i);
                    i--;
                }
            }
        }
        else if (synonymTypes[LHS] == "assign") {
            for (int i = 0; i < tupleList.size(); i++) {
                if (!(std::find(assigns.begin(), assigns.end(), tupleList[i].first) != assigns.end())) {
                    tupleList.erase(tupleList.begin() + i);
                    i--;
                }
            }
        }
        if (synonymTypes[RHS] == "if") {
            for (int i = 0; i < tupleList.size(); i++) {
                if (!(std::find(ifs.begin(), ifs.end(), tupleList[i].second) != ifs.end())) {
                    tupleList.erase(tupleList.begin() + i);
                    i--;
                }
            }
        }
        else if (synonymTypes[RHS] == "while") {
            for (int i = 0; i < tupleList.size(); i++) {
                if (!(std::find(whiles.begin(), whiles.end(), tupleList[i].second) != whiles.end())) {
                    tupleList.erase(tupleList.begin() + i);
                    i--;
                }
            }
        }
        else if (synonymTypes[RHS] == "read") {
            for (int i = 0; i < tupleList.size(); i++) {
                if (!(std::find(reads.begin(), reads.end(), tupleList[i].second) != reads.end())) {
                    tupleList.erase(tupleList.begin() + i);
                    i--;
                }
            }
        }
        else if (synonymTypes[RHS] == "print") {
            for (int i = 0; i < tupleList.size(); i++) {
                if (!(std::find(prints.begin(), prints.end(), tupleList[i].second) != prints.end())) {
                    tupleList.erase(tupleList.begin() + i);
                    i--;
                }
            }
        }
        else if (synonymTypes[RHS] == "assign") {
            for (int i = 0; i < tupleList.size(); i++) {
                if (!(std::find(assigns.begin(), assigns.end(), tupleList[i].second) != assigns.end())) {
                    tupleList.erase(tupleList.begin() + i);
                    i--;
                }
            }
        }
        for (std::pair<int, int> val : tupleList) {
            std::vector<std::string> temp;
            temp.push_back(std::to_string(val.first));
            temp.push_back(std::to_string(val.second));
            list.push_back(temp);
        }
        std::vector<std::string> synList;
        synList.push_back(LHS);
        synList.push_back(RHS);
        if (target != LHS && target != RHS) {
            return ClauseResultBuilder::createBooleanResult(!list.empty());
        }
        else {
            return ClauseResultBuilder::createTupleResult(synList, list);
        }
    } else if (isSynonym(LHS)) {
        //if 1 LHS synonym
        std::vector<std::string> list;
        int preceding = qpsFacade.getPreceding(stoi(RHS));
        if (synonymTypes[LHS] == "if") {
            if ((std::find(ifs.begin(), ifs.end(), preceding) != ifs.end())) {
                list.push_back(std::to_string(preceding));
            }
        }
        else if (synonymTypes[LHS] == "while") {
            if ((std::find(whiles.begin(), whiles.end(), preceding) != whiles.end())) {
                list.push_back(std::to_string(preceding));
            }
        }
        else if (synonymTypes[LHS] == "read") {
            if ((std::find(reads.begin(), reads.end(), preceding) != reads.end())) {
                list.push_back(std::to_string(preceding));
            }
        }
        else if (synonymTypes[LHS] == "print") {
            if ((std::find(prints.begin(), prints.end(), preceding) != prints.end())) {
                list.push_back(std::to_string(preceding));
            }
        }
        else if (synonymTypes[LHS] == "assign") {
            if ((std::find(assigns.begin(), assigns.end(), preceding) != assigns.end())) {
                list.push_back(std::to_string(preceding));
            }
        }
        else if (preceding != 0) {
            list.push_back(std::to_string(preceding));
        }
        if (target != LHS) {
            return ClauseResultBuilder::createBooleanResult(!list.empty());
        }
        else {
            return ClauseResultBuilder::createListResult(LHS, list);
        }
    } else if (isSynonym(RHS)) {
        //if 1 RHS synonym
        std::vector<std::string> list;
        int following = qpsFacade.getFollowing(stoi(LHS));
        if (following != 0) {
            list.push_back(std::to_string(following));
        }
        if (target != RHS) {
            return ClauseResultBuilder::createBooleanResult(!list.empty());
        }
        else {
            return ClauseResultBuilder::createListResult(RHS, list);
        }
    } else {
        //if 0 synonym
        return ClauseResultBuilder::createBooleanResult(qpsFacade.isFollows(stoi(LHS), stoi(RHS)));
    }
    return defaultResult;
}

ClauseResult FollowsTClause::evaluate(QPSFacade& qpsFacade, std::string target, std::unordered_map<std::string, std::string> synonymTypes) {
    std::string LHS = getLHS();
    std::string RHS = getRHS();
    std::vector<int> ifs = qpsFacade.getIfStatements();
    std::vector<int> whiles = qpsFacade.getWhileStatements();
    std::vector<int> prints = qpsFacade.getPrintStatements();
    std::vector<int> reads = qpsFacade.getReadStatements();
    std::vector<int> assigns = qpsFacade.getAssignStatements();
    ClauseResult defaultResult = ClauseResultBuilder::createBooleanResult(true);
    if (isSynonym(LHS) && isSynonym(RHS)) {
        //if 2 synonyms
        std::vector<std::vector<std::string>> list;
        std::vector<std::pair<int, int>> tupleList = qpsFacade.getAllFollowsStar();
        if (synonymTypes[LHS] == "if") {
            for (int i = 0; i < tupleList.size(); i++) {
                if (!(std::find(ifs.begin(), ifs.end(), tupleList[i].first) != ifs.end())) {
                    tupleList.erase(tupleList.begin() + i);
                    i--;
                }
            }
        }
        else if (synonymTypes[LHS] == "while") {
            for (int i = 0; i < tupleList.size(); i++) {
                if (!(std::find(whiles.begin(), whiles.end(), tupleList[i].first) != whiles.end())) {
                    tupleList.erase(tupleList.begin() + i);
                    i--;
                }
            }
        }
        else if (synonymTypes[LHS] == "read") {
            for (int i = 0; i < tupleList.size(); i++) {
                if (!(std::find(reads.begin(), reads.end(), tupleList[i].first) != reads.end())) {
                    tupleList.erase(tupleList.begin() + i);
                    i--;
                }
            }
        }
        else if (synonymTypes[LHS] == "print") {
            for (int i = 0; i < tupleList.size(); i++) {
                if (!(std::find(prints.begin(), prints.end(), tupleList[i].first) != prints.end())) {
                    tupleList.erase(tupleList.begin() + i);
                    i--;
                }
            }
        }
        else if (synonymTypes[LHS] == "assign") {
            for (int i = 0; i < tupleList.size(); i++) {
                if (!(std::find(assigns.begin(), assigns.end(), tupleList[i].first) != assigns.end())) {
                    tupleList.erase(tupleList.begin() + i);
                    i--;
                }
            }
        }
        if (synonymTypes[RHS] == "if") {
            for (int i = 0; i < tupleList.size(); i++) {
                if (!(std::find(ifs.begin(), ifs.end(), tupleList[i].second) != ifs.end())) {
                    tupleList.erase(tupleList.begin() + i);
                    i--;
                }
            }
        }
        else if (synonymTypes[RHS] == "while") {
            for (int i = 0; i < tupleList.size(); i++) {
                if (!(std::find(whiles.begin(), whiles.end(), tupleList[i].second) != whiles.end())) {
                    tupleList.erase(tupleList.begin() + i);
                    i--;
                }
            }
        }
        else if (synonymTypes[RHS] == "read") {
            for (int i = 0; i < tupleList.size(); i++) {
                if (!(std::find(reads.begin(), reads.end(), tupleList[i].second) != reads.end())) {
                    tupleList.erase(tupleList.begin() + i);
                    i--;
                }
            }
        }
        else if (synonymTypes[RHS] == "print") {
            for (int i = 0; i < tupleList.size(); i++) {
                if (!(std::find(prints.begin(), prints.end(), tupleList[i].second) != prints.end())) {
                    tupleList.erase(tupleList.begin() + i);
                    i--;
                }
            }
        }
        else if (synonymTypes[RHS] == "assign") {
            for (int i = 0; i < tupleList.size(); i++) {
                if (!(std::find(assigns.begin(), assigns.end(), tupleList[i].second) != assigns.end())) {
                    tupleList.erase(tupleList.begin() + i);
                    i--;
                }
            }
        }
        for (std::pair<int, int> val : tupleList) {
            std::vector<std::string> temp;
            temp.push_back(std::to_string(val.first));
            temp.push_back(std::to_string(val.second));
            list.push_back(temp);
        }
        std::vector<std::string> synList;
        synList.push_back(LHS);
        synList.push_back(RHS);
        return ClauseResultBuilder::createTupleResult(synList, list);
    } else if (isSynonym(LHS)) {
        //if 1 LHS synonym
        std::vector<std::string> list;
        for (int val : qpsFacade.getFollowsStarBackward(stoi(RHS))) {
            if (synonymTypes[LHS] == "if") {
                if ((std::find(ifs.begin(), ifs.end(), val) != ifs.end())) {
                    list.push_back(std::to_string(val));
                }
            }
            else if (synonymTypes[LHS] == "while") {
                if ((std::find(whiles.begin(), whiles.end(), val) != whiles.end())) {
                    list.push_back(std::to_string(val));
                }
            }
            else if (synonymTypes[LHS] == "read") {
                if ((std::find(reads.begin(), reads.end(), val) != reads.end())) {
                    list.push_back(std::to_string(val));
                }
            }
            else if (synonymTypes[LHS] == "print") {
                if ((std::find(prints.begin(), prints.end(), val) != prints.end())) {
                    list.push_back(std::to_string(val));
                }
            }
            else if (synonymTypes[LHS] == "assign") {
                if ((std::find(assigns.begin(), assigns.end(), val) != assigns.end())) {
                    list.push_back(std::to_string(val));
                }
            }
            else if (val != 0) {
                list.push_back(std::to_string(val));
            }
        }
        return ClauseResultBuilder::createListResult(LHS, list);
    } else if (isSynonym(RHS)) {
        //if 1 RHS synonym
        std::vector<std::string> list;
        for (int val : qpsFacade.getFollowsStarForward(stoi(LHS))) {
            list.push_back(std::to_string(val));
        }
        return ClauseResultBuilder::createListResult(RHS, list);
    } else {
        //if 0 synonym
        return ClauseResultBuilder::createBooleanResult(qpsFacade.isFollowsStar(stoi(LHS), stoi(RHS)));
    }
    return defaultResult;
}

ClauseResult ParentClause::evaluate(QPSFacade& qpsFacade, std::string target, std::unordered_map<std::string, std::string> synonymTypes) {
    std::string LHS = getLHS();
    std::string RHS = getRHS();
    std::vector<int> ifs = qpsFacade.getIfStatements();
    std::vector<int> whiles = qpsFacade.getWhileStatements();
    std::vector<int> prints = qpsFacade.getPrintStatements();
    std::vector<int> reads = qpsFacade.getReadStatements();
    std::vector<int> assigns = qpsFacade.getAssignStatements();
    
    ClauseResult defaultResult = ClauseResultBuilder::createBooleanResult(true);
    if (isSynonym(LHS) && isSynonym(RHS)) {
        //if 2 synonyms (check if have)
        std::vector<std::vector<std::string>> list;
        std::vector<std::pair<int, int>> tupleList = qpsFacade.getAllParent();
        if (synonymTypes[LHS] == "if") {
            for (int i = 0; i < tupleList.size(); i++) {
                if (!(std::find(ifs.begin(), ifs.end(), tupleList[i].first) != ifs.end())) {
                    tupleList.erase(tupleList.begin() + i); 
                    i--;
                }
            }
        }
        else if (synonymTypes[LHS] == "while") {
            for (int i = 0; i < tupleList.size(); i++) {
                if (!(std::find(whiles.begin(), whiles.end(), tupleList[i].first) != whiles.end())) {
                    tupleList.erase(tupleList.begin() + i);
                    i--;
                }
            }
        }
        else if (synonymTypes[LHS] == "read") {
            for (int i = 0; i < tupleList.size(); i++) {
                if (!(std::find(reads.begin(), reads.end(), tupleList[i].first) != reads.end())) {
                    tupleList.erase(tupleList.begin() + i);
                    i--;
                }
            }
        }
        else if (synonymTypes[LHS] == "print") {
            for (int i = 0; i < tupleList.size(); i++) {
                if (!(std::find(prints.begin(), prints.end(), tupleList[i].first) != prints.end())) {
                    tupleList.erase(tupleList.begin() + i);
                    i--;
                }
            }
        }
        else if (synonymTypes[LHS] == "assign") {
            for (int i = 0; i < tupleList.size(); i++) {
                if (!(std::find(assigns.begin(), assigns.end(), tupleList[i].first) != assigns.end())) {
                    tupleList.erase(tupleList.begin() + i);
                    i--;
                }
            }
        }
        if (synonymTypes[RHS] == "if") {
            for (int i = 0; i < tupleList.size(); i++) {
                if (!(std::find(ifs.begin(), ifs.end(), tupleList[i].second) != ifs.end())) {
                    tupleList.erase(tupleList.begin() + i);
                    i--;
                }
            }
        }
        else if (synonymTypes[RHS] == "while") {
            for (int i = 0; i < tupleList.size(); i++) {
                if (!(std::find(whiles.begin(), whiles.end(), tupleList[i].second) != whiles.end())) {
                    tupleList.erase(tupleList.begin() + i);
                    i--;
                }
            }
        }
        else if (synonymTypes[RHS] == "read") {
            for (int i = 0; i < tupleList.size(); i++) {
                if (!(std::find(reads.begin(), reads.end(), tupleList[i].second) != reads.end())) {
                    tupleList.erase(tupleList.begin() + i);
                    i--;
                }
            }
        }
        else if (synonymTypes[RHS] == "print") {
            for (int i = 0; i < tupleList.size(); i++) {
                if (!(std::find(prints.begin(), prints.end(), tupleList[i].second) != prints.end())) {
                    tupleList.erase(tupleList.begin() + i);
                    i--;
                }
            }
        }
        else if (synonymTypes[RHS] == "assign") {
            for (int i = 0; i < tupleList.size(); i++) {
                if (!(std::find(assigns.begin(), assigns.end(), tupleList[i].second) != assigns.end())) {
                    tupleList.erase(tupleList.begin() + i);
                    i--;
                }
            }
        }
        for (std::pair<int, int> val : tupleList) {
            std::vector<std::string> temp;
            temp.push_back(std::to_string(val.first));
            temp.push_back(std::to_string(val.second));
            list.push_back(temp);
        }
        std::vector<std::string> synList;
        synList.push_back(LHS);
        synList.push_back(RHS);
        return ClauseResultBuilder::createTupleResult(synList, list);
    } else if (isSynonym(LHS)) {
        //if 1 LHS synonym
        std::vector<std::string> list;
        int parent = qpsFacade.getParent(stoi(RHS));
        if (synonymTypes[LHS] == "if") {
            if ((std::find(ifs.begin(), ifs.end(), parent) != ifs.end())) {
                list.push_back(std::to_string(parent));
            }
        }
        else if (synonymTypes[LHS] == "while") {
            if ((std::find(whiles.begin(), whiles.end(), parent) != whiles.end())) {
                list.push_back(std::to_string(parent));
            }
        } 
        else if (parent != 0) {
            list.push_back(std::to_string(parent));
        }
        return ClauseResultBuilder::createListResult(LHS, list);
    } else if (isSynonym(RHS)) {
        //if 1 RHS synonym
        std::vector<std::string> list;
        for (int val : qpsFacade.getChildren(stoi(LHS))) {
            list.push_back(std::to_string(val));
        }
        return ClauseResultBuilder::createListResult(RHS, list);
    } else {
        //if 0 synonym
        return ClauseResultBuilder::createBooleanResult(qpsFacade.isParent(stoi(LHS), stoi(RHS)));
    }
    return defaultResult;
}

ClauseResult ParentTClause::evaluate(QPSFacade& qpsFacade, std::string target, std::unordered_map<std::string, std::string> synonymTypes) {
    std::string LHS = getLHS();
    std::string RHS = getRHS();
    std::vector<int> ifs = qpsFacade.getIfStatements();
    std::vector<int> whiles = qpsFacade.getWhileStatements();
    std::vector<int> prints = qpsFacade.getPrintStatements();
    std::vector<int> reads = qpsFacade.getReadStatements();
    std::vector<int> assigns = qpsFacade.getAssignStatements();
    ClauseResult defaultResult = ClauseResultBuilder::createBooleanResult(true);
    if (isSynonym(LHS) && isSynonym(RHS)) {
        //if 2 synonyms
        std::vector<std::vector<std::string>> list;
        std::vector<std::pair<int, int>> tupleList = qpsFacade.getAllParentStar();
        if (synonymTypes[LHS] == "if") {
            for (int i = 0; i < tupleList.size(); i++) {
                if (!(std::find(ifs.begin(), ifs.end(), tupleList[i].first) != ifs.end())) {
                    tupleList.erase(tupleList.begin() + i);
                    i--;
                }
            }
        }
        else if (synonymTypes[LHS] == "while") {
            for (int i = 0; i < tupleList.size(); i++) {
                if (!(std::find(whiles.begin(), whiles.end(), tupleList[i].first) != whiles.end())) {
                    tupleList.erase(tupleList.begin() + i);
                    i--;
                }
            }
        }
        else if (synonymTypes[LHS] == "read") {
            for (int i = 0; i < tupleList.size(); i++) {
                if (!(std::find(reads.begin(), reads.end(), tupleList[i].first) != reads.end())) {
                    tupleList.erase(tupleList.begin() + i);
                    i--;
                }
            }
        }
        else if (synonymTypes[LHS] == "print") {
            for (int i = 0; i < tupleList.size(); i++) {
                if (!(std::find(prints.begin(), prints.end(), tupleList[i].first) != prints.end())) {
                    tupleList.erase(tupleList.begin() + i);
                    i--;
                }
            }
        }
        else if (synonymTypes[LHS] == "assign") {
            for (int i = 0; i < tupleList.size(); i++) {
                if (!(std::find(assigns.begin(), assigns.end(), tupleList[i].first) != assigns.end())) {
                    tupleList.erase(tupleList.begin() + i);
                    i--;
                }
            }
        }
        if (synonymTypes[RHS] == "if") {
            for (int i = 0; i < tupleList.size(); i++) {
                if (!(std::find(ifs.begin(), ifs.end(), tupleList[i].second) != ifs.end())) {
                    tupleList.erase(tupleList.begin() + i);
                    i--;
                }
            }
        }
        else if (synonymTypes[RHS] == "while") {
            for (int i = 0; i < tupleList.size(); i++) {
                if (!(std::find(whiles.begin(), whiles.end(), tupleList[i].second) != whiles.end())) {
                    tupleList.erase(tupleList.begin() + i);
                    i--;
                }
            }
        }
        else if (synonymTypes[RHS] == "read") {
            for (int i = 0; i < tupleList.size(); i++) {
                if (!(std::find(reads.begin(), reads.end(), tupleList[i].second) != reads.end())) {
                    tupleList.erase(tupleList.begin() + i);
                    i--;
                }
            }
        }
        else if (synonymTypes[RHS] == "print") {
            for (int i = 0; i < tupleList.size(); i++) {
                if (!(std::find(prints.begin(), prints.end(), tupleList[i].second) != prints.end())) {
                    tupleList.erase(tupleList.begin() + i);
                    i--;
                }
            }
        }
        else if (synonymTypes[RHS] == "assign") {
            for (int i = 0; i < tupleList.size(); i++) {
                if (!(std::find(assigns.begin(), assigns.end(), tupleList[i].second) != assigns.end())) {
                    tupleList.erase(tupleList.begin() + i);
                    i--;
                }
            }
        }
        for (std::pair<int, int> val : tupleList) {
            std::vector<std::string> temp;
            temp.push_back(std::to_string(val.first));
            temp.push_back(std::to_string(val.second));
            list.push_back(temp);
        }
        std::vector<std::string> synList;
        synList.push_back(LHS);
        synList.push_back(RHS);
        return ClauseResultBuilder::createTupleResult(synList, list);
    } else if (isSynonym(LHS)) {
        //if 1 LHS synonym
        std::vector<std::string> list;
        for (int val : qpsFacade.getParentStar(stoi(RHS))) {
            if (synonymTypes[LHS] == "if") {
                if ((std::find(ifs.begin(), ifs.end(), val) != ifs.end())) {
                    list.push_back(std::to_string(val));
                }
            }
            else if (synonymTypes[LHS] == "while") {
                if ((std::find(whiles.begin(), whiles.end(), val) != whiles.end())) {
                    list.push_back(std::to_string(val));
                }
            }
            else if (val != 0) {
                list.push_back(std::to_string(val));
            }
        }
        return ClauseResultBuilder::createListResult(LHS, list);
    } else if (isSynonym(RHS)) {
        //if 1 RHS synonym
        std::vector<std::string> list;
        for (int val : qpsFacade.getChildStar(stoi(LHS))) {
            list.push_back(std::to_string(val));
        }
        return ClauseResultBuilder::createListResult(RHS, list);
    } else {
        //if 0 synonym
        return ClauseResultBuilder::createBooleanResult(qpsFacade.isParentStar(stoi(LHS), stoi(RHS)));
    }
    return defaultResult;
}

ClauseResult UsesClause::evaluate(QPSFacade& qpsFacade, std::string target, std::unordered_map<std::string, std::string> synonymTypes) {
    std::string LHS = getLHS();
    std::string RHS = getRHS();
    std::vector<int> ifs = qpsFacade.getIfStatements();
    std::vector<int> whiles = qpsFacade.getWhileStatements();
    ClauseResult defaultResult = ClauseResultBuilder::createBooleanResult(true);
    if (isSynonym(LHS) && isSynonym(RHS)) {
        //if 2 synonyms
        RHS.erase(remove(RHS.begin(), RHS.end(), '"'), RHS.end());
        std::vector<std::vector<std::string>> list;
        std::vector<std::pair<int, std::string>> tupleList = qpsFacade.getUses();
        if (synonymTypes[LHS] == "if") {
            for (int i = 0; i < tupleList.size(); i++) {
                if (!(std::find(ifs.begin(), ifs.end(), tupleList[i].first) != ifs.end())) {
                    tupleList.erase(tupleList.begin() + i);
                    i--;
                }
            }
        }
        else if (synonymTypes[LHS] == "while") {
            for (int i = 0; i < tupleList.size(); i++) {
                if (!(std::find(whiles.begin(), whiles.end(), tupleList[i].first) != whiles.end())) {
                    tupleList.erase(tupleList.begin() + i);
                    i--;
                }
            }
        }
        for (std::pair<int, std::string> val : tupleList) {
            std::vector<std::string> temp;
            temp.push_back(std::to_string(val.first));
            temp.push_back(val.second);
            list.push_back(temp);
        }
        std::vector<std::string> synList;
        synList.push_back(LHS);
        synList.push_back(RHS);
        return ClauseResultBuilder::createTupleResult(synList, list);
    } else if (isSynonym(LHS)) {
        //if 1 LHS synonym
        RHS.erase(remove(RHS.begin(), RHS.end(), '"'), RHS.end());
        std::vector<std::string> list;
        std::vector<int> listRes = qpsFacade.getUses(RHS);
        if (synonymTypes[LHS] == "if") {
            for (int i = 0; i < listRes.size(); i++) {
                if (!(std::find(ifs.begin(), ifs.end(), listRes[i]) != ifs.end())) {
                    listRes.erase(listRes.begin() + i);
                    i--;
                }
            }
        }
        else if (synonymTypes[LHS] == "while") {
            for (int i = 0; i < listRes.size(); i++) {
                if (!(std::find(whiles.begin(), whiles.end(), listRes[i]) != whiles.end())) {
                    listRes.erase(listRes.begin() + i);
                    i--;
                }
            }
        }
        for (int val : listRes) {
            list.push_back(std::to_string(val));
        }
        return ClauseResultBuilder::createListResult(LHS, list);
    } else if (isSynonym(RHS)) {
        //if 1 RHS synonym
        std::vector<std::string> list = qpsFacade.getUses(stoi(LHS));
        RHS.erase(remove(RHS.begin(), RHS.end(), '"'), RHS.end());
        return ClauseResultBuilder::createListResult(RHS, list);
    } else {
        //if 0 synonym
        RHS.erase(remove(RHS.begin(), RHS.end(), '"'), RHS.end());
        return ClauseResultBuilder::createBooleanResult(qpsFacade.isUses(stoi(LHS), RHS));
    }
    return defaultResult;
}

ClauseResult ModifiesClause::evaluate(QPSFacade& qpsFacade, std::string target, std::unordered_map<std::string, std::string> synonymTypes) {
    std::string LHS = getLHS();
    std::string RHS = getRHS();
    std::vector<int> ifs = qpsFacade.getIfStatements();
    std::vector<int> whiles = qpsFacade.getWhileStatements();
    ClauseResult defaultResult = ClauseResultBuilder::createBooleanResult(true);
    if (isSynonym(LHS) && isSynonym(RHS)) {
        //if 2 synonyms
        RHS.erase(remove(RHS.begin(), RHS.end(), '"'), RHS.end());
        std::vector<std::vector<std::string>> list;
        std::vector<std::pair<int, std::string>> tupleList = qpsFacade.getModifies();
        if (synonymTypes[LHS] == "if") {
            for (int i = 0; i < tupleList.size(); i++) {
                if (!(std::find(ifs.begin(), ifs.end(), tupleList[i].first) != ifs.end())) {
                    tupleList.erase(tupleList.begin() + i);
                    i--;
                }
            }
        }
        else if (synonymTypes[LHS] == "while") {
            for (int i = 0; i < tupleList.size(); i++) {
                if (!(std::find(whiles.begin(), whiles.end(), tupleList[i].first) != whiles.end())) {
                    tupleList.erase(tupleList.begin() + i);
                    i--;
                }
            }
        }
        for (std::pair<int, std::string> val : tupleList) {
            std::vector<std::string> temp;
            temp.push_back(std::to_string(val.first));
            temp.push_back(val.second);
            list.push_back(temp);
        }
        std::vector<std::string> synList;
        synList.push_back(LHS);
        synList.push_back(RHS);
        return ClauseResultBuilder::createTupleResult(synList, list);
    } else if (isSynonym(LHS)) {
        //if 1 LHS synonym
        RHS.erase(remove(RHS.begin(), RHS.end(), '"'), RHS.end());
        std::vector<std::string> list;
        std::vector<int> listRes = qpsFacade.getModifies(RHS);
        if (synonymTypes[LHS] == "if") {
            for (int i = 0; i < listRes.size(); i++) {
                if (!(std::find(ifs.begin(), ifs.end(), listRes[i]) != ifs.end())) {
                    listRes.erase(listRes.begin() + i);
                    i--;
                }
            }
        }
        else if (synonymTypes[LHS] == "while") {
            for (int i = 0; i < listRes.size(); i++) {
                if (!(std::find(whiles.begin(), whiles.end(), listRes[i]) != whiles.end())) {
                    listRes.erase(listRes.begin() + i);
                    i--;
                }
            }
        }
        for (int val : listRes) {
            list.push_back(std::to_string(val));
        }
        return ClauseResultBuilder::createListResult(LHS, list);
    } else if (isSynonym(RHS)) {
        //if 1 RHS synonym
        RHS.erase(remove(RHS.begin(), RHS.end(), '"'), RHS.end());
        std::vector<std::string> list = qpsFacade.getModifies(stoi(LHS));
        return ClauseResultBuilder::createListResult(RHS, list);
    } else {
        //if 0 synonym
        RHS.erase(remove(RHS.begin(), RHS.end(), '"'), RHS.end());
        return ClauseResultBuilder::createBooleanResult(qpsFacade.isModifies(stoi(LHS), RHS));
    }
    return defaultResult;
}

std::vector<std::string> PatternClause::evaluateSpecial(QPSFacade& qpsFacade, std::string target, std::unordered_map<std::string, std::string> synonymTypes) { //TODO abstract out
    std::string LHS = getLHS();
    std::string RHS = getRHS();
    std::string assignSynonym = getAssignSynonym();
    ClauseResult defaultResult = ClauseResultBuilder::createBooleanResult(true);
    
    //(_, _) LHS wildcard && RHS wildcard 
    if (isWildcard(LHS) && isWildcard(RHS)) {
        std::vector<std::string> list = intArrToStringArr(qpsFacade.getStmtRefBothWild());
        return list;
    }
    //(_, "x") LHS wildcard && RHS constant TODO
    //else if (isWildcard(LHS) && !isSynonym(RHS)) {
    //    std::vector<std::string> list = intArrToStringArr(qpsFacade.getStmtRefRhs(RHS));
    //}
    //(_, _"x"_) LHS wildcard && RHS substring wrong
    else if (isWildcard(LHS) && isSubstring(RHS)) {
        RHS.erase(remove(RHS.begin(), RHS.end(), '_'), RHS.end());
        RHS.erase(remove(RHS.begin(), RHS.end(), '"'), RHS.end());
        std::vector<std::string> list = intArrToStringArr(qpsFacade.getStmtRefRhs(RHS));
        return list;
    }
    //(s, _) // LHS syn && RHS wildcard
    else if (isSynonym(LHS) && isWildcard(RHS)) {
        std::vector<std::string> vars = qpsFacade.getVariables();
        std::cout << vars.size() << std::endl;
        std::vector<int> intList;
        std::vector<std::string> list;
        for (std::string var : vars) {
            std::vector<int> temp = qpsFacade.getStmtRefLhs(var);
            intList.insert(intList.end(), temp.begin(), temp.end());
        }
        sort(intList.begin(), intList.end());
        intList.erase(unique(intList.begin(), intList.end()), intList.end());
        list = intArrToStringArr(intList);
        return list;
        //std::vector<std::string> list = intArrToStringArr(qpsFacade.getStmtRefLhs(LHS));
        //return ClauseResultBuilder::createListResult(LHS, list);
    }
    //(s, "x") // LHS syn && RHS constant 
    //else if (isSynonym(LHS) && !isSynonym(RHS)) {
    //    std::vector<std::string> list = intArrToStringArr(qpsFacade.getStmtRefLhsVarRhsPartial(LHS, RHS));
    //    return ClauseResultBuilder::createListResult(LHS, list);
    //}
    // //(s, _"x"_) LHS syn && RHS substring 
    else if (isSynonym(LHS) && isSubstring(RHS)) {
        RHS.erase(remove(RHS.begin(), RHS.end(), '_'), RHS.end());
        RHS.erase(remove(RHS.begin(), RHS.end(), '"'), RHS.end());
        std::cout << RHS << std::endl;
        std::vector<std::string> vars = qpsFacade.getVariables();
        std::cout <<  vars.size() << std::endl;
        std::vector<int> intList;
        std::vector<std::string> list;
        for (std::string var : vars) {
            std::vector<int> temp = qpsFacade.getStmtRefLhsVarRhsPartial(var, RHS);
            intList.insert(intList.end(), temp.begin(), temp.end());
        }
        sort(intList.begin(), intList.end());
        intList.erase(unique(intList.begin(), intList.end()), intList.end());
        list = intArrToStringArr(intList);
        return list;
    }
    //("x", _) LHS var && RHS wildcard 
    else if (!isSynonym(LHS) && isWildcard(RHS)) {
        LHS.erase(remove(LHS.begin(), LHS.end(), '"'), LHS.end());
        std::vector<std::string> list = intArrToStringArr(qpsFacade.getStmtRefLhs(LHS));
        return list;
    }
    //("x", "x") TODO
    //("x", (_"x"_) LHS var && RHS substring
    else if (!isSynonym(LHS) && isSubstring(RHS)) {
        LHS.erase(remove(LHS.begin(), LHS.end(), '"'), LHS.end());
        RHS.erase(remove(RHS.begin(), RHS.end(), '_'), RHS.end());
        RHS.erase(remove(RHS.begin(), RHS.end(), '"'), RHS.end());
        std::vector<std::string> list = intArrToStringArr(qpsFacade.getStmtRefLhsVarRhsPartial(LHS, RHS));
        return list;
    }
    return {};
}

ClauseResult PatternClause::evaluate(QPSFacade& qpsFacade, std::string target, std::unordered_map<std::string, std::string> synonymTypes) {
    return ClauseResultBuilder::createBooleanResult(true);
}

std::vector<std::string> UsesClause::evaluateSpecial(QPSFacade& qpsFacade, std::string target, std::unordered_map<std::string, std::string> synonymTypes) {
    return {};
}
std::vector<std::string> ModifiesClause::evaluateSpecial(QPSFacade& qpsFacade, std::string target, std::unordered_map<std::string, std::string> synonymTypes) {
    return {};
}
std::vector<std::string> ParentClause::evaluateSpecial(QPSFacade& qpsFacade, std::string target, std::unordered_map<std::string, std::string> synonymTypes) {
    return {};
}
std::vector<std::string> ParentTClause::evaluateSpecial(QPSFacade& qpsFacade, std::string target, std::unordered_map<std::string, std::string> synonymTypes) {
    return {};
}
std::vector<std::string> FollowsClause::evaluateSpecial(QPSFacade& qpsFacade, std::string target, std::unordered_map<std::string, std::string> synonymTypes) {
    return {};
}
std::vector<std::string> FollowsTClause::evaluateSpecial(QPSFacade& qpsFacade, std::string target, std::unordered_map<std::string, std::string> synonymTypes) {
    return {};
}

/*bool operator==(const Relation& other) const {
    return LHS == other.LHS && RHS == other.RHS;
}*/
#include "pkb/RelationshipManager.h"
#include<algorithm>

#include "catch.hpp"


TEST_CASE("addFollows and getFollows working Test") {
	RelationshipManager rm;
	rm.addFollows(1, 2);
	rm.addFollows(2, 3);
	rm.addFollows(3, 4);
	rm.addFollows(4, 10);
	rm.addFollows(10, 11);
	rm.addFollows(5, 6);
	rm.addFollows(6, 7);
	rm.addFollows(8, 9);

	int res = rm.getFollows(1);
	int res1 = rm.getFollows(4);
	int res2 = rm.getFollows(8);
	int res3 = rm.getFollows(9);

	CHECK(res == 2);
	CHECK(res1 == 10);
	CHECK(res2 == 9);
	CHECK(res3 != 10);
	CHECK(res3 == 0);
}

TEST_CASE("addFollows and getPreceding working Test") {
	RelationshipManager rm;
	rm.addFollows(1, 2);
	rm.addFollows(2, 3);
	rm.addFollows(3, 4);
	int res1 = rm.getPreceding(4);
	int res2 = rm.getPreceding(1);

	CHECK(res1 == 3);
	CHECK(res2 == 0);
}

TEST_CASE("addFollows, isFollows and isFollows* working Test") {
	RelationshipManager rm;
	rm.addFollows(1, 2);
	rm.addFollows(2, 3);
	rm.addFollows(3, 4);
	rm.addFollows(4, 10);
	rm.addFollows(10, 11);
	rm.addFollows(5, 6);
	rm.addFollows(6, 7);
	rm.addFollows(8, 9);

	CHECK(rm.isFollows(1, 2));
	CHECK(rm.isFollows(2, 3));
	CHECK(rm.isFollows(3, 4));
	CHECK(rm.isFollows(4, 10));
	CHECK(rm.isFollows(10, 11));
	CHECK(!rm.isFollows(5, 7));
	CHECK(!rm.isFollows(7, 9));
	CHECK(rm.isFollowsStar(1, 4));
	CHECK(rm.isFollowsStar(1, 11));
	CHECK(rm.isFollowsStar(3, 10));
	CHECK(rm.isFollowsStar(5, 7));
	CHECK(!rm.isFollowsStar(4, 6));
	CHECK(!rm.isFollowsStar(5, 9));
	CHECK(!rm.isFollowsStar(2, 7));
}

TEST_CASE("addFollows and getFollowsStarForward Test") {
	RelationshipManager rm;
	rm.addFollows(1, 2);
	rm.addFollows(2, 3);
	rm.addFollows(3, 4);
	rm.addFollows(4, 10);
	rm.addFollows(10, 11);
	rm.addFollows(5, 6);
	rm.addFollows(7, 8);
	rm.addFollows(6, 9);

	std::vector<int> res1 = rm.getFollowsStarForward(1);
	std::vector<int> exp1 = { 2,3,4,10,11 };
	CHECK(res1 == exp1);

	std::vector<int> res2 = rm.getFollowsStarForward(5);
	std::vector<int> exp2 = { 6,9 };
	CHECK(res2 == exp2);
	
	std::vector<int> res3 = rm.getFollowsStarForward(7);
	std::vector<int> exp3 = { 8 };
	CHECK(res3 == exp3);

	std::vector<int> res4 = rm.getFollowsStarForward(4);
	std::vector<int> exp4 = { 10,11 };
	CHECK(res4 == exp4);

	std::vector<int> res5 = rm.getFollowsStarForward(11);
	std::vector<int> exp5 = {};
	CHECK(res5 == exp5);
}

TEST_CASE("addFollows and getFollowsStarsBackward Test") {
	RelationshipManager rm;
	rm.addFollows(1, 2);
	rm.addFollows(2, 3);
	rm.addFollows(3, 4);
	rm.addFollows(4, 10);
	rm.addFollows(10, 11);
	rm.addFollows(5, 6);
	rm.addFollows(7, 8);
	rm.addFollows(6, 9);

	std::vector<int> res1 = rm.getFollowsStarBackward(11);
	std::sort(res1.begin(), res1.end());
	std::vector<int> exp1 = { 1,2,3,4,10 };
	CHECK(res1 == exp1);

	std::vector<int> res2 = rm.getFollowsStarBackward(8);
	std::vector<int> exp2 = { 7 };
	CHECK(res2 == exp2);

	std::vector<int> res3 = rm.getFollowsStarBackward(7);
	std::vector<int> exp3 = { };
	CHECK(res3 == exp3);

	std::vector<int> res4 = rm.getFollowsStarBackward(6);
	std::vector<int> exp4 = { 5 };
	CHECK(res4 == exp4);

	std::vector<int> res5 = rm.getFollowsStarBackward(4);
	std::sort(res5.begin(), res5.end());
	std::vector<int> exp5 = { 1,2,3 };
	CHECK(res5 == exp5);
}

TEST_CASE("addFollows and getAllFollowsStars Test") {
	RelationshipManager rm;
	rm.addFollows(1, 2);
	rm.addFollows(2, 3);
	rm.addFollows(3, 9);
	rm.addFollows(9, 10);
	rm.addFollows(4, 5);
	rm.addFollows(6, 7);
	rm.addFollows(5, 8);

	std::vector<std::pair<int, int>> exp;
	exp.push_back(std::pair(1, 2));
	exp.push_back(std::pair(1, 3));
	exp.push_back(std::pair(1, 9));
	exp.push_back(std::pair(1, 10));
	exp.push_back(std::pair(2, 3));
	exp.push_back(std::pair(2, 9));
	exp.push_back(std::pair(2, 10));
	exp.push_back(std::pair(3, 9));
	exp.push_back(std::pair(3, 10));
	exp.push_back(std::pair(9, 10));
	exp.push_back(std::pair(4, 5));
	exp.push_back(std::pair(4, 8));
	exp.push_back(std::pair(5, 8));
	exp.push_back(std::pair(6, 7));
	std::vector<std::pair<int, int>> res = rm.getAllFollowsStar();
	CHECK(exp.size() == res.size());

	std::sort(exp.begin(), exp.end());
	std::sort(res.begin(), res.end());
	CHECK(res == exp);
}

TEST_CASE("addParentChild and getChildren working Test") {
	RelationshipManager rm;
	rm.addParentChild(2, 3);
	rm.addParentChild(2, 4);
	rm.addParentChild(4, 5);
	rm.addParentChild(4, 6);

	std::vector<int> res = rm.getChildren(2);
	std::vector<int> exp{ 3,4 };
	CHECK(res == exp);

	std::vector<int> res1 = rm.getChildren(4);
	std::vector<int> exp1{ 5,6 };
	CHECK(res1 == exp1);

	std::vector<int> res2 = rm.getChildren(3);
	std::vector<int> exp2{ };
	CHECK(res2 == exp2);
}

TEST_CASE("getParent and isParent working Test") {
	RelationshipManager rm;
	rm.addParentChild(2, 3);
	rm.addParentChild(2, 4);
	rm.addParentChild(4, 5);
	rm.addParentChild(4, 6);
	int res = rm.getParent(5);

	CHECK(res == 4);
	CHECK(rm.isParent(2, 4));
	CHECK(rm.isParent(4, 5));
	CHECK(!rm.isParent(2, 5));
	CHECK(!rm.isParent(3, 6));
}

TEST_CASE("isParentStar working Test") {
	RelationshipManager rm;
	rm.addParentChild(2, 3);
	rm.addParentChild(2, 4);
	rm.addParentChild(4, 5);
	rm.addParentChild(4, 6);
	rm.addParentChild(6, 7);
	rm.addParentChild(6, 8);
	rm.addParentChild(6, 9);
	rm.addParentChild(4, 10);
	rm.addParentChild(2, 11);

	CHECK(rm.isParentStar(2, 3));
	CHECK(rm.isParentStar(2, 6));
	CHECK(rm.isParentStar(2, 9));
	CHECK(rm.isParentStar(2, 10));
	CHECK(rm.isParentStar(2, 11));
	CHECK(rm.isParentStar(4, 8));
	CHECK(rm.isParentStar(4, 5));
	CHECK(rm.isParentStar(4, 10));
	CHECK(rm.isParentStar(6, 9));
	CHECK(!rm.isParentStar(6, 10));
	CHECK(!rm.isParentStar(6, 11));
	CHECK(!rm.isParentStar(4, 11));
	CHECK(!rm.isParentStar(1, 8));
	CHECK(!rm.isParentStar(1, 12));
	CHECK(!rm.isParentStar(2, 13));
}

TEST_CASE("getParentStar working Test") {
	RelationshipManager rm;
	rm.addParentChild(2, 3);
	rm.addParentChild(2, 4);
	rm.addParentChild(4, 5);
	rm.addParentChild(4, 6);
	rm.addParentChild(6, 7);
	rm.addParentChild(6, 8);
	rm.addParentChild(6, 9);
	rm.addParentChild(4, 10);
	rm.addParentChild(2, 11);

	std::vector<int> res = rm.getParentStar(9);
	std::vector<int> exp{ 6,4,2 };
	CHECK(res == exp);

	std::vector<int> res1 = rm.getParentStar(10);
	std::vector<int> exp1{ 4,2 };
	CHECK(res1 == exp1);

	std::vector<int> res2 = rm.getParentStar(6);
	std::vector<int> exp2{ 4,2 };
	CHECK(res2 == exp2);

	std::vector<int> res3 = rm.getParentStar(11);
	std::vector<int> exp3{ 2 };
	CHECK(res3 == exp3);

	std::vector<int> res4 = rm.getParentStar(2);
	std::vector<int> exp4{ };
	CHECK(res4 == exp4);
}

TEST_CASE("getChildStar working Test") {
	RelationshipManager rm;
	rm.addParentChild(2, 3);
	rm.addParentChild(2, 4);
	rm.addParentChild(4, 5);
	rm.addParentChild(4, 6);
	rm.addParentChild(6, 7);
	rm.addParentChild(6, 8);
	rm.addParentChild(6, 9);
	rm.addParentChild(4, 10);
	rm.addParentChild(2, 11);

	std::vector<int> res = rm.getChildStar(2);
	std::vector<int> exp{ 3,4,11,5,6,10,7,8,9 };
	CHECK(res == exp);

	std::vector<int> res1 = rm.getChildStar(4);
	std::vector<int> exp1{ 5,6,10,7,8,9 };
	CHECK(res1 == exp1);

	std::vector<int> res2 = rm.getChildStar(6);
	std::vector<int> exp2{ 7,8,9 };
	CHECK(res2 == exp2);

	std::vector<int> res3 = rm.getChildStar(5);
	std::vector<int> exp3{ };
	CHECK(res3 == exp3);

	std::vector<int> res4 = rm.getChildStar(11);
	std::vector<int> exp4{ };
	CHECK(res4 == exp4);

	std::vector<int> res5 = rm.getChildStar(3);
	std::vector<int> exp5{ };
	CHECK(res5 == exp5);
}

TEST_CASE("getAllParentStar working Test") {
	RelationshipManager rm;
	rm.addParentChild(2, 3);
	rm.addParentChild(2, 4);
	rm.addParentChild(4, 5);
	rm.addParentChild(4, 6);
	rm.addParentChild(6, 7);
	rm.addParentChild(6, 8);
	rm.addParentChild(6, 9);
	rm.addParentChild(4, 10);
	rm.addParentChild(2, 11);

	std::vector<std::pair<int, int>> exp;
	exp.push_back(std::pair(2, 3));
	exp.push_back(std::pair(2, 4));
	exp.push_back(std::pair(2, 11));
	exp.push_back(std::pair(2, 5));
	exp.push_back(std::pair(2, 6));
	exp.push_back(std::pair(2, 10));
	exp.push_back(std::pair(2, 7));
	exp.push_back(std::pair(2, 8));
	exp.push_back(std::pair(2, 9));
	exp.push_back(std::pair(4, 5));
	exp.push_back(std::pair(4, 6));
	exp.push_back(std::pair(4, 10));
	exp.push_back(std::pair(4, 7));
	exp.push_back(std::pair(4, 8));
	exp.push_back(std::pair(4, 9));
	exp.push_back(std::pair(6, 7));
	exp.push_back(std::pair(6, 8));
	exp.push_back(std::pair(6, 9));
	std::vector<std::pair<int, int>> res = rm.getAllParentStar();
	CHECK(res == exp);
}

TEST_CASE("addUses and isUses working Test") {
	RelationshipManager rm;
	rm.addUses(1, "x");
	rm.addUses(1, "a1b2c3");
	rm.addUses(1, "y");
	rm.addUses(2, "x");
	rm.addUses(2, "var");

	CHECK(rm.isUses(1, "a1b2c3"));
	CHECK(rm.isUses(1, "x"));
	CHECK(rm.isUses(2, "x"));
	CHECK(rm.isUses(2, "var"));
	CHECK(!rm.isUses(1, "var"));
	CHECK(!rm.isUses(1, "f"));
	CHECK(!rm.isUses(3, "a1b2c3"));
	CHECK(!rm.isUses(3, "rando"));
}

TEST_CASE("getUses(int) working Test") {
	RelationshipManager rm;
	rm.addUses(1, "x");
	rm.addUses(1, "a1b2c3");
	rm.addUses(1, "y");
	rm.addUses(2, "x");
	rm.addUses(2, "var");
	rm.addUses(3, "x");
	rm.addUses(3, "y");
	rm.addUses(3, "3");

	std::vector<std::string> res = rm.getUses(1);
	std::vector<std::string> exp{ "x","a1b2c3","y" };
	CHECK(res == exp);

	std::vector<std::string> res1 = rm.getUses(2);
	std::vector<std::string> exp1{ "x","var" };
	CHECK(res1 == exp1);

	std::vector<std::string> res2 = rm.getUses(3);
	std::vector<std::string> exp2{ "x","y","3"};
	CHECK(res2 == exp2);

	std::vector<std::string> res3 = rm.getUses(0);
	std::vector<std::string> exp3{ };
	CHECK(res3 == exp3);

	std::vector<std::string> res4 = rm.getUses(5);
	std::vector<std::string> exp4{ };
	CHECK(res4 == exp4);
}

TEST_CASE("getUses(string) working Test") {
	RelationshipManager rm;
	rm.addUses(1, "x");
	rm.addUses(1, "a1b2c3");
	rm.addUses(1, "y");
	rm.addUses(2, "x");
	rm.addUses(2, "var");
	rm.addUses(2, "2");
	rm.addUses(3, "x");
	rm.addUses(3, "y");
	rm.addUses(3, "3");

	std::vector<int> res = rm.getUses("x");
	std::vector<int> exp{ 1,2,3 };
	CHECK(res == exp);

	std::vector<int> res1 = rm.getUses("a1b2c3");
	std::vector<int> exp1{ 1 };
	CHECK(res1 == exp1);

	std::vector<int> res2 = rm.getUses("y");
	std::vector<int> exp2{ 1,3 };
	CHECK(res2 == exp2);

	std::vector<int> res3 = rm.getUses("var");
	std::vector<int> exp3{ 2 };
	CHECK(res3 == exp3);

	std::vector<int> res4 = rm.getUses("3");
	std::vector<int> exp4{ 3 };
	CHECK(res4 == exp4);

	std::vector<int> res5 = rm.getUses("2");
	std::vector<int> exp5{ 2 };
	CHECK(res5 == exp5);
}

TEST_CASE("getUses() working Test") {
	RelationshipManager rm;
	rm.addUses(1, "x");
	rm.addUses(1, "a1b2c3");
	rm.addUses(1, "y");
	rm.addUses(2, "x");
	rm.addUses(2, "var");
	rm.addUses(2, "2");
	rm.addUses(3, "x");
	rm.addUses(3, "y");
	rm.addUses(3, "3");

	std::vector<std::pair<int, std::string>> exp;
	exp.push_back(std::pair(1, "x"));
	exp.push_back(std::pair(1, "a1b2c3"));
	exp.push_back(std::pair(1, "y"));
	exp.push_back(std::pair(2, "x"));
	exp.push_back(std::pair(2, "var"));
	exp.push_back(std::pair(2, "2"));
	exp.push_back(std::pair(3, "x"));
	exp.push_back(std::pair(3, "y"));
	exp.push_back(std::pair(3, "3"));
	auto res = rm.getUses();
	CHECK(res == exp);
}

TEST_CASE("addModifies and isModifies working Test") {
	RelationshipManager rm;
	rm.addModifies(1, "x");
	rm.addModifies(2, "read");
	rm.addModifies(3, "print");
	rm.addModifies(4, "xyz123");
	rm.addModifies(5, "aBC");

	CHECK(rm.isModifies(1, "x"));
	CHECK(rm.isModifies(2, "read"));
	CHECK(rm.isModifies(3, "print"));
	CHECK(rm.isModifies(4, "xyz123"));
	CHECK(!rm.isModifies(5, "abc"));
	CHECK(!rm.isModifies(1, "print"));
	CHECK(!rm.isModifies(0, "x"));
	CHECK(!rm.isModifies(6, "aBC"));
}

TEST_CASE("getModifies(int) working Test") {
	RelationshipManager rm;
	rm.addModifies(1, "x");
	rm.addModifies(2, "read");
	rm.addModifies(3, "print");
	rm.addModifies(4, "xyz123");
	rm.addModifies(5, "aBC987");

	std::vector<std::string>exp1{ "x" };
	std::vector<std::string>exp2{ "read" };
	std::vector<std::string>exp3{ "print" };
	std::vector<std::string>exp4{ "xyz123" };
	std::vector<std::string>exp5{ "aBC987" };
	std::vector<std::string>exp6{ "abc987" };

	CHECK(rm.getModifies(1) == exp1);
	CHECK(rm.getModifies(2) == exp2);
	CHECK(rm.getModifies(3) == exp3);
	CHECK(rm.getModifies(4) == exp4);
	CHECK(rm.getModifies(5) == exp5);
	CHECK(rm.getModifies(5) != exp6);
}

TEST_CASE("getModifies(string) working Test") {
	RelationshipManager rm;
	rm.addModifies(1, "x");
	rm.addModifies(2, "read");
	rm.addModifies(3, "print");
	rm.addModifies(4, "xyz123");
	rm.addModifies(5, "aBC987");
	rm.addModifies(6, "aBC987");
	rm.addModifies(7, "x");
	rm.addModifies(8, "aBC987");
	rm.addModifies(9, "x");

	CHECK(rm.getModifies("x") == std::vector<int>{1, 7, 9});
	CHECK(rm.getModifies("aBC987") == std::vector<int>{5, 6, 8});
	CHECK(rm.getModifies("read") == std::vector<int>{2});
	CHECK(rm.getModifies("xyz123") == std::vector<int>{4});
	CHECK(rm.getModifies("null") == std::vector<int>{});
}

TEST_CASE("getModifies() working Test") {
	RelationshipManager rm;
	rm.addModifies(1, "x");
	rm.addModifies(2, "read");
	rm.addModifies(3, "print");
	rm.addModifies(4, "xyz123");
	rm.addModifies(5, "aBC987");
	rm.addModifies(6, "aBC987");
	rm.addModifies(7, "x");
	rm.addModifies(8, "aBC987");
	rm.addModifies(9, "x");

	std::vector<std::pair<int, std::string>> exp;
	exp.push_back(std::pair(1, "x"));
	exp.push_back(std::pair(2, "read"));
	exp.push_back(std::pair(3, "print"));
	exp.push_back(std::pair(4, "xyz123"));
	exp.push_back(std::pair(5, "aBC987"));
	exp.push_back(std::pair(6, "aBC987"));
	exp.push_back(std::pair(7, "x"));
	exp.push_back(std::pair(8, "aBC987"));
	exp.push_back(std::pair(9, "x"));
	CHECK(rm.getModifies() == exp);
}
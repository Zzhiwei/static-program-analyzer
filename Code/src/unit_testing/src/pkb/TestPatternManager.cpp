#include "pkb/PatternManager.h"

#include "catch.hpp"
#include <sstream>

TEST_CASE("addPattern and getStmtRefLhs test") {
	PatternManager pm;


	Variable lhs1 = std::make_shared<VariableNode>(nullptr, 1, "varX");
	Variable rhs11 = std::make_shared<VariableNode>(nullptr, 1, "varY");
	Variable rhs12 = std::make_shared<VariableNode>(nullptr, 1, "123");
	Variable rhs13 = std::make_shared<VariableNode>(nullptr, 1, "y123");
	BinaryOp rhsOp11 = std::make_shared<BinaryOpNode>(nullptr, 1, rhs11, rhs12);
	BinaryOp rhsOp12 = std::make_shared<BinaryOpNode>(nullptr, 1, rhs12, rhs13);
//	rhsOp12->setBinaryOp("+", rhs12, rhs13);
//	rhsOp11->setBinaryOp("+", rhs11, rhsOp12);
	Assign root1 = std::make_shared<AssignNode>(nullptr, 1);
	root1->setAssign(lhs1, rhsOp11);

	pm.addPattern(1, root1);

	Variable lhs2 = std::make_shared<VariableNode>(nullptr, 2, "abcd");
	Variable rhs21 = std::make_shared<VariableNode>(nullptr, 2, "varX");
	Assign root2 = std::make_shared<AssignNode>(nullptr, 2);
	root2->setAssign(lhs2, rhs21);

	pm.addPattern(2, root2);

	Variable lhs3 = std::make_shared<VariableNode>(nullptr, 3, "varX");
	Variable rhs31 = std::make_shared<VariableNode>(nullptr, 3, "defy");
	Variable rhs32 = std::make_shared<VariableNode>(nullptr, 3, "123");
	Variable rhs33 = std::make_shared<VariableNode>(nullptr, 3, "abcd");
	BinaryOp rhsOp31 = std::make_shared<BinaryOpNode>(nullptr, 3, rhs32, rhs33);
	BinaryOp rhsOp32 = std::make_shared<BinaryOpNode>(nullptr, 3, rhs31, rhs32);
//	rhsOp32->setBinaryOp("*", rhs31, rhs32);
//	rhsOp31->setBinaryOp("+", rhsOp32, rhs33);
	Assign root3 = std::make_shared<AssignNode>(nullptr, 3);
	root3->setAssign(lhs3, rhsOp31);

	pm.addPattern(3, root3);

	std::string S1 = "varX = varY + 123 + y123";

	std::string S2 = "abcd = varX";

	std::string S3 = "varX = defy * 123 + abcd";

	auto res = pm.getStmtRefLhs("varX");
	std::vector<int>exp{ 1,3 };

	auto res1 = pm.getStmtRefLhs("abcd");
	std::vector<int>exp1{ 2 };

	auto res2 = pm.getStmtRefLhs("123");
	std::vector<int>exp2{  };

	CHECK(res == exp);
	CHECK(res1 == exp1);
	CHECK(res2 == exp2);
}

TEST_CASE("addPattern and getStmtRefRhs test") {
	PatternManager pm;

	Variable lhs1 = std::make_shared<VariableNode>(nullptr, 1, "varX");
	Variable rhs11 = std::make_shared<VariableNode>(nullptr, 1, "varY");
	Variable rhs12 = std::make_shared<VariableNode>(nullptr, 1, "123");
	Variable rhs13 = std::make_shared<VariableNode>(nullptr, 1, "y123");
	BinaryOp rhsOp12 = std::make_shared<BinaryOpNode>(nullptr, 1, rhs12, rhs13);
	BinaryOp rhsOp11 = std::make_shared<BinaryOpNode>(nullptr, 1, rhs11, rhsOp12);
//	rhsOp12->setBinaryOp("+", rhs12, rhs13);
//	rhsOp11->setBinaryOp("+", rhs11, rhsOp12);
	Assign root1 = std::make_shared<AssignNode>(nullptr, 1);
	root1->setAssign(lhs1, rhsOp11);

	pm.addPattern(1, root1);

	Variable lhs2 = std::make_shared<VariableNode>(nullptr, 2, "abcd");
	Variable rhs21 = std::make_shared<VariableNode>(nullptr, 2, "varX");
	Assign root2 = std::make_shared<AssignNode>(nullptr, 2);
	root2->setAssign(lhs2, rhs21);

	pm.addPattern(2, root2);

	Variable lhs3 = std::make_shared<VariableNode>(nullptr, 3, "varX");
	Variable rhs31 = std::make_shared<VariableNode>(nullptr, 3, "defy");
	Variable rhs32 = std::make_shared<VariableNode>(nullptr, 3, "123");
	Variable rhs33 = std::make_shared<VariableNode>(nullptr, 3, "abcd");
	
	BinaryOp rhsOp32 = std::make_shared<BinaryOpNode>(nullptr, 3, rhs31, rhs32);
	BinaryOp rhsOp31 = std::make_shared<BinaryOpNode>(nullptr, 3, rhsOp32, rhs33);
//	rhsOp32->setBinaryOp("*", rhs31, rhs32);
//	rhsOp31->setBinaryOp("+", rhsOp32, rhs33);
	Assign root3 = std::make_shared<AssignNode>(nullptr, 3);
	root3->setAssign(lhs3, rhsOp31);

	pm.addPattern(3, root3);

	std::string S1 = "varX = varY + 123 + y123";

	std::string S2 = "abcd = varX";

	std::string S3 = "varX = defy * 123 + abcd";
	
	auto res = pm.getStmtRefRhs("123");
	std::vector<int>exp{ 1,3 };

	auto res1 = pm.getStmtRefRhs("y123");
	std::vector<int>exp1{ 1 };

	auto res2 = pm.getStmtRefRhs("abcd");
	std::vector<int>exp2{ 3 };

	auto res3 = pm.getStmtRefRhs("var");
	std::vector<int>exp3{  };

	auto res4 = pm.getStmtRefRhs("varX");
	std::vector<int>exp4{ 2 };

	CHECK(res == exp);
	CHECK(res1 == exp1);
	CHECK(res2 == exp2);
	CHECK(res3 == exp3);
	CHECK(res4 == exp4);
}

TEST_CASE("addPattern, getStmtRefLhsVarRhsPartial test") {
	PatternManager pm;


	Variable lhs1 = std::make_shared<VariableNode>(nullptr, 1, "varX");
	Variable rhs11 = std::make_shared<VariableNode>(nullptr, 1, "varY");
	Variable rhs12 = std::make_shared<VariableNode>(nullptr, 1, "123");
	Variable rhs13 = std::make_shared<VariableNode>(nullptr, 1, "y123");
	BinaryOp rhsOp11 = std::make_shared<BinaryOpNode>(nullptr, 1, rhs11, rhs12);
	BinaryOp rhsOp12 = std::make_shared<BinaryOpNode>(nullptr, 1, rhs12, rhs13);
	//	rhsOp12->setBinaryOp("+", rhs12, rhs13);
	//	rhsOp11->setBinaryOp("+", rhs11, rhsOp12);
	Assign root1 = std::make_shared<AssignNode>(nullptr, 1);
	root1->setAssign(lhs1, rhsOp11);

	pm.addPattern(1, root1);

	Variable lhs2 = std::make_shared<VariableNode>(nullptr, 2, "abcd");
	Variable rhs21 = std::make_shared<VariableNode>(nullptr, 2, "varX");
	Assign root2 = std::make_shared<AssignNode>(nullptr, 2);
	root2->setAssign(lhs2, rhs21);

	pm.addPattern(2, root2);

	Variable lhs3 = std::make_shared<VariableNode>(nullptr, 3, "varX");
	Variable rhs31 = std::make_shared<VariableNode>(nullptr, 3, "defy");
	Variable rhs32 = std::make_shared<VariableNode>(nullptr, 3, "123");
	Variable rhs33 = std::make_shared<VariableNode>(nullptr, 3, "abcd");
	BinaryOp rhsOp31 = std::make_shared<BinaryOpNode>(nullptr, 3, rhs32, rhs33);
	BinaryOp rhsOp32 = std::make_shared<BinaryOpNode>(nullptr, 3, rhs31, rhs32);
	//	rhsOp32->setBinaryOp("*", rhs31, rhs32);
	//	rhsOp31->setBinaryOp("+", rhsOp32, rhs33);
	Assign root3 = std::make_shared<AssignNode>(nullptr, 3);
	root3->setAssign(lhs3, rhsOp31);

	pm.addPattern(3, root3);

	std::string S1 = "varX = varY + 123 + y123";

	std::string S2 = "abcd = varX";

	std::string S3 = "varX = defy * 123 + abcd";

	std::vector<int> res = pm.getStmtRefLhsVarRhsPartial("varX", "varY");
	std::vector<int> exp{ 1 };

	std::vector<int> res1 = pm.getStmtRefLhsVarRhsPartial("varX", "vary");
	std::vector<int> exp1{  };

	std::vector<int> res2 = pm.getStmtRefLhsVarRhsPartial("varx", "123");
	std::vector<int> exp2{  };

	std::vector<int> res3 = pm.getStmtRefLhsVarRhsPartial("defy", "defy");
	std::vector<int> exp3{  };

	std::vector<int> res4 = pm.getStmtRefLhsVarRhsPartial("varX", "123");
	std::vector<int> exp4{ 1,3 };

	std::vector<int> res5 = pm.getStmtRefLhsVarRhsPartial("varX", "defy");
	std::vector<int> exp5{ 2 };

	CHECK(res == exp);
	CHECK(res1 == exp1);
	CHECK(res2 == exp2);
	CHECK(res3 == exp3);
	CHECK(res4 == exp4);

}

TEST_CASE("addPattern and getStmtRefBothWild test") {
	PatternManager pm;


	Variable lhs1 = std::make_shared<VariableNode>(nullptr, 1, "varX");
	Variable rhs11 = std::make_shared<VariableNode>(nullptr, 1, "varY");
	Variable rhs12 = std::make_shared<VariableNode>(nullptr, 1, "123");
	Variable rhs13 = std::make_shared<VariableNode>(nullptr, 1, "y123");
	BinaryOp rhsOp11 = std::make_shared<BinaryOpNode>(nullptr, 1, rhs11, rhs12);
	BinaryOp rhsOp12 = std::make_shared<BinaryOpNode>(nullptr, 1, rhs12, rhs13);
	//	rhsOp12->setBinaryOp("+", rhs12, rhs13);
	//	rhsOp11->setBinaryOp("+", rhs11, rhsOp12);
	Assign root1 = std::make_shared<AssignNode>(nullptr, 1);
	root1->setAssign(lhs1, rhsOp11);

	pm.addPattern(1, root1);

	Variable lhs2 = std::make_shared<VariableNode>(nullptr, 2, "abcd");
	Variable rhs21 = std::make_shared<VariableNode>(nullptr, 2, "varX");
	Assign root2 = std::make_shared<AssignNode>(nullptr, 2);
	root2->setAssign(lhs2, rhs21);

	pm.addPattern(2, root2);

	Variable lhs3 = std::make_shared<VariableNode>(nullptr, 3, "varX");
	Variable rhs31 = std::make_shared<VariableNode>(nullptr, 3, "defy");
	Variable rhs32 = std::make_shared<VariableNode>(nullptr, 3, "123");
	Variable rhs33 = std::make_shared<VariableNode>(nullptr, 3, "abcd");
	BinaryOp rhsOp31 = std::make_shared<BinaryOpNode>(nullptr, 3, rhs32, rhs33);
	BinaryOp rhsOp32 = std::make_shared<BinaryOpNode>(nullptr, 3, rhs31, rhs32);
	//	rhsOp32->setBinaryOp("*", rhs31, rhs32);
	//	rhsOp31->setBinaryOp("+", rhsOp32, rhs33);
	Assign root3 = std::make_shared<AssignNode>(nullptr, 3);
	root3->setAssign(lhs3, rhsOp31);

	pm.addPattern(3, root3);

	std::string S1 = "varX = varY + 123 + y123";

	std::string S2 = "abcd = varX";

	std::string S3 = "varX = defy * 123 + abcd";

	auto res = pm.getStmtRefBothWild();
	std::vector<int>exp{ 1,2,3 };

	CHECK(res == exp);
}
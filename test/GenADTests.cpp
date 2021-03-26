#include <gtest/gtest.h>

#include "../src/auto_diff.h"

TEST(GenADTests, Test1) {
    int x = 5;
    EXPECT_NEAR(x, 5, 0.01);
}

TEST(GenADTests, Test2) {
	
	std::string input = "sqrt(x0)*x1+x2*3";
	map<std::string, double> inits;
	inits["x0"] = 1.2;
	inits["x1"] = 1.3;
	inits["x2"] = 2.0;

	std::vector<Token> token_list = parser(input, inits).token_list;
	AutoDiffRev ad2 = AutoDiffRev(token_list, inits);
	Token result2 = ad2.eval();
	EXPECT_NEAR(result2.num_val, 7.424, 0.0001);
}

TEST(GenADTests, Test3) {
	
	std::string input = "sin(x0)*sqrt(x1)+cos(x2)*3";
	map<std::string, double> inits;
	inits["x0"] = 10.0;
	inits["x1"] = 0.3;
	inits["x2"] = 1.0;

	std::vector<Token> token_list = parser(input, inits).token_list;
	AutoDiffRev ad2 = AutoDiffRev(token_list, inits);
	Token result2 = ad2.eval();
	EXPECT_NEAR(result2.num_val, 1.3229, 0.0001);
}

TEST(GenADTests, Test4) {
	
	std::string input = "sin(x0)*log(x1)+exp(x2)*3";
	map<std::string, double> inits;
	inits["x0"] = 7.0;
	inits["x1"] = 8.3;
	inits["x2"] = -1.0;

	std::vector<Token> token_list = parser(input, inits).token_list;
	AutoDiffRev ad2 = AutoDiffRev(token_list, inits);
	Token result2 = ad2.eval();
	EXPECT_NEAR(result2.num_val,  1.414086, 0.000001);
}


TEST(GenADTests, Test5) {
	
	std::string input = "exp(-x0)+x1-x2";
	map<std::string, double> inits;
	inits["x0"] = -5.2;
	inits["x1"] = 0.3;
	inits["x2"] = 2.0;

	std::vector<Token> token_list = parser(input, inits).token_list;
	AutoDiffRev ad2 = AutoDiffRev(token_list, inits);
	Token result2 = ad2.eval();
	EXPECT_NEAR(result2.num_val,  -0.7, 0.000001);
}

TEST(GenADTests, Test6) {
	
	std::string input = "exp(-x0)*csc(x1)-tan(x2)";
	map<std::string, double> inits;
	inits["x0"] = -3.2;
	inits["x1"] = 10.4;
	inits["x2"] = 1.0;

	std::vector<Token> token_list = parser(input, inits).token_list;
	AutoDiffRev ad2 = AutoDiffRev(token_list, inits);
	Token result2 = ad2.eval();
	EXPECT_NEAR(result2.num_val,  -2.7654, 0.00001);
}
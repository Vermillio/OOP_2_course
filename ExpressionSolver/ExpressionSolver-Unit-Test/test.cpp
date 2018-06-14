#include "pch.h"
#include "../ExpressionSolver/ExpressionSolver.h"


//	Before running test make sure that ExpressionSolver -> Properties -> Project Defaults -> Configuration Type
//	is static library (.lib)

TEST(TestCaseName, MainTest) {

	string expr[] = {
		"5+5",
		"8-3",
		"6*7",
		"36/6",
		"5/0",
		"100/(5-5)",
		"0/100",
		"6.78*2.36",
		"15.5+10*(9-90)/10.5",
		"780/5*(6.31-87.4)",
		"9.56+(0.3*(8.3256+0.893)+1.0/1000)*4.8",
	};

	string ans[] = {
		"10",
		"5",
		"42",
		"6",
		"Can't divide by zero",
		"Can't divide by zero",
		"0",
		"16.0008",
		"-61.642857",
		"-12650.04",
		"22.839584"
	};

	int size = 10;
	ExprSolve::ExprSolverWrapper solver;

	for (int i = 0; i < size; ++i) {
		string k = solver.solve(expr[i]);
		ASSERT_EQ(k, ans[i]);
	}
}
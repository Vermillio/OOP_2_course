#include "ExpressionSolver.h"

using namespace std;
using namespace ExprSolve;

int main(int argc, char ** argv) 
{
	string expr;
	if (argc == 2)
		expr = string(argv[1]);
	else {
		cout << "       ---EXPRESSION SOLVER---       " << endl;
		cout << "Available operators: +, -, *, =, (, )" << endl;
		cout << "Enter expr >>> ";
		cin >> expr;
	}
	ExpressionSolver Solver;
	Operand answer = Solver.solve(expr);
	answer.shrink_to_fit();
	answer.show();
	system("pause");
	return 0;
}
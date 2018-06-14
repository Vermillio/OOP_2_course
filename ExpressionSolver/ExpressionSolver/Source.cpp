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
		cout << "Available operators: '+' '-' '*' '=' '(' ')' '.' (enter ',' to exit)" << endl;
		cout << "Enter expr >>> ";
		cin >> expr;
	}
	ExprSolverWrapper solver;
	string answer = solver.solve(expr);
	answer.shrink_to_fit();
	cout << " = " << answer << endl;
	system("pause");
	return 0;
}
#include "ExpressionSolver.h"

ExprSolve::Compp ExprSolve::ExpressionSolver::parse(string &expr)
{
	int pivot=0;
	int start = 0;
	int end = expr.length();

	int i = start;

	while (i < end) {
		if (expr[i] == ')')
		{
			exception("Extra closing bracket.");
		}
		if (expr[i] == '(')
		{
			int s = i;
			int counter = 1;
			++i;
			while (counter != 0 && i < end) {
				if (expr[i] == '(')
					++counter;
				else if (expr[i] == ')')
					--counter;
				++i;
			}
			if (s == 0 && i == end)
				pivot = -1;
			if (counter > 0)
			{
				exception("Extra opening bracket.");
			}
			--i;
		}
		else if (expr[i] == '+' || expr[i] == '-') {
			pivot = i;
			break;
		}
		else if (expr[i] == '*' || expr[i] == '/') {
			pivot = i;
		}
		else if (!isdigit(expr[i]) && expr[i]!='.') {
			exception("Unresolved symbol.");
		}
		++i;
	}
	if (pivot == -1)
		return Operandp(new Operand(expr.substr(start+1, end - 2)));
	if (pivot == 0)
		return Operandp(new Operand(expr.substr(start, end-start)));

	string l = expr.substr(start, pivot-start);
	string r = expr.substr(pivot + 1, end-pivot);
	Compp *lEvaluated = new Compp(parse(l));
	Compp *rEvaluated = new Compp(parse(r));
	return Operatorp(new Operator(expr[pivot], *lEvaluated, *rEvaluated));
}

ExprSolve::Operand ExprSolve::ExpressionSolver::solve(string &expr)
{
	Compp treeExpr = parse(expr);
	return treeExpr->evaluate();
}

bool ExprSolve::Operator::remove(Compp &t)
{
	if (left == t) {
		left = nullptr;
		return true;
	}
	else if (right == t) {
		right = nullptr;
		return true;
	}
	if (left && left->remove(t))
		return true;
	if (right && right->remove(t))
		return true;
	return false;
}

void ExprSolve::Operator::show()
{
	if (left)
		left->show();
	cout << oper;
	if (right)
		right->show();
}

ExprSolve::Operand ExprSolve::Operator::evaluate()
{
	switch (oper) {
	case '+':
		return add(left->evaluate(), right->evaluate());
	case '-':
		return substract(left->evaluate(), right->evaluate());
	case '*':
		return multiply(left->evaluate(), right->evaluate());
	case '/':
		return divide(left->evaluate(), right->evaluate());
	default:
		exception("Can't resolve expression.");
		return Operand();
	}
}

ExprSolve::Operand ExprSolve::add(Operand x, Operand y)
{
	return Operand(x.toDouble() + y.toDouble());
}

ExprSolve::Operand ExprSolve::substract(Operand x, Operand y)
{
	return Operand(x.toDouble() - y.toDouble());
}

ExprSolve::Operand ExprSolve::multiply(Operand x, Operand y)
{
	return Operand(x.toDouble() * y.toDouble());
}

ExprSolve::Operand ExprSolve::divide(Operand x, Operand y)
{
	return Operand(x.toDouble() / y.toDouble());
}

void ExprSolve::Operand::shrink_to_fit()
{
	int numOfZeros = 0;
	while (value[value.length() - 1-numOfZeros] == '0')
		++numOfZeros;
	value = value.substr(0, value.length()-numOfZeros);
}

#include "ExpressionSolver.h"

ExprSolve::ExprSolverWrapper::Compp ExprSolve::ExprSolverWrapper::ExpressionSolver::parse(const string &expr)
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
		}
		else if (expr[i] == '*' || expr[i] == '/') {
			if (pivot!=-1 && expr[pivot]!='+' && expr[pivot]!='-')
				pivot = i;
		}
		else if (!isdigit(expr[i]) && expr[i]!='.') {
			exception("Unresolved symbol.");
		}
		++i;
	}
	if (pivot == -1)
		return parse(expr.substr(start+1, end - 2));
	if (pivot == 0)
		return make_shared<Operand>(expr.substr(start, end-start));

	string l = expr.substr(start, pivot-start);
	string r = expr.substr(pivot + 1, end-pivot);
	Compp lEvaluated = parse(l);
	Compp rEvaluated = parse(r);
	return make_shared<Operator>(expr[pivot], lEvaluated, rEvaluated);
}

ExprSolve::ExprSolverWrapper::Operand ExprSolve::ExprSolverWrapper::ExpressionSolver::solve(const string &expr, int &error)
{
	error = -1;
	Compp treeExpr = parse(expr);
	return treeExpr->evaluate(error).shrink_to_fit();
}

bool ExprSolve::ExprSolverWrapper::Operator::remove(Compp &t)
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

void ExprSolve::ExprSolverWrapper::Operator::show() const
{
	if (left)
		left->show();
	cout << oper;
	if (right)
		right->show();
}

ExprSolve::ExprSolverWrapper::Operand ExprSolve::ExprSolverWrapper::Operator::evaluate(int &error)
{
	if (error != -1)
		return Operand();
	switch (oper) {
		case '+':
			return left->evaluate(error).add(right->evaluate(error), error);
		case '-':
			return left->evaluate(error).substract(right->evaluate(error), error);
		case '*':
			return left->evaluate(error).multiply(right->evaluate(error), error);
		case '/':
			return left->evaluate(error).divide(right->evaluate(error), error);
		default: {
			error = 1;
			return Operand();
		}
	}
}

ExprSolve::ExprSolverWrapper::Operand ExprSolve::ExprSolverWrapper::Operand::add(const ExprSolve::ExprSolverWrapper::Operand & x, int & error) const 
{
	if (error != -1) {
		return Operand();
	}
	return Operand(this->toDouble() + x.toDouble());
}

ExprSolve::ExprSolverWrapper::Operand ExprSolve::ExprSolverWrapper::Operand::substract(const ExprSolve::ExprSolverWrapper::Operand & x, int & error) const 
{
	if (error != -1) {
		return Operand();
	}
	return Operand(this->toDouble() - x.toDouble());
}

ExprSolve::ExprSolverWrapper::Operand ExprSolve::ExprSolverWrapper::Operand::multiply(const ExprSolve::ExprSolverWrapper::Operand & x, int & error) const 
{
	if (error != -1) {
		return Operand();
	}
	return Operand(this->toDouble() * x.toDouble());
}

ExprSolve::ExprSolverWrapper::Operand ExprSolve::ExprSolverWrapper::Operand::divide(const ExprSolve::ExprSolverWrapper::Operand & x, int & error) const 
{
	if (error != -1) {
		return Operand();
	}
	if (abs(x.toDouble()) < 0.0000001) {
		error = 0;
		return Operand();
	}
	return Operand(this->toDouble() / x.toDouble());
}

ExprSolve::ExprSolverWrapper::Operand ExprSolve::ExprSolverWrapper::Operand::shrink_to_fit() const
{
	int numOfZeros = 0;
	while (numOfZeros < value.length() && value[value.length() - 1-numOfZeros] == '0')
		++numOfZeros;
	if (numOfZeros < value.length() && value[value.length() - 1 - numOfZeros] == '.')
		++numOfZeros;
	if (numOfZeros == value.length())
		return Operand(0);
	return Operand(value.substr(0, value.length()-numOfZeros));
}


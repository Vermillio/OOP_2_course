#pragma once
#include <list>
#include <memory>
#include <iostream>
#include <string>
#include <algorithm>
#include <stack>
#include <vector>

using namespace std;


namespace ExprSolve {

	class Component;
	class Operand;
	class Operator;
	class ExpressionSolver;
	
	typedef shared_ptr<Component> Compp;
	typedef shared_ptr<Operand> Operandp;
	typedef shared_ptr<Operator> Operatorp;

	const vector<char> opers = { '+', '-', '*', '/' };

	class Component {
	public:
		Component() {};
		virtual bool isOperand() = 0;
		virtual void show() = 0;
		virtual bool remove(Compp &p) = 0;
		virtual Operand evaluate() = 0;
	};

	class Operand : public Component {
	public:
		Operand() {};
		Operand(string _value) : value(_value) {};
		Operand(int _value) : value(to_string(_value)) {};
		Operand(float _value) : value(to_string(_value)) {};
		Operand(double _value) : value(to_string(_value)) {};

		bool isOperand() { return true; };
		void show() { cout << value; };
		Operand evaluate() { return *this; };
		double toDouble() { return stod(value); };
		int toInt() { return stoi(value); };
		float toFloat() { return stof(value); };

		void shrink_to_fit();
		bool remove(Compp &p) { return false; };
	private:
		string value;
	};

	class Operator : public Component {
	public:
		Operator() {};
		Operator(char _oper, Compp &l, Compp &r) : left(l), right(r) {
			if (find(opers.begin(), opers.end(), _oper) == opers.end())
				exception("Incorrect operator.");
			else oper = _oper;
		};
		bool isOperand() { return false; };
		void show();
		Compp& l() { return left; };
		Compp& r() { return right; };

		void setLeft(Compp &p) { left = p; };
		void setRight(Compp &p) { right = p; };
		bool remove(Compp &);

		Operand evaluate();
	private:
		char oper;
		Compp left;
		Compp right;
	};


	class ExpressionSolver
	{
	public:
		Compp parse(string &expr);
		Operand solve(string &expr);
		ExpressionSolver() {};
		~ExpressionSolver() {};
	};

	Operand add(Operand x, Operand y);

	Operand substract(Operand x, Operand y);

	Operand multiply(Operand x, Operand y);

	Operand divide(Operand x, Operand y);
}
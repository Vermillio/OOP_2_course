#pragma once
#include <list>
#include <memory>
#include <iostream>
#include <string>
#include <algorithm>
#include <stack>
#include <vector>

using std::vector;
using std::cout;
using std::shared_ptr;
using std::make_shared;
using std::string;
using std::to_string;
using std::exception;


namespace ExprSolve {


	class ExprSolverWrapper {
	public:
		class Component;
		class Operand;
		class Operator;
		class ExpressionSolver;

		using Compp = shared_ptr<Component>;
		using Operandp = shared_ptr<Operand>;
		using Operatorp = shared_ptr<Operator>;

		class Component {
		public:
			Component() {};
			virtual bool isOperand() const = 0;
			virtual void show() const = 0;
			virtual bool remove(Compp &p) = 0;
			virtual Operand evaluate(int &error) = 0;
		};

		class Operand : public Component {
		public:
			Operand() : value("") {};
			Operand(string _value) : value(_value) {};
			Operand(int _value) : value(to_string(_value)) {};
			Operand(float _value) : value(to_string(_value)) {};
			Operand(double _value) : value(to_string(_value)) {};

			bool isOperand() const { return true; };
			void show() const { cout << value; };
			Operand evaluate(int &error) 
			{
				if (error != -1)
					return Operand();
				return *this; 
			};
			double toDouble() const { return stod(value); };
			int toInt() const { return stoi(value); };
			float toFloat() const { return stof(value); };
			string getValue() const { return value; }

			Operand shrink_to_fit() const;
			bool remove(Compp &p) { return false; };

			Operand add(const Operand & x, int & error) const;
			Operand substract(const Operand & x, int & error) const;
			Operand multiply(const Operand & x, int & error) const;
			Operand divide(const Operand & x, int & error) const;

		private:
			string value;
		};

		class Operator : public Component {
		public:
			Operator() {};
			Operator(char t_oper, Compp &l, Compp &r) : left(l), right(r), oper(t_oper) {
			};
			bool isOperand() const { return false; };
			void show() const;
			Compp& l() { return left; };
			Compp& r() { return right; };

			void setLeft(const Compp &p) { left = p; };
			void setRight(const Compp &p) { right = p; };
			bool remove(Compp &);

			Operand evaluate(int &error);
		private:
			char oper;
			Compp left;
			Compp right;
		};

		class ExpressionSolver
		{
		public:
			Compp parse(const string &expr);
			Operand solve(const string &expr, int &error);
			ExpressionSolver() {};
			~ExpressionSolver() {};
		};

		string solve(const string &expr) {
			ExpressionSolver solver;
			int error;
			Operand x = solver.solve(expr, error);
			if (error == 0)
				return "Can't divide by zero";
			else if (error == 1)
				return "Unknown operator";
			else return x.getValue();
		}
	};
}
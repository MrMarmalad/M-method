#pragma once
#include <string>
#include <cassert>
#include <cmath>
using namespace std;
class elem
{
public:
	double var_coeff;
	string var_mnemo;
	int index;
	double m_coeff;
	//methods
	elem(double var_coeff = 0, string var_mnemo = "", double m_coeff = 0, int index = -1, double d_number = 0, bool type_is_double = false);
	elem(const elem& el);
	elem operator-() const;
	friend ostream& operator<<(std::ostream& out, const elem& el);
	elem calculate(double x);
	bool compareMnemos(elem el);
	bool is_null();
	elem round_elem(int precision);
	friend bool operator == (const elem el1, const elem el2);
	friend bool operator > (const elem el1, const elem el2);
	friend bool operator < (const elem el1, const elem el2);
	friend elem operator + (elem el1, elem el2);
	friend elem operator * (double num, elem el1);
	friend elem operator * (elem el1, double num);
	friend elem operator / (elem el1, double num);
	friend elem operator / (double num, elem el1);
	friend elem operator - (elem el1, elem el2);
};


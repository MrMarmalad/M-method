#include "elem.h"
#include <string>
#include <iostream>
#include <cassert>
#include <cmath>
using namespace std;

elem::elem(double var_coeff, string var_mnemo, double m_coeff, int index, double d_number, bool type_is_double) {
	this->var_coeff = var_coeff;
	this->var_mnemo = var_mnemo;
	this->m_coeff = m_coeff;
	this->index = index;
}

elem::elem(const elem& el)
{
	this->var_coeff = el.var_coeff;
	this->var_mnemo = el.var_mnemo;
	this->m_coeff = el.m_coeff;
	this->index = el.index;
}

elem elem::operator-() const
{
	return elem(-(this->var_coeff), this->var_mnemo, -(this->m_coeff), this->index);
}

elem elem::calculate(double var)
{	
	elem ret_val(this->var_coeff, "", this->m_coeff, this->index);
	ret_val.var_coeff *= var;
	ret_val.m_coeff *= var;
	return ret_val;
}

bool elem::compareMnemos(elem el)
{
	if ((this->var_mnemo == el.var_mnemo)
		&& (this->index == el.index)) {
		return true;
	}
	return false;
}

bool elem::is_null()
{
	if ((this->m_coeff == 0) && (this->var_coeff == 0)) {
		return true;
	}
	return false;
}

elem elem::round_elem(int precision)
{
	elem ret_val(*this);
	ret_val.var_coeff = round(ret_val.var_coeff * pow(10, precision)) / pow(10, precision);
	ret_val.m_coeff = round(ret_val.m_coeff * pow(10, precision)) / pow(10, precision);
	return ret_val;
}

elem operator + (elem el1, elem el2) 
{
	elem ret_val;
	ret_val.var_mnemo = el1.var_mnemo;
	ret_val.var_coeff = el1.var_coeff + el2.var_coeff;
	ret_val.m_coeff = el1.m_coeff + el2.m_coeff;
	ret_val.index = el1.index;
	return ret_val;
}

elem operator*(double num, elem el1)
{
	elem ret_val = elem(el1);
	ret_val.m_coeff *= num;
	ret_val.var_coeff *= num;
	return ret_val;
}

elem operator*(elem el1, double num)
{
	elem ret_val = elem(el1);
	return num * ret_val;
}

elem operator/(elem el1, double num)
{
	return el1 * (1 / num);
}

elem operator/(double num, elem el1)
{
	return el1 / num;
}

elem operator - (elem el1, elem el2) {
	elem ret_val(el1.var_coeff - el2.var_coeff, el1.var_mnemo,
		el1.m_coeff - el2.m_coeff, el1.index);
	return ret_val;
}

elem operator / (elem el1, elem el2) {
	assert(el2.m_coeff != 0);
	assert(el2.var_coeff == 0);
	elem ret_val(el1);

	ret_val.var_coeff /= el2.var_coeff;
	ret_val.m_coeff /= el2.var_coeff;
	return ret_val;
}

elem operator *(elem el1, elem el2) {
	assert(el2.m_coeff == 0 && el2.var_coeff == 0);
	elem ret_val;
	ret_val.var_coeff = el1.var_coeff * el2.var_coeff;
	return ret_val;
}

ostream& operator<<(ostream& out, const elem& el)
{
	if (el.m_coeff != 0 && el.var_coeff != 0) {
		out << "+(" << el.var_coeff;
		if (el.m_coeff > 0) { 
			out <<"+" << el.m_coeff << "M)";
		}
		else {
			out << el.m_coeff << "M)";
		}
		if (el.var_mnemo != "") {
			out<<"*" << el.var_mnemo << el.index + 1;
		} 
	}
	if (el.var_coeff != 0 && el.m_coeff == 0) {
		if (el.var_coeff > 0) {
			out << "+" << el.var_coeff;
		}
		else {
			out << el.var_coeff;
		}
		if (el.var_mnemo != "") {
			out << "*" << el.var_mnemo << el.index + 1;
		}
	}
	if (el.var_coeff == 0 && el.m_coeff != 0) {
		if (el.m_coeff > 0) {
			out << "+" << el.m_coeff<<"M";
		}
		else {
			out << el.m_coeff<<"M";
		}
		if (el.var_mnemo != "") {
			out << "*" << el.var_mnemo << el.index + 1;
		}
	}
	if (el.var_coeff == 0 && el.m_coeff == 0) {
		out << "+0";
		if (el.var_mnemo != "") {
			out << "*" << el.var_mnemo << el.index + 1;
		}
	}
	
	return out;
}

bool operator==(const elem el1, const elem el2)
{
	if ((el1.var_mnemo == el2.var_mnemo) &&
		(el1.var_coeff == el2.var_coeff) &&
		(el1.m_coeff == el2.m_coeff) &&
		(el1.index == el2.index)) {
		return true;
	}
	return false;
}

bool operator>(const elem el1, const elem el2)
{
	if (el1.m_coeff > el2.m_coeff) {
		return true;
	}
	if ((el1.m_coeff == el2.m_coeff) || 
		(el1.m_coeff == 0) && (el2.m_coeff ==0 )) {
		if (el1.var_coeff > el2.var_coeff) {
			return true;
		}
	}
	return false;
}

bool operator<(elem el1, elem el2)
{
	if (el1.m_coeff < el2.m_coeff) {
		return true;
	}
	if (el1.m_coeff == 0 && el2.m_coeff == 0) {
		return el1.var_coeff < el2.var_coeff;
	}
	return false;
}

bool operator>(const elem el1, double val) {
	if (el1.m_coeff > 0) {
		return true;
	}
	else {
		return false;
	}
}

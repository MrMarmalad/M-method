#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <cstdio>
//#include <cmath>
#include "elem.h"

using namespace std;
double max_double = 9223372036854775807.0;


double solve(vector<vector<double>> rest, vector<double> rest_vals,
	vector<int> signs, vector<double> obj_func, string type);
vector<double> get_col(vector<vector<double>> arr, int col_num);
void print_vector(vector<double> input);
void print_vector(vector<int> input);
void print_vector(vector<elem> input);
vector<double> crt_unit_vector(int len, int pos);
elem get_func_value(vector<elem> func, vector<int> basis, vector<double> vals);


int main() {
	setlocale(LC_ALL, "Russian");
	// restriction coefficients (above zero used automatically)
	vector<vector<double>> rest = {
		{16, -3},
		{48, 2},
		{24, -3},
		{40, 2},
	};
	// restriction values
	vector<double> rest_vals = { 442, 2052, 570, 1580 };
	//signs: -1 = "<=", 1 = ">=", 0 = "="
	vector<int> signs = { -1, -1, 1, 1 };
	// coeffs object function
	vector<double> obj_func = { 126, 163 };
	// object function -> min or max
	string type = "min";
	solve(rest, rest_vals, signs, obj_func, type);
	return 0;
}

double solve(vector<vector<double>> rest, vector<double> rest_vals,
			vector<int> signs, vector<double> obj_func, string type) {
	double precision = 3;
	vector<elem> func;
	string tmp_mnemo = "x";
	for (int i = 0; i < obj_func.size(); i++) {
		
		func.push_back(elem(obj_func[i], tmp_mnemo, 0.0, i));
	}
	cout << "�������: ";
	for (int i = 0; i < func.size(); i++) {
		cout << func[i];
	}
	cout << endl;
	//min to max
	if (type == "min") {
		for (int i = 0; i < obj_func.size(); i++) {
			obj_func[i] = -obj_func[i];
		}
	}
	vector<vector<elem>> s_table;
	//to canonical form
	for (int i = 0; i < signs.size(); i++) {
		if (signs[i] != 0) {
			for (int j = 0; j < rest.size(); j++) {
				if (i != j) {
					rest[i].push_back(0);
				}
				else {
					if (signs[i] == -1) {
						rest[i].push_back(1);
					}
					if (signs[i] == 1) {
						rest[i].push_back(-1);
					}
				}
			}
		}
	}
	//add M variables
	vector<double> tmp_col;
	vector<double>::iterator unit_index;
	vector<int> unit_vector_col;
	vector<int> basis;
	int tmp_unit_vector_col;
	vector<int> synth_vars;

	for (int i = 0; i < rest[0].size(); i++) {
		tmp_col = get_col(rest, i);
		if (count(tmp_col.begin(), tmp_col.end(), 0) == (tmp_col.size() - 1)
			&& (count(tmp_col.begin(), tmp_col.end(), 1) == 1)) 
		{
			unit_index = find(tmp_col.begin(), tmp_col.end(), 1);
			tmp_unit_vector_col = distance(tmp_col.begin(), unit_index);
			unit_vector_col.push_back(tmp_unit_vector_col);
			basis.push_back(i);
		}
	}
	//delete non-unique elements
	unit_vector_col.erase(unique(unit_vector_col.begin(), unit_vector_col.end()), unit_vector_col.end());

	
	for (int i = 0; i < rest.size(); i++) {
		if (find(unit_vector_col.begin(), unit_vector_col.end(), i) != unit_vector_col.end() ) {
			continue;
		}
		else {
			basis.push_back(rest[0].size());
			func.push_back(elem(0, "x", 1, rest[0].size()));
			synth_vars.push_back(rest[0].size());
			tmp_col = crt_unit_vector(rest.size(), i);
			for (int j = 0; j < rest.size(); j++) {
				rest[j].push_back(tmp_col[j]);
			}
			
		}
	}

	cout << "����� ��������������:" << endl;
	for (int i = 0; i < rest.size(); i++) {
		for (int j = 0; j < rest[i].size(); j++) {
			cout << rest[i][j] << '\t';
		}
		cout << endl;
	}

	cout << "������� �������� ����������: ";
	print_vector(basis);
	cout << "�������: ";
	print_vector(func);
	cout << "������������� ����������: ";
	print_vector(synth_vars);
	cout << "�� ��������� �������� ������������� ����������"<<endl;

	vector<vector<elem>> synth_eq(synth_vars.size());
	//i = ������� � ������������� ����������
	for (int i = 0; i < synth_vars.size(); i++) {
		//���� ������ �� ������� ����� ��������
		tmp_col = get_col(rest, synth_vars[i]);
		unit_index = find(tmp_col.begin(), tmp_col.end(), 1);
		// ������ ������� �������������
		tmp_unit_vector_col = distance(tmp_col.begin(), unit_index);
		//��������� �������� �� ����������� ������ �� �����
		synth_eq[i].push_back(elem(rest_vals[tmp_unit_vector_col]));
		//��������� ��������� ��������
		for (int j = 0; j < rest[0].size(); j++) {
			// ���� �� ������������� ���������� � �������� �� 0 � ������� �������������
			if ((j != synth_vars[i]) && (rest[tmp_unit_vector_col][j] != 0)) {
				//��������� � ��������������� ������
				synth_eq[i].push_back(
					elem(-(rest[tmp_unit_vector_col][j]), "x", 0, j)
				);
			}
		}
	}
	//����������� � ���������

	cout << "���������� ����������:" << endl;
	for (int i = 0; i < synth_eq.size(); i++) {
		print_vector(synth_eq[i]);
	}


	//����������� ���������� ���������� � ������� � ��������
	vector<elem>::iterator result;
	vector<elem> trans_func;
	for (int i = 0; i < synth_eq.size(); i++) {
		for (int j = 0; j < synth_eq[i].size(); j++) {
			result = find_if(trans_func.begin(), trans_func.end(),
				[i, j, synth_eq](elem el1) {
					if (el1.compareMnemos(synth_eq[i][j])) {
						
						return true;
					}
					return false;
				});
			if (result != trans_func.end()) {
				
				if (synth_eq[i][j].var_mnemo == "") {
					trans_func[distance(trans_func.begin(), result)] = *result + elem(0, synth_eq[i][j].var_mnemo, synth_eq[i][j].var_coeff, synth_eq[i][j].index);
				}
				else {
					trans_func[distance(trans_func.begin(), result)] = *result + elem(0, synth_eq[i][j].var_mnemo, synth_eq[i][j].var_coeff, synth_eq[i][j].index);
				}
				
			}
			else {
				trans_func.push_back(
					elem(0, synth_eq[i][j].var_mnemo, synth_eq[i][j].var_coeff, synth_eq[i][j].index)
				);
			}
		}
	}

	for (int i = 0; i < func.size(); i++) {
		result = find_if(
			trans_func.begin(), trans_func.end(),
			[i, func, synth_vars](elem el) {
				if (el.compareMnemos(func[i])) {
					return true;
				}
				else {
					return false;
				}
			}
		);
		if (result != trans_func.end()) {
			trans_func[distance(trans_func.begin(), result)] = *result + func[i];
		}
		else {
			trans_func.push_back(func[i]);
		}
	}

	vector<elem> tmp_trans_func;
	bool is_synth = false;
	for (int i = 0; i < trans_func.size(); i++) {
		for (int j = 0; j < synth_vars.size(); j++) {
			if (trans_func[i].compareMnemos(
				elem(0,"x",0, synth_vars[j])
			)) {
				is_synth = true;
			}
		}
		if (is_synth) {
			is_synth = false;
			continue;
		}
		tmp_trans_func.push_back(trans_func[i]);
	}
	for (int i = 0; i < rest[0].size(); i++) {
		result = find_if(tmp_trans_func.begin(), tmp_trans_func.end(),
			[i](elem el) {return i == el.index; });
		if (result != tmp_trans_func.end()) {

		}
		else {
			tmp_trans_func.push_back(elem(0, "x", 0, i));
		}
	}


	trans_func = tmp_trans_func;
	//tmp_trans_func.clear();

	cout << "������� ����� ��������������:" << endl;
	for (int i = 0; i < trans_func.size(); i++) {
		if (i != 0) trans_func[i] = (-trans_func[i]);
	}
	sort(trans_func.begin(), trans_func.end(), [](elem el1, elem el2) {return el1.index < el2.index;  });
	print_vector(trans_func);

	//������ ������� ����
	vector<double> plan(rest_vals);
	
	cout << "������ ������� ����:";
	print_vector(plan);

	//�������� ����� ���������
	vector<elem>::iterator max_elem;
	vector <double>::iterator min_elem;
	elem func_val;
	//��������� �������
	vector<double> tmp_plan(plan);
	vector<vector<double>> tmp_rest(rest);
	tmp_trans_func = trans_func;

	double se, a, b, re;
	int min_index, max_index;
	int counter = 0;
	bool iteration = true;
	double funcVal = 0;
	while (iteration) {
		cout << "=====================================================================" << endl;
		cout << "�������� �" << counter << endl;
		cout << "1. �������� �������� �������������." << endl;
		result = find_if(trans_func.begin() + 1, trans_func.end(), [](elem el) {return el > 0; });
		if (result != trans_func.end()) {
			cout << "������� ������� ���� �����������, ��� ��� � ��������� ������ ��������� ������������� ������������" << endl;
			cout << "2. ����������� ����� �������� ����������." << endl;
			max_elem = max_element(trans_func.begin() + 1, trans_func.end());
			max_index = distance(trans_func.begin() + 1, max_elem);
			cout << "� �������� �������� ������� �������, ��������������� ���������� " <<
				max_elem->var_mnemo<<" "<<max_index << " ��� ��� ��� ���������� �����������." << endl;
			
			cout << "3. ����������� ����� ��������� ����������." << endl;

			for (int i = 0; i < plan.size(); i++) {
				if (tmp_rest[i][max_elem->index] > 0) {
					tmp_plan[i] = plan[i] / tmp_rest[i][max_elem->index];
				}
				else {
					tmp_plan[i] = max_double;
				}
				
			}
			min_elem = min_element(tmp_plan.begin(), tmp_plan.end());
			min_index = distance(tmp_plan.begin(), min_elem);
			//��������������� ��������� ���� ����� ����������
			tmp_plan = plan;
			//����������� �������
			re = rest[min_index][max_index];
			cout << "����������� �������: " << re << endl;
			cout << "������ �� �����������:" << min_index << endl;
			cout << "������ �� ���������: " << max_index << endl;
			cout << "4. �������� ��������-�������." << endl;
			for (int i = 0; i < tmp_rest.size(); i++) {
				for (int j = 0; j < tmp_rest[i].size(); j++) {
					if (i != min_index) {
						//������ �������
						se = rest[i][j];
						//� � � - �������� ������� �����, ���������� ������������� � ���������� se � re.
						a = rest[min_index][j];
						b = rest[i][max_index];
						cout << "row: " << i << " col: " << j << " se:" << se << " a: "
							<< a << " b: " << b << " �������: ";
						printf("ne = %.3f - (%.3f * %.3f) / %.3f\n", se, a, b, re);
						tmp_rest[i][j] = se - (a * b) / re;
					}
					else {
						cout << "row: " << i << " col: " << j << " val: "<< tmp_rest[i][j]<< " �������: ";
						printf("ne = %.3f / %.3f\n", tmp_rest[i][j], re);
						tmp_rest[i][j] /= re;
					}
				}
			}
			a = plan[min_index];
			//�������� �����
			for (int i = 0; i < plan.size(); i++) {
				b = rest[i][max_index];
				se = plan[i];
				if (i == min_index) {
					tmp_plan[i] = tmp_plan[i] / re;
				}
				else {
					tmp_plan[i] = se - (a * b) / re;
				}
			}

			func_val = get_func_value(trans_func, basis, plan);
			// �������� ��������� ������
			cout << "�������� ��������� ������" << endl;
			for (int i = 0; i < trans_func.size(); i++) {
				cout << "index: ";
				if (i == 0) {
					cout << "ne = " << tmp_trans_func[i] << "- (" << plan[min_index] << " * " << trans_func[max_index + 1] << ") / " << re << endl;
					tmp_trans_func[i] = tmp_trans_func[i] - (plan[min_index] * trans_func[max_index + 1]) / re;
				}
				else {
					cout << "ne = " << trans_func[i] << "- (" << rest[min_index][i-1] << " * " << trans_func[max_index + 1] << ") / " << re << endl;
					tmp_trans_func[i] = trans_func[i] - (rest[min_index][i-1] * trans_func[max_index + 1]) / re;
				}
			}
			
			//�������� ���������� � ������
			for (int i = 0; i < basis.size(); i++) {
				if (i == min_index) {
					basis[i] = max_index;
					break;
				}
			}
			//
			trans_func = tmp_trans_func;
			for (int k = 0; k < trans_func.size(); k++) {
				trans_func[k] = trans_func[k].round_elem(precision);
			}
			rest = tmp_rest;
			for (int k = 0; k < rest.size(); k++) {
				for (int l = 0; l < rest[k].size(); l++) {
					rest[k][l] = round(rest[k][l] * pow(10, precision)) / pow(10, precision);
				}
			}
			plan = tmp_plan;
			for (int k = 0; k < plan.size(); k++) {
				plan[k] = round(plan[k] * pow(10, precision)) / pow(10, precision);
			}
			//������������� ����������
			cout << "�����������"<<endl;
			for (int i = 0; i < rest.size(); i++) {
				print_vector(rest[i]);
			}
			cout << "�������"<<endl;
			print_vector(trans_func);
			cout << "�����" << endl;
			print_vector(basis);
			cout << "����" << endl;
			print_vector(plan);
			cout << "---------------------------------------------------------------------" << endl;

		}
		else {
			iteration = false;
			cout << "� ��������� ������ ����������� ������������� ������������. ���� ���������." << endl;
		}
		counter++;
	}
	
	cout << "\n\n����� ���������" << endl;
	cout << "�������� ����������:";
	print_vector(basis);
	//��������� ������������ �������
	is_synth = false;
	vector<int>::iterator in_basis;
	for (int i = 0; i < synth_vars.size(); i++) {
		in_basis = find(basis.begin(), basis.end(), synth_vars[i]);
		if (in_basis != basis.end()) {
			is_synth = true;
		}
	}

	if (is_synth) {
		cout << "������� �� �������� ����������!\n� ������ ������������ ������������� ���������" << endl;
	}
	else {
		cout << "������� �������� ����������, �� ������������� ���������� ";
		print_vector(synth_vars);
		cout << "����������� � ������" << endl;
		cout << "����������� ����: " << endl;;
		for (int i = 0; i < func.size(); i++) {
			tmp_col = get_col(rest, func[i].index);
			for (int j = 0; j < tmp_col.size(); j++) {
				if (tmp_col[j] == 1) {
					cout << func[i].var_mnemo << func[i].index << " = " << plan[j] << endl;
				}
			}
		}
		cout << "F(X) = " << trans_func[0];
	}


	return 0;
}

vector<double> get_col(vector<vector<double>> arr, int col_num) {
	vector<double> ret_val;
	for (int i = 0; i < arr.size(); i++) {
		ret_val.push_back(arr[i][col_num]);
	}
	return ret_val;
}

void print_vector(vector<double> input) {
	//cout.precision(2);
	for (int i = 0; i < input.size(); i++) {
		cout << input[i] << '\t';
	}
	cout << endl;
}

void print_vector(vector<int> input) {
	for (int i = 0; i < input.size(); i++) {
		cout << input[i]<<'\t';
	}
	cout << endl;
}


void print_vector(vector<elem> input) {
	for (int i = 0; i < input.size(); i++) {
		cout << input[i];
	}
	cout << endl;
}

vector<double> crt_unit_vector(int len, int pos) {
	vector<double> unit_vector;
	for (int i = 0; i < len; i++) {
		if (i == pos) {
			unit_vector.push_back(1);
		}
		else {
			unit_vector.push_back(0);
		}
	}
	return unit_vector;
}

elem get_func_value(vector<elem> func, vector<int> basis, vector<double> vals) {
	vector<elem>::iterator x;
	elem sum(0, "", 0, -1);
	for (int i = 0; i < basis.size(); i++) {
		x = find_if(func.begin(), func.end(),
			[i, basis](elem el) { return el.index == basis[i]; });
		if (x != func.end()) {
			x->calculate(basis[i]);
			sum = sum +  (* x);
		}
	}
	return sum;
}
// lab5.cpp: 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
using namespace std;

string file = "matrix.txt";

void input(vector<vector<int>> &A, vector<int> &b, int &row, int &col) {
	ifstream fin(file);
	fin >> row >> col;
	/*cout << row << '\t' << col << endl;*/
	A.clear();
	A.resize(row);
	int tmp;
	for (int i = 0; i < row; i++) {
		for (int j = 0; j < col; j++) {
			fin >> tmp;
			A[i].push_back(tmp);
		}
	}
	/*for (auto row : A) {
		for (auto num : row) {
			cout << num << '\t';
		}
		cout << endl;
	}*/

	//cout << endl;
	b.clear();
	for (int i = 0; i < row; i++) {
		fin >> tmp;
		b.push_back(tmp);
	}
	/*for (auto num : b) {
		cout << num << endl;
	}*/
	return;
}

double norm(vector<double> a, vector<double> b) {
	if (a.size() != b.size()) return -1;
	double rowMax = -1;
	for (int i = 0; i < a.size(); i++) {
		rowMax = max(rowMax, abs(a[i] - b[i]));
	}
	return rowMax;
}

void Jacobi(vector<vector<int>> A, vector<int> b, int row, int col) {
	printf("Jacobi\nstep\t");
	for (int k = 0; k < col; k++) {
		printf("x%d\t", k + 1);
	}
	printf("\n");
	vector<double> x(col, 0.0);
	vector<double> last(col, 0.0);
	int step = 0;
	printf("%d\t", step++);
	for (auto xi : x) {
		printf("%lf\t", xi);
	}
	printf("\n");
	do {
		last.assign(x.begin(), x.end());
		for (int i = 0; i < col; i++) {
			x[i] = (double)b[i];
			for (int j = 0; j < col; j++) {
				if (i == j) continue;
				x[i] -= A[i][j] * last[j];
			}
			x[i] = x[i] / (double)A[i][i];
		}
		printf("%d\t", step++);
		for (auto xi : x) {
			printf("%lf\t", xi);
		}
		printf("\n");
	} while (norm(x, last) > 0.00001);
	
	return;
}

void GaussSeidel(vector<vector<int>> A, vector<int> b, int row, int col) {
	printf("GaussSeidel\nstep\t");
	for (int k = 0; k < col; k++) {
		printf("x%d\t", k + 1);
	}
	printf("\n");
	vector<double> x(col, 0.0);
	vector<double> last(col, 0.0);
	int step = 0;
	printf("%d\t", step++);
	for (auto xi : x) {
		printf("%lf\t", xi);
	}
	printf("\n");
	do {
		last.assign(x.begin(), x.end());
		for (int i = 0; i < col; i++) {
			x[i] = (double)b[i];
			for (int j = 0; j < col; j++) {
				if (i == j) continue;
				x[i] -= A[i][j] * x[j];
			}
			x[i] = x[i] / (double)A[i][i];
		}
		printf("%d\t", step++);
		for (auto xi : x) {
			printf("%lf\t", xi);
		}
		printf("\n");
	} while (norm(x, last) > 0.00001);

	return;
}

int main()
{
	int row, col;
	vector<vector<int>> A;
	vector<int> b;
	input(A, b, row, col);

	freopen("output.txt", "w", stdout);

	Jacobi(A, b, row, col);
	printf("\n");
	GaussSeidel(A, b, row, col);

	system("pause");
	return 0;
}


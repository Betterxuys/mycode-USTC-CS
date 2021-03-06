// lab2.cpp: 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include<iostream>
#include<fstream>
#include<iomanip>
#include<cmath>
#include<vector>
#include<climits>
#include <algorithm>
using namespace std;

double const PI = acos(double(-1));

double Lx(vector<double> X, vector<double> Y, double x, int n) {
	double lx = 0.0, tmp = 0.0;
	int i, j;
	for (i = 0; i <= n; i++) {
		tmp = 1.0;
		for (j = 0; j < i; j++) {
			tmp = tmp * (x - X[j]) / (X[i] - X[j]);
		}
		for (j = i + 1; j <= n; j++) {
			tmp = tmp * (x - X[j]) / (X[i] - X[j]);
		}
		lx = lx + tmp * Y[i];
	}
	return lx;
}

double fx(double x) {
	return 1 / (4 + x + x * x);
}

int main()
{
	ofstream fout("out.txt");
	
	fout << setprecision(12);
	fout << setiosflags(ios::scientific);
	
	vector<int> N(3);
	N[0] = (N[1] = ((N[2] = 16) >> 1)) >> 1;
	//fout << N[0] <<" "<< N[1] <<" "<< N[2] << endl

	vector<double> x, y;
	double error = INT_MIN;

	for (int k = 0; k < 3; k++) {
		fout << "N = " << N[k] << endl;
		// first
		fout << "first" << endl;
		fout << "fx" << '\t' << "px" << endl;
		
		error = INT_MIN;
		x.clear();
		y.clear();
		for (int i = 0; i <= N[k]; i++) {
			x.push_back((10.0*i - 5 * N[k]) / N[k]);
			y.push_back(fx(x[i]));
		}
		for (int i = 0; i <= 500; i++) {
			double x_tmp = (i - 250.0) / 50.0;
			double f = fx(x_tmp);
			double p = Lx(x, y, x_tmp, N[k]);
			fout << f << '\t' << p << endl;
			error = max(error, abs(f - p));
		}
		fout << "error = " << error << endl;

		// second
		fout << "second" << endl;
		fout << "fx" << '\t' << "px" << endl;
		
		error = INT_MIN;
		x.clear();
		y.clear();
		for (int i = 0; i <= N[k]; i++) {
			x.push_back(-5 * cos((2 * i + 1)*PI / (2 * N[k] + 2)));
			y.push_back(fx(x[i]));
		}
		for (int i = 0; i <= 500; i++) {
			double x_tmp = (i - 250.0) / 50.0;
			//cout << x_tmp << endl;
			double f = fx(x_tmp);
			double p = Lx(x, y, x_tmp, N[k]);
			fout << f << '\t' << p << endl;
			error = max(error, abs(f - p));
		}
		fout << "error = " << error << endl;
	}
	fout.close();
	system("pause");
    return 0;
}


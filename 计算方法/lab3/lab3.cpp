// lab3.cpp: 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include<iostream>
#include<cstdio>
#include<cmath>

using namespace std;

const int Low = 1, High = 6;
const int N = 12;

double F(double x) {
	return sin(x);
}

double Trapezoid(double(*f)(double), double low, double high, int n) {
	double result = 0.0;
	double h = (high - low) / n;
	for (int i = 1; i < n; i++) {
		result += (*f)(low + i * h);
	}
	return h * (result + (*f)(low) / 2.0 + (*f)(high) / 2.0);
}

double Simpson(double(*f)(double), double low, double high, int n) {
	double result = 0.0;
	double h = (high - low) / n;
	int m = n / 2;
	result += 4 * (*f)(low + h);
	for (int i = 1; i < m; i++) {
		result += 4 * (*f)(low + (2 * i + 1)*h) + 2 * (*f)(low + 2 * i*h);
	}
	return h / 3.0*((*f)(low) + result + (*f)(high)); 
}


void cal(double(*f)(double), int low, int high) {
	double true_ans = cos(low) - cos(high);
	printf("true answer: %7lf\n", true_ans);

	int n;
	double d = 0.0;
	double e = 0., e2 = 0.;
	double res = 0.;
	cout << "Trapezoid" << endl;
	for (int k = 0; k <= N; k++) {
		printf("k = %d\t\t", k);
		res = Trapezoid(f, low, high, pow(2, k));
		e2 = true_ans - res;
		printf("e = %.12e\t\t", e2);
		if (k > 0) {
			d = -log(e2 / e) / log(2);
			printf("d = %.6lf", d);
		}
		e = e2;
		cout << endl;
	}

	cout << "Simpson" << endl;
	for (int k = 1; k <= N; k++) {
		printf("k = %d\t\t", k);
		res = Simpson(f, low, high, pow(2, k));
		e2 = true_ans - res;
		printf("e = %.12e\t\t", e2);
		if (k > 1) {
			d = -log(e2 / e) / log(2);
			printf("d = %.6lf", d);
		}
		e = e2;
		cout << endl;
	}
}

int main()
{
	double low = Low, high = High;
	double(*f)(double) = F;
	cal(f, low, high);

	system("pause");
    return 0;
}


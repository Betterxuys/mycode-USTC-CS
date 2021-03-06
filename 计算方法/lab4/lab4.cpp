// lab4.cpp: 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include <iostream>
#include <cstdio>
#include <fstream>
#include <cmath>

using namespace std;

const double epsilon = 1e-10;
const int N = 2;

double fd(double x){
	return ((8 * x + 72)*x + 122)*x - 16;
}

double f(double x) {
	return (((2 * x + 24)*x + 61)*x - 16)*x + 1;
}

void Newton(double(*fx)(double), double(*fdx)(double), double x0) {
	printf("k\t\tx_k\t\tf(x_k)\n");
	int k = 0;
	double xk = x0;
	double fxk = (*fx)(xk);
	printf("%d\t\t%.10e\t\t%.10e\n", k, xk, fxk);
	while (abs(fxk) >= epsilon) {
		k++;
		xk = xk - (*fx)(xk) / (*fdx)(xk);
		fxk = (*fx)(xk);
		printf("%d\t\t%.10e\t\t%.10e\n", k, xk, fxk);
	}
	return;
}

void Secant(double(*fx)(double), double x0, double x1) {
	printf("k\t\tx_k\t\tf(x_k)\n");
	int k = 0;
	double xk = x0;
	double fxk = (*fx)(xk);
	printf("%d\t\t%.10e\t\t%.10e\n", k, xk, fxk);
	k++;
	double xkplus1 = x1;
	fxk = (*fx)(xkplus1);
	printf("%d\t\t%.10e\t\t%.10e\n", k, xkplus1, fxk);
	while (abs(fxk) >= epsilon) {
		k++;
		double tmp = xkplus1;
		xkplus1 = xkplus1 - (*fx)(xkplus1)*((xkplus1 - xk) / ((*fx)(xkplus1) - (*fx)(xk)));
		fxk = (*fx)(xkplus1);
		printf("%d\t\t%.10e\t\t%.10e\n", k, xkplus1, fxk);
		xk = tmp;
	}
	return;
}

int main()
{
	double NewtonX0[2] = { 0,1 };
	double SecantX0X1[2][2] = { {0,0.1}, {0.5,1.0} };
	double(*fx)(double) = f;
	double(*fdx)(double) = fd;

	for (int i = 0; i < N; i++) {
		cout << "Newton i = " << i << endl;
		cout << "x0 = " << NewtonX0[i] << endl;
		Newton(fx, fdx, NewtonX0[i]);
		cout << endl;
	}

	for (int i = 0; i < N; i++) {
		cout << "Secant i = " << i << endl;
		cout << "x0 = " << SecantX0X1[i][0] << endl;
		cout << "x1 = " << SecantX0X1[i][1] << endl;
		Secant(fx, SecantX0X1[i][0], SecantX0X1[i][1]);
		cout << endl;
	}

	system("pause");
    return 0;
}


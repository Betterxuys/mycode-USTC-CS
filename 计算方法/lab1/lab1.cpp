// lab1.cpp: 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include<iostream>
#include<vector>
#include<iomanip>
#include<math.h>
using namespace std;

int main()
{
	cout << setprecision(12);
	cout << setiosflags(ios::scientific);
	float x = 1.0 / 8.0;
	float fx, gx;
	cout << "problem 1" << endl;
	cout << "x" << "\t\t\t" << "fx" << "\t\t\t" << "gx" << endl;
	for (int i = 0; i < 10; i++) {
		fx = sqrtf(x*x + 4) - 2;
		gx = (x*x) / (sqrtf(x*x + 4) + 2);
		cout << x << "\t" << fx << "\t" << gx << endl;
		x = x / 8.0;
	}

	cout << endl << "problem 2" << endl;
	cout << setprecision(6);

	vector<double> vec(5);
	//4040.045551380452, -2759471.276702747, -31.64291531266504,  2755462.874010974,  0.0000557052996742893
	vec[0] = 4040.045551380452;
	vec[1] = -2759471.276702747;
	vec[2] = -31.64291531266504;
	vec[3] = 2755462.874010974;
	vec[4] = 0.0000557052996742893;

	

	double ans1 = 0.0, ans2 = 0.0, ans3 = 0.0;
	cout << "ans1" << endl;
	for (int i = 0; i < 5; i++) {
		ans1 += vec[i];
		cout << ans1 << endl;
	}
	cout << "ans2" << endl;
	for (int i = 4; i >= 0; i--) {
		ans2 += vec[i];
		cout << ans2 << endl;
	}
	cout << "ans3" << endl;
	double pos = 0.0, neg = 0.0;
	pos += vec[3];
	cout << pos << endl;
	pos += vec[0];
	cout << pos << endl;
	pos += vec[4];
	cout << pos << endl;
	neg += vec[1];
	cout << neg << endl;
	neg += vec[2];
	cout << neg << endl;
	ans3 = pos + neg;
	
	cout << "final result" << endl;
	cout << "ans1" << "\t\t" << "ans2" << "\t\t" << "ans3" << endl;
	cout << ans1 << "\t" << ans2 << "\t" << ans3 << endl;

	system("pause");
    return 0;
}


// IDAstar.cpp: 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include <iostream>
#include <fstream>
#include <chrono>
#include <string>
#include <queue>
#include <cctype>

#define mkdot(a,b) (dot{a,b})
#define USEMANHATTAN true
#define DEBUG false
#define MAXROW 30
#define MAXCOL 60

using namespace std;
using namespace chrono;

const string infile = "input2.txt";
const string outfile = "output_IDA.txt";

// map's parameters
int row = 18, col = 25;
// start dot's location
int startX = 1, startY = 0;
// end dot's location
int endX = 16, endY = 24;

// four direction
const int numOfDirections = 4;
const int dr[] = { 1,-1,0,0 };
const int dc[] = { 0,0,-1,1 };

int A[MAXROW][MAXCOL];

// g value
int gval[MAXROW][MAXCOL] = { 0 };

// found a path
bool found = false;

class dot {
public:
	int x;
	int y;
	// for A star search
	int f;
	int g;
	int h;

	dot() {}
	dot(int inx, int iny) {
		x = inx;
		y = iny;
	}
};

// previous dot for each
dot prevdot[MAXROW][MAXCOL];

int h(int x, int y) {
	// Manhattan distance
	if (USEMANHATTAN) {
		return abs(x - endX) + abs(y - endY);
	}

	// Eu
	else {
		return sqrt((x - endX) * (x - endX) + (y - endY) * (y - endY));
	}
}

int f(int x, int y, int g) {
	return g + h(x, y);
}

int dfs(int x, int y, int g, int bound) {
	int fval = f(x, y, g);
	if (fval > bound) {
		return fval;
	}
	if (x == endX && y == endY) {
		found = true;
		// 0 means found
		return 0;
	}
	
	int minf = INT_MAX;
	int nextX, nextY;
	for (int i = 0; i < numOfDirections; i++) {
		nextX = x + dr[i];
		nextY = y + dc[i];
		// across board
		if (nextX < 0 || nextX >= row || nextY < 0 || nextY >= col) continue;
		// is a wall
		if (A[nextX][nextY] == 1) continue;

		if (g + 1 < gval[nextX][nextY]) {
			gval[nextX][nextY] = g + 1;
			prevdot[nextX][nextY] = mkdot(x, y);
			minf = min(minf, dfs(nextX, nextY, gval[nextX][nextY], bound));
		}
	}
	return minf;
}

void IDAstar() {
	// start or end is wall
	if (A[startX][startY] == 1 || A[endX][endY] == 1) {
		return;
	}

	int bound = f(startX, startY, gval[startX][startY]);
	while (bound < INT_MAX - 1 && found == false) {
		// initial g value unreachable for each node
		for (int i = 0; i < row; i++) {
			for (int j = 0; j < col; j++) {
				gval[i][j] = INT_MAX;
			}
		}
		gval[startX][startY] = 0;

		int nextbound = dfs(startX, startY, gval[startX][startY], bound);
		bound = nextbound;
	}

	return;
}

void run() {
	auto start = system_clock::now();

	IDAstar();

	auto end = system_clock::now();
	auto duration = duration_cast<microseconds>(end - start);
	cout << "花费了"
		<< double(duration.count()) * microseconds::period::num / microseconds::period::den
		<< "秒" << endl;
	return;
}

void input() {
	ifstream fin(infile);
	int tmp;
	fin >> row >> col;
	for (int i = 0; i < row; i++) {
		for (int j = 0; j < col; j++) {
			fin >> tmp;
			A[i][j] = tmp;
		}
	}
	fin >> startX >> startY >> endX >> endY;

	printf("迷宫大小为%d*%d\n", row, col);
	printf("起点为(%d, %d), 终点为(%d, %d)\n", startX, startY, endX, endY);
}

void output() {
	if (USEMANHATTAN) {
		printf("使用曼哈顿距离作为启发式函数\n");
	}
	else {
		printf("使用欧氏距离作为启发式函数\n");
	}

	int px, py;
	int prevx, prevy;
	if (found == false)
	{
		printf("无法到达!\n");
		return;
	}
	printf("从起点到终点所需步数为:%d\n", gval[endX][endY]);

	px = endX;
	py = endY;
	while (px != startX || py != startY)
	{
		A[px][py] = '*' - '0';
		prevx = prevdot[px][py].x;
		prevy = prevdot[px][py].y;
		px = prevx;
		py = prevy;
	}

	A[startX][startY] = '$' - '0';
	A[endX][endY] = '#' - '0';

	printf("走法如下 $是起点，*是路径，#是终点:\n");
	for (int i = 0; i < row; i++)
	{
		for (int j = 0; j < col; j++)
		{
			printf("%c ", A[i][j] + '0');
		}
		putchar('\n');
	}
	return;
}

int main()
{
	if (!DEBUG) freopen(outfile.c_str(), "w", stdout);

	input();

	run();

	output();

	if (DEBUG) system("pause");
	return 0;
}
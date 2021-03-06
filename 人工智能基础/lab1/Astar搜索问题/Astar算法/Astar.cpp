// Astar.cpp: 定义控制台应用程序的入口点。
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
const string outfile = "output_A.txt";

// parameters needs input
// map's parameters
int row, col;
// start dot's location
int startX, startY;
// end dot's location
int endX, endY;

// four direction
const int dr[] = { 1,-1,0,0 };
const int dc[] = { 0,0,-1,1 };

int A[MAXROW][MAXCOL];

// in open list
bool inOpen[MAXROW][MAXCOL] = { false };

// in close list
bool inClose[MAXROW][MAXCOL] = { false };

// g value
int gval[MAXROW][MAXCOL] = { 0 };

// found a path
bool flag = false;

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

	int calh() {
		// Manhattan distance
		if (USEMANHATTAN) {
			return abs(x - endX) + abs(y - endY);
		}
		
		// Eu
		else {
			return sqrt((x - endX) * (x - endX) + (y - endY) * (y - endY));
		}
	}

	void calf() {
		f = g + calh();
	}

	void setg_calf(int _g) {
		g = _g;
		calf();
	}

	bool operator < (const dot &a) const {
		return f > a.f;						//最小值优先
	}
};

// previous dot for each
dot prevdot[MAXROW][MAXCOL];

void Astar() {
	// start or end is wall
	if (A[startX][startY] == 1 || A[endX][endY] == 1) {
		return;
	}

	// initial g value unreachable for each node
	for (int i = 0; i < row; i++) {
		for (int j = 0; j < col; j++) {
			gval[i][j] = INT_MAX;
		}
	}
	
	// open list implement
	priority_queue<dot> q;

	// processing dot's X and Y
	int nowX, nowY;
	int nowGval;
	// processing dot's neighbor
	int nextX, nextY;

	dot start(startX, startY);
	start.setg_calf(0);
	gval[startX][startY] = 0;
	q.push(start);
	inOpen[startX][startY] = true;
	while (q.size()) {
		nowX = q.top().x;
		nowY = q.top().y;
		nowGval = q.top().g;
		q.pop();
		// already been closed, means a optimal path has been found
		//if (inClose[nowX][nowY] == true) continue;
		if (gval[nowX][nowY] < nowGval) continue;
		// found a path
		if (nowX == endX && nowY == endY) {
			flag = true;
			break;
			//continue;
		}

		//if(DEBUG) printf("size:%d\n", q.size());

		// append in close list
		//inClose[nowX][nowY] = true;
		for (int i = 0; i < 4; i++) {
			nextX = nowX + dr[i];
			nextY = nowY + dc[i];
			// across board
			if (nextX < 0 || nextX >= row || nextY < 0 || nextY >= col) continue;
			// is a wall
			if (A[nextX][nextY] == 1) continue;
			// in close list
			//if (inClose[nextX][nextY] == true) continue;

			// found a better path or a new dot
			if ((inOpen[nextX][nextY] == true && gval[nextX][nextY] > nowGval + 1) || inOpen[nextX][nextY] == false) {
				dot next(nextX, nextY);
				next.setg_calf(nowGval + 1);
				gval[nextX][nextY] = nowGval + 1;
				prevdot[nextX][nextY] = mkdot(nowX, nowY);
				q.push(next);
				inOpen[nextX][nextY] = true;
			}
		}
	}
	return;
}

void run() {
	auto start = system_clock::now();

	Astar();

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
	if (flag == false)
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
	if(!DEBUG) freopen(outfile.c_str(), "w", stdout);

	input();

	run();

	output();

	if(DEBUG) system("pause");
	return 0;
}


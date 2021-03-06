// Gobang.cpp: 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include <iostream>
#include <fstream>
#include <cctype>
#include <algorithm>

#define DEBUG true

#define NONE 0
// chess color
#define BLACK 1
#define WHITE 2

// alpha beta decision depth
#define MAXDEPTH 1
// When MAXDEPTH = 1
// AlphaBetaDecision is first Max layer
// MinValue is second layer
// Utility is third layer

// Evaluating
#define CHANGLIAN 50000
#define HUOSI 1000
#define CHONGSI 500
#define HUOSAN 300
#define HUOER 150
#define MIANSAN 100
#define MIANER 50
#define SISI -5
#define SISAN -5
#define SIER -5



using namespace std;

// parameter
const int row = 15, col = 15;

// chess board
int board[row][col] = { 0 };

//位置重要性价值表,此表从中间向外,越往外价值越低
int PosValue[row][col] =
{
	{ 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 },
	{ 0,1,1,1,1,1,1,1,1,1,1,1,1,1,0 },
	{ 0,1,2,2,2,2,2,2,2,2,2,2,2,1,0 },
	{ 0,1,2,3,3,3,3,3,3,3,3,3,2,1,0 },
	{ 0,1,2,3,4,4,4,4,4,4,4,3,2,1,0 },
	{ 0,1,2,3,4,5,5,5,5,5,4,3,2,1,0 },
	{ 0,1,2,3,4,5,6,6,6,5,4,3,2,1,0 },
	{ 0,1,2,3,4,5,6,7,6,5,4,3,2,1,0 },
	{ 0,1,2,3,4,5,6,6,6,5,4,3,2,1,0 },
	{ 0,1,2,3,4,5,5,5,5,5,4,3,2,1,0 },
	{ 0,1,2,3,4,4,4,4,4,4,4,3,2,1,0 },
	{ 0,1,2,3,3,3,3,3,3,3,3,3,2,1,0 },
	{ 0,1,2,2,2,2,2,2,2,2,2,2,2,1,0 },
	{ 0,1,1,1,1,1,1,1,1,1,1,1,1,1,0 },
	{ 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 }
};

// four direction
// down/right/down-right/up-right
const int numOfDirections = 4;
int dx[numOfDirections] = { 1,0,1,-1 };
int dy[numOfDirections] = { 0,1,1,1 };

class Move {
public:
	int x;
	int y;
};

bool InBoard(int x, int y);
int MaxValue(Move lastMove, int alpha, int beta, int depth);

void DoMove(Move next, int color) {
	board[next.x][next.y] = color;
}

void unDoMove(Move next) {
	board[next.x][next.y] = 0;
}

bool isTerminal(Move now) {
	int color = board[now.x][now.y];
	bool Terminal = false;

	for (int i = 0; i < numOfDirections; i++) {
		int tmpx = now.x;
		int tmpy = now.y;
		int sum = 1;
		while (InBoard(tmpx + dx[i], tmpy + dy[i]) &&
			color == board[tmpx + dx[i]][tmpy + dy[i]]) {
			sum++;
			tmpx += dx[i];
			tmpy += dy[i];
		}

		tmpx = now.x;
		tmpy = now.y;
		while (InBoard(tmpx - dx[i], tmpy - dy[i]) &&
			color == board[tmpx - dx[i]][tmpy - dy[i]]) {
			sum++;
			tmpx -= dx[i];
			tmpy -= dy[i];
		}
		if (sum == 5) {
			Terminal = true;
			return true;
			// break;
		}
	}

	return false;
}

bool InBoard(int x, int y) {
	if (x >= 0 && x < row && y >= 0 && y < col) {
		return true;
	}
	else return false;
}

bool FiveInBoard(int x, int y, int dir) {
	if (!InBoard(x, y)) {
		return false;
	}
	else {
		// down
		if (dir == 0) {
			if (x < row - 4) {
				return true;
			}
			else return false;
		}
		// right
		else if (dir == 1) {
			if (y < col - 4) {
				return true;
			}
			else return false;
		}
		// down-right
		else if (dir == 2) {
			if (x < row - 4 && y < col - 4) {
				return true;
			}
			else return false;
		}
		// up-right
		else {
			if (x >= 4 && y < col - 4) {
				return true;
			}
			else return false;
		}
	}
}

int Score(int x, int y) {
	int score = 0;
	int color = board[x][y];
	int oppenent = (color == BLACK) ? WHITE : BLACK;

	// four directions
	for (int i = 0; i < numOfDirections; i++) {
		// 长连
		if (
			FiveInBoard(x, y, i) &&
			board[x + dx[i]][y + dy[i]] == color &&
			board[x + 2 * dx[i]][y + 2 * dy[i]] == color &&
			board[x + 3 * dx[i]][y + 3 * dy[i]] == color &&
			board[x + 4 * dx[i]][y + 4 * dy[i]] == color) {
			score += CHANGLIAN;
		}
		// 活四
		else if (
			FiveInBoard(x, y, i) &&
			board[x +  dx[i]][y + dy[i]] == color &&
			board[x + 2 * dx[i]][y + 2 * dy[i]] == color &&
			board[x + 3 * dx[i]][y + 3 * dy[i]] == color &&
			board[x + 4 * dx[i]][y + 4 * dy[i]] == NONE &&
			InBoard(x - dx[i], y - dy[i]) &&
			board[x - dx[i]][y - dy[i]] == NONE) {
			score += HUOSI;
		}
		// 冲四
		else if (FiveInBoard(x, y, i) &&
			(
				(board[x + dx[i]][y + dy[i]] == color &&
				 board[x + 2 * dx[i]][y + 2 * dy[i]] == color &&
				 board[x + 3 * dx[i]][y + 3 * dy[i]] == color &&
				 board[x + 4 * dx[i]][y + 4 * dy[i]] == NONE &&
				 ( (InBoard(x - dx[i], y - dy[i]) && board[x - dx[i]][y - dy[i]] == oppenent) ||
					!InBoard(x - dx[i], y - dy[i]) ) 
				) ||
				(board[x + dx[i]][y + dy[i]] == color &&
				 board[x + 2 * dx[i]][y + 2 * dy[i]] == color &&
				 board[x + 3 * dx[i]][y + 3 * dy[i]] == NONE &&
				 board[x + 4 * dx[i]][y + 4 * dy[i]] == color
				) ||
				(board[x + dx[i]][y + dy[i]] == color &&
				 board[x + 2 * dx[i]][y + 2 * dy[i]] == NONE &&
				 board[x + 3 * dx[i]][y + 3 * dy[i]] == color &&
				 board[x + 4 * dx[i]][y + 4 * dy[i]] == color
				)
			)
		) {
			score += CHONGSI;
		}
		// 活三 条件宽松，未完全判断是否是活四
		else if (FiveInBoard(x, y, i) &&
			((board[x + dx[i]][y + dy[i]] == color &&
				board[x + 2 * dx[i]][y + 2 * dy[i]] == color &&
				board[x + 3 * dx[i]][y + 3 * dy[i]] == NONE &&
				board[x + 4 * dx[i]][y + 4 * dy[i]] == NONE &&
				InBoard(x - dx[i], y - dy[i]) &&
				board[x - dx[i]][y - dy[i]] == NONE
				) ||
				(board[x + dx[i]][y + dy[i]] == color &&
					board[x + 2 * dx[i]][y + 2 * dy[i]] == NONE &&
					board[x + 3 * dx[i]][y + 3 * dy[i]] == color &&
					board[x + 4 * dx[i]][y + 4 * dy[i]] == NONE &&
					InBoard(x - dx[i], y - dy[i]) &&
					board[x - dx[i]][y - dy[i]] == NONE))
			) {
			score += HUOSAN;
		}
		// 眠三 只判断常见的三种情况
		else if (FiveInBoard(x, y, i) &&
			((board[x + dx[i]][y + dy[i]] == color &&
				board[x + 2 * dx[i]][y + 2 * dy[i]] == color &&
				board[x + 3 * dx[i]][y + 3 * dy[i]] == NONE &&
				board[x + 4 * dx[i]][y + 4 * dy[i]] == NONE &&
				((InBoard(x - dx[i], y - dy[i]) && board[x - dx[i]][y - dy[i]] == oppenent) ||
					!InBoard(x - dx[i], y - dy[i]))
				) ||
				(board[x + dx[i]][y + dy[i]] == color &&
					board[x + 2 * dx[i]][y + 2 * dy[i]] == NONE &&
					board[x + 3 * dx[i]][y + 3 * dy[i]] == color &&
					board[x + 4 * dx[i]][y + 4 * dy[i]] == NONE &&
					((InBoard(x - dx[i], y - dy[i]) && board[x - dx[i]][y - dy[i]] == oppenent) ||
						!InBoard(x - dx[i], y - dy[i]))
				) ||
				(board[x + dx[i]][y + dy[i]] == NONE &&
					board[x + 2 * dx[i]][y + 2 * dy[i]] == color &&
					board[x + 3 * dx[i]][y + 3 * dy[i]] == color &&
					board[x + 4 * dx[i]][y + 4 * dy[i]] == NONE &&
					((InBoard(x - dx[i], y - dy[i]) && board[x - dx[i]][y - dy[i]] == oppenent) ||
						!InBoard(x - dx[i], y - dy[i]))
				)
			)
		) {
			score += MIANSAN;
		}
		// 活二
		else if (FiveInBoard(x - dx[i], y - dy[i], i) &&
			((board[x + dx[i]][y + dy[i]] == color &&
				board[x + 2 * dx[i]][y + 2 * dy[i]] == NONE &&
				board[x + 3 * dx[i]][y + 3 * dy[i]] == NONE &&
				board[x - dx[i]][y - dy[i]] == NONE
				) ||
				(board[x + dx[i]][y + dy[i]] == NONE &&
					board[x + 2 * dx[i]][y + 2 * dy[i]] == color &&
					board[x + 3 * dx[i]][y + 3 * dy[i]] == NONE &&
					board[x - dx[i]][y - dy[i]] == NONE
					))
			) {
			score += HUOER;
		}
		// 眠二
		else if (FiveInBoard(x, y, i) &&
			((board[x + dx[i]][y + dy[i]] == color &&
			board[x + 2 * dx[i]][y + 2 * dy[i]] == NONE &&
			board[x + 3 * dx[i]][y + 3 * dy[i]] == NONE &&
			board[x + 4 * dx[i]][y + 4 * dy[i]] == NONE &&
			((InBoard(x - dx[i], y - dy[i]) && board[x - dx[i]][y - dy[i]] == oppenent) ||
			!InBoard(x - dx[i], y - dy[i]))
			) ||
			(board[x + dx[i]][y + dy[i]] == NONE &&
			board[x + 2 * dx[i]][y + 2 * dy[i]] == color &&
			board[x + 3 * dx[i]][y + 3 * dy[i]] == NONE &&
			board[x + 4 * dx[i]][y + 4 * dy[i]] == NONE &&
			((InBoard(x - dx[i], y - dy[i]) && board[x - dx[i]][y - dy[i]] == oppenent) ||
			!InBoard(x - dx[i], y - dy[i]))
			) ||
			(board[x + dx[i]][y + dy[i]] == NONE &&
			board[x + 2 * dx[i]][y + 2 * dy[i]] == NONE &&
			board[x + 3 * dx[i]][y + 3 * dy[i]] == color &&
			board[x + 4 * dx[i]][y + 4 * dy[i]] == NONE &&
			((InBoard(x - dx[i], y - dy[i]) && board[x - dx[i]][y - dy[i]] == oppenent) ||
			!InBoard(x - dx[i], y - dy[i]))
			) ||
			(board[x + dx[i]][y + dy[i]] == NONE &&
			board[x + 2 * dx[i]][y + 2 * dy[i]] == NONE &&
			board[x + 3 * dx[i]][y + 3 * dy[i]] == NONE &&
			board[x + 4 * dx[i]][y + 4 * dy[i]] == color
			))
		) {
			score += MIANER;
		}
		// 死四
		else if (InBoard(x + dx[i], y + dy[i]) && InBoard(x + 2 * dx[i], y + 2 * dy[i]) && InBoard(x + 3 * dx[i], y + 3 * dy[i]) &&
			board[x + dx[i]][y + dy[i]] == color && board[x + 2 * dx[i]][y + 2 * dy[i]] == color && board[x + 3 * dx[i]][y + 3 * dy[i]] == color &&
			((InBoard(x + 4 * dx[i], y + 4 * dy[i]) && board[x + 4 * dx[i]][y + 4 * dy[i]] == oppenent) || (!InBoard(x + 4 * dx[i], y + 4 * dy[i]))) &&
			((InBoard(x - dx[i], y - dy[i]) && board[x - dx[i]][y - dy[i]] == oppenent) || (!InBoard(x - dx[i], y - dy[i])))
			) {
			score += SISI;
		}
		// 死三
		else if (InBoard(x + dx[i], y + dy[i]) && InBoard(x + 2 * dx[i], y + 2 * dy[i]) &&
			board[x + dx[i]][y + dy[i]] == color && board[x + 2 * dx[i]][y + 2 * dy[i]] == color &&
			((InBoard(x + 3 * dx[i], y + 3 * dy[i]) && board[x + 3 * dx[i]][y + 3 * dy[i]] == oppenent) || (!InBoard(x + 3 * dx[i], y + 3 * dy[i]))) &&
			((InBoard(x - dx[i], y - dy[i]) && board[x - dx[i]][y - dy[i]] == oppenent) || (!InBoard(x - dx[i], y - dy[i])))
			) {
			score += SISAN;
		}
		// 死二
		else if (InBoard(x + dx[i], y + dy[i]) &&
			board[x + dx[i]][y + dy[i]] == color &&
			((InBoard(x + 2 * dx[i], y + 2 * dy[i]) && board[x + 2 * dx[i]][y + 2 * dy[i]] == oppenent) || (!InBoard(x + 2 * dx[i], y + 2 * dy[i]))) &&
			((InBoard(x - dx[i], y - dy[i]) && board[x - dx[i]][y - dy[i]] == oppenent) || (!InBoard(x - dx[i], y - dy[i])))
			) {
			score += SIER;
		}
	}

	score += PosValue[x][y];
	return score;
}

int Utility() {
	int aiScore = 0, playerScore = 0;

	for (int i = 0; i < row; i++) {
		for (int j = 0; j < col; j++) {
			if (board[i][j] == BLACK) {
				aiScore += Score(i, j);
			}
			else if (board[i][j] == WHITE) {
				playerScore += Score(i, j);
			}
		}
	}
	return aiScore - playerScore;
}

int MinValue(Move lastMove, int alpha, int beta, int depth) {
	if (isTerminal(lastMove) == true || depth == 0) {
		return Utility();
	}
	int minValue = INT_MAX;
	Move test;
	for (int i = 0; i < row; i++) {
		for (int j = 0; j < col; j++) {
			if (board[i][j] == NONE) {
				test.x = i;
				test.y = j;
				DoMove(test, WHITE);
				minValue = min(minValue, MaxValue(test, alpha, beta, depth - 1));
				unDoMove(test);
				if (minValue <= alpha) {
					return minValue;
				}
				beta = min(beta, minValue);
			}
		}
	}
	return minValue;
}

int MaxValue(Move lastMove, int alpha, int beta, int depth) {
	if (isTerminal(lastMove) == true || depth == 0) {
		return Utility();
	}
	int maxValue = INT_MIN;
	Move test;
	for (int i = 0; i < row; i++) {
		for (int j = 0; j < col; j++) {
			if (board[i][j] == NONE) {
				test.x = i;
				test.y = j;
				DoMove(test, BLACK);
				maxValue = max(maxValue, MinValue(test, alpha, beta, depth - 1));
				unDoMove(test);
				if (maxValue >= beta) {
					return maxValue;
				}
				alpha = max(alpha, maxValue);
			}
		}
	}
	return maxValue;
}


int AlphaBetaDecision(Move &ai) {
	int nowValue = INT_MIN;
	int testValue;
	Move test;
	int alpha = INT_MIN;
	int beta = INT_MAX;
	for (int i = 0; i < row; i++) {
		for (int j = 0; j < col; j++) {
			if (board[i][j] == NONE) {
				test.x = i;
				test.y = j;
				DoMove(test, BLACK);
				testValue = MinValue(test, alpha, beta, MAXDEPTH);
				unDoMove(test);
				if (nowValue < testValue) {
					nowValue = testValue;
					ai = test;
				}
			}
		}
	}
	board[ai.x][ai.y] = BLACK;
	return 0;
}

void Show(bool isAi) {
	printf("\n\n");
	if (isAi) {
		printf("Ai的回合\n");
	}
	else {
		printf("玩家的回合\n");
	}
	printf("0表示空，1表示黑棋，2表示白棋\n");
	printf(" ");
	for (int k = 0; k < row; k++) {
		printf("%3d", k);
	}
	printf("\n");
	for (int i = 0; i < row; i++) {
		printf("%c", 'A' + i);
		for (int j = 0; j < col; j++) {
			if (board[i][j] == NONE) {
				printf("  .");
			}
			else {
				printf("%3d", board[i][j]);
			}
		}
		printf("\n");
	}
	return;
}

// return 0 for normal exit
int Input(Move &player) {
	printf("-----------------------------\n");
	printf("请输入落子位置\n");
	char inrow;
	int incol;
	cin >> inrow >> incol;
	int x, y;
	x = inrow - 'A';
	y = incol;

	// check
	if (!InBoard(x, y)) {
		printf("落子位置在棋盘外!\n");
		return -1;
	}
	else if (board[x][y] != NONE) {
		printf("落子位置非空!\n");
		return -1;
	}
	else {
		board[x][y] = WHITE;

		player.x = x;
		player.y = y;
		return 0;
	}
}

void GameOn() {
	printf("AI执黑先行\n");
	printf("局面效用值越高越利于Ai，越低越利于玩家\n");
	bool isAi = true;
	while (true) {
		Move nowMove;
		Show(isAi);
		if (isAi) {
			AlphaBetaDecision(nowMove);
			printf("Ai落子在%c %d\n", nowMove.x + 'A', nowMove.y);
			printf("当前局面效用值:%d", Utility());
		}
		else {
			while (Input(nowMove) == -1);
			printf("玩家落子在%c %d\n", nowMove.x + 'A', nowMove.y);
			printf("当前局面效用值:%d", Utility());
		}
		
		if (isTerminal(nowMove)) {
			Show(isAi);
			if (isAi) {
				printf("AI Wins!\n");
				break;
			}
			else {
				printf("You Win!\n");
				break;
			}
		}

		isAi = !isAi;
	}
}

int main()
{
	GameOn();
	system("pause");
	return 0;
}


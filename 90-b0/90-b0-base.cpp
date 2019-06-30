//1850059 计1班 杨志远

#include "90-b0.h"

static bool check_instruction(int map[][DefaultMaxCol + 1], char instruction[], const char HelpInformation[], int mode, int &X, int &Y, int realRow, int realCol);

//输入指令（例如：c4）
void input_instruction(int mode, int map[][DefaultMaxCol + 1], const char HelpInformation[][DefaultMaxInfoLength], int &sX, int &sY, int &eX, int &eY, int realRow, int realCol)
{
	char instructionA[5] = "", instructionB[5] = "";
	char tempStr[200] = "";
	while (1)
	{
		input_information((int)strlen(HelpInformation[4]), 4, mode, HelpInformation, tempStr, 0, 0, instructionA);
		if (!check_instruction(map, instructionA, HelpInformation[0], 0, sX, sY, realRow, realCol))
			continue;
		else
			cout << "输入为" << char(sY + 'A') << "行" << (sX)+1 << "列          " << endl;
		break;
	}
	while (1)
	{
		input_information((int)strlen(HelpInformation[5]), 5, mode, HelpInformation, tempStr, 0, 0, instructionB);
		if (!check_instruction(map, instructionB, HelpInformation[0], 1, eX, eY, realRow, realCol))
			continue;
		else
			cout << "输入为" << char(eY + 'A') << "行" << (eX)+1 << "列          " << endl;
		break;
	}
}

void mode_2(int map[][MaxCol + 1], int row, int col)
{
	int pathX[MaxPath], pathY[MaxPath], steps = 0, sX = -1, sY = -1, eX = -1, eY = -1, ballsColor[BallsEachStep];
	set_balls_color(ballsColor, MaxColors, BallsEachStep);
	create_balls(map, row, col, BallsEachStep, MaxColors, ballsColor);
	print_balls_color(ballsColor, BallsEachStep);
	input_instruction(2, map, HelpInformation ,sX, sY, eX, eY, row, col);
	steps = find_path(row, col, sX, sY, eX, eY, map, pathX, pathY); 
	if (steps == 0)
		cout << "无法找到路径              " << endl;
	else
	{
		cout << endl << endl; 
		draw_path(map, row, col, pathX, pathY, steps);
		cout << endl;
		print_map_path(map, row, col, pathX, pathY, steps);
	}
}

void mode_3(int map[][MaxCol + 1], int row, int col)
{
	int pathX[MaxPath], pathY[MaxPath], ballsColor[BallsEachStep], steps = 0, sX = -1, sY = -1, eX = -1, eY = -1, scoreAll = 0, score = 0, ballsX[MaxColors], ballsY[MaxColors];
	char instructionA[5] = "", instructionB[5] = "";
	set_balls_color(ballsColor, MaxColors, BallsEachStep);
	while (!game_over(map, row, col))
	{
		input_instruction(3, map, HelpInformation, sX, sY, eX, eY, row, col);
		steps = find_path(row, col, sX, sY, eX, eY, map, pathX, pathY);
		if (steps == 0)
			cout << "无法找到路径              " << endl;
		else
		{
			move_ball(map, eX, eY, sX, sY);
			if (check_map(map, row, col, eX, eY, score, scoreAll) == 0)
			{
				set_balls_color(ballsColor, MaxColors, BallsEachStep);
				create_balls(map, row, col, BallsEachStep, MaxColors, false, ballsColor, ballsX, ballsY);
				for(int i = 0; i < BallsEachStep; i++)
					check_map(map, row, col, ballsX[i], ballsY[i], score, scoreAll);
			}
		}
		print_map_with_info(map, row, col, "移动后的数组：");
		cout << "本次得分: " << score << "  总得分: " << scoreAll << endl;
		print_map_with_info(map, row, col, "当前数组：");
		print_balls_color(ballsColor, BallsEachStep);
	
	}
	cout << endl << "游戏结束" << endl;
}

void draw_path(int map[][MaxCol + 1], int row, int col, int pathX[], int pathY[], int steps)
{
	int temp[MaxRow + 1][MaxCol + 1] = { 0 };
	for (int k = 0; k < steps; k++)
		temp[pathY[k]][pathX[k]] = -1;
	cout << "查找结果数组：" << endl;
	print_map(map, row, col, 1, temp);
}

void print_map_path(int map[][MaxCol + 1], int row, int col, int pathX[], int pathY[], int steps)
{
	int temp[MaxRow + 1][MaxCol + 1] = { 0 };
	for (int k = 0; k < steps; k++)
		temp[pathY[k]][pathX[k]] = -1;
	cout << "移动路径(不同色标识)：" << endl;
	print_map(map, row, col, 2, temp);
}

void print_balls_color(int ballsColor[], int colors)
{
	cout << "之后" << BallsEachStep << "个球的颜色:";
	for (int i = 0; i < colors; i++)
		cout << " " << ballsColor[i];
	cout << endl;
}

//mode:   偶数->起点   奇数->终点
static bool check_instruction(int map[][DefaultMaxCol + 1], char instruction[], const char HelpInformation[], int mode, int &X, int &Y, int realRow, int realCol)
{
	int x, y;
	getxy(x, y);
	instruction[0] = (instruction[0] >= 'A' && instruction[0] <= 'Z') ? instruction[0] - 'A' + 'a' : instruction[0];
	if (strlen(instruction) != 2 || instruction[0] < 'a' || instruction[0] > 'a' + realRow - 1 || instruction[1] < '1' || instruction[1] > '1' + realCol - 1)
	{
		gotoxy(0, y);
		cout << HelpInformation << endl;
		gotoxy(0, y - 1);
		return false;
	}
	X = instruction[1] - '1';
	Y = instruction[0] - 'a';
	if ((mode % 2 == 0 && map[Y][X] == 0) || (mode % 2 == 1 && map[Y][X] != 0))
	{
		cout << (mode % 2 == 0 ? "起点坐标错误，请重新输入    " : "终点坐标错误，请重新输入    ") << endl;
		return false;
	}
	return true;
}

//1850059 ��1�� ��־Զ

#include "90-b0.h"

static bool check_instruction(int map[][DefaultMaxCol + 1], char instruction[], const char HelpInformation[], int mode, int &X, int &Y, int realRow, int realCol);

//����ָ����磺c4��
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
			cout << "����Ϊ" << char(sY + 'A') << "��" << (sX)+1 << "��          " << endl;
		break;
	}
	while (1)
	{
		input_information((int)strlen(HelpInformation[5]), 5, mode, HelpInformation, tempStr, 0, 0, instructionB);
		if (!check_instruction(map, instructionB, HelpInformation[0], 1, eX, eY, realRow, realCol))
			continue;
		else
			cout << "����Ϊ" << char(eY + 'A') << "��" << (eX)+1 << "��          " << endl;
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
		cout << "�޷��ҵ�·��              " << endl;
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
			cout << "�޷��ҵ�·��              " << endl;
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
		print_map_with_info(map, row, col, "�ƶ�������飺");
		cout << "���ε÷�: " << score << "  �ܵ÷�: " << scoreAll << endl;
		print_map_with_info(map, row, col, "��ǰ���飺");
		print_balls_color(ballsColor, BallsEachStep);
	
	}
	cout << endl << "��Ϸ����" << endl;
}

void draw_path(int map[][MaxCol + 1], int row, int col, int pathX[], int pathY[], int steps)
{
	int temp[MaxRow + 1][MaxCol + 1] = { 0 };
	for (int k = 0; k < steps; k++)
		temp[pathY[k]][pathX[k]] = -1;
	cout << "���ҽ�����飺" << endl;
	print_map(map, row, col, 1, temp);
}

void print_map_path(int map[][MaxCol + 1], int row, int col, int pathX[], int pathY[], int steps)
{
	int temp[MaxRow + 1][MaxCol + 1] = { 0 };
	for (int k = 0; k < steps; k++)
		temp[pathY[k]][pathX[k]] = -1;
	cout << "�ƶ�·��(��ͬɫ��ʶ)��" << endl;
	print_map(map, row, col, 2, temp);
}

void print_balls_color(int ballsColor[], int colors)
{
	cout << "֮��" << BallsEachStep << "�������ɫ:";
	for (int i = 0; i < colors; i++)
		cout << " " << ballsColor[i];
	cout << endl;
}

//mode:   ż��->���   ����->�յ�
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
		cout << (mode % 2 == 0 ? "��������������������    " : "�յ������������������    ") << endl;
		return false;
	}
	return true;
}

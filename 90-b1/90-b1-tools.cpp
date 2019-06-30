//1850059 计1班 杨志远

#include "90-b1.h"

//根据输入的模式mode计算对应的窗口大小
void calc_console_border(int mode, int row, int col, int &X, int &Y)
{
	if (mode == tolower(ModeInfomation[3]))
	{
		X = WindowXSize2 - (MaxCol - col);
		Y = WindowYSize2 - (MaxRow - row);
	}
	else if (mode == tolower(ModeInfomation[4]) || mode == tolower(ModeInfomation[5]) || mode == tolower(ModeInfomation[6]) || mode == tolower(ModeInfomation[7]))
	{
		X = WindowXSize3 - (MaxCol - col) + 1;
		Y = WindowYSize3 - (MaxRow - row) * 2;
	}
	else
	{
		X = WindowXSize1;
		Y = WindowYSize1;
	}
}

//计算可消除的彩球的数量，彩球的位置记录在erasableArray中
int erasable_count(int map[][DefaultMaxCol + 1], int row, int col, int erasableArray[][DefaultMaxCol + 1])
{
	int result = 0;
	if (erasableArray != NULL)
		memset(erasableArray, 0, sizeof(int) * (MaxCol + 1) * (MaxRow + 1));
	for (int i = 0; i < row; i++)
	{
		for (int j = 0; j < col; j++)
		{
			if (j + 2 < col && map[i][j] == map[i][j + 1] && map[i][j] == map[i][j + 2])
			{
				if (erasableArray == NULL)
					return -1;
				result += bool(!erasableArray[i][j]);
				erasableArray[i][j] = map[i][j];
				result += bool(!erasableArray[i][j + 1]);
				erasableArray[i][j + 1] = map[i][j + 1];
				result += bool(!erasableArray[i][j + 2]);
				erasableArray[i][j + 2] = map[i][j + 2];
			}
			if (i + 2 < row && map[i][j] == map[i + 1][j] && map[i][j] == map[i + 2][j])
			{
				if (erasableArray == NULL)
					return -1;
				result += bool(!erasableArray[i][j]);
				erasableArray[i][j] = map[i][j];
				result += bool(!erasableArray[i + 1][j]);
				erasableArray[i + 1][j] = map[i + 1][j];
				result += bool(!erasableArray[i + 2][j]);
				erasableArray[i + 2][j] = map[i + 2][j];
			}
		}
	}
	return result;
}

//判断两个相邻小球是否可以在互换后发生消除（不判断是否相邻），如果传入了erasableArray，那就将这两个小球记录在erasableArray中
bool exchangable(int map[][DefaultMaxCol + 1], int row, int col, int X1, int Y1, int X2, int Y2, int erasableArray[][DefaultMaxCol + 1])
{
	bool result = false;
	exchange(map[Y1][X1], map[Y2][X2]);
	if (erasable_count(map, row, col))
	{
		result = true;
		if (erasableArray)
		{
			erasableArray[Y1][X1] = map[Y1][X1];
			erasableArray[Y2][X2] = map[Y2][X2];
		}
	}
	exchange(map[Y1][X1], map[Y2][X2]);
	return result;
}

//判断游戏是否结束
bool game_over(int map[][DefaultMaxCol + 1], int row, int col)
{
	for (int i = 0; i < row; i++)
	{
		for (int j = 0; j < col; j++)
		{
			if (i < row - 1 && exchangable(map, row, col, j, i, j, i + 1))
			{
				return false;
			}
			if (j < col - 1 && exchangable(map, row, col, j, i, j + 1, i))
			{
				return false;
			}
		}
	}
	return true;
}

//计算分数，将行数和列数的平均值作为基数
int calc_score(int row, int col, int erasableBalls, const int base[])
{
	return base[int((row + col) / 2)] * erasableBalls;
}
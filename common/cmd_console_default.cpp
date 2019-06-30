//1850059 计1班 杨志远

#include "cmd_console_default.h"
#include <iostream>
#include <stdio.h>
#include <string.h>
#include <conio.h>
#include <ctype.h>
#include <io.h> //读取文件夹内容的相关函数

//等待输入回车或其他字符
void wait_enter(const char ch)
{
	while (_getch() != ch)
		;
}

//重载，等待输入回车或其他字符，支持多个字符（注意大小写）
char wait_enter(const char *str)
{
	char ch;
	while (ch = _getch())
	{
		for (int i = 0; str[i] != '\0'; i++)
		{
			if (ch == str[i])
				return str[i];
		}
	}
	return 0;
}

//移动小球并置零
void move_ball(int map[][DefaultMaxCol + 1], int dX, int dY, int sX, int sY)
{
	map[dY][dX] = map[sY][sX];
	map[sY][sX] = 0;
}

//设置colors个小球的颜色，在1~maxColors范围之间
void set_balls_color(int ballsColor[], int maxColors, int colors)
{
	for (int i = 0; i < colors; i++)
		ballsColor[i] = rand() % maxColors + 1;
}

//判断地图中是否还有空位
bool map_is_full(const int map[][DefaultMaxCol + 1], int row, int col)
{
	for (int i = 0; i < row; i++)
		for (int j = 0; j < col; j++)
		{
			if (map[i][j] == 0)
				return false;
		}
	return true;
}

//重置，判断地图中是否还有空位
bool map_is_full(const int map[][10], int row, int col, bool (*compare)(int))
{
	for (int i = 0; i < row; i++)
		for (int j = 0; j < col; j++)
		{
			if (compare(map[i][j]) == false)
				return false;
		}
	return true;
}

//创造多个小球，如果提供了ballsColor参数，那么可以用ballsColor中的数据修改map、在ballsX和ballsY中保存多个随机的color信息，否则直接使用随机的color数据修改map
void create_balls(int map[][DefaultMaxCol + 1], int row, int col, int balls, int maxColors, bool sequent, int ballsColor[], int ballsX[], int ballsY[])
{
	int x, y, cur = 0;
	for (int i = 0; i < balls; i++)
	{
		if (sequent)
		{
			x = cur / row;
			y = cur++ % row;
			if (map[y][x] != 0)
			{
				i--;
				continue;
			}
		}
		else
		{
			do
			{
				y = rand() % row;
				x = rand() % col;
			} while (map[y][x] != 0 && !map_is_full(map, row, col));
		}
		map[y][x] = (ballsColor != NULL) ? ballsColor[i] : rand() % maxColors + 1;
		if (ballsY != NULL)
		{
			ballsX[i] = x;
			ballsY[i] = y;
		}
	}
}

//判断某个（按照地图位置整理完的）小球坐标是否在地图中
bool in_map(int row, int col, int x, int y)
{
	if (x < 0 || x >= col || y < 0 || y >= row)
		return false;
	else
		return true;
}

//判断某个坐标是否对应地图上的某个小球
bool is_point_xy(int X, int Y, int row, int col, bool mark)
{
	X -= mark * 2;
	Y -= mark;
	int mapX = (X - mark) / 4;
	int mapY = (Y - mark) / 2;
	if (mapX >= col || mapY >= row || (fabs(((double)X - 2) / 4 - mapX) > (0.25 + 1e-4)) || (fabs((double)Y - 2) / 2 - mapY) > 1e-4)
		return false;
	return true;
}

//判断某点是否对应地图上的某个小球，并计算对应的彩球（不论判断结果如何）
bool get_point_xy(int X, int Y, int &mapX, int &mapY, int row, int col, bool mark)
{
	mapX = (X - 2 - mark) / 4;
	mapY = (Y - 2 - mark) / 2;
	return is_point_xy(X, Y, row, col, mark);
}

//判断某个字符是否是汉字（注意：只允许判断汉字的第一个字符）
bool is_Chinese_character(const char ch)
{
	return ((unsigned int)(ch) > 0x7F);
}

//获取文件大小
long get_file_size(const char filePath[])
{
	if (filePath == NULL)
		return -1;
	struct _finddata_t file;
	long HANDLE, fileSize = -1;
	HANDLE = (long)_findfirst(filePath, &file);
	fileSize = file.size;
	_findclose(HANDLE);
	return fileSize;
}

//将某个字符串处理为限定长度下没有半个字符问题的字符串
bool convert_half_character(char buf[], const int len)
{
	int chineseCh = 0, index;
	for (int i = 0; i < len && buf[i] != '\0'; i++)
	{
		if ((unsigned int)(buf[i]) > 0x7F) //汉字
		{
			chineseCh++;
			index = i;
		}
	}
	if (chineseCh % 2 && index >= len - 1) //处理问题
	{
		buf[index] = ' ';
		return true;
	}
	else //不需要处理
	{
		return false;
	}
}

//判断是不是学生学号
bool is_stu_no(const int NO)
{
	if (NO >= 1000000 && NO <= 9999999)
		return true;
	else
		return false;
}

//重载，判断是不是学生学号
bool is_stu_no(const char *const str)
{
	//int len = 0;
	//for (len = 0; str[len] != '\0'; len++)
	//{
	//	if (!isdigit(str[len]))
	//		return false;
	//}
	//if (len == 7)
	//	return true;
	//else
	//	return false;
	return is_stu_no(atoi(str));
}

//十六进制数字转为一个字符
char hex_to_char(const int n)
{
	if (n >= 0 && n <= 9)
		return '0' + n;
	else if (n >= 10 && n <= 15)
		return 'a' + (n - 10);
	else
		return 'X';
}
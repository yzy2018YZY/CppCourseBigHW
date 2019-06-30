//1850059 计1班 杨志远

#pragma once
#define _CRT_SECURE_NO_WARNINGS
#include "cmd_console_tools.h"
using namespace std;

const int DefaultMaxCol = 9;          //默认的最大列数，所有用户选择的行列数必须小于等于DefinedMaxCol
const int DefaultMaxInfoLength = 80;  //提示信息字符串长度

//交换两个变量
template<typename T>
void exchange(T &x, T &y)
{
	T temp = x;
	x = y;
	y = temp;
}

template<typename T>
T my_min(const T &x, const T &y)
{
	return (x < y) ? x : y;
}

template<typename T>
T my_max(const T &x, const T &y)
{
	return (x > y) ? x : y;
}

void wait_enter(const char ch = '\r');

char wait_enter(const char *str);

void move_ball(int map[][DefaultMaxCol + 1], int dX, int dY, int sX, int sY);

void set_balls_color(int ballsColor[], int maxColors, int colors);

bool map_is_full(const int map[][DefaultMaxCol + 1], int row, int col);

//判断地图中是否还有空位
template <unsigned int N>
bool map_is_full(const int map[][N], int row, int col, bool(*compare)(int))
{
	for (int i = 0; i < row; i++)
		for (int j = 0; j < col; j++)
		{
			if (compare(map[i][j]) == false)
				return false;
		}
	return true;
}

void create_balls(int map[][DefaultMaxCol + 1], int row, int col, int balls, int maxColors, bool sequent = false, int ballsColor[] = NULL, int ballsX[] = NULL, int ballsY[] = NULL);

bool in_map(int row, int col, int x, int y);

bool is_Chinese_character(const char ch);

long get_file_size(const char filePath[]);

bool convert_half_character(char buf[], const int len);

bool is_stu_no(const int NO);

bool is_stu_no(const char *const str);

char hex_to_char(const int n);

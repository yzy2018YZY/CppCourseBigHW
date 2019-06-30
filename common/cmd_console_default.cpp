//1850059 ��1�� ��־Զ

#include "cmd_console_default.h"
#include <iostream>
#include <stdio.h>
#include <string.h>
#include <conio.h>
#include <ctype.h>
#include <io.h> //��ȡ�ļ������ݵ���غ���

//�ȴ�����س��������ַ�
void wait_enter(const char ch)
{
	while (_getch() != ch)
		;
}

//���أ��ȴ�����س��������ַ���֧�ֶ���ַ���ע���Сд��
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

//�ƶ�С������
void move_ball(int map[][DefaultMaxCol + 1], int dX, int dY, int sX, int sY)
{
	map[dY][dX] = map[sY][sX];
	map[sY][sX] = 0;
}

//����colors��С�����ɫ����1~maxColors��Χ֮��
void set_balls_color(int ballsColor[], int maxColors, int colors)
{
	for (int i = 0; i < colors; i++)
		ballsColor[i] = rand() % maxColors + 1;
}

//�жϵ�ͼ���Ƿ��п�λ
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

//���ã��жϵ�ͼ���Ƿ��п�λ
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

//������С������ṩ��ballsColor��������ô������ballsColor�е������޸�map����ballsX��ballsY�б����������color��Ϣ������ֱ��ʹ�������color�����޸�map
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

//�ж�ĳ�������յ�ͼλ��������ģ�С�������Ƿ��ڵ�ͼ��
bool in_map(int row, int col, int x, int y)
{
	if (x < 0 || x >= col || y < 0 || y >= row)
		return false;
	else
		return true;
}

//�ж�ĳ�������Ƿ��Ӧ��ͼ�ϵ�ĳ��С��
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

//�ж�ĳ���Ƿ��Ӧ��ͼ�ϵ�ĳ��С�򣬲������Ӧ�Ĳ��򣨲����жϽ����Σ�
bool get_point_xy(int X, int Y, int &mapX, int &mapY, int row, int col, bool mark)
{
	mapX = (X - 2 - mark) / 4;
	mapY = (Y - 2 - mark) / 2;
	return is_point_xy(X, Y, row, col, mark);
}

//�ж�ĳ���ַ��Ƿ��Ǻ��֣�ע�⣺ֻ�����жϺ��ֵĵ�һ���ַ���
bool is_Chinese_character(const char ch)
{
	return ((unsigned int)(ch) > 0x7F);
}

//��ȡ�ļ���С
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

//��ĳ���ַ�������Ϊ�޶�������û�а���ַ�������ַ���
bool convert_half_character(char buf[], const int len)
{
	int chineseCh = 0, index;
	for (int i = 0; i < len && buf[i] != '\0'; i++)
	{
		if ((unsigned int)(buf[i]) > 0x7F) //����
		{
			chineseCh++;
			index = i;
		}
	}
	if (chineseCh % 2 && index >= len - 1) //��������
	{
		buf[index] = ' ';
		return true;
	}
	else //����Ҫ����
	{
		return false;
	}
}

//�ж��ǲ���ѧ��ѧ��
bool is_stu_no(const int NO)
{
	if (NO >= 1000000 && NO <= 9999999)
		return true;
	else
		return false;
}

//���أ��ж��ǲ���ѧ��ѧ��
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

//ʮ����������תΪһ���ַ�
char hex_to_char(const int n)
{
	if (n >= 0 && n <= 9)
		return '0' + n;
	else if (n >= 10 && n <= 15)
		return 'a' + (n - 10);
	else
		return 'X';
}
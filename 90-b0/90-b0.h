//1850059 ��1�� ��־Զ

#pragma once
#include "../common/cmd_console_base.h"
#include "../common/cmd_console_graphics.h"
#include "../common/cmd_gmw_tools.h"
#include <time.h>
using namespace std;

const int MinRow = 7;			//������С��
const int MinCol = 7;			//������С��
const int MaxRow = 9;			//���������
const int MaxCol = 9;			//���������
const int MaxColors = 7;		//��ɫ���ֵ
const int FontSize1 = 16;		//ģʽ1-3�����С
const int FontSize2 = 24;		//ģʽ4-8�����С
const int WindowXSize1 = 80;	//��ʼ���ں����С
const int WindowYSize1 = 26;	//��ʼ���������С
const int WindowXSize2 = 35;	//ģʽ4���ں����С
const int WindowYSize2 = 13;	//ģʽ4���������С
const int WindowXSize3 = 40;	//ģʽ5���ں����С
const int WindowYSize3 = 19;	//ģʽ5���������С
const int MaxPath = MaxRow * MaxCol * 2;	//Ѱ·�㷨���洢�ĵ�
const int BallsEachStep = 3;	//ÿ�غ����ӵĲ�����
const int MinBallsCount = 5;	//�����ߵ���С������
const int DefaultStringLength = 256;
const char ConsoleTitle[] = "������Ϸ-1850059-��1��-��־Զ";
const char FontName[] = "������";
const int DelayOfMoveBlock = 25;
const int DelayOfDrawFrame = 0;
const int DelayOfDrawBlock = 0;

//����ʱ����ʾ��Ϣ����0��Ϊ������ʾ
const char HelpInformation[][DefaultMaxInfoLength] =
{
	"�����������������", 
	"����������(7-9): ",
	"����������(7-9): ", 
	"����End����: ", 
	"������ĸ+������ʽ[����c2]����Ҫ�ƶ������ʼ���꣺", 
	"������ĸ+������ʽ[����c2]����Ҫ�ƶ����Ŀ�����꣺"
};

//��ʼ�������Ϣ
extern const char *WelcomeInfomation[];

//��ʼ�����ѡ��
const char ModeInfomation[] = "123456780";

const int BDI_SELECTED = 100;
const BLOCK_DISPLAY_INFO BDI[] = {
		{BDI_VALUE_BLANK, -1, -1, "  "},  //0����ʾ���ÿո���伴��
		{1, 1, COLOR_HWHITE, "��"},
		{2, 2, COLOR_HWHITE, "��"},
		{3, 3, COLOR_HWHITE, "��"},
		{4, 4, COLOR_HWHITE, "��"},
		{5, 5, COLOR_HWHITE, "��"},
		{6, 6, COLOR_HWHITE, "��"},
		{7, 7, COLOR_HWHITE, "��"},
		{8, 8, COLOR_HWHITE, "��"},
		{9, 9, COLOR_HWHITE, "��"},
		{1 + BDI_SELECTED, 1, COLOR_HWHITE, "��"},
		{2 + BDI_SELECTED, 2, COLOR_HWHITE, "��"},
		{3 + BDI_SELECTED, 3, COLOR_HWHITE, "��"},
		{4 + BDI_SELECTED, 4, COLOR_HWHITE, "��"},
		{5 + BDI_SELECTED, 5, COLOR_HWHITE, "��"},
		{6 + BDI_SELECTED, 6, COLOR_HWHITE, "��"},
		{7 + BDI_SELECTED, 7, COLOR_HWHITE, "��"},
		{8 + BDI_SELECTED, 8, COLOR_HWHITE, "��"},
		{9 + BDI_SELECTED, 9, COLOR_HWHITE, "��"},
		{BDI_VALUE_END, -1, -1, NULL} //�жϽ�������ΪcontentΪNULL��ǰ��-999����ν
};

//90-b2-tools
int find_path(int row, int col, int startX, int startY, int endX, int endY, int map[][MaxCol + 1], int *pointListX, int *pointListY);
bool game_over(int map[][MaxCol + 1], int row, int col);
int check_map(int map[][MaxCol + 1], int row, int col, int x, int y, int &score, int &scoreAll, bool consoleMode = false, int ballsRemoved[] = NULL);
int statistics_balls(int map[][MaxCol + 1], int row, int col, int color);

//90-b2-base
void draw_path(int map[][MaxCol + 1], int row, int col, int pathX[], int pathY[], int steps);
void print_map_path(int map[][MaxCol + 1], int row, int col, int pathX[], int pathY[], int steps);
void input_instruction(int mode, int map[][DefaultMaxCol + 1], const char HelpInformation[][DefaultMaxInfoLength], int &sX, int &sY, int &eX, int &eY, int realRow, int realCol);
void mode_2(int map[][MaxCol + 1], int row, int col);
void mode_3(int map[][MaxCol + 1], int row, int col);
void print_balls_color(int ballsColor[], int colors);

//90-b2-console
void mode_6(const CONSOLE_GRAPHICS_INFO * const pCGI, int map[][MaxCol + 1], int row, int col);
void mode_7_and_8(const CONSOLE_GRAPHICS_INFO * const pCGI, int map[][MaxCol + 1], int row, int col, int mode);

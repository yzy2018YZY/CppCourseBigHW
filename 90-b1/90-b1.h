//1850059 ��1�� ��־Զ

#pragma once
#include "../common/cmd_console_base.h"
#include "../common/cmd_console_graphics.h"
#include "../common/cmd_gmw_tools.h"
#include <time.h>
#include <fstream>
using namespace std;

const int MinRow = 5;			//������С��
const int MinCol = 5;			//������С��
const int MaxRow = 9;			//���������
const int MaxCol = 9;			//���������
const int MaxColors = 9;		//��ɫ���ֵ
const int FontSize1 = 16;		//ģʽ1-3�����С
const int FontSize2 = 28;		//ģʽ4-8�����С
const int WindowXSize1 = 80;	//��ʼ���ں����С
const int WindowYSize1 = 26;	//��ʼ���������С
const int WindowXSize2 = 35;	//ģʽ4���ں����С�����
const int WindowYSize2 = 17;	//ģʽ4���������С�����
const int WindowXSize3 = 41;	//ģʽ5���ں����С�����
const int WindowYSize3 = 25;	//ģʽ5���������С�����
const int Delay1 = 50;		//�����ӳ٣��Ե�һ����
const int Delay2 = 10;		//�����ӳ٣��Բ�ͬ����֮�䣩
const int DefaultStringLength = 256;
const char FontName[] = "������";
const char ConsoleTitle[] = "������Ϸ-1850059-��1��-��־Զ";

//����ʱ����ʾ��Ϣ����0��Ϊ������ʾ
const char HelpInformation[][DefaultMaxInfoLength] =
{
	"�����������������",
	"����������(5-9): ",
	"����������(5-9): ",
	"����End����: ",
	"������ĸ+������ʽ[����c2]����Ҫ�ƶ������ʼ���꣺",
	"������ĸ+������ʽ[����c2]����Ҫ�ƶ����Ŀ�����꣺"
};

//��ʼ�������Ϣ
extern const char *WelcomeInfomation[];

//��ʼ�����ѡ��
const char ModeInfomation[] = "ABCDEFGHIQ";

//��������Ļ���
const int base[MaxRow + 1] = {0, 0, 0, 0, 0, 12, 8, 5, 3, 1 };

const int BDI_EXCHANGABLE = 100;
const int BDI_ERASABLE = 200;
const int BDI_SELECTED = 300;
const BLOCK_DISPLAY_INFO BDI[] = {
		{BDI_VALUE_BLANK, -1, -1, "  "},  //0����ʾ���ÿո���伴��
		{1, 1, COLOR_BLACK, "��"},
		{2, 2, COLOR_BLACK, "��"},
		{3, 3, COLOR_BLACK, "��"},
		{4, 4, COLOR_BLACK, "��"},
		{5, 5, COLOR_BLACK, "��"},
		{6, 6, COLOR_BLACK, "��"},
		{7, 7, COLOR_BLACK, "��"},
		{8, 8, COLOR_BLACK, "��"},
		{9, 9, COLOR_BLACK, "��"},
		{1 + BDI_EXCHANGABLE, 1, COLOR_BLACK, "��"},
		{2 + BDI_EXCHANGABLE, 2, COLOR_BLACK, "��"},
		{3 + BDI_EXCHANGABLE, 3, COLOR_BLACK, "��"},
		{4 + BDI_EXCHANGABLE, 4, COLOR_BLACK, "��"},
		{5 + BDI_EXCHANGABLE, 5, COLOR_BLACK, "��"},
		{6 + BDI_EXCHANGABLE, 6, COLOR_BLACK, "��"},
		{7 + BDI_EXCHANGABLE, 7, COLOR_BLACK, "��"},
		{8 + BDI_EXCHANGABLE, 8, COLOR_BLACK, "��"},
		{9 + BDI_EXCHANGABLE, 9, COLOR_BLACK, "��"},
		{1 + BDI_ERASABLE, 1, COLOR_BLACK, "��"},
		{2 + BDI_ERASABLE, 2, COLOR_BLACK, "��"},
		{3 + BDI_ERASABLE, 3, COLOR_BLACK, "��"},
		{4 + BDI_ERASABLE, 4, COLOR_BLACK, "��"},
		{5 + BDI_ERASABLE, 5, COLOR_BLACK, "��"},
		{6 + BDI_ERASABLE, 6, COLOR_BLACK, "��"},
		{7 + BDI_ERASABLE, 7, COLOR_BLACK, "��"},
		{8 + BDI_ERASABLE, 8, COLOR_BLACK, "��"},
		{9 + BDI_ERASABLE, 9, COLOR_BLACK, "��"},
		{1 + BDI_SELECTED, 1, COLOR_BLACK, "��"},
		{2 + BDI_SELECTED, 2, COLOR_BLACK, "��"},
		{3 + BDI_SELECTED, 3, COLOR_BLACK, "��"},
		{4 + BDI_SELECTED, 4, COLOR_BLACK, "��"},
		{5 + BDI_SELECTED, 5, COLOR_BLACK, "��"},
		{6 + BDI_SELECTED, 6, COLOR_BLACK, "��"},
		{7 + BDI_SELECTED, 7, COLOR_BLACK, "��"},
		{8 + BDI_SELECTED, 8, COLOR_BLACK, "��"},
		{9 + BDI_SELECTED, 9, COLOR_BLACK, "��"},
		{BDI_VALUE_END, -1, -1, NULL} //�жϽ�������ΪcontentΪNULL��ǰ��-999����ν
};

//90-b1-tools
int erasable_count(int map[][DefaultMaxCol + 1], int row, int col, int erasableArray[][DefaultMaxCol + 1] = NULL);
void calc_console_border(int mode, int row, int col, int &X, int &Y);
bool exchangable(int map[][DefaultMaxCol + 1], int row, int col, int X1, int Y1, int X2, int Y2, int erasableArray[][DefaultMaxCol + 1] = NULL);
bool game_over(int map[][DefaultMaxCol + 1], int row, int col);
int calc_score(int row, int col, int erasableBalls, const int base[]);

//90-b1-base
void mode_1_2_3(int map[][DefaultMaxCol + 1], int row, int col, int erasableArray[][DefaultMaxCol + 1], int &erasableBalls, int mode);
void mode_9(int map[][DefaultMaxCol + 1], int erasableArray[][DefaultMaxCol + 1]);

//90-b1-console
void mode_4_5_6_7(CONSOLE_GRAPHICS_INFO * const pCGI, int map[][DefaultMaxCol + 1], int row, int col, int mode, int erasableArray[][DefaultMaxCol + 1], int &erasableBalls);
void mode_8(CONSOLE_GRAPHICS_INFO * const pCGI, int map[][DefaultMaxCol + 1], int row, int col, int mode, int erasableArray[][DefaultMaxCol + 1], int &erasableBalls);
void draw_erasable_mark(const CONSOLE_GRAPHICS_INFO * const pCGI, int map[][DefaultMaxCol + 1], int row, int col, int erasableArray[][DefaultMaxCol + 1]);
void draw_exchange_graphics(const CONSOLE_GRAPHICS_INFO * const pCGI, int map[][DefaultMaxCol + 1], int row, int col, int X1, int Y1, int X2, int Y2);
void erase_balls(const CONSOLE_GRAPHICS_INFO * const pCGI, int map[][DefaultMaxCol + 1], int row, int col, int erasableArray[][DefaultMaxCol + 1], bool graphics);
void fill_blanks(const CONSOLE_GRAPHICS_INFO * const pCGI, int map[][DefaultMaxCol + 1], int row, int col, int balls, bool graphics);
int mark_exchangable_balls(const CONSOLE_GRAPHICS_INFO * const pCGI, int map[][DefaultMaxCol + 1], int row, int col, int erasableArray[][DefaultMaxCol + 1], bool graphics);

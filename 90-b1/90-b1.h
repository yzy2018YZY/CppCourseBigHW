//1850059 计1班 杨志远

#pragma once
#include "../common/cmd_console_base.h"
#include "../common/cmd_console_graphics.h"
#include "../common/cmd_gmw_tools.h"
#include <time.h>
#include <fstream>
using namespace std;

const int MinRow = 5;			//棋盘最小行
const int MinCol = 5;			//棋盘最小列
const int MaxRow = 9;			//棋盘最大行
const int MaxCol = 9;			//棋盘最大列
const int MaxColors = 9;		//颜色最大值
const int FontSize1 = 16;		//模式1-3字体大小
const int FontSize2 = 28;		//模式4-8字体大小
const int WindowXSize1 = 80;	//初始窗口横向大小
const int WindowYSize1 = 26;	//初始窗口纵向大小
const int WindowXSize2 = 35;	//模式4窗口横向大小（最大）
const int WindowYSize2 = 17;	//模式4窗口纵向大小（最大）
const int WindowXSize3 = 41;	//模式5窗口横向大小（最大）
const int WindowYSize3 = 25;	//模式5窗口纵向大小（最大）
const int Delay1 = 50;		//动画延迟（对单一对象）
const int Delay2 = 10;		//动画延迟（对不同对象之间）
const int DefaultStringLength = 256;
const char FontName[] = "新宋体";
const char ConsoleTitle[] = "彩球游戏-1850059-计1班-杨志远";

//输入时的提示信息，第0个为错误提示
const char HelpInformation[][DefaultMaxInfoLength] =
{
	"输入错误，请重新输入",
	"请输入行数(5-9): ",
	"请输入列数(5-9): ",
	"输入End结束: ",
	"请以字母+数字形式[例：c2]输入要移动球的起始坐标：",
	"请以字母+数字形式[例：c2]输入要移动球的目的坐标："
};

//开始界面的信息
extern const char *WelcomeInfomation[];

//开始界面的选项
const char ModeInfomation[] = "ABCDEFGHIQ";

//计算分数的基数
const int base[MaxRow + 1] = {0, 0, 0, 0, 0, 12, 8, 5, 3, 1 };

const int BDI_EXCHANGABLE = 100;
const int BDI_ERASABLE = 200;
const int BDI_SELECTED = 300;
const BLOCK_DISPLAY_INFO BDI[] = {
		{BDI_VALUE_BLANK, -1, -1, "  "},  //0不显示，用空格填充即可
		{1, 1, COLOR_BLACK, "〇"},
		{2, 2, COLOR_BLACK, "〇"},
		{3, 3, COLOR_BLACK, "〇"},
		{4, 4, COLOR_BLACK, "〇"},
		{5, 5, COLOR_BLACK, "〇"},
		{6, 6, COLOR_BLACK, "〇"},
		{7, 7, COLOR_BLACK, "〇"},
		{8, 8, COLOR_BLACK, "〇"},
		{9, 9, COLOR_BLACK, "〇"},
		{1 + BDI_EXCHANGABLE, 1, COLOR_BLACK, "◎"},
		{2 + BDI_EXCHANGABLE, 2, COLOR_BLACK, "◎"},
		{3 + BDI_EXCHANGABLE, 3, COLOR_BLACK, "◎"},
		{4 + BDI_EXCHANGABLE, 4, COLOR_BLACK, "◎"},
		{5 + BDI_EXCHANGABLE, 5, COLOR_BLACK, "◎"},
		{6 + BDI_EXCHANGABLE, 6, COLOR_BLACK, "◎"},
		{7 + BDI_EXCHANGABLE, 7, COLOR_BLACK, "◎"},
		{8 + BDI_EXCHANGABLE, 8, COLOR_BLACK, "◎"},
		{9 + BDI_EXCHANGABLE, 9, COLOR_BLACK, "◎"},
		{1 + BDI_ERASABLE, 1, COLOR_BLACK, "●"},
		{2 + BDI_ERASABLE, 2, COLOR_BLACK, "●"},
		{3 + BDI_ERASABLE, 3, COLOR_BLACK, "●"},
		{4 + BDI_ERASABLE, 4, COLOR_BLACK, "●"},
		{5 + BDI_ERASABLE, 5, COLOR_BLACK, "●"},
		{6 + BDI_ERASABLE, 6, COLOR_BLACK, "●"},
		{7 + BDI_ERASABLE, 7, COLOR_BLACK, "●"},
		{8 + BDI_ERASABLE, 8, COLOR_BLACK, "●"},
		{9 + BDI_ERASABLE, 9, COLOR_BLACK, "●"},
		{1 + BDI_SELECTED, 1, COLOR_BLACK, "¤"},
		{2 + BDI_SELECTED, 2, COLOR_BLACK, "¤"},
		{3 + BDI_SELECTED, 3, COLOR_BLACK, "¤"},
		{4 + BDI_SELECTED, 4, COLOR_BLACK, "¤"},
		{5 + BDI_SELECTED, 5, COLOR_BLACK, "¤"},
		{6 + BDI_SELECTED, 6, COLOR_BLACK, "¤"},
		{7 + BDI_SELECTED, 7, COLOR_BLACK, "¤"},
		{8 + BDI_SELECTED, 8, COLOR_BLACK, "¤"},
		{9 + BDI_SELECTED, 9, COLOR_BLACK, "¤"},
		{BDI_VALUE_END, -1, -1, NULL} //判断结束条件为content为NULL，前面-999无所谓
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

//1850059 计1班 杨志远

#pragma once
#include "../common/cmd_console_base.h"
#include "../common/cmd_console_graphics.h"
#include "../common/cmd_gmw_tools.h"
#include <time.h>
using namespace std;

const int MinRow = 7;			//棋盘最小行
const int MinCol = 7;			//棋盘最小列
const int MaxRow = 9;			//棋盘最大行
const int MaxCol = 9;			//棋盘最大列
const int MaxColors = 7;		//颜色最大值
const int FontSize1 = 16;		//模式1-3字体大小
const int FontSize2 = 24;		//模式4-8字体大小
const int WindowXSize1 = 80;	//初始窗口横向大小
const int WindowYSize1 = 26;	//初始窗口纵向大小
const int WindowXSize2 = 35;	//模式4窗口横向大小
const int WindowYSize2 = 13;	//模式4窗口纵向大小
const int WindowXSize3 = 40;	//模式5窗口横向大小
const int WindowYSize3 = 19;	//模式5窗口纵向大小
const int MaxPath = MaxRow * MaxCol * 2;	//寻路算法最多存储的点
const int BallsEachStep = 3;	//每回合增加的彩球数
const int MinBallsCount = 5;	//连成线的最小彩球数
const int DefaultStringLength = 256;
const char ConsoleTitle[] = "彩球游戏-1850059-计1班-杨志远";
const char FontName[] = "新宋体";
const int DelayOfMoveBlock = 25;
const int DelayOfDrawFrame = 0;
const int DelayOfDrawBlock = 0;

//输入时的提示信息，第0个为错误提示
const char HelpInformation[][DefaultMaxInfoLength] =
{
	"输入错误，请重新输入", 
	"请输入行数(7-9): ",
	"请输入列数(7-9): ", 
	"输入End结束: ", 
	"请以字母+数字形式[例：c2]输入要移动球的起始坐标：", 
	"请以字母+数字形式[例：c2]输入要移动球的目的坐标："
};

//开始界面的信息
extern const char *WelcomeInfomation[];

//开始界面的选项
const char ModeInfomation[] = "123456780";

const int BDI_SELECTED = 100;
const BLOCK_DISPLAY_INFO BDI[] = {
		{BDI_VALUE_BLANK, -1, -1, "  "},  //0不显示，用空格填充即可
		{1, 1, COLOR_HWHITE, "〇"},
		{2, 2, COLOR_HWHITE, "〇"},
		{3, 3, COLOR_HWHITE, "〇"},
		{4, 4, COLOR_HWHITE, "〇"},
		{5, 5, COLOR_HWHITE, "〇"},
		{6, 6, COLOR_HWHITE, "〇"},
		{7, 7, COLOR_HWHITE, "〇"},
		{8, 8, COLOR_HWHITE, "〇"},
		{9, 9, COLOR_HWHITE, "〇"},
		{1 + BDI_SELECTED, 1, COLOR_HWHITE, "◎"},
		{2 + BDI_SELECTED, 2, COLOR_HWHITE, "◎"},
		{3 + BDI_SELECTED, 3, COLOR_HWHITE, "◎"},
		{4 + BDI_SELECTED, 4, COLOR_HWHITE, "◎"},
		{5 + BDI_SELECTED, 5, COLOR_HWHITE, "◎"},
		{6 + BDI_SELECTED, 6, COLOR_HWHITE, "◎"},
		{7 + BDI_SELECTED, 7, COLOR_HWHITE, "◎"},
		{8 + BDI_SELECTED, 8, COLOR_HWHITE, "◎"},
		{9 + BDI_SELECTED, 9, COLOR_HWHITE, "◎"},
		{BDI_VALUE_END, -1, -1, NULL} //判断结束条件为content为NULL，前面-999无所谓
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

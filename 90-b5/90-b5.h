//1850059 计1班 杨志远

#pragma once
#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <iomanip>
#include <conio.h>
#include <string>
#include <ctype.h>
#include <stdlib.h>
#include <time.h>
#include <fstream>
#include "../common/cmd_net_tools.h"
#include "../common/md5.h"
#include "../common/cmd_gmw_tools.h"
#include "../common/cmd_console_graphics.h"
#include "../common/cmd_console_base.h"
using namespace std;

const int RowNum = 10;				//棋盘行数
const int ColNum = 10;				//棋盘列数
const int Planes = 3;				//棋盘上飞机数
const int ExternRow = 5;			//界面下方增加的行数
const int ExternCol = 20;			//界面右方增加的行数
const int FontSize = 24;			//字体大小
const int DelayRetry = 1000;		//重连的延迟
const int DelayAuto = 0;			//自动模式动画效果的延迟
const char FontName[] = "新宋体";	//字体
const char Connection[] = " ";		//连接服务器时的符号（[0]为一个空格表示使用ConnectorString中的字符循环）
const int  AutoModeLimit = 15;		//自动模式的一个阈值（范围0~20，值越大越稳定）
const bool AutoModeRetrySwitch = false;	//自动模式是否需要用户选择GameOver时重开或关闭（52次循环时应该关闭）
const char ConnectorString[] = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";
const char HelpInfo[] = "蓝色为机头 绿色为机身 左键单坐标 右键双坐标";	//手动模式的提示信息
//坐标行列序列
const int CoordinateRowList[] = { 6, 4, 3, 6, 5, 3, 4, 5, 4, 4, 5, 5, 6, 3, 3, 6, 7, 2, 7, 5, 2, 5, 4, 4, 2, 7, 3, 2, 7, 6, 3, 6, 7, 2, 7, 2, 1, 4, 4, 5, 8, 1, 5, 8, 6, 3, 8, 6, 1, 8, 3, 1, 8, 2, 7, 1, 1, 7, 2, 8, 7, 0, 9, 0, 7, 2, 9, 2, 4, 0, 9, 5, 5, 4, 9, 0, 6, 3, 9, 0, 0, 9, 3, 6, 1, 8, 8, 1, 9, 1, 8, 9, 1, 0, 8, 0, 9, 9, 0, 0, };
const int CoordinateColList[] = { 4, 3, 5, 5, 3, 4, 6, 6, 5, 4, 4, 5, 6, 3, 6, 3, 4, 5, 5, 2, 4, 7, 7, 2, 3, 6, 2, 6, 3, 7, 7, 2, 2, 2, 7, 7, 5, 1, 8, 1, 4, 4, 8, 5, 1, 1, 6, 8, 3, 3, 8, 6, 7, 1, 1, 2, 7, 8, 8, 2, 0, 7, 7, 2, 9, 0, 2, 9, 9, 5, 5, 0, 9, 0, 4, 4, 0, 0, 6, 3, 6, 3, 9, 9, 1, 1, 8, 8, 1, 9, 9, 8, 0, 1, 0, 8, 9, 0, 9, 0, };

const int BDI_VALUE_HEAD = 2;
const int BDI_VALUE_BODY = 1;
const int BDI_VALUE_WRONG = -1;
const int BDI_VALUE_CLICKED = 3;
const int BDI_VALUE_TAIL = 4;
const int BDI_VALUE_HEADCLICKED = 102;
const int BDI_VALUE_BODYCLICKED = 101;

const BLOCK_DISPLAY_INFO bdi[]
{
	BDI_VALUE_BLANK, COLOR_WHITE, COLOR_BLACK, "  ",
	BDI_VALUE_HEAD, COLOR_BLUE, COLOR_BLACK,  "  ",
	BDI_VALUE_BODY, COLOR_HGREEN, COLOR_BLACK, "  ",
	BDI_VALUE_WRONG, COLOR_HWHITE, COLOR_BLACK, "::",
	BDI_VALUE_CLICKED, COLOR_HWHITE, COLOR_BLACK, "◎",
	BDI_VALUE_TAIL, COLOR_HYELLOW, COLOR_BLACK,  "  ",
	BDI_VALUE_HEADCLICKED, COLOR_HBLUE, COLOR_BLACK,  "  ",
	BDI_VALUE_BODYCLICKED, COLOR_HGREEN, COLOR_BLACK,  "  ",
	-10, COLOR_HWHITE, COLOR_BLACK, "XX",
	-999, COLOR_HWHITE, COLOR_BLACK, "&&",
	BDI_VALUE_END, -1, -1, NULL,
};

//90-b5-tools
//int game_progress(cmd_tcp_socket &client);
int get_gameprogress_string_single(cmd_tcp_socket &client, string &s);
int get_gameprogress_string_double(cmd_tcp_socket &client, string &s);

//90-b5-console
int game_start(cmd_tcp_socket &client, const bool autoMode);
int game_over(bool autoMode = false);

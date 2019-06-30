//1850059 计1班 杨志远

#pragma once
#define _CRT_SECURE_NO_WARNINGS
#include "../common/cmd_gmw_tools.h"
#include <iostream>
#include <fstream>
#include <time.h>
#include <stdio.h>
#include <conio.h>

const int SettingInfoX = 2;
const int SettingInfoY = 3;
const int SettingBorderX = 40;
const int SettingBorderY = 20;
const int DelayBlockMovedDefault = 10;
const int MaxRow = 8;
const int MaxCol = 10;
const int MinRow = 4;
const int MinCol = 4;
const int MinDelayMode = 0;
const int MaxDelayMode = 5;
const char ButtonRestart = 'R';
const char ButtonQuit = 'Q';
const char Font[] = "新宋体";
const int FontSize = 16;
const int DefaultStringLength = 256;
const int ScoreChoices = 4;
const char SaveFileName[] = "HighScore.2048";

const BLOCK_DISPLAY_INFO BDI[] = {
		{BDI_VALUE_BLANK, -1, -1, "  "},  //0不显示，用空格填充即可
		{2, COLOR_HBLACK, -1, NULL},
		{4, COLOR_HYELLOW, -1, NULL},
		{8, COLOR_HGREEN, -1, NULL},
		{16, COLOR_HCYAN, -1, NULL},
		{32, COLOR_HRED, -1, NULL},
		{64, COLOR_HPINK, -1, NULL},
		{128, COLOR_HYELLOW, -1, NULL},
		{256, COLOR_CYAN, -1, NULL},
		{512, COLOR_WHITE, -1, NULL},
		{1024, COLOR_WHITE, -1, NULL},
		{2048, COLOR_WHITE, -1, NULL},
		{4096, COLOR_WHITE, -1, NULL},
		{8192, COLOR_WHITE, -1, NULL},
		{16384, COLOR_WHITE, -1, NULL},
		{BDI_VALUE_END, -1, -1, NULL} //判断结束条件为content为NULL，前面-999无所谓
};

//90-b4-base
int input_number(const int helpInfoLength, const int offsetX, const int offsetY);
int create_new_number(int map[][MaxCol], const int row, const int col, int &x, int &y);
int move_map(const CONSOLE_GRAPHICS_INFO * const pCGI, int map[][MaxCol], const int row, const int col, const int keycode, int &score);
int draw_numbers(const CONSOLE_GRAPHICS_INFO * const pCGI, const int map[][MaxCol], const int row, const int col);
bool game_fail(const int map[][MaxCol], const int row, const int col);
bool game_win(const int map[][MaxCol], const int row, const int col, const int targetScore);
int update_top_status_message(char message[], const int blockLength, const int targetScore, const int score, const int hiScore, const int time);
void after_game_win_of_false(const CONSOLE_GRAPHICS_INFO * const pCGI, const bool win);
int load_hiScore(int hiScoreList[][MaxCol - MinCol + 1], const int row, const int col);
bool save_hiScore(const int hiScoreList[][MaxCol - MinCol + 1]);

//90-b4-main
int game2048();
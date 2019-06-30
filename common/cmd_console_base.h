//1850059 º∆1∞‡ —Ó÷æ‘∂

#pragma once
#include "cmd_console_default.h"
#include <iostream>
#include <conio.h>
using namespace std;

char welcome_and_select_mode(const char *str[], const char modeInfo[]);

void print_map(int map[][DefaultMaxCol + 1], int row, int col, int printMode = 0, int tempArray[][DefaultMaxCol + 1] = NULL);

void print_map_with_info(int map[][DefaultMaxCol + 1], int row, int col, const char message[], int printMode = 0, int tempArray[][DefaultMaxCol + 1] = NULL);

char *input_information(int winX, int inputMode, int mode, const char HelpInformation[][DefaultMaxInfoLength], char tempStr[], int *row = NULL, int *col = NULL, char instruction[] = NULL);

void input_checked_infomation(int winX, int inputMode, int mode, const char HelpInformation[][DefaultMaxInfoLength], char tempStr[], int minRow = 7, int maxRow = 9, int minCol = 9, int maxCol = 9, int *row = NULL, int *col = NULL, char instruction[] = NULL, int realRow = -1, int realCol = -1);

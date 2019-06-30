//1850059 计1班 杨志远

#pragma once
#include "cmd_console_default.h"
#include "cmd_gmw_tools.h"

using namespace std;

int get_x_in_frame(const CONSOLE_GRAPHICS_INFO *const pCGI, const int col);

int get_y_in_frame(const CONSOLE_GRAPHICS_INFO *const pCGI, const int row);

bool rowcol_in_frame(const CONSOLE_GRAPHICS_INFO *const pCGI, const int x, const int y, int &row, int &col);

//绘制所有的小球
template <unsigned int N>
void draw_points(const CONSOLE_GRAPHICS_INFO *const pCGI, const int map[][N], int row, int col, const BLOCK_DISPLAY_INFO *bdi)
{
	{
		for (int i = 0; i < row; i++)
		{
			for (int j = 0; j < col; j++)
			{
				if (map[i][j] != 0)
					gmw_draw_block(pCGI, i, j, map[i][j], bdi);
				if (map[i][j] == 0)
					gmw_draw_block(pCGI, i, j, 0, bdi);
			}
		}
	}
}

void draw_score_framework(const CONSOLE_GRAPHICS_INFO *const pCGI, int scoreAll);

int top_status_info(char buf[], const char score[] = NULL, const char message[] = NULL);

int lower_status_info(char buf[], int sX, int sY, int eX, int eY, int move, const char message[]);

//1850059 计1班 杨志远

#include "90-b1.h"

static bool move_in_console_left_button(const CONSOLE_GRAPHICS_INFO * const pCGI, int map[][DefaultMaxCol + 1], const int row, const int col, const int mapX, const int mapY, int &mapXselected, int &mapYselected, const int erasableArray[][DefaultMaxCol + 1]);
static bool move_in_console(const CONSOLE_GRAPHICS_INFO * const pCGI, int map[][DefaultMaxCol + 1], int row, int col, const char scoreStr[], int erasableArray[][DefaultMaxCol + 1]);

void mode_4_5_6_7(CONSOLE_GRAPHICS_INFO * const pCGI, int map[][DefaultMaxCol + 1], int row, int col, int mode, int erasableArray[][DefaultMaxCol + 1], int &erasableBalls)
{
	char topStatusInfo[DefaultStringLength] = "", lowerStatusInfo[DefaultStringLength] = "";
	if (mode == tolower(ModeInfomation[3]) || mode == tolower(ModeInfomation[4]))
	{
		print_map_with_info(map, row, col, "初始数组：",-1);
		cout << "按回车键显示图形..." << endl;
		wait_enter();
		cls();
	}
	gmw_draw_frame(pCGI);
	draw_points(pCGI, map, row, col, BDI);
	top_status_info(topStatusInfo, NULL);
	gmw_status_line(pCGI, TOP_STATUS_LINE, topStatusInfo);
	if (mode == tolower(ModeInfomation[3]) || mode == tolower(ModeInfomation[4]))
		return;
	erasableBalls = erasable_count(map, row, col, erasableArray);
	draw_erasable_mark(pCGI, map, row, col, erasableArray);
	if (erasableBalls == 0)
		gmw_status_line(pCGI, LOWER_STATUS_LINE, "未找到可消除项");
	if (mode == tolower(ModeInfomation[5]))
		return;
	while (erasableBalls)
	{
		gmw_status_line(pCGI, LOWER_STATUS_LINE, "按回车键进行数组下落除0操作...        ");
		wait_enter();
		gmw_status_line(pCGI, LOWER_STATUS_LINE, "下落除0后的数组(不同色标识)：         ");
		erase_balls(pCGI, map, row, col, erasableArray, true);
		gmw_status_line(pCGI, LOWER_STATUS_LINE, "按回车键进行新值填充...               ");
		wait_enter();
		gmw_status_line(pCGI, LOWER_STATUS_LINE, "新值填充后的数组(不同色标识)：        ");
		fill_blanks(pCGI, map, row, col, erasableBalls, true);
		erasableBalls = erasable_count(map, row, col, erasableArray);
		draw_erasable_mark(pCGI, map, row, col, erasableArray);
	}
	gmw_status_line(pCGI, LOWER_STATUS_LINE, "已无可消除项           ");
	if (mode == tolower(ModeInfomation[6]))
	{
		gmw_status_line(pCGI, LOWER_STATUS_LINE, "按回车显示消除提示（不同色标识）：    ");
		wait_enter();
		mark_exchangable_balls(pCGI, map, row, col, erasableArray, true);
	}
}

void mode_8(CONSOLE_GRAPHICS_INFO * const pCGI, int map[][DefaultMaxCol + 1], int row, int col, int mode, int erasableArray[][DefaultMaxCol + 1], int &erasableBalls)
{
	CONSOLE_GRAPHICS_INFO *tCGI = new(CONSOLE_GRAPHICS_INFO);
	memcpy(tCGI, pCGI, sizeof(CONSOLE_GRAPHICS_INFO));
	tCGI->delay_of_draw_block = pCGI->delay_of_draw_block / 5;
	int score = 0;
	char scoreStr[50], topStatusInfo[DefaultStringLength] = "", lowerStatusInfo[DefaultStringLength] = "";
	gmw_draw_frame(pCGI);
	draw_points(pCGI, map, row, col, BDI);
	top_status_info(topStatusInfo, _itoa(score, scoreStr, 10), "右键退出");
	gmw_status_line(pCGI, TOP_STATUS_LINE, topStatusInfo);
	enable_mouse();
	setcursor(CURSOR_INVISIBLE);
	erasableBalls = erasable_count(map, row, col, erasableArray);
	while (erasableBalls)
	{
		draw_erasable_mark(tCGI, map, row, col, erasableArray);
		erase_balls(pCGI, map, row, col, erasableArray, true);
		fill_blanks(tCGI, map, row, col, erasableBalls, true);
		erasableBalls = erasable_count(map, row, col, erasableArray);
	}
	while (1)
	{
		while (erasableBalls)
		{
			score += calc_score(row, col, erasableBalls, base);
			top_status_info(topStatusInfo, _itoa(score, scoreStr, 10), "右键退出");
			gmw_status_line(pCGI, TOP_STATUS_LINE, topStatusInfo);
			draw_erasable_mark(tCGI, map, row, col, erasableArray);
			erase_balls(pCGI, map, row, col, erasableArray, true);
			fill_blanks(tCGI, map, row, col, erasableBalls, true);
			erasableBalls = erasable_count(map, row, col, erasableArray);
		}
		if (game_over(map, row, col))
		{
			gmw_status_line(pCGI, LOWER_STATUS_LINE, "游戏结束      ");
			break;
		}
		mark_exchangable_balls(pCGI, map, row, col, erasableArray, true);
		if (!move_in_console(pCGI, map, row, col, _itoa(score, scoreStr, 10), erasableArray))
			break;
		erasableBalls = erasable_count(map, row, col, erasableArray);
	}
	setcursor(CURSOR_VISIBLE_NORMAL);
	disable_mouse();
	delete tCGI;
}

//将erasableArray中的小球以传入的shape标记出来
void draw_erasable_mark(const CONSOLE_GRAPHICS_INFO * const pCGI, int map[][DefaultMaxCol + 1], int row, int col, int erasableArray[][DefaultMaxCol + 1])
{
	for (int i = 0; i < row; i++)
		for (int j = 0; j < col; j++)
			gmw_draw_block(pCGI, i, j, map[i][j] + (erasableArray[i][j] ? BDI_ERASABLE : 0), BDI);
}

//演示两小球交换动画（不判断是否可互换）
void draw_exchange_graphics(const CONSOLE_GRAPHICS_INFO * const pCGI, int map[][DefaultMaxCol + 1], int row, int col, int X1, int Y1, int X2, int Y2)
{
	if (X1 < X2)
	{
		gmw_move_block(pCGI, Y1, X1, map[Y1][X1], 0, BDI, LEFT_TO_RIGHT, X2 - X1);
		gmw_move_block(pCGI, Y2, X2, map[Y2][X2], 0, BDI, RIGHT_TO_LEFT, X2 - X1);
	}
	if (X1 > X2)
	{
		gmw_move_block(pCGI, Y1, X1, map[Y1][X1], 0, BDI, RIGHT_TO_LEFT, X1 - X2);
		gmw_move_block(pCGI, Y2, X2, map[Y2][X2], 0, BDI, LEFT_TO_RIGHT, X1 - X2);
	}
	if (Y1 < Y2)
	{
		gmw_move_block(pCGI, Y1, X1, map[Y1][X1], 0, BDI, UP_TO_DOWN, Y2 - Y1);
		gmw_move_block(pCGI, Y2, X2, map[Y2][X2], 0, BDI, DOWN_TO_UP, Y2 - Y1);
	}
	if (Y1 > Y2)
	{
		gmw_move_block(pCGI, Y1, X1, map[Y1][X1], 0, BDI, DOWN_TO_UP, Y1 - Y2);
		gmw_move_block(pCGI, Y2, X2, map[Y2][X2], 0, BDI, UP_TO_DOWN, Y1 - Y2);
	}
	exchange(map[Y1][X1], map[Y2][X2]);
	gmw_draw_block(pCGI, Y1, X1, map[Y1][X1], BDI);
	gmw_draw_block(pCGI, Y2, X2, map[Y2][X2], BDI);
}

static bool move_in_console_left_button(const CONSOLE_GRAPHICS_INFO * const pCGI, int map[][DefaultMaxCol + 1], const int row, const int col, const int mapX, const int mapY, int &mapXselected, int &mapYselected, const int erasableArray[][DefaultMaxCol + 1])
{
	char lowerStatusInfo[DefaultStringLength] = "";
	if (mapXselected < 0)		//选中某个小球
	{
		mapXselected = mapX;
		mapYselected = mapY;
		gmw_draw_block(pCGI, mapY, mapX, map[mapY][mapX] + BDI_SELECTED, BDI);
	}
	else if (mapXselected != -1 && ((abs(mapX - mapXselected) == 1 && mapY - mapYselected == 0) || (abs(mapY - mapYselected) == 1 && mapX - mapXselected == 0)))	//判断是否相邻
	{
		if (!exchangable(map, row, col, mapX, mapY, mapXselected, mapYselected))	//不能移动
		{
			gmw_status_line(pCGI, LOWER_STATUS_LINE, "不能移动这两个小球");
			gmw_draw_block(pCGI, mapYselected, mapXselected, map[mapYselected][mapXselected] + (erasableArray[mapYselected][mapXselected] ? BDI_EXCHANGABLE : 0), BDI);
			mapXselected = -1;
			mapYselected = -1;
		}
		else					//可以移动
		{
			lower_status_info(lowerStatusInfo, mapXselected, mapYselected, mapX, mapY, 2, "右键退出");
			gmw_status_line(pCGI, LOWER_STATUS_LINE, lowerStatusInfo);
			draw_exchange_graphics(pCGI, map, row, col, mapX, mapY, mapXselected, mapYselected);
			mapXselected = -1;
			mapYselected = -1;
			return true;
		}
	}
	else						//选中其他小球
	{
		gmw_draw_block(pCGI, mapYselected, mapXselected, map[mapYselected][mapXselected] + (erasableArray[mapYselected][mapXselected] ? BDI_EXCHANGABLE : 0), BDI);
		gmw_draw_block(pCGI, mapY, mapX, map[mapY][mapX] + BDI_SELECTED, BDI);
		mapXselected = mapX;
		mapYselected = mapY;
	}
	return false;
}

static bool move_in_console(const CONSOLE_GRAPHICS_INFO * const pCGI, int map[][DefaultMaxCol + 1], int row, int col, const char scoreStr[], int erasableArray[][DefaultMaxCol + 1])
{
	int ret, maction, keycode1, keycode2, mapX = 0, mapY = 0, mapXselected = -1, mapYselected = -1;
	while (1)
	{
		ret = gmw_read_keyboard_and_mouse(pCGI, maction, mapY, mapX, keycode1, keycode2, true);
		if (ret == CCT_MOUSE_EVENT)
		{
			setcursor(CURSOR_INVISIBLE);
			switch (maction) {
				case MOUSE_ONLY_MOVED:
					break;
				case MOUSE_LEFT_BUTTON_CLICK:			//按下左键
					if (move_in_console_left_button(pCGI, map, row, col, mapX, mapY, mapXselected, mapYselected, erasableArray))
						return true;
					break;
				case MOUSE_RIGHT_BUTTON_CLICK:			//按下右键
					return false;
			}
		}
	}
	return false;
}

//消除可以消除的小球，可消除的小球记录在erasableArray中
void erase_balls(const CONSOLE_GRAPHICS_INFO * const pCGI, int map[][DefaultMaxCol + 1], int row, int col, int erasableArray[][DefaultMaxCol + 1], bool graphics)
{
	int flag[DefaultMaxCol + 1] = { 0 };
	for (int j = 0; j < col; j++)
	{
		for (int i = 0; i < row; i++)
		{
			if (erasableArray[i][j])
			{
				if (graphics)
				{
					gmw_draw_block(pCGI, i, j, map[i][j], BDI);
					gmw_draw_block(pCGI, i, j, 0, BDI);
				}
				map[i][j] = 0;
				erasableArray[i][j] = 0;
				flag[j]++;
			}
		}
	}
	for (int j = 0; j < col; j++)
	{
		for (int i = row - 1; i > 0; i--)
		{
			if (map[i][j] == 0)
			{
				int temp = i;
				for (int k = i - 1; k >= 0; k--)
				{
					if (map[k][j])
					{
						if (graphics)
						{
							gmw_move_block(pCGI, k, j, map[k][j], 0, BDI, UP_TO_DOWN, temp - k);
						}
						map[temp--][j] = map[k][j];
						map[k][j] = 0;
					}
				}
			}
		}
	}
	for (int j = 0; j < col; j++)
		for (int i = 0; i < flag[j]; i++)
			erasableArray[i][j] = 1;
}

//将地图中空缺的位置用新的彩球填充
void fill_blanks(const CONSOLE_GRAPHICS_INFO * const pCGI, int map[][DefaultMaxCol + 1], int row, int col, int balls, bool graphics)
{
	if (graphics)
	{
		int ballsColor[MaxCol * MaxRow] = { 0 }, ballsX[MaxCol * MaxRow] = { 0 }, ballsY[MaxCol * MaxRow] = { 0 };
		for (int i = 0; i < balls; i++)
			ballsColor[i] = rand() % MaxColors + 1;
		create_balls(map, row, col, balls, MaxColors, true, ballsColor, ballsX, ballsY);
		for (int i = 0; i < balls; i++)
		{
			gmw_draw_block(pCGI, ballsY[i], ballsX[i], map[ballsY[i]][ballsX[i]], BDI);
		}
	}
	else
		create_balls(map, row, col, balls, MaxColors, true);
}

//将可以发生互换的小球标记出来，并记录在erasableArray中
int mark_exchangable_balls(const CONSOLE_GRAPHICS_INFO * const pCGI, int map[][DefaultMaxCol + 1], int row, int col, int erasableArray[][DefaultMaxCol + 1], bool graphics)
{
	int result = 0;
	memset(erasableArray, 0, sizeof(int) * (MaxCol + 1) * (MaxRow + 1));
	for (int i = 0; i < row; i++)
	{
		for (int j = 0; j < col; j++)
		{
			if (i < row - 1)
			{
				if (exchangable(map, row, col, j, i, j, i + 1, erasableArray))
				{
					result++;
					if (graphics)
					{
						gmw_draw_block(pCGI, i, j, map[i][j] + BDI_EXCHANGABLE, BDI);
						gmw_draw_block(pCGI, i + 1, j, map[i + 1][j] + BDI_EXCHANGABLE, BDI);
					}
				}
			}
			if (j < col - 1)
			{
				if (exchangable(map, row, col, j, i, j + 1, i, erasableArray))
				{
					result++;
					if (graphics)
					{
						gmw_draw_block(pCGI, i, j, map[i][j] + BDI_EXCHANGABLE, BDI);
						gmw_draw_block(pCGI, i, j + 1, map[i][j + 1] + BDI_EXCHANGABLE, BDI);
					}
				}
			}
		}
	}
	return result;
}
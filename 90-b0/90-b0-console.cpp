//1850059 计1班 杨志远

#include "90-b0.h"
#include <iomanip>

static int draw_score(const CONSOLE_GRAPHICS_INFO * const pCGI, int map[][DefaultMaxCol + 1], int row, int col, int eX, int eY, int &score, int &scoreAll, int ballsRemoved[]);
static void draw_next_balls(const CONSOLE_GRAPHICS_INFO * const pCGI, int map[][MaxCol + 1], int row, int col, int ballsColor[]);
static void draw_statistics(int map[][MaxCol + 1], int row, int col, int ballsRemoved[]);
static bool move_in_console_left_button_or_enter(const CONSOLE_GRAPHICS_INFO * const pCGI, int map[][MaxCol + 1], const int row, const int col, const int mapX, const int mapY, int &mapXselected, int &mapYselected);
static bool move_in_console_esc(const CONSOLE_GRAPHICS_INFO * const pCGI, int map[][MaxCol + 1], int row, int col, int &mapXselected, int &mapYselected);
static bool move_in_console_arraw_pressed(const int keycode, const int row, const int col, int &mapX, int &mapY);
static bool move_in_console(const CONSOLE_GRAPHICS_INFO * const pCGI, int map[][MaxCol + 1], const int row, const int col, int &eX, int &eY, const bool keyboard = false);

static bool move_in_console_left_button_or_enter(const CONSOLE_GRAPHICS_INFO * const pCGI, int map[][MaxCol + 1], const int row, const int col, const int mapX, const int mapY, int &mapXselected, int &mapYselected)
{
	int pointListX[MaxPath], pointListY[MaxPath];
	char lowerStatusInfo[DefaultStringLength] = "";
	if (1)
	{
		if (map[mapY][mapX] != 0 && mapXselected == -1) //选中某个小球
		{
			mapXselected = mapX;
			mapYselected = mapY;
			//showstr(X - X % 2, Y, "◎", map[mapY][mapX], 15);
			gmw_draw_block(pCGI, mapY, mapX, map[mapY][mapX] + BDI_SELECTED, BDI);
		}
		else if (map[mapY][mapX] == 0 && mapXselected != -1) //判断是否可移动
		{
			int steps = find_path(row, col, mapXselected, mapYselected, mapX, mapY, map, pointListX, pointListY);
			if (steps == 0)
			{
				gotoxy(0, 2 * row + 2);
				cout << "没有可行的路线" << endl;
				Sleep(150);
			}
			else
			{
				lower_status_info(lowerStatusInfo, mapXselected, mapYselected, mapX, mapY, 1, NULL);
				gmw_status_line(pCGI, LOWER_STATUS_LINE, lowerStatusInfo);
				for (int i = 1; i < steps; i++)
				{
					//draw_ball_movement(map, pointListX[i - 1], pointListY[i - 1], pointListX[i], pointListY[i], map[mapYselected][mapXselected]);
					int direction = 0;
					if (pointListY[i - 1] - pointListY[i] > 0)
						direction = DOWN_TO_UP;
					if (pointListY[i - 1] - pointListY[i] < 0)
						direction = UP_TO_DOWN;
					if (pointListX[i - 1] - pointListX[i] < 0)
						direction = LEFT_TO_RIGHT;
					if (pointListX[i - 1] - pointListX[i] > 0)
						direction = RIGHT_TO_LEFT;
					gmw_move_block(pCGI, pointListY[i - 1], pointListX[i - 1], map[mapYselected][mapXselected], 0, BDI, direction, 1);
				}
				move_ball(map, mapX, mapY, mapXselected, mapYselected);
				//showstr(mapX * 4 + 2, mapY * 2 + 2, "〇", map[mapY][mapX], 15);
				gmw_draw_block(pCGI, mapY, mapX, map[mapY][mapX], BDI);
				mapXselected = -1;
				mapYselected = -1;
				return true;
			}
		}
		else if (map[mapY][mapX] != 0 && mapXselected != -1)//选中其他小球
		{
			//showstr(mapXselected * 4 + 2, mapYselected * 2 + 2, "〇", map[mapYselected][mapXselected], 15);
			gmw_draw_block(pCGI, mapYselected, mapXselected, map[mapYselected][mapXselected], BDI);
			//showstr(mapX * 4 + 2, mapY * 2 + 2, "◎", map[mapY][mapX], 15);
			gmw_draw_block(pCGI, mapY, mapX, map[mapY][mapX] + BDI_SELECTED, BDI);
			mapXselected = mapX;
			mapYselected = mapY;
		}
	}
	return false;
}

static bool move_in_console_esc(const CONSOLE_GRAPHICS_INFO * const pCGI, int map[][MaxCol + 1], int row, int col, int &mapXselected, int &mapYselected)
{
	if (mapXselected != -1)
	{
		//showstr(mapXselected * 4 + 2, mapYselected * 2 + 2, "〇", map[mapYselected][mapXselected], 15);
		gmw_draw_block(pCGI, mapYselected, mapXselected, map[mapYselected][mapXselected], BDI);
		mapXselected = -1;
		mapYselected = -1;
		return true;
	}
	return false;
}

static bool move_in_console_arraw_pressed(const int keycode, const int row, const int col, int &mapX, int &mapY)
{
	if (keycode == KB_ARROW_DOWN)
	{
		if (mapY >= row - 1)
			return false;
		mapY++;
	}
	else if (keycode == KB_ARROW_UP)
	{
		if (mapY <= 0)
			return false;
		mapY--;
	}
	else if (keycode == KB_ARROW_LEFT)
	{
		if (mapX <= 0)
			return false;
		mapX--;
	}
	else if (keycode == KB_ARROW_RIGHT)
	{
		if (mapX >= col - 1)
			return false;
		mapX++;
	}
	return true;
}

static bool move_in_console(const CONSOLE_GRAPHICS_INFO * const pCGI, int map[][MaxCol + 1], const int row, const int col, int &eX, int &eY, const bool keyboard)
{
	int ret, maction, keycode1, keycode2;
	int mapX = eX, mapY = eY, keyX = eX, keyY = eY, mapXselected = -1, mapYselected = -1;
	char topStatusInfo[DefaultStringLength] = "", lowerStatusInfo[DefaultStringLength] = "";
	gotoxy(eX * 4 + 2, eY * 2 + 2);
	while (1) 
	{
		ret = gmw_read_keyboard_and_mouse(pCGI, maction, mapY, mapX, keycode1, keycode2, true);
		setcolor();
		if (ret == CCT_MOUSE_EVENT) 
		{
			setcursor(CURSOR_INVISIBLE);
			switch (maction) {
				case MOUSE_ONLY_MOVED:					//移动鼠标
					break;
				case MOUSE_LEFT_BUTTON_CLICK:			//按下左键
					eX = mapX;
					eY = mapY;
					if (move_in_console_left_button_or_enter(pCGI, map, row, col, mapX, mapY, mapXselected, mapYselected))
						return true;
					break;
				case MOUSE_RIGHT_BUTTON_CLICK:			//按下右键
					move_in_console_esc(pCGI, map, row, col, mapXselected, mapYselected);
					return false;
			}
		}
		else if (keyboard && ret == CCT_KEYBOARD_EVENT)
		{
			setcursor(CURSOR_VISIBLE_NORMAL);
			switch (keycode1) 
			{
				case 27: //ESC键
					move_in_console_esc(pCGI, map, row, col, mapXselected, mapYselected);
					return false;
				case 13: //ENTER键
					eX = keyX;
					eY = keyY;
					if (move_in_console_left_button_or_enter(pCGI, map, row, col, keyX, keyY, mapXselected, mapYselected))
						return true;
					break;
				case 0xE0: //方向键
					move_in_console_arraw_pressed(keycode2, row, col, keyX, keyY);
					lower_status_info(lowerStatusInfo, mapXselected, mapYselected, keyX, keyY, 0, NULL);
					gmw_status_line(pCGI, LOWER_STATUS_LINE, lowerStatusInfo);
					gotoxy(keyX * 4 + 2, keyY * 2 + 2);
					break;
			}
		}
	}
	setcursor(CURSOR_INVISIBLE);
	return false;
}

static int draw_score(const CONSOLE_GRAPHICS_INFO * const pCGI, int map[][DefaultMaxCol + 1], int row, int col, int eX, int eY, int &score, int &scoreAll, int ballsRemoved[])
{
	int result = check_map(map, row, col, eX, eY, score, scoreAll, true, ballsRemoved);
	draw_score_framework(pCGI, scoreAll);
	return result;
}

static void draw_next_balls(const CONSOLE_GRAPHICS_INFO * const pCGI, int map[][MaxCol + 1], int row, int col, int ballsColor[])
{
	int x, y;
	setcolor(15, 0);
	getxy(x, y);
	gotoxy(40, 5);
	cout << "┏━━━━━┓" << endl;
	gotoxy(40, 6);
	cout << "┃";
	for (int i = 0; i < BallsEachStep; i++)
	{
		showstr(42 + i * 4, 6, BDI[ballsColor[i]].content, BDI[ballsColor[i]].bgcolor, 15);
		setcolor(15, 0);
		cout << "┃";
	}
	cout << endl;
	gotoxy(40, 7);
	cout << "┗━━━━━┛" << endl;
	gotoxy(x, y);
}

static void draw_statistics(int map[][MaxCol + 1], int row, int col, int ballsRemoved[])
{
	int x, y;
	setcolor(15, 0);
	getxy(x, y);
	gotoxy(40, 9);
	cout << "┏━━━━━━━━━━━┓";
	for (int i = 0; i < MaxColors + 1; i++)
	{
		gotoxy(40, 9 + i + 1);
		cout << "┃                      ┃";
	}
	for (int i = 0; i < MaxColors + 1; i++)
	{
		int balls = statistics_balls(map, row, col, i);
		gotoxy(40, 9 + i + 1);
		cout << "┃";
		//showstr(42, 9 + i + 1, "〇", ((i == 0) ? 15 : i), COLOR_HWHITE);
		showstr(42, 9 + i + 1, BDI[i].content, BDI[i].bgcolor, COLOR_HWHITE);
		setcolor(15, 0);
		cout << ":" << setfill('0') << setw(2) << balls << "/(" << setw(4) << setiosflags(ios::fixed) << setprecision(2) << (double)balls / row / col * 100 <<  "%) del-" << ballsRemoved[i];
	}
	gotoxy(40, 9 + MaxColors + 2);
	cout << "┗━━━━━━━━━━━┛";
	setcolor();
	gotoxy(x, y);
}

void mode_7_and_8(const CONSOLE_GRAPHICS_INFO * const pCGI, int map[][MaxCol + 1], int row, int col, int mode)
{
	int eX = 0, eY = 0, keyX = 2, keyY = 2, ballsColor[BallsEachStep], score = 0, scoreAll = 0, ballsX[BallsEachStep], ballsY[BallsEachStep], ballsRemoved[MaxColors + 1] = { 0 };
	setcursor(CURSOR_INVISIBLE);
	draw_score_framework(pCGI, scoreAll);
	set_balls_color(ballsColor, MaxColors, BallsEachStep);
	draw_next_balls(pCGI, map, row, col, ballsColor);
	draw_statistics(map, row, col, ballsRemoved);
	while (!game_over(map, row, col))
	{
		gotoxy(keyX, keyY);
		draw_statistics(map, row, col, ballsRemoved);
		if (!move_in_console(pCGI, map, row, col, eX, eY, (mode == tolower(ModeInfomation[7]))))
			break;
		if (draw_score(pCGI, map, row, col, eX, eY, score, scoreAll, ballsRemoved) != 0)
			continue;
		create_balls(map, row, col, BallsEachStep, MaxColors, false, ballsColor, ballsX, ballsY);
		for (int i = 0; i < BallsEachStep; i++)
			draw_score(pCGI, map, row, col, ballsX[i], ballsY[i], score, scoreAll, ballsRemoved);
		set_balls_color(ballsColor, MaxColors, BallsEachStep);
		draw_next_balls(pCGI, map, row, col, ballsColor);
		draw_points(pCGI, map, row, col, BDI);
	}
	setcolor();
	setcursor(CURSOR_VISIBLE_NORMAL);
}

void mode_6(const CONSOLE_GRAPHICS_INFO * const pCGI, int map[][MaxCol + 1], int row, int col)
{
	int eX = 0, eY = 0;
	setcursor(CURSOR_INVISIBLE);
	move_in_console(pCGI, map, row, col, eX, eY);
	setcolor();
	setcursor(CURSOR_VISIBLE_NORMAL);
}

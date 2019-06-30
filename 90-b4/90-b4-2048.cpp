//1850059 计1班 杨志远

#include "90-b4.h"

static int start_game(CONSOLE_GRAPHICS_INFO * const pCGI)
{
	const char *helpInfo[] = { "请输入行数", "请输入列数", "请输入目标分数", "请输入动画延迟" };
	gmw_init(pCGI);		//各项初始化
	gmw_set_color(pCGI, COLOR_BLACK, COLOR_WHITE);
	gmw_set_frame_style(pCGI, 2, 1, false);
	gmw_set_font(pCGI, Font, FontSize);
	gmw_set_frame_color(pCGI, COLOR_WHITE, COLOR_BLACK);
	gmw_set_rowcol(pCGI, SettingBorderY, SettingBorderX);
	gmw_draw_frame(pCGI);	//绘制框架
	int row = 0, col = 0, targetScore = 0, delayMode = -1, cur = 0;
	char tempStr[DefaultStringLength] = "";
	showstr(SettingInfoX, SettingInfoY, "游戏2048参数设置：", COLOR_WHITE, COLOR_BLACK);
	cur = sprintf(tempStr, "%s[%d-%d] ", helpInfo[0], MinRow, MaxRow);			//输入行数
	showstr(SettingInfoX, SettingInfoY + 1, tempStr, COLOR_WHITE, COLOR_BLACK);
	while (row < MinRow || row > MaxRow)
		row = input_number(cur, SettingInfoX, SettingInfoY + 1);
	cur = sprintf(tempStr, "%s[%d-%d] ", helpInfo[1], MinCol, MaxCol);			//输入列数
	showstr(SettingInfoX, SettingInfoY + 2, tempStr, COLOR_WHITE, COLOR_BLACK);
	while (col < MinCol || col > MaxCol)
		col = input_number(cur, SettingInfoX, SettingInfoY + 2);
	cur = sprintf(tempStr, "%s[", helpInfo[2]);	//合成帮助信息的字符串
	for (int i = 0; i < ScoreChoices; i++)				//添加4个可选的分数档数
		cur += sprintf(&tempStr[cur], "%d/", int(pow(2, 5 + (row + col) / 2 + i)));
	cur--;	//删除最后一个斜线
	cur += sprintf(&tempStr[cur], "] ");
	tempStr[cur] = '\0';	//添加尾0
	showstr(SettingInfoX, SettingInfoY + 3, tempStr, COLOR_WHITE, COLOR_BLACK);	//输入分数档数
	bool correct = false;
	while (!correct)
	{
		targetScore = input_number(cur, SettingInfoX, SettingInfoY + 3);
		for (int i = 0; i < ScoreChoices; i++)
			if (targetScore == int(pow(2, 5 + (row + col) / 2 + i)))	//输入了合法的分数档数
				correct = true;
	}
	cur = sprintf(tempStr, "%s[%d-%d] ", helpInfo[3], MinDelayMode, MaxDelayMode);
	showstr(SettingInfoX, SettingInfoY + 4, tempStr, COLOR_WHITE, COLOR_BLACK);	//输入延迟
	while (delayMode < 0 || delayMode > MaxDelayMode)
		delayMode = input_number(cur, SettingInfoX, SettingInfoY + 4);
	gmw_set_rowcol(pCGI, row, col);
	pCGI->delay_of_block_moved = DelayBlockMovedDefault * delayMode;
	return targetScore;
}

//正式开始游戏圈的准备操作，在start_game后调用
static int game_prepare(CONSOLE_GRAPHICS_INFO * const pCGI)
{
	gmw_set_block_border_switch(pCGI, true);
	gmw_set_colno_switch(pCGI, false);
	gmw_set_frame_style(pCGI, 12, 5, true);
	gmw_set_rowno_switch(pCGI, false);
	gmw_set_status_line_switch(pCGI, TOP_STATUS_LINE, true);
	gmw_set_status_line_switch(pCGI, LOWER_STATUS_LINE, false);
	gmw_set_status_line_color(pCGI, TOP_STATUS_LINE, COLOR_WHITE, COLOR_BLACK, COLOR_WHITE, COLOR_BLACK);
	cls();
	setcolor(pCGI->area_bgcolor, pCGI->area_fgcolor);
	return 0;
}

//游戏主循环
static int game_cycle(CONSOLE_GRAPHICS_INFO * const pCGI, const int targetScore, int &hiScore)
{
	const int blankLength = (pCGI->cols - 48) / 2;
	int map[MaxRow][MaxCol] = { 0 }, x, y, mAction, mRow, mCol, keyCode1, keycode2;
	int score = 0, timeStart = int(GetTickCount());		//记录程序开始时间
	char topStatusMessage[DefaultStringLength] = "";	//上状态栏信息
	gmw_draw_frame(pCGI);
	update_top_status_message(topStatusMessage, blankLength, targetScore, score, hiScore, int(GetTickCount() - timeStart) / 1000);	//更新上状态栏信息
	gmw_status_line(pCGI, TOP_STATUS_LINE, topStatusMessage);	//输出上状态栏信息
	create_new_number(map, pCGI->row_num, pCGI->col_num, x, y);	//添加新数字
	gmw_draw_block(pCGI, y, x, map[y][x], BDI);
	while (1)
	{
		int ret = gmw_read_keyboard_and_mouse(pCGI, mAction, mRow, mCol, keyCode1, keycode2, false);	//读取键盘
		if (ret == CCT_KEYBOARD_EVENT)
		{
			if (keyCode1 == 0xE0)	//方向键
			{
				if (move_map(pCGI, map, pCGI->row_num, pCGI->col_num, keycode2, score))	//如果发生了移动
				{
					hiScore = max(hiScore, score);	//更新最高分数
					update_top_status_message(topStatusMessage, blankLength, targetScore, score, hiScore, int(GetTickCount() - timeStart) / 1000);	//更新上状态栏信息
					gmw_status_line(pCGI, TOP_STATUS_LINE, topStatusMessage);		//输出上状态栏信息
					if (game_win(map, pCGI->row_num, pCGI->col_num, targetScore))	//如果游戏胜利
					{
						after_game_win_of_false(pCGI, true);		//输出胜利信息
						while(!(keyCode1 == tolower(ButtonRestart) || keyCode1 == toupper(ButtonRestart) || keyCode1 == tolower(ButtonQuit) || keyCode1 == toupper(ButtonQuit)))		//直到读取到Restart键和Quit键
							ret = gmw_read_keyboard_and_mouse(pCGI, mAction, mRow, mCol, keyCode1, keycode2, false);
					}
					else
					{
						create_new_number(map, pCGI->row_num, pCGI->col_num, x, y);	//添加新数字
						gmw_draw_block(pCGI, y, x, map[y][x], BDI);
						if (game_fail(map, pCGI->row_num, pCGI->col_num))			//如果游戏失败
						{
							after_game_win_of_false(pCGI, false);	//输出失败信息
							while (!(keyCode1 == tolower(ButtonRestart) || keyCode1 == toupper(ButtonRestart) || keyCode1 == tolower(ButtonQuit) || keyCode1 == toupper(ButtonQuit)))	//直到读取到Restart键和Quit键
								ret = gmw_read_keyboard_and_mouse(pCGI, mAction, mRow, mCol, keyCode1, keycode2, false);
						}
					}
				}
			}
			if (keyCode1 == tolower(ButtonRestart) || keyCode1 == toupper(ButtonRestart))	//如果按下Restart
				return 1;
			if (keyCode1 == tolower(ButtonQuit) || keyCode1 == toupper(ButtonQuit))			//如果按下Quit
				return 0;
		}
	}
	return 0;
}

//2048游戏入口
int game2048()
{
	CONSOLE_GRAPHICS_INFO CGI;
	srand((unsigned int)time(0));		//加入随机种子
	setcursor(CURSOR_INVISIBLE);		//隐藏光标
	int hiScoreList[MaxRow - MinRow + 1][MaxCol - MinCol + 1] = { 0 };	//记录不同大小地图的最高分信息
	int targetScore = start_game(&CGI);	//获取行列、目标分数等信息
	int hiScore = load_hiScore(hiScoreList, CGI.row_num, CGI.col_num);	//读取最高分信息
	game_prepare(&CGI);			//游戏开始前的准备工作
	while (game_cycle(&CGI, targetScore, hiScore))	//直到按下Quit键
	{
		hiScoreList[CGI.row_num - MinRow][CGI.col_num - MinCol] = hiScore;	//向数组中记录最高分信息
		save_hiScore(hiScoreList);		//写入最高分信息，防止用户突然关闭窗口导致分数丢失
		targetScore = start_game(&CGI);	//重新获取行列、目标分数等信息
		hiScore = hiScoreList[CGI.row_num - MinRow][CGI.col_num - MinCol];	//向数组中获取最高分信息
		game_prepare(&CGI);		//游戏开始前的准备工作
	}
	hiScoreList[CGI.row_num - MinRow][CGI.col_num - MinCol] = hiScore;		//向数组中记录最高分信息
	save_hiScore(hiScoreList);	//写入最高分信息
	return 0;
}

//int main()
//{
//	game2048();
//	return 0;
//}
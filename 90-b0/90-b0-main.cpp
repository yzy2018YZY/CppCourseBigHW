//1850059 计1班 杨志远

#include "90-b0.h"

const char *WelcomeInfomation[] =
{
	"1.内部数组，随机生成初始5个球",
	"2.内部数组，随机生成60%的球，寻找移动路径",
	"3.内部数组，完整版",
	"4.画出n*n的框架（无分隔线），随机显示5个球",
	"5.画出n*n的框架（有分隔线），随机显示5个球",
	"6.n*n的框架，60%的球，支持鼠标，完成一次移动",
	"7.cmd图形界面完整版",
	"8.cmd图形界面完整版 - 支持同时读键",
	"0.退出"
};

int game_prepare(CONSOLE_GRAPHICS_INFO * const pCGI, int row, int col, bool separator)
{
	gmw_init(pCGI);		//各项初始化
	gmw_set_color(pCGI, COLOR_BLACK, COLOR_HWHITE, true);
	gmw_set_frame_style(pCGI, 2, 1, separator);
	gmw_set_font(pCGI, FontName, FontSize2);
	gmw_set_frame_color(pCGI, COLOR_HWHITE, COLOR_BLACK);
	gmw_set_rowcol(pCGI, row, col);
	gmw_set_block_border_switch(pCGI, false);
	gmw_set_colno_switch(pCGI, false);
	gmw_set_rowno_switch(pCGI, false);
	gmw_set_status_line_switch(pCGI, TOP_STATUS_LINE, true);
	gmw_set_status_line_switch(pCGI, LOWER_STATUS_LINE, true);
	gmw_set_status_line_color(pCGI, TOP_STATUS_LINE, COLOR_BLACK, COLOR_HWHITE, COLOR_BLACK, COLOR_HYELLOW);
	gmw_set_delay(pCGI, DELAY_OF_BLOCK_MOVED, DelayOfMoveBlock);
	gmw_set_delay(pCGI, DELAY_OF_DRAW_BLOCK, DelayOfDrawBlock);
	gmw_set_delay(pCGI, DELAY_OF_DRAW_FRAME, DelayOfDrawFrame);
	cls();
	setcolor(pCGI->area_bgcolor, pCGI->area_fgcolor);
	return 0;
}

void choose_mode(CONSOLE_GRAPHICS_INFO * const pCGI, int mode, int map[][MaxCol + 1], int row, int col)
{
	char topStatusInfo[DefaultStringLength] = "";
	memset(map, 0, sizeof(int) * (MaxRow + 1) * (MaxCol + 1));
	create_balls(map, row, col, (mode == tolower(ModeInfomation[1]) || mode == tolower(ModeInfomation[5])) ? int(row * col * 0.6) : 5, MaxColors);
	if (mode != tolower(ModeInfomation[5]) && mode != tolower(ModeInfomation[6]))
		print_map_with_info(map, row, col, "初始数组：");
	if (mode == tolower(ModeInfomation[1]))
		mode_2(map, row, col);
	if (mode == tolower(ModeInfomation[2]))
		mode_3(map, row, col);
	if (mode == tolower(ModeInfomation[3]) || mode == tolower(ModeInfomation[4]) || mode == tolower(ModeInfomation[5]) || mode == tolower(ModeInfomation[6]) || mode == tolower(ModeInfomation[7]))
	{
		if (mode == tolower(ModeInfomation[3]) || mode == tolower(ModeInfomation[4]))
		{
			cout << "按回车继续: ";
			wait_enter();
		}
		game_prepare(pCGI, row, col, (mode == tolower(ModeInfomation[3]) ? false : true));
		if (mode == tolower(ModeInfomation[6]) || mode == tolower(ModeInfomation[7]))
			gmw_set_ext_rowcol(pCGI, 0, 0, 0, 28);
		gmw_draw_frame(pCGI);
		draw_points(pCGI, map, row, col, BDI);
		top_status_info(topStatusInfo, NULL, "鼠标右键退出");
		gmw_status_line(pCGI, TOP_STATUS_LINE, topStatusInfo);
	}
	if (mode == tolower(ModeInfomation[5]))
		mode_6(pCGI, map, row, col);
	if (mode == tolower(ModeInfomation[6]) || mode == tolower(ModeInfomation[7]))
		mode_7_and_8(pCGI, map, row, col, mode);
}

int main()
{
	int row, col, winX, winY, winBufferX, winBufferY;
	char mode;
	CONSOLE_GRAPHICS_INFO CGI;
	int map[MaxRow + 1][MaxCol + 1] = { 0 };
	char tempStr[200] = "";
	srand(unsigned int(time(0)));
	setconsoletitle(ConsoleTitle);
	while (1)
	{
		setconsoleborder(WindowXSize1, WindowYSize1, WindowXSize1, 5000);
		setfontsize(FontName, FontSize1);
		cls();
		gotoxy(0, 0);
		mode = welcome_and_select_mode(WelcomeInfomation, ModeInfomation);
		//mode = tolower(ModeInfomation[7]);			//锁定完整版
		cls();
		input_checked_infomation(strlen(HelpInformation[2]), 1, 0, HelpInformation, tempStr, MinRow, MaxRow, MinCol, MaxCol, &row, &col);
		input_checked_infomation(strlen(HelpInformation[2]), 2, 0, HelpInformation, tempStr, MinRow, MaxRow, MinCol, MaxCol, &row, &col);
		choose_mode(&CGI, mode, map, row, col);
		getconsoleborder(winX, winY, winBufferX, winBufferY);
		if (mode == tolower(ModeInfomation[3]) || mode == tolower(ModeInfomation[4]) || mode == tolower(ModeInfomation[5]) || mode == tolower(ModeInfomation[6]) || mode == tolower(ModeInfomation[7]))
			gotoxy(0, winY - 2);
		input_checked_infomation(strlen(HelpInformation[3]), 3, 0, HelpInformation, tempStr);
	}
	return 0;
}

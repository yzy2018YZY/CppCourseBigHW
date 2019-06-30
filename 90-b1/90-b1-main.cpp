//1850059 计1班 杨志远

#include "90-b1.h"
using namespace std;

const char *WelcomeInfomation[] =
{
	" A.内部数组，生成初始状态，寻找是否有初始可消除项",
	" B.内部数组，消除初始可消除项后非0项下落并用0填充",
	" C.内部数组，消除初始可消除项后查找消除提示",
	" D.n*n的框架(无分隔线)，显示初始状态",
	" E.n*n的框架(有分隔线)，显示初始状态",
	" F.n*n的框架(有分隔线)，显示初始状态及初始可消除项",
	" G.n*n的框架(有分隔线)，消除初始可消除项后显示消除提示",
	" H.cmd图形界面完整版",
	" I.从文件中读取数据以验证查找消除提示的算法的正确性",
	" Q.退出"
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
	gmw_set_colno_switch(pCGI, true);
	gmw_set_rowno_switch(pCGI, true);
	gmw_set_status_line_switch(pCGI, TOP_STATUS_LINE, true);
	gmw_set_status_line_switch(pCGI, LOWER_STATUS_LINE, true);
	gmw_set_status_line_color(pCGI, TOP_STATUS_LINE, COLOR_BLACK, COLOR_HWHITE, COLOR_BLACK, COLOR_HYELLOW);
	gmw_set_delay(pCGI, DELAY_OF_BLOCK_MOVED, Delay1);
	gmw_set_delay(pCGI, DELAY_OF_DRAW_BLOCK, Delay2);
	cls();
	setcolor(pCGI->area_bgcolor, pCGI->area_fgcolor);
	return 0;
}

void choose_mode(CONSOLE_GRAPHICS_INFO * const pCGI, int mode, int map[][DefaultMaxCol + 1], int row, int col)
{
	int erasableArray[DefaultMaxCol + 1][DefaultMaxCol + 1] = { 0 }, erasableBalls;
	memset(map, 0, sizeof(int) * (MaxRow + 1) * (MaxCol + 1));
	create_balls(map, row, col, row * col, MaxColors, true);
	if (mode == tolower(ModeInfomation[0]) || mode == tolower(ModeInfomation[1]) || mode == tolower(ModeInfomation[2]))
		mode_1_2_3(map, row, col, erasableArray, erasableBalls, mode);
	else
	{
		game_prepare(pCGI, row, col, (mode != tolower(ModeInfomation[3])));
		if (mode == tolower(ModeInfomation[3]) || mode == tolower(ModeInfomation[4]) || mode == tolower(ModeInfomation[5]) || mode == tolower(ModeInfomation[6]))
			mode_4_5_6_7(pCGI, map, row, col, mode, erasableArray, erasableBalls);
		if (mode == tolower(ModeInfomation[7]))
			mode_8(pCGI, map, row, col, mode, erasableArray, erasableBalls);
		if (mode == tolower(ModeInfomation[8]))
			mode_9(map, erasableArray);
	}
}

int main()
{
	char mode;
	int row = 0, col = 0, winX, winY, winBufferX, winBufferY, map[MaxRow + 1][MaxCol + 1] = { 0 };
	char tempStr[200] = "";
	CONSOLE_GRAPHICS_INFO CGI;
	srand(unsigned int(time(0)));
	setconsoletitle(ConsoleTitle);
	while (1)
	{
		setconsoleborder(WindowXSize1, WindowYSize1, WindowXSize1, 5000);
		setfontsize(FontName, FontSize1);
		cls();
		gotoxy(0, 0);
		mode = welcome_and_select_mode(WelcomeInfomation, ModeInfomation);
		cls();
		if (mode != tolower(ModeInfomation[8]))
		{
			input_checked_infomation(strlen(HelpInformation[2]), 1, 0, HelpInformation, tempStr, MinRow, MaxRow, MinCol, MaxCol, &row, &col);
			input_checked_infomation(strlen(HelpInformation[2]), 2, 0, HelpInformation, tempStr, MinRow, MaxRow, MinCol, MaxCol, &row, &col);
		}
		choose_mode(&CGI, mode, map, row, col);
		getconsoleborder(winX, winY, winBufferX, winBufferY);
		if (mode >= tolower(ModeInfomation[3]) && mode <= tolower(ModeInfomation[7]))
			gotoxy(0, winY - 2);
		input_checked_infomation(strlen(HelpInformation[3]), 3, 0, HelpInformation, tempStr);
	}
	return 0;
}

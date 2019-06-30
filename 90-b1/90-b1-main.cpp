//1850059 ��1�� ��־Զ

#include "90-b1.h"
using namespace std;

const char *WelcomeInfomation[] =
{
	" A.�ڲ����飬���ɳ�ʼ״̬��Ѱ���Ƿ��г�ʼ��������",
	" B.�ڲ����飬������ʼ����������0�����䲢��0���",
	" C.�ڲ����飬������ʼ������������������ʾ",
	" D.n*n�Ŀ��(�޷ָ���)����ʾ��ʼ״̬",
	" E.n*n�Ŀ��(�зָ���)����ʾ��ʼ״̬",
	" F.n*n�Ŀ��(�зָ���)����ʾ��ʼ״̬����ʼ��������",
	" G.n*n�Ŀ��(�зָ���)��������ʼ�����������ʾ������ʾ",
	" H.cmdͼ�ν���������",
	" I.���ļ��ж�ȡ��������֤����������ʾ���㷨����ȷ��",
	" Q.�˳�"
};

int game_prepare(CONSOLE_GRAPHICS_INFO * const pCGI, int row, int col, bool separator)
{
	gmw_init(pCGI);		//�����ʼ��
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

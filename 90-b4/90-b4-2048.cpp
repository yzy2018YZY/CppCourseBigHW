//1850059 ��1�� ��־Զ

#include "90-b4.h"

static int start_game(CONSOLE_GRAPHICS_INFO * const pCGI)
{
	const char *helpInfo[] = { "����������", "����������", "������Ŀ�����", "�����붯���ӳ�" };
	gmw_init(pCGI);		//�����ʼ��
	gmw_set_color(pCGI, COLOR_BLACK, COLOR_WHITE);
	gmw_set_frame_style(pCGI, 2, 1, false);
	gmw_set_font(pCGI, Font, FontSize);
	gmw_set_frame_color(pCGI, COLOR_WHITE, COLOR_BLACK);
	gmw_set_rowcol(pCGI, SettingBorderY, SettingBorderX);
	gmw_draw_frame(pCGI);	//���ƿ��
	int row = 0, col = 0, targetScore = 0, delayMode = -1, cur = 0;
	char tempStr[DefaultStringLength] = "";
	showstr(SettingInfoX, SettingInfoY, "��Ϸ2048�������ã�", COLOR_WHITE, COLOR_BLACK);
	cur = sprintf(tempStr, "%s[%d-%d] ", helpInfo[0], MinRow, MaxRow);			//��������
	showstr(SettingInfoX, SettingInfoY + 1, tempStr, COLOR_WHITE, COLOR_BLACK);
	while (row < MinRow || row > MaxRow)
		row = input_number(cur, SettingInfoX, SettingInfoY + 1);
	cur = sprintf(tempStr, "%s[%d-%d] ", helpInfo[1], MinCol, MaxCol);			//��������
	showstr(SettingInfoX, SettingInfoY + 2, tempStr, COLOR_WHITE, COLOR_BLACK);
	while (col < MinCol || col > MaxCol)
		col = input_number(cur, SettingInfoX, SettingInfoY + 2);
	cur = sprintf(tempStr, "%s[", helpInfo[2]);	//�ϳɰ�����Ϣ���ַ���
	for (int i = 0; i < ScoreChoices; i++)				//���4����ѡ�ķ�������
		cur += sprintf(&tempStr[cur], "%d/", int(pow(2, 5 + (row + col) / 2 + i)));
	cur--;	//ɾ�����һ��б��
	cur += sprintf(&tempStr[cur], "] ");
	tempStr[cur] = '\0';	//���β0
	showstr(SettingInfoX, SettingInfoY + 3, tempStr, COLOR_WHITE, COLOR_BLACK);	//�����������
	bool correct = false;
	while (!correct)
	{
		targetScore = input_number(cur, SettingInfoX, SettingInfoY + 3);
		for (int i = 0; i < ScoreChoices; i++)
			if (targetScore == int(pow(2, 5 + (row + col) / 2 + i)))	//�����˺Ϸ��ķ�������
				correct = true;
	}
	cur = sprintf(tempStr, "%s[%d-%d] ", helpInfo[3], MinDelayMode, MaxDelayMode);
	showstr(SettingInfoX, SettingInfoY + 4, tempStr, COLOR_WHITE, COLOR_BLACK);	//�����ӳ�
	while (delayMode < 0 || delayMode > MaxDelayMode)
		delayMode = input_number(cur, SettingInfoX, SettingInfoY + 4);
	gmw_set_rowcol(pCGI, row, col);
	pCGI->delay_of_block_moved = DelayBlockMovedDefault * delayMode;
	return targetScore;
}

//��ʽ��ʼ��ϷȦ��׼����������start_game�����
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

//��Ϸ��ѭ��
static int game_cycle(CONSOLE_GRAPHICS_INFO * const pCGI, const int targetScore, int &hiScore)
{
	const int blankLength = (pCGI->cols - 48) / 2;
	int map[MaxRow][MaxCol] = { 0 }, x, y, mAction, mRow, mCol, keyCode1, keycode2;
	int score = 0, timeStart = int(GetTickCount());		//��¼����ʼʱ��
	char topStatusMessage[DefaultStringLength] = "";	//��״̬����Ϣ
	gmw_draw_frame(pCGI);
	update_top_status_message(topStatusMessage, blankLength, targetScore, score, hiScore, int(GetTickCount() - timeStart) / 1000);	//������״̬����Ϣ
	gmw_status_line(pCGI, TOP_STATUS_LINE, topStatusMessage);	//�����״̬����Ϣ
	create_new_number(map, pCGI->row_num, pCGI->col_num, x, y);	//���������
	gmw_draw_block(pCGI, y, x, map[y][x], BDI);
	while (1)
	{
		int ret = gmw_read_keyboard_and_mouse(pCGI, mAction, mRow, mCol, keyCode1, keycode2, false);	//��ȡ����
		if (ret == CCT_KEYBOARD_EVENT)
		{
			if (keyCode1 == 0xE0)	//�����
			{
				if (move_map(pCGI, map, pCGI->row_num, pCGI->col_num, keycode2, score))	//����������ƶ�
				{
					hiScore = max(hiScore, score);	//������߷���
					update_top_status_message(topStatusMessage, blankLength, targetScore, score, hiScore, int(GetTickCount() - timeStart) / 1000);	//������״̬����Ϣ
					gmw_status_line(pCGI, TOP_STATUS_LINE, topStatusMessage);		//�����״̬����Ϣ
					if (game_win(map, pCGI->row_num, pCGI->col_num, targetScore))	//�����Ϸʤ��
					{
						after_game_win_of_false(pCGI, true);		//���ʤ����Ϣ
						while(!(keyCode1 == tolower(ButtonRestart) || keyCode1 == toupper(ButtonRestart) || keyCode1 == tolower(ButtonQuit) || keyCode1 == toupper(ButtonQuit)))		//ֱ����ȡ��Restart����Quit��
							ret = gmw_read_keyboard_and_mouse(pCGI, mAction, mRow, mCol, keyCode1, keycode2, false);
					}
					else
					{
						create_new_number(map, pCGI->row_num, pCGI->col_num, x, y);	//���������
						gmw_draw_block(pCGI, y, x, map[y][x], BDI);
						if (game_fail(map, pCGI->row_num, pCGI->col_num))			//�����Ϸʧ��
						{
							after_game_win_of_false(pCGI, false);	//���ʧ����Ϣ
							while (!(keyCode1 == tolower(ButtonRestart) || keyCode1 == toupper(ButtonRestart) || keyCode1 == tolower(ButtonQuit) || keyCode1 == toupper(ButtonQuit)))	//ֱ����ȡ��Restart����Quit��
								ret = gmw_read_keyboard_and_mouse(pCGI, mAction, mRow, mCol, keyCode1, keycode2, false);
						}
					}
				}
			}
			if (keyCode1 == tolower(ButtonRestart) || keyCode1 == toupper(ButtonRestart))	//�������Restart
				return 1;
			if (keyCode1 == tolower(ButtonQuit) || keyCode1 == toupper(ButtonQuit))			//�������Quit
				return 0;
		}
	}
	return 0;
}

//2048��Ϸ���
int game2048()
{
	CONSOLE_GRAPHICS_INFO CGI;
	srand((unsigned int)time(0));		//�����������
	setcursor(CURSOR_INVISIBLE);		//���ع��
	int hiScoreList[MaxRow - MinRow + 1][MaxCol - MinCol + 1] = { 0 };	//��¼��ͬ��С��ͼ����߷���Ϣ
	int targetScore = start_game(&CGI);	//��ȡ���С�Ŀ���������Ϣ
	int hiScore = load_hiScore(hiScoreList, CGI.row_num, CGI.col_num);	//��ȡ��߷���Ϣ
	game_prepare(&CGI);			//��Ϸ��ʼǰ��׼������
	while (game_cycle(&CGI, targetScore, hiScore))	//ֱ������Quit��
	{
		hiScoreList[CGI.row_num - MinRow][CGI.col_num - MinCol] = hiScore;	//�������м�¼��߷���Ϣ
		save_hiScore(hiScoreList);		//д����߷���Ϣ����ֹ�û�ͻȻ�رմ��ڵ��·�����ʧ
		targetScore = start_game(&CGI);	//���»�ȡ���С�Ŀ���������Ϣ
		hiScore = hiScoreList[CGI.row_num - MinRow][CGI.col_num - MinCol];	//�������л�ȡ��߷���Ϣ
		game_prepare(&CGI);		//��Ϸ��ʼǰ��׼������
	}
	hiScoreList[CGI.row_num - MinRow][CGI.col_num - MinCol] = hiScore;		//�������м�¼��߷���Ϣ
	save_hiScore(hiScoreList);	//д����߷���Ϣ
	return 0;
}

//int main()
//{
//	game2048();
//	return 0;
//}
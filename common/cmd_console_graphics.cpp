//1850059 ��1�� ��־Զ

#include "cmd_console_graphics.h"
#include <iomanip>

//������ת��Ϊ���ڵ�x����
int get_x_in_frame(const CONSOLE_GRAPHICS_INFO *const pCGI, const int col)
{
	return 2 + (pCGI->CFI.block_width + 2 * !!(pCGI->CFI.separator)) * col + !!(pCGI->draw_frame_with_row_no) * 2 + pCGI->start_x;
}

//������ת��Ϊ���ڵ�y����
int get_y_in_frame(const CONSOLE_GRAPHICS_INFO *const pCGI, const int row)
{
	return (pCGI->CFI.block_high + !!(pCGI->CFI.separator)) * row + !!(pCGI->draw_frame_with_col_no) + pCGI->start_y + 1;
}

//�ж������Ƿ��ڿ����
bool rowcol_in_frame(const CONSOLE_GRAPHICS_INFO *const pCGI, const int x, const int y, int &row, int &col)
{
	bool getRow = false, getCol = false;
	for (int i = 0; !getCol && i < pCGI->col_num; i++)
	{
		int offsetX = x - get_x_in_frame(pCGI, i);
		if (offsetX >= 0 && offsetX < pCGI->CFI.block_width)
		{
			col = i;
			getCol = true;
		}
	}
	for (int i = 0; !getRow && i < pCGI->row_num; i++)
	{
		int offsetY = y - get_y_in_frame(pCGI, i);
		if (offsetY >= 0 && offsetY < pCGI->CFI.block_high)
		{
			row = i;
			getRow = true;
		}
	}
	if (getRow && getCol)
		return true;
	else
		return false;
}

//���Ʒ������
void draw_score_framework(const CONSOLE_GRAPHICS_INFO *const pCGI, int scoreAll)
{
	int x, y;
	setcolor(15, 0);
	getxy(x, y);
	gotoxy(40, 1);
	cout << "��������������" << endl;
	gotoxy(40, 2);
	cout << "���÷�: " << setw(4) << scoreAll << "��" << endl;
	gotoxy(40, 3);
	cout << "��������������" << endl;
	gotoxy(x, y);
	setcolor();
}

//����״̬������ʾ��Ϣ
int top_status_info(char buf[], const char score[], const char message[])
{
	int winX, winY, winBufferX, winBufferY, cur = 0;
	getconsoleborder(winX, winY, winBufferX, winBufferY);
	cur += sprintf(&buf[cur], "��Ļ: %d��%d�� ", winY, winX);
	if (message)
		cur += sprintf(&buf[cur], "%s ", message);
	if (score)
		cur += sprintf(&buf[cur], "������%s ", score);
	cur += sprintf(&buf[cur], "       ");
	return cur;
}

//������״̬������ʾ��Ϣ
int lower_status_info(char buf[], int sX, int sY, int eX, int eY, int mode, const char message[])
{
	int cur = 0;
	if (mode == 1)
		cur += sprintf(&buf[cur], "[��ʾ] ��%c%d�ƶ���%c%d ", char(sY + 'A'), sX, char(eY + 'A'), eX);
	else if (mode == 2)
		cur += sprintf(&buf[cur], "[��ʾ] %c%d��%c%d���� ", char(sY + 'A'), sX, char(eY + 'A'), eX);
	else
		cur += sprintf(&buf[cur], "[��ǰ���] %c��%d�� ", char(eY + 'A'), eX);
	if (message)
		cur += sprintf(&buf[cur], message);
	return cur;
}

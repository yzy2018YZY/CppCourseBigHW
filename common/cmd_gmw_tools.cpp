//1850059 ��1�� ��־Զ

#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <climits>
#include <conio.h>
#include "cmd_console_tools.h"
#include "cmd_gmw_tools.h"
using namespace std;

/* --------------------------------------------------
		�˴����Ը�����Ҫ�ľ�̬ȫ�ֱ������������٣����û�У�����̬ȫ��ֻ������/�궨�壨�������ޣ���
   -------------------------------------------------- */

//1 - ȫ˫�� 2 - ȫ���� 3 - ��˫���� 4 - �ᵥ��˫
enum TabSettings
{
	TabAllDouble = 1,
	TabAllSingle = 2,
	TabDoubleSingle = 3,
	TabSingleDouble = 4,
};

//��TabElements��Ӧ
enum FramePosition
{
	FrameHorizonal = 0,
	FrameVertical = 1,
	FrameLeftUp = 2,
	FrameRightUp = 3,
	FrameLeftDown = 4,
	FrameRightDown = 5,
	FrameLeft = 6,
	FrameRight = 7,
	FrameUp = 8,
	FrameDown = 9,
	FrameCenter = 10,
};

//1 - ȫ˫�� 2 - ȫ���� 3 - ��˫���� 4 - �ᵥ��˫
const char TabElements[4][11][3] = 
{ 
	{"�T", "�U", "�X", "�[", "�^", "�a", "�d", "�g", "�j", "�m", "�p"},
	{"��", "��", "��", "��", "��", "��", "��", "��", "��", "��", "��"},
	{"�T", "��", "�V", "�Y", "�\", "�_", "�b", "�e", "�h", "�k", "�n"},
	{"��", "�U", "�W", "�Z", "�]", "�`", "�c", "�f", "�i", "�l", "�o"},
};

/* --------------------------------------------------
		�˴����Ը�����Ҫ���ڲ��������ߺ���
		1�����������ޣ�����Ϊ gmw_inner_* 
		2����������
		3��������static������ȷ��ֻ�ڱ�Դ�ļ���ʹ��
   -------------------------------------------------- */

//����һ������TabElements���ַ���source��buf������buf�ĳ���
static int gmv_inner_set_frame_one_symbol(char *buf, const char *sourse)
{
	char tempStr[3] = "  ";
	int len = strlen(sourse);
	if (len >= 2)		//source�ַ������ȴ���2
	{
		strncpy(tempStr, sourse, 2);
	}
	else if (len == 1)	//source�ַ������Ȳ���
	{
		tempStr[0] = sourse[0];
	}
	strcpy(buf, tempStr);
	return strlen(buf);
}

//���¼���һ��pCGI����row��col�йص����ݣ��ı䴰�ڴ�С
static int gmw_inner_update_rowcol(CONSOLE_GRAPHICS_INFO *const pCGI)
{
	pCGI->start_y = pCGI->extern_up_lines + !!(pCGI->top_status_line);
	pCGI->start_x = pCGI->extern_left_cols;
	pCGI->lines = pCGI->row_num * pCGI->CFI.block_high + pCGI->row_num * !!(pCGI->CFI.separator) + pCGI->extern_up_lines + pCGI->extern_down_lines + !!(pCGI->top_status_line) + !!(pCGI->lower_status_line) + !!(pCGI->draw_frame_with_col_no) + 4 + !(pCGI->CFI.separator) + 1;
	pCGI->cols  = pCGI->col_num * pCGI->CFI.block_width + pCGI->col_num * 2 * !!(pCGI->CFI.separator) + pCGI->extern_left_cols + pCGI->extern_right_cols + !!(pCGI->draw_frame_with_row_no) * 2 + 2 * !(pCGI->CFI.separator) + 3;
	pCGI->SLI.lower_start_y = pCGI->lines - pCGI->extern_down_lines - 5;
	return 0; //�˾�ɸ�����Ҫ�޸�
}

//������ת��Ϊ���ڵ�x����
static int gmw_inner_get_x_in_frame(const CONSOLE_GRAPHICS_INFO *const pCGI, const int col)
{
	return 2 + (pCGI->CFI.block_width + 2 * !!(pCGI->CFI.separator)) * col + !!(pCGI->draw_frame_with_row_no) * 2 + pCGI->start_x;
}

//������ת��Ϊ���ڵ�y����
static int gmw_inner_get_y_in_frame(const CONSOLE_GRAPHICS_INFO *const pCGI, const int row)
{
	return (pCGI->CFI.block_high + !!(pCGI->CFI.separator)) * row + !!(pCGI->draw_frame_with_col_no) + pCGI->start_y + 1;
}

//��x��yת��Ϊ����е�row��col�����ʧ���򷵻�false������ɹ��򷵻�true
static bool gmw_inner_check_rowcol_in_frame(const CONSOLE_GRAPHICS_INFO *const pCGI, const int x, const int y, int &row, int &col)
{
	bool getRow = false, getCol = false;
	for (int i = 0; !getCol && i < pCGI->col_num; i++)
	{
		int offsetX = x - gmw_inner_get_x_in_frame(pCGI, i);
		if (offsetX >= 0 && offsetX < pCGI->CFI.block_width)
		{
			col = i;
			getCol = true;
		}
	}
	for (int i = 0; !getRow && i < pCGI->row_num; i++)
	{
		int offsetY = y - gmw_inner_get_y_in_frame(pCGI, i);
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

//���б�ת��Ϊһ����Ӧ���ַ�����������ַ�
static char gmw_inner_get_row_number(const int row_No)
{
	if (row_No >= 0 && row_No < 26)			//A��Z
	{
		return 'A' + row_No;
	}
	else if (row_No >= 26 && row_No < 52)	//a��z
	{
		return 'a' + row_No - 26;
	}
	else									//�������
	{
		return '*';
	}
}

//���б�ת��Ϊһ����Ӧ���ַ�������������ַ�����ͷָ��
static char* gmw_inner_get_col_number(const int col_No, char buf[])
{
	if (col_No >= 0 && col_No <= 9)			//��λ��
	{
		buf[0] = '0' + col_No;
		buf[1] = '\0';
	}
	else if (col_No >= 10 && col_No <= 99)	//ʮλ��
	{
		buf[0] = '0' + col_No / 10;
		buf[1] = '0' + col_No % 10;
		buf[2] = '\0';
	}
	else									//�������
	{
		buf[0] = '*';
		buf[1] = '*';
		buf[2] = '\0';
	}
	return buf;
}

//�޸���״̬���еĹ��λ����Ϣ
static void gmw_inner_update_lower_statue_line(const CONSOLE_GRAPHICS_INFO *const pCGI, const int row, const int col)
{
	int cur = 0;
	char temp[100] = "";
	cur += sprintf(&temp[cur], "[��ǰ���] %c��%d��", gmw_inner_get_row_number(row), col);
	gmw_status_line(pCGI, LOWER_STATUS_LINE, temp);
}

//���һ��״̬���е���Ϣ
static void gmw_inner_print_statue_line(const char *msg, const char *catchy_msg, const int cols, const int extern_right_cols, const int extern_left_cols, const int start_x, const int start_y, const int normal_bgcolor, const int normal_fgcolor, const int catchy_bgcolor, const int catchy_fgcolor)
{
	char tempMsg[256] = "", tempCatchyMsg[256] = "";
	if (catchy_msg != NULL)
	{
		strcpy(tempCatchyMsg, catchy_msg);
		convert_half_character(tempCatchyMsg, cols - extern_right_cols - extern_left_cols - 1);
	}
	strcpy(tempMsg, msg);
	convert_half_character(tempMsg, cols - extern_right_cols - extern_left_cols - 1 - (catchy_msg == NULL ? 0 : strlen(tempCatchyMsg)));
	showch(start_x, start_y, ' ', normal_bgcolor, normal_fgcolor, cols - extern_right_cols - extern_left_cols - 1);	//ˢ�µ�ǰ��
	if (tempCatchyMsg == NULL)
	{
		showstr(start_x, start_y, tempMsg, normal_bgcolor, normal_fgcolor, 1, cols - extern_right_cols - extern_left_cols - 1);			//ֻ���������Ϣ
	}
	else
	{
		showstr(start_x, start_y, tempCatchyMsg, catchy_bgcolor, catchy_fgcolor);				//�����ǿ����Ϣ
		showstr(start_x + strlen(tempCatchyMsg), start_y, tempMsg,normal_bgcolor, normal_fgcolor, 1, cols - extern_right_cols - extern_left_cols - 1 - strlen(tempCatchyMsg));//�����������Ϣ
	}
}

//��(x,y)Ϊ���Ͻǣ�����һ��ɫ��
static int gmw_inner_draw_block(const CONSOLE_GRAPHICS_INFO *const pCGI, const int x, const int y, const int width, const int high, const int index, const int bgColor, const int fgColor, int delay, const BLOCK_DISPLAY_INFO *const bdi)
{
	if (bdi[index].value == BDI_VALUE_BLANK)
	{
		for (int i = 0; i < high; i++)
			showch(x, y + i, ' ', pCGI->CFI.bgcolor, pCGI->CFI.fgcolor, width);
		Sleep(delay);
		return 0;
	}
	if (pCGI->CBI.block_border && bdi[index].value != BDI_VALUE_BLANK)	//�����߿�
	{
		showstr(x, y, pCGI->CBI.top_left, bgColor, fgColor);	//����
		for (int j = 2; j < width - 2; j += 2)
		{
			showstr(x + j, y, pCGI->CBI.h_normal, bgColor, fgColor);	//��
		}
		showstr(x + width - 2, y, pCGI->CBI.top_right, bgColor, fgColor);	//����
		for (int i = 1; i < high - 1; i++)
		{
			showstr(x, y + i, pCGI->CBI.v_normal, bgColor, fgColor);	//��
			showch(x + 2, y + i, ' ', bgColor, fgColor, width - 4);		//�ն����һ����
			showstr(x + width - 2, y + i, pCGI->CBI.v_normal, bgColor, fgColor);	//��
		}
		showstr(x, y + high - 1, pCGI->CBI.lower_left, bgColor, fgColor);	//����
		for (int j = 2; j < width - 2; j += 2)
		{
			showstr(x + j, y + high - 1, pCGI->CBI.h_normal, bgColor, fgColor);	//��
		}
		showstr(x + width - 2, y + high - 1, pCGI->CBI.lower_right, bgColor, fgColor);	//����
	}
	if (bdi[index].content == NULL)					//û������
		showint(x + (width - int(log10(bdi[index].value)) - 1) / 2, y + high / 2, bdi[index].value, bgColor, fgColor);	//�������
	else if (bdi[index].value != BDI_VALUE_END)		//û�е�ĩβ���������ո��������
		showstr(x + (width - strlen(bdi[index].content)) / 2, y + high / 2, bdi[index].content, bgColor, fgColor);	//���content
	Sleep(delay);
	return 0;
}

/* ----------------------------------------------- 
		ʵ����������ĺ���������������׼����
   ----------------------------------------------- */
/***************************************************************************
  �������ƣ�
  ��    �ܣ�������Ϸ����ܵ�������
  ���������CONSOLE_GRAPHICS_INFO *const pCGI	������ṹָ��
			const int row						������(������Ϊ0���������ޣ���Ϊ��֤��ȷ��)
			const int col						������(������Ϊ0���������ޣ���Ϊ��֤��ȷ��)
  �� �� ֵ��
  ˵    ����1��ָ��������Ϸ�ľ������������ֵ
            2�����еı仯�ᵼ��CONSOLE_GRAPHICS_INFO�ṹ����������Աֵ�ı仯��Ҫ����
***************************************************************************/
int gmw_set_rowcol(CONSOLE_GRAPHICS_INFO *const pCGI, const int row, const int col)
{
	int _row = max(row, 0), _col = max(col, 0);
	pCGI->row_num = _row;
	pCGI->col_num = _col;
	gmw_inner_update_rowcol(pCGI);
	return 0; //�˾�ɸ�����Ҫ�޸�
}

/***************************************************************************
  �������ƣ�
  ��    �ܣ������������ڣ�����Ϸ�������������ڵ�����cmd���ڣ�����ɫ
  ���������CONSOLE_GRAPHICS_INFO *const pCGI	������ṹָ��
		   const int bg_color					��ǰ��ɫ��ȱʡCOLOR_BLACK��
		   const int fg_color					������ɫ��ȱʡCOLOR_WHITE��
		   const int cascade					���Ƿ�����ȡֵ0/1��ȱʡΪ0-��������
  �� �� ֵ��
  ˵    ����1��cascade = 1ʱ
				ͬ���޸���Ϸ���������ɫ
				ͬ���޸�����״̬���������ı��ı���ɫ��ǰ��ɫ����Ŀ�ı��ı���ɫ��ǰ��ɫ���䣩
			2���������ɫֵ���󼰳�ͻ����Ҫ��Ϊ��֤
				������ɫ��0-15
				    ǰ��ɫ����ɫ��ֵһ�µ����޷���������
					ǰ��ɫ������״̬����Ŀǰ��ɫ�������޷�������Ŀ��ʾ
					...
***************************************************************************/
int gmw_set_color(CONSOLE_GRAPHICS_INFO *const pCGI, const int bgcolor, const int fgcolor, const bool cascade)
{
	pCGI->area_bgcolor = bgcolor;
	pCGI->area_fgcolor = fgcolor;
	setcolor(bgcolor, fgcolor);			//�޸Ĵ�����ɫ

	if (cascade == 1)					//����
	{
		pCGI->CFI.bgcolor = bgcolor;	//���ǰ��ɫ�ͱ���ɫ
		pCGI->CFI.fgcolor = fgcolor;

		pCGI->SLI.top_normal_bgcolor = bgcolor;		//��״̬��ǰ��ɫ�ͱ���ɫ
		pCGI->SLI.top_normal_fgcolor = fgcolor;
		pCGI->SLI.top_catchy_bgcolor = bgcolor;

		pCGI->SLI.lower_normal_bgcolor = bgcolor;	//��״̬��ǰ��ɫ�ͱ���ɫ
		pCGI->SLI.lower_normal_fgcolor = fgcolor;
		pCGI->SLI.lower_catchy_bgcolor = bgcolor;
	}

	return 0; //�˾�ɸ�����Ҫ�޸�
}

/***************************************************************************
  �������ƣ�
  ��    �ܣ����ô��ڵ�����
  ���������CONSOLE_GRAPHICS_INFO *const pCGI	������ṹָ��
		   const char *fontname					���������ƣ�ֻ����"Terminal"��"������"���֣������򷵻�-1�����ı����壩
		   const int fs_high					������߶ȣ�ȱʡ������Ϊ16�������������ƣ���Ϊ��֤��
		   const int fs_width					������߶ȣ�ȱʡ������Ϊ8�������������ƣ���Ϊ��֤��
  �� �� ֵ��
  ˵    ����1����cmd_console_tools�е�setfontsize���ƣ�Ŀǰֻ֧�֡��������塱�͡������塱
			2������������������ֱ�ӷ��أ�����ԭ�������ò���
***************************************************************************/
int gmw_set_font(CONSOLE_GRAPHICS_INFO *const pCGI, const char *fontname, const int fs_high, const int fs_width)
{
	if (strcmp(fontname, "������") == 0 || strcmp(fontname, "Terminal") == 0)		//��������������
	{
		int fsHigh = fs_high, fsWidth = fs_width;
		strcpy(pCGI->CFT.font_type, fontname);
		if (fsHigh <= 0)
			fsHigh = 16;
		if (fsWidth <= 0)
			fsWidth = 8;
		pCGI->CFT.font_size_high = fsHigh;
		pCGI->CFT.font_size_width = fsWidth;
		return 0;
	}
	else
	{
		return -1;
	}
	
}

/***************************************************************************
  �������ƣ�
  ��    �ܣ�������ʱ
  ���������CONSOLE_GRAPHICS_INFO *const pCGI	������ṹָ��
		   const int type						����ʱ�����ͣ�ĿǰΪ3�֣�
		   const int delay_ms					����msΪ��λ����ʱ
			   ���߿����ʱ��0 ~ �������ޣ���Ϊ��֤��ȷ��<0����0��
			   ��ɫ�����ʱ��0 ~ �������ޣ���Ϊ��֤��ȷ��<0����0��
			   ɫ���ƶ�����ʱ��BLOCK_MOVED_DELAY_MS ~ �������ޣ���Ϊ��֤��ȷ�� <BLOCK_MOVED_DELAY_MS ���� BLOCK_MOVED_DELAY_MS��
  �� �� ֵ��
  ˵    ����
***************************************************************************/
int gmw_set_delay(CONSOLE_GRAPHICS_INFO *const pCGI, const int type, const int delay_ms)
{
	if (type == DELAY_OF_DRAW_FRAME)	//�������
	{
		pCGI->delay_of_draw_frame = max(0, delay_ms);
	}
	if (type == DELAY_OF_DRAW_BLOCK)	//��ɫ��
	{
		pCGI->delay_of_draw_block = max(0, delay_ms);
	}
	if (type == DELAY_OF_BLOCK_MOVED)	//ɫ���ƶ�
	{
		pCGI->delay_of_block_moved = max(BLOCK_MOVED_DELAY_MS, delay_ms);
	}
	return 0; //�˾�ɸ�����Ҫ�޸�
}

/***************************************************************************
  �������ƣ�
  ���������������Ϸ����ܽṹ֮����Ҫ�����Ķ�������
  ��    �ܣ�CONSOLE_GRAPHICS_INFO *const pCGI	��
		   const int up_lines					���ϲ�������У�ȱʡ������Ϊ0���������ޣ���Ϊ��֤��
		   const int down_lines				���²�������У�ȱʡ������Ϊ0���������ޣ���Ϊ��֤��
		   const int left_cols					����߶�����У�ȱʡ������Ϊ0���������ޣ���Ϊ��֤��
		   const int right_cols				���ұ߶�����У�ȱʡ������Ϊ0���������ޣ���Ϊ��֤��
  �� �� ֵ��
  ˵    �����������еı仯�ᵼ��CONSOLE_GRAPHICS_INFO�ṹ����������Աֵ�ı仯��Ҫ����
***************************************************************************/
int gmw_set_ext_rowcol(CONSOLE_GRAPHICS_INFO *const pCGI, const int up_lines, const int down_lines, const int left_cols, const int right_cols)
{
	int upLines = max(up_lines, 0), downLines = max(down_lines, 0), leftCols = max(left_cols, 0), rightCols = max(right_cols, 0);
	pCGI->extern_up_lines = upLines;		//�������Ҷ��������
	pCGI->extern_down_lines = downLines;
	pCGI->extern_left_cols = leftCols;
	pCGI->extern_right_cols = rightCols;

	pCGI->SLI.top_start_x = leftCols;		//��״̬����λ��
	pCGI->SLI.top_start_y = upLines;

	pCGI->SLI.lower_start_x = leftCols;	//��״̬����x����

	gmw_inner_update_rowcol(pCGI);

	return 0; //�˾�ɸ�����Ҫ�޸�
}

/***************************************************************************
  �������ƣ�
  ��    �ܣ���� CONSOLE_BORDER_TYPE �ṹ�е�11�����ͣ�ȱʡ4�֣�
  ���������CONSOLE_GRAPHICS_INFO *const pCGI	������ṹָ��
			const int type						��1 - ȫ�� 2 - ȫ���� 3 - ��˫���� 4 - �ᵥ��˫
  �� �� ֵ��
  ˵    ����
***************************************************************************/
int gmw_set_frame_default_linetype(CONSOLE_GRAPHICS_INFO *const pCGI, const int type)
{
	if (type < 1 || type > 4)
		return -1;
	gmv_inner_set_frame_one_symbol(pCGI->CFI.top_left, TabElements[type - 1][FrameLeftUp]);
	gmv_inner_set_frame_one_symbol(pCGI->CFI.lower_left, TabElements[type - 1][FrameLeftDown]);
	gmv_inner_set_frame_one_symbol(pCGI->CFI.top_right, TabElements[type - 1][FrameRightUp]);
	gmv_inner_set_frame_one_symbol(pCGI->CFI.lower_right, TabElements[type - 1][FrameRightDown]);
	gmv_inner_set_frame_one_symbol(pCGI->CFI.h_normal, TabElements[type - 1][FrameHorizonal]);
	gmv_inner_set_frame_one_symbol(pCGI->CFI.v_normal, TabElements[type - 1][FrameVertical]);
	gmv_inner_set_frame_one_symbol(pCGI->CFI.h_top_separator, TabElements[type - 1][FrameUp]);
	gmv_inner_set_frame_one_symbol(pCGI->CFI.h_lower_separator, TabElements[type - 1][FrameDown]);
	gmv_inner_set_frame_one_symbol(pCGI->CFI.v_left_separator, TabElements[type - 1][FrameLeft]);
	gmv_inner_set_frame_one_symbol(pCGI->CFI.v_right_separator, TabElements[type - 1][FrameRight]);
	gmv_inner_set_frame_one_symbol(pCGI->CFI.mid_separator, TabElements[type - 1][FrameCenter]);
	return 0; //�˾�ɸ�����Ҫ�޸�
}

/***************************************************************************
  �������ƣ�
  ��    �ܣ���� CONSOLE_BORDER_TYPE �ṹ�е�11������
  ���������CONSOLE_GRAPHICS_INFO *const pCGI	������ṹָ��
			const char *...						����11�֣������.h���˴���
  �� �� ֵ��
  ˵    ����Լ��Ϊһ�������Ʊ��������ʹ���������ݣ���Ϊ��֤2�ֽ�
			1������2�ֽ���ֻȡǰ2�ֽ�
			2�������NULL���������ո����
			3�������1�ֽڣ���һ���ո������˶�������ʾ�ң������
***************************************************************************/
int gmw_set_frame_linetype(CONSOLE_GRAPHICS_INFO *const pCGI, const char *top_left, const char *lower_left, const char *top_right,
	const char *lower_right, const char *h_normal, const char *v_normal, const char *h_top_separator,
	const char *h_lower_separator, const char *v_left_separator, const char *v_right_separator, const char *mid_separator)
{
	gmv_inner_set_frame_one_symbol(pCGI->CFI.top_left, top_left);
	gmv_inner_set_frame_one_symbol(pCGI->CFI.lower_left, lower_left);
	gmv_inner_set_frame_one_symbol(pCGI->CFI.top_right, top_right);
	gmv_inner_set_frame_one_symbol(pCGI->CFI.lower_right, lower_right);
	gmv_inner_set_frame_one_symbol(pCGI->CFI.h_normal, h_normal);
	gmv_inner_set_frame_one_symbol(pCGI->CFI.v_normal, v_normal);
	gmv_inner_set_frame_one_symbol(pCGI->CFI.h_top_separator, h_top_separator);
	gmv_inner_set_frame_one_symbol(pCGI->CFI.h_lower_separator, h_lower_separator);
	gmv_inner_set_frame_one_symbol(pCGI->CFI.v_left_separator, v_left_separator);
	gmv_inner_set_frame_one_symbol(pCGI->CFI.v_right_separator, v_right_separator);
	gmv_inner_set_frame_one_symbol(pCGI->CFI.mid_separator, mid_separator);
	return 0; //�˾�ɸ�����Ҫ�޸�
}

/***************************************************************************
  �������ƣ�
  ��    �ܣ���� CONSOLE_BORDER_TYPE �ṹ�е�ɫ��������С���Ƿ���Ҫ�ָ��ߵ�
  ������������������CONSOLE_GRAPHICS_INFO *const pCGI	������ṹָ��
			const int block_width						����ȣ�����ȱʡ2����ΪԼ�������Ϊ�����Ʊ�����������������Ҫ+1��
			const int block_high						���߶ȣ�����ȱʡ1��
			const int separator						���Ƿ���Ҫ�ָ��ߣ�0/1��
  �� �� ֵ��
  ˵    ������ܴ�С/�Ƿ���Ҫ�ָ��ߵȵı仯�ᵼ��CONSOLE_GRAPHICS_INFO�ṹ����������Աֵ�ı仯��Ҫ����
***************************************************************************/
int gmw_set_frame_style(CONSOLE_GRAPHICS_INFO *const pCGI, const int block_width, const int block_high, const bool separator)
{
	pCGI->CFI.block_width = ((max(block_width, 2) + 1) / 2) * 2;
	pCGI->CFI.block_high = max(block_high, 1);
	pCGI->CFI.separator = separator;
	gmw_inner_update_rowcol(pCGI);
	return 0; //�˾�ɸ�����Ҫ�޸�
}

/***************************************************************************
  �������ƣ�
  ��    �ܣ���� CONSOLE_BORDER_TYPE �ṹ�е���ɫ
  ���������CONSOLE_GRAPHICS_INFO *const pCGI	������ṹָ��
			const int bg_color					������ɫ��ȱʡ -1��ʾ�ô��ڱ���ɫ��
			const int fg_color					��ǰ��ɫ��ȱʡ -1��ʾ�ô���ǰ��ɫ��
  �� �� ֵ��
  ˵    �����������ɫֵ���󼰳�ͻ����Ҫ��Ϊ��֤
				������ɫ��0-15��ǰ��ɫ����ɫ��ֵһ�µ����޷��������ݵ�
***************************************************************************/
int gmw_set_frame_color(CONSOLE_GRAPHICS_INFO *const pCGI, const int bgcolor, const int fgcolor)
{
	pCGI->CFI.bgcolor = (bgcolor == -1) ? pCGI->area_bgcolor : bgcolor;
	pCGI->CFI.fgcolor = (fgcolor == -1) ? pCGI->area_fgcolor : fgcolor;
	return 0; //�˾�ɸ�����Ҫ�޸�
}

/***************************************************************************
  �������ƣ�
  ��    �ܣ���� CONSOLE_BLOCK_INFO �ṹ�е�6�����ͣ�ȱʡ4�֣�
  ���������CONSOLE_GRAPHICS_INFO *const pCGI	������ṹָ��
			const int type						��1 - ȫ˫�� 2 - ȫ���� 3 - ��˫���� 4 - �ᵥ��˫
  �� �� ֵ��
  ˵    ����
***************************************************************************/
int gmw_set_block_default_linetype(CONSOLE_GRAPHICS_INFO *const pCGI, const int type)
{
	if (type < 1 || type > 4)
		return -1;
	gmv_inner_set_frame_one_symbol(pCGI->CBI.top_left, TabElements[type - 1][FrameLeftUp]);
	gmv_inner_set_frame_one_symbol(pCGI->CBI.lower_left, TabElements[type - 1][FrameLeftDown]);
	gmv_inner_set_frame_one_symbol(pCGI->CBI.top_right, TabElements[type - 1][FrameRightUp]);
	gmv_inner_set_frame_one_symbol(pCGI->CBI.lower_right, TabElements[type - 1][FrameRightDown]);
	gmv_inner_set_frame_one_symbol(pCGI->CBI.h_normal, TabElements[type - 1][FrameHorizonal]);
	gmv_inner_set_frame_one_symbol(pCGI->CBI.v_normal, TabElements[type - 1][FrameVertical]);
	return 0; //�˾�ɸ�����Ҫ�޸�
}

/***************************************************************************
  �������ƣ�
  ��    �ܣ���� CONSOLE_BLOCK_INFO �ṹ�е�6������
  ���������CONSOLE_GRAPHICS_INFO *const pCGI	������ṹָ��
		   const char *...					����6�֣������.h���˴���
  �� �� ֵ��
  ˵    ����Լ��Ϊһ�������Ʊ��������ʹ���������ݣ���Ϊ��֤2�ֽ�
			1������2�ֽ���ֻȡǰ2�ֽ�
			2�������NULL���������ո����
			3�������1�ֽڣ���һ���ո������˶�������ʾ�ң������
***************************************************************************/
int gmw_set_block_linetype(CONSOLE_GRAPHICS_INFO *const pCGI, const char *top_left, const char *lower_left, const char *top_right, const char *lower_right, const char *h_normal, const char *v_normal)
{
	gmv_inner_set_frame_one_symbol(pCGI->CBI.top_left, top_left);
	gmv_inner_set_frame_one_symbol(pCGI->CBI.lower_left, lower_left);
	gmv_inner_set_frame_one_symbol(pCGI->CBI.top_right, top_right);
	gmv_inner_set_frame_one_symbol(pCGI->CBI.lower_right, lower_right);
	gmv_inner_set_frame_one_symbol(pCGI->CBI.h_normal, h_normal);
	gmv_inner_set_frame_one_symbol(pCGI->CBI.v_normal, v_normal);
	return 0; //�˾�ɸ�����Ҫ�޸�
}

/***************************************************************************
  �������ƣ�
  ��    �ܣ�����ÿ����Ϸɫ��(����)�Ƿ���ҪС�߿�
  ���������CONSOLE_GRAPHICS_INFO *const pCGI	������ṹָ��
		   const bool on_off					��true - ��Ҫ flase - ����Ҫ��ȱʡfalse��
  �� �� ֵ��
  ˵    �����߿�Լ��Ϊ�����Ʊ����˫��
***************************************************************************/
int gmw_set_block_border_switch(CONSOLE_GRAPHICS_INFO *const pCGI, const bool on_off)
{
	pCGI->CBI.block_border = on_off;
	return 0; //�˾�ɸ�����Ҫ�޸�
}

/***************************************************************************
  �������ƣ�
  ��    �ܣ������Ƿ���ʾ����״̬��
  ���������CONSOLE_GRAPHICS_INFO *const pCGI	������ṹָ��
			const int type						��״̬�����ͣ���/�£�
			const bool on_off					����ʾ/����ʾ��0/1��
  �� �� ֵ��
  ˵    ����1��״̬�������Լ�����£�
			1����״̬��ֻ��һ�У������������Ϸ�����/�б�����棬Ϊ��������ʼһ�У�����������Ͻ����������״̬�������꣩
			2����״̬��ֻ��һ�У������������·����ߵ�����
			3��״̬���Ŀ��Ϊ�������ȣ������Ϣ������ض�
		   2�����еı仯�ᵼ��CONSOLE_GRAPHICS_INFO�ṹ����������Աֵ�ı仯��Ҫ����
***************************************************************************/
int gmw_set_status_line_switch(CONSOLE_GRAPHICS_INFO *const pCGI, const int type, const bool on_off)
{
	if (type == TOP_STATUS_LINE)
	{
		pCGI->top_status_line = on_off;
	}
	if (type == LOWER_STATUS_LINE)
	{
		pCGI->lower_status_line = on_off;
	}
	gmw_inner_update_rowcol(pCGI);
	return 0; //�˾�ɸ�����Ҫ�޸�
}

/***************************************************************************
  �������ƣ�
  ��    �ܣ���������״̬������ɫ
  ���������CONSOLE_GRAPHICS_INFO *const pCGI	������ṹָ��
			const int type						��״̬�����ͣ���/�£�
			const int normal_bgcolor			�������ı�����ɫ��ȱʡ -1��ʾʹ�ô��ڱ���ɫ��
			const int normal_fgcolor			�������ı�ǰ��ɫ��ȱʡ -1��ʾʹ�ô���ǰ��ɫ��
			const int catchy_bgcolor			����Ŀ�ı�����ɫ��ȱʡ -1��ʾʹ�ô��ڱ���ɫ��
			const int catchy_fgcolor			����Ŀ�ı�ǰ��ɫ��ȱʡ -1��ʾʹ������ɫ��
  ���������
  �� �� ֵ��
  ˵    �����������ɫֵ���󼰳�ͻ����Ҫ��Ϊ��֤
				������ɫ��0-15��ǰ��ɫ����ɫ��ֵһ�µ����޷��������ݵ�
***************************************************************************/
int gmw_set_status_line_color(CONSOLE_GRAPHICS_INFO *const pCGI, const int type, const int normal_bgcolor, const int normal_fgcolor, const int catchy_bgcolor, const int catchy_fgcolor)
{
	if (type == TOP_STATUS_LINE)
	{
		pCGI->SLI.top_normal_bgcolor = (normal_bgcolor == -1) ? pCGI->area_bgcolor : normal_bgcolor;
		pCGI->SLI.top_normal_fgcolor = (normal_fgcolor == -1) ? pCGI->area_fgcolor : normal_fgcolor;
		pCGI->SLI.top_catchy_bgcolor = (catchy_bgcolor == -1) ? pCGI->area_bgcolor : catchy_bgcolor;
		pCGI->SLI.top_catchy_fgcolor = (catchy_fgcolor == -1) ? COLOR_HYELLOW : catchy_fgcolor;
	}
	if (type == LOWER_STATUS_LINE)
	{
		pCGI->SLI.lower_normal_bgcolor = (normal_bgcolor == -1) ? pCGI->area_bgcolor : normal_bgcolor;
		pCGI->SLI.lower_normal_fgcolor = (normal_fgcolor == -1) ? pCGI->area_fgcolor : normal_fgcolor;
		pCGI->SLI.lower_catchy_bgcolor = (catchy_bgcolor == -1) ? pCGI->area_bgcolor : catchy_bgcolor;
		pCGI->SLI.lower_catchy_fgcolor = (catchy_fgcolor == -1) ? COLOR_HYELLOW : catchy_fgcolor;
	}
	return 0; //�˾�ɸ�����Ҫ�޸�
}

/***************************************************************************
  �������ƣ�
  ��    �ܣ������Ƿ���ʾ�к�
  ���������CONSOLE_GRAPHICS_INFO *const pCGI	������ṹָ��
			const bool on_off					����ʾ/����ʾ��0/1��
  �� �� ֵ��
  ˵    ����1���к�Լ��Ϊ��ĸA��ʼ�������У��������26�����a��ʼ������52��ͳһΪ*��ʵ��Ӧ�ò����ܣ�
            2���Ƿ���ʾ�кŵı仯�ᵼ��CONSOLE_GRAPHICS_INFO�ṹ����������Աֵ�ı仯��Ҫ����
***************************************************************************/
int gmw_set_rowno_switch(CONSOLE_GRAPHICS_INFO *const pCGI, const bool on_off)
{
	pCGI->draw_frame_with_row_no = on_off;
	gmw_inner_update_rowcol(pCGI);
	return 0; //�˾�ɸ�����Ҫ�޸�
}

/***************************************************************************
  �������ƣ�
  ��    �ܣ������Ƿ���ʾ�б�
  ���������CONSOLE_GRAPHICS_INFO *const pCGI	������ṹָ��
			const bool on_off					����ʾ/����ʾ��0/1��
  �� �� ֵ��
  ˵    ����1���б�Լ��Ϊ����0��ʼ�������У�����0-99������99ͳһΪ**��ʵ��Ӧ�ò����ܣ�
            2���Ƿ���ʾ�б�ı仯�ᵼ��CONSOLE_GRAPHICS_INFO�ṹ����������Աֵ�ı仯��Ҫ����
***************************************************************************/
int gmw_set_colno_switch(CONSOLE_GRAPHICS_INFO *const pCGI, const bool on_off)
{
	pCGI->draw_frame_with_col_no = on_off;
	gmw_inner_update_rowcol(pCGI);
	return 0; //�˾�ɸ�����Ҫ�޸�
}

/***************************************************************************
  �������ƣ�
  ��    �ܣ���ӡ CONSOLE_GRAPHICS_INFO �ṹ���еĸ���Աֵ
  ���������
  �� �� ֵ��
  ˵    ����1�����������ã���ӡ��ʽ�Զ���
            2������������������δ���ù������Բ�ʵ��
***************************************************************************/
int gmw_print(const CONSOLE_GRAPHICS_INFO *const pCGI)
{
	return 0; //�˾�ɸ�����Ҫ�޸�
}

/***************************************************************************
  �������ƣ�
  ��    �ܣ��� CONSOLE_GRAPHICS_INFO �ṹ����ȱʡֵ���г�ʼ��
  ���������CONSOLE_GRAPHICS_INFO *const pCGI������ṹָ��
		   const int row					����Ϸ����ɫ��������ȱʡ10��
		   const int col					����Ϸ����ɫ��������ȱʡ10��
		   const int bgcolor				���������ڱ���ɫ��ȱʡ COLOR_BLACK��
		   const int fgcolor				���������ڱ���ɫ��ȱʡ COLOR_WHITE��
  �� �� ֵ��
  ˵    �������ڱ�����/ǰ���ף�����16*8�����������޶������У�����״̬�����У����к�/�б꣬�������Ϊ˫�ߣ�ɫ����2/�߶�1/��С�߿���ɫ��
***************************************************************************/
int gmw_init(CONSOLE_GRAPHICS_INFO *const pCGI, const int row, const int col, const int bgcolor, const int fgcolor)
{
	//memset(pCGI, 0, sizeof(_console_graphics_info_));
	pCGI->row_num = row;							//���ÿ������
	pCGI->col_num = col;
	gmw_set_color(pCGI, COLOR_BLACK, COLOR_WHITE, true);	//����Ĭ�ϴ�����ɫ
	gmw_set_font(pCGI, "��������", 16, 8);			//���ô�������
	pCGI->extern_up_lines = 0;						//��������ܶ�������
	pCGI->extern_down_lines = 0;
	pCGI->extern_left_cols = 0;
	pCGI->extern_right_cols = 0;
	pCGI->top_status_line = true;					//��������״̬��
	pCGI->lower_status_line = true;
	gmw_set_status_line_color(pCGI, TOP_STATUS_LINE, COLOR_BLACK, COLOR_WHITE, COLOR_BLACK, COLOR_HYELLOW);		//��������״̬����ɫ
	gmw_set_status_line_color(pCGI, LOWER_STATUS_LINE, COLOR_BLACK, COLOR_WHITE, COLOR_BLACK, COLOR_HYELLOW);
	pCGI->draw_frame_with_row_no = false;			//������������к�
	pCGI->draw_frame_with_col_no = false;
	gmw_set_frame_default_linetype(pCGI, 1);		//�������������
	gmw_set_block_default_linetype(pCGI, 1);		//����ɫ������
	pCGI->CFI.block_width = 2;						//����ɫ�鳤��
	pCGI->CFI.block_high = 1;
	pCGI->CFI.separator = true;						//����ɫ��ķָ���
	pCGI->CBI.block_border = false;					//����ɫ��С�߿�
	gmw_set_delay(pCGI, DELAY_OF_BLOCK_MOVED, 0);	//�����ӳ�
	gmw_set_delay(pCGI, DELAY_OF_DRAW_BLOCK, 0);
	gmw_set_delay(pCGI, DELAY_OF_DRAW_FRAME, 0);
	pCGI->SLI.top_start_x = 0;						//��������״̬����λ��
	pCGI->SLI.top_start_y = 0;
	pCGI->SLI.lower_start_x = 0;
	pCGI->SLI.lower_start_y = 0;
	gmw_inner_update_rowcol(pCGI);					//���¸�����������
	pCGI->other_setting_enable_right_mouse = 0;
	pCGI->other_setting2 = 0;
	return 0; //�˾�ɸ�����Ҫ�޸�
}

/***************************************************************************
  �������ƣ�
  ��    �ܣ�������Ϸ���
  ���������const CONSOLE_GRAPHICS_INFO *const pCGI	������ṹָ��
  �� �� ֵ��
  ˵    ��������ɲο�demo��Ч��
***************************************************************************/
int gmw_draw_frame(const CONSOLE_GRAPHICS_INFO *const pCGI)
{
	setfontsize(pCGI->CFT.font_type, pCGI->CFT.font_size_high, pCGI->CFT.font_size_width);	//�޸Ĵ�������
	setconsoleborder(pCGI->cols, pCGI->lines);	//�޸Ĵ��ڴ�С
	const int gap = !!(pCGI->CFI.separator), rowMark = !!(pCGI->draw_frame_with_row_no), colMark = !!(pCGI->draw_frame_with_col_no);
	const int row = pCGI->row_num, col = pCGI->col_num, offsetX = pCGI->start_x, offsetY = pCGI->start_y, bgColor = pCGI->CFI.bgcolor, fgColor = pCGI->CFI.fgcolor, width = pCGI->CFI.block_width, high = pCGI->CFI.block_high;
	char buf[100] = "";
	int x = 0, y = 0;
	for (int i = 0; colMark && i < col; i++)	//����б�
	{
		gmw_inner_get_col_number(i, buf);		//�����б��ַ����������в���д��һ��
		showstr(gmw_inner_get_x_in_frame(pCGI, i) + (width - strlen(buf)) / 2, gmw_inner_get_y_in_frame(pCGI, 0) - 2, buf, pCGI->area_bgcolor, pCGI->area_fgcolor);	//����б��ַ���
	}
	showstr(rowMark * 2 + offsetX, gmw_inner_get_y_in_frame(pCGI, 0) - 1, pCGI->CFI.top_left, bgColor, fgColor);	//���Ϸָ���
	for (int j = 0; j < col; j++)
	{
		for (int k = 0; k < width; k += 2)	//�������ӷ�
			showstr(gmw_inner_get_x_in_frame(pCGI, j) + k, gmw_inner_get_y_in_frame(pCGI, 0) - 1, pCGI->CFI.h_normal, bgColor, fgColor);
		if (gap)	//���Ϸָ���
			showstr(gmw_inner_get_x_in_frame(pCGI, j) + width, gmw_inner_get_y_in_frame(pCGI, 0) - 1, pCGI->CFI.h_top_separator, bgColor, fgColor);
		Sleep(pCGI->delay_of_draw_frame);		//�ӳ�
	}
	showstr(gmw_inner_get_x_in_frame(pCGI, col) - 2 * gap, gmw_inner_get_y_in_frame(pCGI, 0) - 1, pCGI->CFI.top_right, bgColor, fgColor);	//���·ָ���
	for (int i = 0; i < row; i++)	//ѭ��ÿ��
	{
		y = gmw_inner_get_y_in_frame(pCGI, i);
		if (rowMark)	//�б�
			showch(offsetX, (high + gap) * i + colMark + offsetY + high / 2 + !!(pCGI->top_status_line), gmw_inner_get_row_number(i), pCGI->area_bgcolor, pCGI->area_fgcolor); //����б�
		for (int m = 0; m < high; m++)	//ѭ��ÿ��ɫ��ĸ߶�
		{
			showstr(rowMark * 2 + offsetX, y + m, pCGI->CFI.v_normal, bgColor, fgColor);	//����������ӷ�
			for (int j = 0; j < col; j++)	//ѭ��ÿ��
			{
				showch(gmw_inner_get_x_in_frame(pCGI, j), y + m, ' ', bgColor, fgColor, width);	//����ո�
				if (gap)
				{
					showstr(gmw_inner_get_x_in_frame(pCGI, j) + width, y + m, pCGI->CFI.v_normal, bgColor, fgColor);	//����м��������ӷ�
					Sleep(pCGI->delay_of_draw_frame);		//�ӳ�
				}
			}
			showstr(gmw_inner_get_x_in_frame(pCGI, col) - 2 * gap, y + m, pCGI->CFI.v_normal, bgColor, fgColor);	//����������ӷ�
		}
		if (gap)
			showstr(rowMark * 2 + offsetX, y + high, pCGI->CFI.v_left_separator, bgColor, fgColor);	//����������ӷ�
		for (int j = 0; gap && j < col && i < row - 1; j++)
		{
			for (int k = 0; k < width; k += 2)
			{
				showstr(gmw_inner_get_x_in_frame(pCGI, j) + k, y + high, pCGI->CFI.h_normal, bgColor, fgColor);	//����м�������ӷ�
				Sleep(pCGI->delay_of_draw_frame);		//�ӳ�
			}
			showstr(gmw_inner_get_x_in_frame(pCGI, j) + width, y + high, pCGI->CFI.mid_separator, bgColor, fgColor);	//����м����ӷ�
		}
		if (gap)
			showstr(gmw_inner_get_x_in_frame(pCGI, col) - 2 * gap, y + high, pCGI->CFI.v_right_separator, bgColor, fgColor);	//����������ӷ�
		Sleep(pCGI->delay_of_draw_frame);		//�ӳ�
	}
	showstr(rowMark * 2 + offsetX, gmw_inner_get_y_in_frame(pCGI, row) - !!(pCGI->CFI.separator), pCGI->CFI.lower_left, bgColor, fgColor);	//������·ָ���
	for (int j = 0; j < col; j++)
	{
		for (int k = 0; k < width; k += 2)
			showstr(gmw_inner_get_x_in_frame(pCGI, j) + k, gmw_inner_get_y_in_frame(pCGI, row) - !!(pCGI->CFI.separator), pCGI->CFI.h_normal, bgColor, fgColor);	//����������ӷ�
		if (gap)
			showstr(gmw_inner_get_x_in_frame(pCGI, j) + width, gmw_inner_get_y_in_frame(pCGI, row) - !!(pCGI->CFI.separator), pCGI->CFI.h_lower_separator, bgColor, fgColor);	//������·ָ���
		Sleep(pCGI->delay_of_draw_frame);		//�ӳ�
	}
	showstr(gmw_inner_get_x_in_frame(pCGI, col) - 2 * gap, gmw_inner_get_y_in_frame(pCGI, row) - !!(pCGI->CFI.separator), pCGI->CFI.lower_right, bgColor, fgColor);	//������·ָ���
	return 0; //�˾�ɸ�����Ҫ�޸�
}

/***************************************************************************
  �������ƣ�
  ��    �ܣ���״̬������ʾ��Ϣ
  ���������const CONSOLE_GRAPHICS_INFO *const pCGI	������ṹָ��
		   const int type							��ָ������/��״̬��
		   const char *msg						��������Ϣ
		   const char *catchy_msg					����Ҫ�ر��ע����Ϣ����������Ϣǰ��ʾ��
  �� �� ֵ��
  ˵    ����1���������޶�Ϊ����ܵĿ�ȣ����к��б�λ�ã����������ȥ
            2��������һ���ַ���ĳ���ֵ�ǰ������ᵼ�º������룬Ҫ����
***************************************************************************/
int gmw_status_line(const CONSOLE_GRAPHICS_INFO *const pCGI, const int type, const char *msg, const char *catchy_msg)
{
	if (type == TOP_STATUS_LINE && pCGI->top_status_line)
	{
		gmw_inner_print_statue_line(msg, catchy_msg, pCGI->cols, pCGI->extern_right_cols, pCGI->extern_left_cols, pCGI->SLI.top_start_x, pCGI->SLI.top_start_y, pCGI->SLI.top_normal_bgcolor, pCGI->SLI.top_normal_fgcolor, pCGI->SLI.top_catchy_bgcolor, pCGI->SLI.top_catchy_fgcolor);
	}
	if (type == LOWER_STATUS_LINE && pCGI->lower_status_line)
	{
		gmw_inner_print_statue_line(msg, catchy_msg, pCGI->cols, pCGI->extern_right_cols, pCGI->extern_left_cols, pCGI->SLI.lower_start_x, pCGI->SLI.lower_start_y, pCGI->SLI.lower_normal_bgcolor, pCGI->SLI.lower_normal_fgcolor, pCGI->SLI.lower_catchy_bgcolor, pCGI->SLI.lower_catchy_fgcolor);
	}
	return 0; //�˾�ɸ�����Ҫ�޸�
}

/***************************************************************************
  �������ƣ�
  ��    �ܣ���ʾĳһ��ɫ��(����Ϊ�ַ���������Ϊrow/col)
  ���������const CONSOLE_GRAPHICS_INFO *const pCGI	������ṹָ��
		   const int row_no						���кţ���0��ʼ����Ϊ��֤��ȷ�ԣ����򲻼�飩
		   const int col_no						���кţ���0��ʼ����Ϊ��֤��ȷ�ԣ����򲻼�飩
		   const int bdi_value						����Ҫ��ʾ��ֵ
		   const BLOCK_DISPLAY_INFO *const bdi		����Ÿ�ֵ��Ӧ����ʾ��Ϣ�Ľṹ������
  �� �� ֵ��
  ˵    ����1��BLOCK_DISPLAY_INFO �ĺ����ͷ�ļ����÷��ο���������
            2��bdi_valueΪ BDI_VALUE_BLANK ��ʾ�հ׿飬Ҫ���⴦��
***************************************************************************/
int gmw_draw_block(const CONSOLE_GRAPHICS_INFO *const pCGI, const int row_no, const int col_no, const int bdi_value, const BLOCK_DISPLAY_INFO *const bdi)
{
	int index = -1;
	for (int i = 0; bdi[i].value != BDI_VALUE_END; i++)	//����bdi_value��Ӧ��ɫ����Ϣ
		if (bdi[i].value == bdi_value)
			index = i;
	if (index == -1)	//û���ҵ���Ӧ��ɫ����Ϣ
		return -1;
	const int gap = !!(pCGI->CFI.separator), rowMark = !!(pCGI->draw_frame_with_row_no), colMark = !!(pCGI->draw_frame_with_row_no);
	const int row = pCGI->row_num, col = pCGI->col_num, offsetX = pCGI->start_x, offsetY = pCGI->start_y, bgColor = (bdi[index].bgcolor != -1 ? bdi[index].bgcolor : pCGI->CFI.bgcolor), fgColor = (bdi[index].fgcolor != -1 ? bdi[index].fgcolor : pCGI->CFI.fgcolor), width = pCGI->CFI.block_width, high = pCGI->CFI.block_high;
	const int x = gmw_inner_get_x_in_frame(pCGI, col_no), y = gmw_inner_get_y_in_frame(pCGI, row_no);
	gmw_inner_draw_block(pCGI, x, y, width, high, index, bgColor, fgColor, pCGI->delay_of_draw_block, bdi);
	setcolor(pCGI->area_bgcolor, pCGI->area_fgcolor);
	return 0; //�˾�ɸ�����Ҫ�޸�
}

/***************************************************************************
  �������ƣ�
  ��    �ܣ��ƶ�ĳһ��ɫ��
  ���������const CONSOLE_GRAPHICS_INFO *const pCGI	������ṹָ��
		   const int row_no						���кţ���0��ʼ����Ϊ��֤��ȷ�ԣ����򲻼�飩
		   const int col_no						���кţ���0��ʼ����Ϊ��֤��ȷ�ԣ����򲻼�飩
		   const int bdi_value						����Ҫ��ʾ��ֵ
		   const int blank_bdi_value				���ƶ����������ڶ���Ч����ʾʱ���ڱ�ʾ�հ׵�ֵ��һ��Ϊ0���˴���Ϊ�������룬�ǿ��ǵ����ܳ��ֵ����������
		   const BLOCK_DISPLAY_INFO *const bdi		�������ʾֵ/�հ�ֵ��Ӧ����ʾ��Ϣ�Ľṹ������
		   const int direction						���ƶ�����һ�����֣������cmd_gmw_tools.h
		   const int distance						���ƶ����루��1��ʼ����Ϊ��֤��ȷ�ԣ����򲻼�飩
  �� �� ֵ��
  ˵    ����
***************************************************************************/
int gmw_move_block(const CONSOLE_GRAPHICS_INFO *const pCGI, const int row_no, const int col_no, const int bdi_value, const int blank_bdi_value, const BLOCK_DISPLAY_INFO *const bdi, const int direction, const int distance)
{
	if (distance <= 0 || pCGI == NULL)
		return -1;
	int index = -1;
	for (int i = 0; bdi[i].value != BDI_VALUE_END; i++)	//����bdi_value��Ӧ��ɫ����Ϣ
		if (bdi[i].value == bdi_value)
			index = i;
	if (index == -1)	//û���ҵ���Ӧ��ɫ����Ϣ
		return -1;
	const int bgColor = (bdi[index].bgcolor != -1) ? bdi[index].bgcolor : pCGI->CFI.bgcolor;
	const int fgColor = (bdi[index].fgcolor != -1) ? bdi[index].fgcolor : pCGI->CFI.fgcolor;
	const int blankBgColor = (bdi[blank_bdi_value].bgcolor != -1) ? bdi[blank_bdi_value].bgcolor : pCGI->CFI.bgcolor;
	const int blankFgColor = (bdi[blank_bdi_value].fgcolor != -1) ? bdi[blank_bdi_value].fgcolor : pCGI->CFI.fgcolor;
	const int width = pCGI->CFI.block_width, high = pCGI->CFI.block_high;
	const int x = gmw_inner_get_x_in_frame(pCGI, col_no), y = gmw_inner_get_y_in_frame(pCGI, row_no);
	const int symbol = (direction == UP_TO_DOWN || direction == LEFT_TO_RIGHT) ? 1 : -1;	//����
	CONSOLE_GRAPHICS_INFO *tCGI = new CONSOLE_GRAPHICS_INFO;	//����delay_of_draw_block��ֵ���ӿ���ʾ�ٶ�
	memcpy(tCGI, pCGI, sizeof(CONSOLE_GRAPHICS_INFO));
	tCGI->delay_of_draw_block = 0;
	if (direction == UP_TO_DOWN || direction == DOWN_TO_UP)						//�����ƶ�
	{
		for (int i = 0; i < distance * (high + !!(tCGI->CFI.separator)); i++)	//�ƶ�distance������
		{
			if (tCGI->CBI.block_border)				//���б߿�����
			{
				if (direction == UP_TO_DOWN)		//����
				{
					if (tCGI->CFI.separator && i % (high + 1) == 0)				//����ָ���
					{
						showstr(x, y + (i - 1) * symbol, tCGI->CFI.h_normal, tCGI->CFI.bgcolor, tCGI->CFI.fgcolor, width / 2);
						showstr(x, y + (i - 0) * symbol, " ", blankBgColor, blankFgColor, width);
					}
					else if (!(i == 0 && !tCGI->CFI.separator))												//����ո񣬲���ԭ����ͼ����ע��ѭ����һ��ʱ���ܲ���
						showstr(x, y + (i - 0) * symbol, " ", blankBgColor, blankFgColor, width);
				}
				else								//����
				{
					if (tCGI->CFI.separator && i % (high + 1) == 0)				//����ָ���
					{
						showstr(x, y + (i - high) * symbol, tCGI->CFI.h_normal, tCGI->CFI.bgcolor, tCGI->CFI.fgcolor, width / 2);
						showstr(x, y + (i - high + 1) * symbol, " ", blankBgColor, blankFgColor, width);
					}
					else if(!(i == 0 && !tCGI->CFI.separator))												//����ո񣬲���ԭ����ͼ����ע��ѭ����һ��ʱ���ܲ���
						showstr(x, y + (i - high + 1) * symbol, " ", blankBgColor, blankFgColor, width);
				}
			}
			else									//�����߿�����
			{
				if (tCGI->CFI.separator && i % (high + 1) == high - high / 2)	//����ָ���
					showstr(x, y + (i)* symbol, tCGI->CFI.h_normal, tCGI->CFI.bgcolor, tCGI->CFI.fgcolor, width / 2);
				else															//����ո񣬲���ԭ����ͼ��
					showstr(x, y + i * symbol + high / 2, " ", blankBgColor, blankFgColor, width);
			}
			gmw_inner_draw_block(tCGI, x, y + (i + 1) * symbol, width, high, index, bgColor, fgColor, tCGI->delay_of_draw_block, bdi);	//����ɫ��
			Sleep(tCGI->delay_of_block_moved);						//�ӳ�
		}
		showstr(x, y + (distance * (high + !!(tCGI->CFI.separator)) - ((direction == UP_TO_DOWN) ? 1 : high)) * symbol, (tCGI->CFI.separator ? tCGI->CFI.h_normal : "  "), tCGI->CFI.bgcolor, tCGI->CFI.fgcolor, width / 2);	//�����������ͼ��������ո��ָ���
	}
	if (direction == LEFT_TO_RIGHT || direction == RIGHT_TO_LEFT)				//�����ƶ�
	{
		for (int i = 0; i < distance * (width + 2 * !!(tCGI->CFI.separator)); i++)
		{
			if (tCGI->CBI.block_border)				//���б߿�����
			{
				if (direction == LEFT_TO_RIGHT)		//����
				{
					if (tCGI->CFI.separator && i % (width + 2) == 0)			//����ָ���
						for (int k = 0; k < high; k++)
						{
							showstr(x + (i - 2) * symbol, y + k, tCGI->CFI.v_normal, tCGI->CFI.bgcolor, tCGI->CFI.fgcolor);
							showstr(x + (i - 0) * symbol, y + k, " ", blankBgColor, blankFgColor);
						}
					else if(!(i == 0 && !tCGI->CFI.separator))														//����ո񣬲���ԭ����ͼ����ע��ѭ����һ��ʱ���ܲ���
						for (int k = 0; k < high; k++)
							showstr(x + (i - 0) * symbol, y + k, " ", blankBgColor, blankFgColor);
				}
				else								//����
				{
					if (tCGI->CFI.separator && i % (width + 2) == 0)			//����ָ���
						for (int k = 0; k < high; k++)
						{
							showstr(x + (i - width + 0) * symbol, y + k, tCGI->CFI.v_normal, tCGI->CFI.bgcolor, tCGI->CFI.fgcolor);
							showstr(x + (i - width + 1) * symbol, y + k, " ", blankBgColor, blankFgColor);
						}
					else if (!(i == 0 && !tCGI->CFI.separator))														//����ո񣬲���ԭ����ͼ����ע��ѭ����һ��ʱ���ܲ���
						for (int k = 0; k < high; k++)
							showstr(x + (i - width + 1) * symbol, y + k, " ", blankBgColor, blankFgColor);
				}
			}
			else									//�����߿�����
			{
				if (direction == LEFT_TO_RIGHT)
				{
					if (tCGI->CFI.separator && i % (width + 2) == width + 1)	//����ָ���
						for (int k = 0; k < high; k++)
							showstr(x + (i - 1) * symbol, y + k, tCGI->CFI.v_normal, tCGI->CFI.bgcolor, tCGI->CFI.fgcolor);
					else														//����ո񣬲���ԭ����ͼ��
						showstr(x + i * symbol, y + high / 2, " ", blankBgColor, blankFgColor);
				}
				else
				{
					if (tCGI->CFI.separator && i % (width + 2) == width + 1)	//����ָ���
						for (int k = 0; k < high; k++)
							showstr(x + (i - 1) * symbol + width - 2, y + k, tCGI->CFI.v_normal, tCGI->CFI.bgcolor, tCGI->CFI.fgcolor);
					else														//����ո񣬲���ԭ����ͼ��
						showstr(x + i * symbol + width - 1, y + high / 2, " ", blankBgColor, blankFgColor);
				}
			}
			gmw_inner_draw_block(tCGI, x + (i + 1) * symbol, y, width, high, index, bgColor, fgColor, tCGI->delay_of_draw_block, bdi);
			Sleep(tCGI->delay_of_block_moved / 2);						//�ӳ�
		}
		for (int k = 0; k < high; k++)
			showstr(x + (distance * (width + 2 * !!(tCGI->CFI.separator)) - (direction == LEFT_TO_RIGHT ? 2 : width)) * symbol, y + k, (tCGI->CFI.separator ? tCGI->CFI.v_normal : "  "), tCGI->CFI.bgcolor, tCGI->CFI.fgcolor);	//�����������ͼ��������ո��ָ���
	}
	delete tCGI;
	return 0; //�˾�ɸ�����Ҫ�޸�
}

/***************************************************************************
  �������ƣ�
  ��    �ܣ������̻����
  ���������const CONSOLE_GRAPHICS_INFO *const pCGI	������ṹָ��
		   int &MAction							��������� CCT_MOUSE_EVENT�����ֵ��Ч��Ϊ MOUSE_ONLY_MOVED/MOUSE_LEFT_BUTTON_CLICK/MOUSE_RIGHT_BUTTON_CLICK ����֮һ
													   ������� CCT_KEYBOARD_EVENT�����ֵ��Ч
		   int &MRow								��������� CCT_MOUSE_EVENT �� MAction = MOUSE_ONLY_MOVED/MOUSE_LEFT_BUTTON_CLICK�����ֵ��Ч����ʾ���ѡ�����Ϸ������кţ���0��ʼ��
												  ���������ֵ��Ч�����������Чֵ���´��󣬲��Ǳ������Ĵ�!!!��
		   int &MCol								��������� CCT_MOUSE_EVENT �� MAction = MOUSE_ONLY_MOVED/MOUSE_LEFT_BUTTON_CLICK�����ֵ��Ч����ʾ���ѡ�����Ϸ������кţ���0��ʼ��
												  ���������ֵ��Ч�����������Чֵ���´��󣬲��Ǳ������Ĵ�!!!��
		   int &KeyCode1							��������� CCT_KEYBOARD_EVENT�����ֵ��Ч��Ϊ�����ļ��루���˫���룬��Ϊ��һ����
												  ���������ֵ��Ч�����������Чֵ���´��󣬲��Ǳ������Ĵ�!!!��
		   int &KeyCode2							��������� CCT_KEYBOARD_EVENT�����ֵ��Ч��Ϊ�����ļ��루���˫���룬��Ϊ�ڶ���������ǵ����룬��Ϊ0��
												  ���������ֵ��Ч�����������Чֵ���´��󣬲��Ǳ������Ĵ�!!!��
		   const bool update_lower_status_line		������ƶ�ʱ���Ƿ�Ҫ�ڱ���������ʾ"[��ǰ���] *��*��/λ�÷Ƿ�"����Ϣ��true=��ʾ��false=����ʾ��ȱʡΪtrue��
  �� �� ֵ����������Լ��
		   1�����������ƶ����õ���MRow/MCol�봫�����ͬ(���ָ��΢С���ƶ�)���򲻷��أ�������
							  �õ����зǷ�λ�ã��򲻷��أ����� update_lower_status_line ����������״̬����ʾ"[��ǰ���] λ�÷Ƿ�"
							  �õ���MRow/MCol�봫��Ĳ�ͬ(��������һ���仯)������ update_lower_status_line ����������״̬����ʾ"[��ǰ���] *��*��"���ٷ���MOUSE_ONLY_MOVED����Щ��Ϸ���غ�Ҫ����ɫ��Ĳ�ͬ��ɫ��ʾ��
		   2������ǰ������������ҵ�ǰ���ָ��ͣ��������Ϸ�����*��*���ϣ��򷵻� CCT_MOUSE_EVENT ��MAction Ϊ MOUSE_LEFT_BUTTON_CLICK, MRow Ϊ�кţ�MCol Ϊ�б�
								  �ҵ�ǰ���ָ��ͣ���ڷǷ����򣨷���Ϸ������Ϸ�����еķָ��ߣ����򲻷��أ����� update_lower_status_line ����������״̬����ʾ"[��ǰ���] λ�÷Ƿ�"
		   3������ǰ�������Ҽ������ж����ָ��ͣ�������Ƿ�Ϸ���ֱ�ӷ��� CCT_MOUSE_EVENT ��MAction Ϊ MOUSE_LEFT_BUTTON_CLICK, MRow��MCol������
		   4��������¼����ϵ�ĳ������˫���밴��������ֱ�ӷ��� CCT_KEYBOARD_EVENT��KeyCode1/KeyCode2��Ϊ��Ӧ�ļ���ֵ
 ˵    ����ͨ������ cmd_console_tools.cpp �е� read_keyboard_and_mouse ����ʵ��
***************************************************************************/
int gmw_read_keyboard_and_mouse(const CONSOLE_GRAPHICS_INFO *const pCGI, int &MAction, int &MRow, int &MCol, int &KeyCode1, int &KeyCode2, const bool update_lower_status_line)
{
	static const HANDLE __hin = GetStdHandle(STD_INPUT_HANDLE);		//ȡ��׼�����豸��Ӧ�ľ��
	static int MX_old = -1, MY_old = -1, MAction_old = MOUSE_ONLY_MOVED, MX, MY, MRow_old = -1, MCol_old = -1;
	INPUT_RECORD InputRec;
	DWORD        res;
	COORD        crPos;
	enable_mouse();
	while (1) 
	{
		/* ��hin�ж�����״̬��������ꡢ���̵ȣ� */
		ReadConsoleInput(__hin, &InputRec, 1, &res);

		/* �����¼���Ҫ��������¼����������������Ŀ�������޷������� */
		if (InputRec.EventType == KEY_EVENT) {
			KeyCode1 = 0x00;
			KeyCode2 = 0x00;
			if (InputRec.Event.KeyEvent.bKeyDown)	//ֻ�ڰ���ʱ�жϣ�����ʱ���ж�
			{
				switch (InputRec.Event.KeyEvent.wVirtualKeyCode) {
					case VK_UP:
						KeyCode1 = 0xe0;
						KeyCode2 = KB_ARROW_UP;	//ģ�� _getch()��ʽ���ص��������룬�ֱ���224��0xE0����72��0x48��
						break;
					case VK_DOWN:
						KeyCode1 = 0xe0;
						KeyCode2 = KB_ARROW_DOWN;	//ģ�� _getch()��ʽ���ص��������룬�ֱ���224��0xE0����80��0x50��
						break;
					case VK_LEFT:
						KeyCode1 = 0xe0;
						KeyCode2 = KB_ARROW_LEFT;	//ģ�� _getch()��ʽ���ص��������룬�ֱ���224��0xE0����75��0x4B��
						break;
					case VK_RIGHT:
						KeyCode1 = 0xe0;
						KeyCode2 = KB_ARROW_RIGHT;	//ģ�� _getch()��ʽ���ص��������룬�ֱ���224��0xE0����77��0x4D��
						break;
					case VK_PRIOR:
						KeyCode1 = 0xe0;
						KeyCode2 = VK_PRIOR;		//Page Up
						break;
					case VK_NEXT:
						KeyCode1 = 0xe0;
						KeyCode2 = VK_NEXT;			//Page Down
						break;
					default:
						break;
				} //end of switch

				  /* �Ǽ�ͷ��ֱ�ӷ���ASCII��ʽ��Fn��Insert��Delete�Ⱦ�δ���� */
				if (KeyCode1 == 0)
					KeyCode1 = InputRec.Event.KeyEvent.uChar.AsciiChar;

				return CCT_KEYBOARD_EVENT;
			} //end of if (KEYDOWN)
		} // end of if (�����¼�)

		/* ����¼� */
		if (InputRec.EventType == MOUSE_EVENT)	/* �ӷ����ж����ָ�뵱ǰ������ */
		{
			crPos = InputRec.Event.MouseEvent.dwMousePosition;
			MX = crPos.X;
			MY = crPos.Y;

			if (InputRec.Event.MouseEvent.dwEventFlags == MOUSE_MOVED)	//����ƶ�
			{
				/* ���ʼ����MOUSE_MOVED�¼������겻�䣬����Ϊ��MOUSE_MOVED */
				if (MX_old == MX && MY_old == MY && MAction_old == MOUSE_ONLY_MOVED)
					continue;
				if (pCGI && gmw_inner_check_rowcol_in_frame(pCGI, MX, MY, MRow, MCol) == true)	//���������ڿ�ܵ�ĳ��ɫ����
				{
					if (MRow_old != MRow || MCol_old != MCol)	//��������ָ��ɫ�������仯
					{
						gmw_inner_update_lower_statue_line(pCGI, MRow, MCol);	//������״̬����Ϣ
						MAction = MOUSE_ONLY_MOVED;
						disable_mouse();
						return CCT_MOUSE_EVENT;
					}
					MRow_old = MRow;	//��¼��ɫ��
					MCol_old = MCol;
				}
				MX_old = MX;	//��¼��λ��
				MY_old = MY;
				MAction = MOUSE_ONLY_MOVED;	//�޸�Maction
				MAction_old = MAction;		//��¼��Maction
			}
			MAction_old = MOUSE_NO_ACTION; //�÷�MOUSE_ONLY_MOVEDֵ����

			if (InputRec.Event.MouseEvent.dwButtonState == FROM_LEFT_1ST_BUTTON_PRESSED)		//�������
			{
				if (pCGI && gmw_inner_check_rowcol_in_frame(pCGI, MX, MY, MRow, MCol) == false)	//������û�����ڿ�ܵ�ĳ��ɫ����
				{
					gmw_status_line(pCGI, LOWER_STATUS_LINE, "[��ǰ���] λ�÷Ƿ�");	//������״̬����Ϣ
				}
				else
				{
					MAction = MOUSE_LEFT_BUTTON_CLICK;	//�޸�Maction���˳�����
					disable_mouse();
					return CCT_MOUSE_EVENT;
				}
			}
			else if (InputRec.Event.MouseEvent.dwButtonState == RIGHTMOST_BUTTON_PRESSED)		//�����Ҽ�
			{
				if (pCGI && pCGI->other_setting_enable_right_mouse == 1 && gmw_inner_check_rowcol_in_frame(pCGI, MX, MY, MRow, MCol) == false)	//������û�����ڿ�ܵ�ĳ��ɫ����
				{
					gmw_status_line(pCGI, LOWER_STATUS_LINE, "[��ǰ���] λ�÷Ƿ�");	//������״̬����Ϣ
				}
				else
				{
					MAction = MOUSE_RIGHT_BUTTON_CLICK;	//�޸�Maction���˳�����
					disable_mouse();
					return CCT_MOUSE_EVENT;
				}
			}
			else //����������������
				continue;
		} // end of if(����¼�)
	} //end of while(1)

	return CCT_MOUSE_EVENT;
}

//1850059 计1班 杨志远

#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <climits>
#include <conio.h>
#include "cmd_console_tools.h"
#include "cmd_gmw_tools.h"
using namespace std;

/* --------------------------------------------------
		此处可以给出需要的静态全局变量（尽可能少，最好没有）、静态全局只读变量/宏定义（个数不限）等
   -------------------------------------------------- */

//1 - 全双线 2 - 全单线 3 - 横双竖单 4 - 横单竖双
enum TabSettings
{
	TabAllDouble = 1,
	TabAllSingle = 2,
	TabDoubleSingle = 3,
	TabSingleDouble = 4,
};

//与TabElements对应
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

//1 - 全双线 2 - 全单线 3 - 横双竖单 4 - 横单竖双
const char TabElements[4][11][3] = 
{ 
	{"═", "║", "╔", "╗", "╚", "╝", "╠", "╣", "╦", "╩", "╬"},
	{"━", "┃", "┏", "┓", "┗", "┛", "┣", "┫", "┳", "┻", "╋"},
	{"═", "│", "╒", "╕", "╘", "╛", "╞", "╡", "╤", "╧", "╪"},
	{"─", "║", "╓", "╖", "╙", "╜", "╟", "╢", "╥", "╨", "╫"},
};

/* --------------------------------------------------
		此处可以给出需要的内部辅助工具函数
		1、函数名不限，建议为 gmw_inner_* 
		2、个数不限
		3、必须是static函数，确保只在本源文件中使用
   -------------------------------------------------- */

//复制一个类似TabElements的字符串source到buf，返回buf的长度
static int gmv_inner_set_frame_one_symbol(char *buf, const char *sourse)
{
	char tempStr[3] = "  ";
	int len = strlen(sourse);
	if (len >= 2)		//source字符串长度大于2
	{
		strncpy(tempStr, sourse, 2);
	}
	else if (len == 1)	//source字符串长度不足
	{
		tempStr[0] = sourse[0];
	}
	strcpy(buf, tempStr);
	return strlen(buf);
}

//重新计算一次pCGI中与row和col有关的数据，改变窗口大小
static int gmw_inner_update_rowcol(CONSOLE_GRAPHICS_INFO *const pCGI)
{
	pCGI->start_y = pCGI->extern_up_lines + !!(pCGI->top_status_line);
	pCGI->start_x = pCGI->extern_left_cols;
	pCGI->lines = pCGI->row_num * pCGI->CFI.block_high + pCGI->row_num * !!(pCGI->CFI.separator) + pCGI->extern_up_lines + pCGI->extern_down_lines + !!(pCGI->top_status_line) + !!(pCGI->lower_status_line) + !!(pCGI->draw_frame_with_col_no) + 4 + !(pCGI->CFI.separator) + 1;
	pCGI->cols  = pCGI->col_num * pCGI->CFI.block_width + pCGI->col_num * 2 * !!(pCGI->CFI.separator) + pCGI->extern_left_cols + pCGI->extern_right_cols + !!(pCGI->draw_frame_with_row_no) * 2 + 2 * !(pCGI->CFI.separator) + 3;
	pCGI->SLI.lower_start_y = pCGI->lines - pCGI->extern_down_lines - 5;
	return 0; //此句可根据需要修改
}

//将列数转化为窗口的x坐标
static int gmw_inner_get_x_in_frame(const CONSOLE_GRAPHICS_INFO *const pCGI, const int col)
{
	return 2 + (pCGI->CFI.block_width + 2 * !!(pCGI->CFI.separator)) * col + !!(pCGI->draw_frame_with_row_no) * 2 + pCGI->start_x;
}

//将行数转化为窗口的y坐标
static int gmw_inner_get_y_in_frame(const CONSOLE_GRAPHICS_INFO *const pCGI, const int row)
{
	return (pCGI->CFI.block_high + !!(pCGI->CFI.separator)) * row + !!(pCGI->draw_frame_with_col_no) + pCGI->start_y + 1;
}

//将x与y转化为框架中的row和col，如果失败则返回false，如果成功则返回true
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

//将行标转化为一个对应的字符，返回这个字符
static char gmw_inner_get_row_number(const int row_No)
{
	if (row_No >= 0 && row_No < 26)			//A到Z
	{
		return 'A' + row_No;
	}
	else if (row_No >= 26 && row_No < 52)	//a到z
	{
		return 'a' + row_No - 26;
	}
	else									//更大的数
	{
		return '*';
	}
}

//将列表转化为一个对应的字符串，返回这个字符串的头指针
static char* gmw_inner_get_col_number(const int col_No, char buf[])
{
	if (col_No >= 0 && col_No <= 9)			//个位数
	{
		buf[0] = '0' + col_No;
		buf[1] = '\0';
	}
	else if (col_No >= 10 && col_No <= 99)	//十位数
	{
		buf[0] = '0' + col_No / 10;
		buf[1] = '0' + col_No % 10;
		buf[2] = '\0';
	}
	else									//更大的数
	{
		buf[0] = '*';
		buf[1] = '*';
		buf[2] = '\0';
	}
	return buf;
}

//修改下状态栏中的光标位置信息
static void gmw_inner_update_lower_statue_line(const CONSOLE_GRAPHICS_INFO *const pCGI, const int row, const int col)
{
	int cur = 0;
	char temp[100] = "";
	cur += sprintf(&temp[cur], "[当前光标] %c行%d列", gmw_inner_get_row_number(row), col);
	gmw_status_line(pCGI, LOWER_STATUS_LINE, temp);
}

//输出一行状态栏中的信息
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
	showch(start_x, start_y, ' ', normal_bgcolor, normal_fgcolor, cols - extern_right_cols - extern_left_cols - 1);	//刷新当前行
	if (tempCatchyMsg == NULL)
	{
		showstr(start_x, start_y, tempMsg, normal_bgcolor, normal_fgcolor, 1, cols - extern_right_cols - extern_left_cols - 1);			//只输出正常信息
	}
	else
	{
		showstr(start_x, start_y, tempCatchyMsg, catchy_bgcolor, catchy_fgcolor);				//先输出强调信息
		showstr(start_x + strlen(tempCatchyMsg), start_y, tempMsg,normal_bgcolor, normal_fgcolor, 1, cols - extern_right_cols - extern_left_cols - 1 - strlen(tempCatchyMsg));//再输出正常信息
	}
}

//以(x,y)为左上角，绘制一个色块
static int gmw_inner_draw_block(const CONSOLE_GRAPHICS_INFO *const pCGI, const int x, const int y, const int width, const int high, const int index, const int bgColor, const int fgColor, int delay, const BLOCK_DISPLAY_INFO *const bdi)
{
	if (bdi[index].value == BDI_VALUE_BLANK)
	{
		for (int i = 0; i < high; i++)
			showch(x, y + i, ' ', pCGI->CFI.bgcolor, pCGI->CFI.fgcolor, width);
		Sleep(delay);
		return 0;
	}
	if (pCGI->CBI.block_border && bdi[index].value != BDI_VALUE_BLANK)	//输出外边框
	{
		showstr(x, y, pCGI->CBI.top_left, bgColor, fgColor);	//左上
		for (int j = 2; j < width - 2; j += 2)
		{
			showstr(x + j, y, pCGI->CBI.h_normal, bgColor, fgColor);	//上
		}
		showstr(x + width - 2, y, pCGI->CBI.top_right, bgColor, fgColor);	//右上
		for (int i = 1; i < high - 1; i++)
		{
			showstr(x, y + i, pCGI->CBI.v_normal, bgColor, fgColor);	//左
			showch(x + 2, y + i, ' ', bgColor, fgColor, width - 4);		//空额填充一整行
			showstr(x + width - 2, y + i, pCGI->CBI.v_normal, bgColor, fgColor);	//右
		}
		showstr(x, y + high - 1, pCGI->CBI.lower_left, bgColor, fgColor);	//左下
		for (int j = 2; j < width - 2; j += 2)
		{
			showstr(x + j, y + high - 1, pCGI->CBI.h_normal, bgColor, fgColor);	//下
		}
		showstr(x + width - 2, y + high - 1, pCGI->CBI.lower_right, bgColor, fgColor);	//右下
	}
	if (bdi[index].content == NULL)					//没有内容
		showint(x + (width - int(log10(bdi[index].value)) - 1) / 2, y + high / 2, bdi[index].value, bgColor, fgColor);	//输出数字
	else if (bdi[index].value != BDI_VALUE_END)		//没有到末尾或者遇到空格，正常输出
		showstr(x + (width - strlen(bdi[index].content)) / 2, y + high / 2, bdi[index].content, bgColor, fgColor);	//输出content
	Sleep(delay);
	return 0;
}

/* ----------------------------------------------- 
		实现下面给出的函数（函数声明不准动）
   ----------------------------------------------- */
/***************************************************************************
  函数名称：
  功    能：设置游戏主框架的行列数
  输入参数：CONSOLE_GRAPHICS_INFO *const pCGI	：整体结构指针
			const int row						：行数(错误则为0，不设上限，人为保证正确性)
			const int col						：列数(错误则为0，不设上限，人为保证正确性)
  返 回 值：
  说    明：1、指消除类游戏的矩形区域的行列值
            2、行列的变化会导致CONSOLE_GRAPHICS_INFO结构体中其它成员值的变化，要处理
***************************************************************************/
int gmw_set_rowcol(CONSOLE_GRAPHICS_INFO *const pCGI, const int row, const int col)
{
	int _row = max(row, 0), _col = max(col, 0);
	pCGI->row_num = _row;
	pCGI->col_num = _col;
	gmw_inner_update_rowcol(pCGI);
	return 0; //此句可根据需要修改
}

/***************************************************************************
  函数名称：
  功    能：设置整个窗口（含游戏区、附加区在内的整个cmd窗口）的颜色
  输入参数：CONSOLE_GRAPHICS_INFO *const pCGI	：整体结构指针
		   const int bg_color					：前景色（缺省COLOR_BLACK）
		   const int fg_color					：背景色（缺省COLOR_WHITE）
		   const int cascade					：是否级联（取值0/1，缺省为0-不级联）
  返 回 值：
  说    明：1、cascade = 1时
				同步修改游戏主区域的颜色
				同步修改上下状态栏的正常文本的背景色和前景色，醒目文本的背景色（前景色不变）
			2、不检查颜色值错误及冲突，需要人为保证
				例：颜色非0-15
				    前景色背景色的值一致导致无法看到内容
					前景色正好是状态栏醒目前景色，导致无法看到醒目提示
					...
***************************************************************************/
int gmw_set_color(CONSOLE_GRAPHICS_INFO *const pCGI, const int bgcolor, const int fgcolor, const bool cascade)
{
	pCGI->area_bgcolor = bgcolor;
	pCGI->area_fgcolor = fgcolor;
	setcolor(bgcolor, fgcolor);			//修改窗口颜色

	if (cascade == 1)					//级联
	{
		pCGI->CFI.bgcolor = bgcolor;	//框架前景色和背景色
		pCGI->CFI.fgcolor = fgcolor;

		pCGI->SLI.top_normal_bgcolor = bgcolor;		//上状态栏前景色和背景色
		pCGI->SLI.top_normal_fgcolor = fgcolor;
		pCGI->SLI.top_catchy_bgcolor = bgcolor;

		pCGI->SLI.lower_normal_bgcolor = bgcolor;	//下状态栏前景色和背景色
		pCGI->SLI.lower_normal_fgcolor = fgcolor;
		pCGI->SLI.lower_catchy_bgcolor = bgcolor;
	}

	return 0; //此句可根据需要修改
}

/***************************************************************************
  函数名称：
  功    能：设置窗口的字体
  输入参数：CONSOLE_GRAPHICS_INFO *const pCGI	：整体结构指针
		   const char *fontname					：字体名称（只能是"Terminal"和"新宋体"两种，错误则返回-1，不改变字体）
		   const int fs_high					：字体高度（缺省及错误为16，不设其它限制，人为保证）
		   const int fs_width					：字体高度（缺省及错误为8，不设其它限制，人为保证）
  返 回 值：
  说    明：1、与cmd_console_tools中的setfontsize相似，目前只支持“点阵字体”和“新宋体”
			2、若设置其它字体则直接返回，保持原字体设置不变
***************************************************************************/
int gmw_set_font(CONSOLE_GRAPHICS_INFO *const pCGI, const char *fontname, const int fs_high, const int fs_width)
{
	if (strcmp(fontname, "新宋体") == 0 || strcmp(fontname, "Terminal") == 0)		//新宋体或点阵字体
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
  函数名称：
  功    能：设置延时
  输入参数：CONSOLE_GRAPHICS_INFO *const pCGI	：整体结构指针
		   const int type						：延时的类型（目前为3种）
		   const int delay_ms					：以ms为单位的延时
			   画边框的延时：0 ~ 不设上限，人为保证正确（<0则置0）
			   画色块的延时：0 ~ 不设上限，人为保证正确（<0则置0）
			   色块移动的延时：BLOCK_MOVED_DELAY_MS ~ 不设上限，人为保证正确（ <BLOCK_MOVED_DELAY_MS 则置 BLOCK_MOVED_DELAY_MS）
  返 回 值：
  说    明：
***************************************************************************/
int gmw_set_delay(CONSOLE_GRAPHICS_INFO *const pCGI, const int type, const int delay_ms)
{
	if (type == DELAY_OF_DRAW_FRAME)	//画主框架
	{
		pCGI->delay_of_draw_frame = max(0, delay_ms);
	}
	if (type == DELAY_OF_DRAW_BLOCK)	//画色块
	{
		pCGI->delay_of_draw_block = max(0, delay_ms);
	}
	if (type == DELAY_OF_BLOCK_MOVED)	//色块移动
	{
		pCGI->delay_of_block_moved = max(BLOCK_MOVED_DELAY_MS, delay_ms);
	}
	return 0; //此句可根据需要修改
}

/***************************************************************************
  函数名称：
  输入参数：设置游戏主框架结构之外需要保留的额外区域
  功    能：CONSOLE_GRAPHICS_INFO *const pCGI	：
		   const int up_lines					：上部额外的行（缺省及错误为0，不设上限，人为保证）
		   const int down_lines				：下部额外的行（缺省及错误为0，不设上限，人为保证）
		   const int left_cols					：左边额外的列（缺省及错误为0，不设上限，人为保证）
		   const int right_cols				：右边额外的列（缺省及错误为0，不设上限，人为保证）
  返 回 值：
  说    明：额外行列的变化会导致CONSOLE_GRAPHICS_INFO结构体中其它成员值的变化，要处理
***************************************************************************/
int gmw_set_ext_rowcol(CONSOLE_GRAPHICS_INFO *const pCGI, const int up_lines, const int down_lines, const int left_cols, const int right_cols)
{
	int upLines = max(up_lines, 0), downLines = max(down_lines, 0), leftCols = max(left_cols, 0), rightCols = max(right_cols, 0);
	pCGI->extern_up_lines = upLines;		//上下左右额外的行列
	pCGI->extern_down_lines = downLines;
	pCGI->extern_left_cols = leftCols;
	pCGI->extern_right_cols = rightCols;

	pCGI->SLI.top_start_x = leftCols;		//上状态栏的位置
	pCGI->SLI.top_start_y = upLines;

	pCGI->SLI.lower_start_x = leftCols;	//下状态栏的x坐标

	gmw_inner_update_rowcol(pCGI);

	return 0; //此句可根据需要修改
}

/***************************************************************************
  函数名称：
  功    能：填充 CONSOLE_BORDER_TYPE 结构中的11种线型（缺省4种）
  输入参数：CONSOLE_GRAPHICS_INFO *const pCGI	：整体结构指针
			const int type						：1 - 全线 2 - 全单线 3 - 横双竖单 4 - 横单竖双
  返 回 值：
  说    明：
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
	return 0; //此句可根据需要修改
}

/***************************************************************************
  函数名称：
  功    能：填充 CONSOLE_BORDER_TYPE 结构中的11种线型
  输入参数：CONSOLE_GRAPHICS_INFO *const pCGI	：整体结构指针
			const char *...						：共11种，具体见.h，此处略
  返 回 值：
  说    明：约定为一个中文制表符，可以使用其它内容，人为保证2字节
			1、超过2字节则只取前2字节
			2、如果给NULL，用两个空格替代
			3、如果给1字节，则补一个空格，如果因此而导致显示乱，不算错
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
	return 0; //此句可根据需要修改
}

/***************************************************************************
  函数名称：
  功    能：填充 CONSOLE_BORDER_TYPE 结构中的色块数量大小、是否需要分隔线等
  输入参数：输入参数：CONSOLE_GRAPHICS_INFO *const pCGI	：整体结构指针
			const int block_width						：宽度（错误及缺省2，因为约定表格线为中文制表符，如果给出奇数，要+1）
			const int block_high						：高度（错误及缺省1）
			const int separator						：是否需要分隔线（0/1）
  返 回 值：
  说    明：框架大小/是否需要分隔线等的变化会导致CONSOLE_GRAPHICS_INFO结构体中其它成员值的变化，要处理
***************************************************************************/
int gmw_set_frame_style(CONSOLE_GRAPHICS_INFO *const pCGI, const int block_width, const int block_high, const bool separator)
{
	pCGI->CFI.block_width = ((max(block_width, 2) + 1) / 2) * 2;
	pCGI->CFI.block_high = max(block_high, 1);
	pCGI->CFI.separator = separator;
	gmw_inner_update_rowcol(pCGI);
	return 0; //此句可根据需要修改
}

/***************************************************************************
  函数名称：
  功    能：填充 CONSOLE_BORDER_TYPE 结构中的颜色
  输入参数：CONSOLE_GRAPHICS_INFO *const pCGI	：整体结构指针
			const int bg_color					：背景色（缺省 -1表示用窗口背景色）
			const int fg_color					：前景色（缺省 -1表示用窗口前景色）
  返 回 值：
  说    明：不检查颜色值错误及冲突，需要人为保证
				例：颜色非0-15，前景色背景色的值一致导致无法看到内容等
***************************************************************************/
int gmw_set_frame_color(CONSOLE_GRAPHICS_INFO *const pCGI, const int bgcolor, const int fgcolor)
{
	pCGI->CFI.bgcolor = (bgcolor == -1) ? pCGI->area_bgcolor : bgcolor;
	pCGI->CFI.fgcolor = (fgcolor == -1) ? pCGI->area_fgcolor : fgcolor;
	return 0; //此句可根据需要修改
}

/***************************************************************************
  函数名称：
  功    能：填充 CONSOLE_BLOCK_INFO 结构中的6种线型（缺省4种）
  输入参数：CONSOLE_GRAPHICS_INFO *const pCGI	：整体结构指针
			const int type						：1 - 全双线 2 - 全单线 3 - 横双竖单 4 - 横单竖双
  返 回 值：
  说    明：
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
	return 0; //此句可根据需要修改
}

/***************************************************************************
  函数名称：
  功    能：填充 CONSOLE_BLOCK_INFO 结构中的6种线型
  输入参数：CONSOLE_GRAPHICS_INFO *const pCGI	：整体结构指针
		   const char *...					：共6种，具体见.h，此处略
  返 回 值：
  说    明：约定为一个中文制表符，可以使用其它内容，人为保证2字节
			1、超过2字节则只取前2字节
			2、如果给NULL，用两个空格替代
			3、如果给1字节，则补一个空格，如果因此而导致显示乱，不算错
***************************************************************************/
int gmw_set_block_linetype(CONSOLE_GRAPHICS_INFO *const pCGI, const char *top_left, const char *lower_left, const char *top_right, const char *lower_right, const char *h_normal, const char *v_normal)
{
	gmv_inner_set_frame_one_symbol(pCGI->CBI.top_left, top_left);
	gmv_inner_set_frame_one_symbol(pCGI->CBI.lower_left, lower_left);
	gmv_inner_set_frame_one_symbol(pCGI->CBI.top_right, top_right);
	gmv_inner_set_frame_one_symbol(pCGI->CBI.lower_right, lower_right);
	gmv_inner_set_frame_one_symbol(pCGI->CBI.h_normal, h_normal);
	gmv_inner_set_frame_one_symbol(pCGI->CBI.v_normal, v_normal);
	return 0; //此句可根据需要修改
}

/***************************************************************************
  函数名称：
  功    能：设置每个游戏色块(彩球)是否需要小边框
  输入参数：CONSOLE_GRAPHICS_INFO *const pCGI	：整体结构指针
		   const bool on_off					：true - 需要 flase - 不需要（缺省false）
  返 回 值：
  说    明：边框约定为中文制表符，双线
***************************************************************************/
int gmw_set_block_border_switch(CONSOLE_GRAPHICS_INFO *const pCGI, const bool on_off)
{
	pCGI->CBI.block_border = on_off;
	return 0; //此句可根据需要修改
}

/***************************************************************************
  函数名称：
  功    能：设置是否显示上下状态栏
  输入参数：CONSOLE_GRAPHICS_INFO *const pCGI	：整体结构指针
			const int type						：状态栏类型（上/下）
			const bool on_off					：显示/不显示（0/1）
  返 回 值：
  说    明：1、状态栏的相关约定如下：
			1、上状态栏只能一行，在主区域最上方框线/列标的上面，为主区域的最开始一行（主区域的左上角坐标就是上状态栏的坐标）
			2、下状态栏只能一行，在主区域最下方框线的下面
			3、状态栏的宽度为主区域宽度，如果信息过长则截断
		   2、行列的变化会导致CONSOLE_GRAPHICS_INFO结构体中其它成员值的变化，要处理
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
	return 0; //此句可根据需要修改
}

/***************************************************************************
  函数名称：
  功    能：设置上下状态栏的颜色
  输入参数：CONSOLE_GRAPHICS_INFO *const pCGI	：整体结构指针
			const int type						：状态栏类型（上/下）
			const int normal_bgcolor			：正常文本背景色（缺省 -1表示使用窗口背景色）
			const int normal_fgcolor			：正常文本前景色（缺省 -1表示使用窗口前景色）
			const int catchy_bgcolor			：醒目文本背景色（缺省 -1表示使用窗口背景色）
			const int catchy_fgcolor			：醒目文本前景色（缺省 -1表示使用亮黄色）
  输入参数：
  返 回 值：
  说    明：不检查颜色值错误及冲突，需要人为保证
				例：颜色非0-15，前景色背景色的值一致导致无法看到内容等
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
	return 0; //此句可根据需要修改
}

/***************************************************************************
  函数名称：
  功    能：设置是否显示行号
  输入参数：CONSOLE_GRAPHICS_INFO *const pCGI	：整体结构指针
			const bool on_off					：显示/不显示（0/1）
  返 回 值：
  说    明：1、行号约定为字母A开始连续排列（如果超过26，则从a开始，超过52的统一为*，实际应用不可能）
            2、是否显示行号的变化会导致CONSOLE_GRAPHICS_INFO结构体中其它成员值的变化，要处理
***************************************************************************/
int gmw_set_rowno_switch(CONSOLE_GRAPHICS_INFO *const pCGI, const bool on_off)
{
	pCGI->draw_frame_with_row_no = on_off;
	gmw_inner_update_rowcol(pCGI);
	return 0; //此句可根据需要修改
}

/***************************************************************************
  函数名称：
  功    能：设置是否显示列标
  输入参数：CONSOLE_GRAPHICS_INFO *const pCGI	：整体结构指针
			const bool on_off					：显示/不显示（0/1）
  返 回 值：
  说    明：1、列标约定为数字0开始连续排列（数字0-99，超过99统一为**，实际应用不可能）
            2、是否显示列标的变化会导致CONSOLE_GRAPHICS_INFO结构体中其它成员值的变化，要处理
***************************************************************************/
int gmw_set_colno_switch(CONSOLE_GRAPHICS_INFO *const pCGI, const bool on_off)
{
	pCGI->draw_frame_with_col_no = on_off;
	gmw_inner_update_rowcol(pCGI);
	return 0; //此句可根据需要修改
}

/***************************************************************************
  函数名称：
  功    能：打印 CONSOLE_GRAPHICS_INFO 结构体中的各成员值
  输入参数：
  返 回 值：
  说    明：1、仅供调试用，打印格式自定义
            2、本函数测试用例中未调用过，可以不实现
***************************************************************************/
int gmw_print(const CONSOLE_GRAPHICS_INFO *const pCGI)
{
	return 0; //此句可根据需要修改
}

/***************************************************************************
  函数名称：
  功    能：将 CONSOLE_GRAPHICS_INFO 结构体用缺省值进行初始化
  输入参数：CONSOLE_GRAPHICS_INFO *const pCGI：整体结构指针
		   const int row					：游戏区域色块行数（缺省10）
		   const int col					：游戏区域色块列数（缺省10）
		   const int bgcolor				：整个窗口背景色（缺省 COLOR_BLACK）
		   const int fgcolor				：整个窗口背景色（缺省 COLOR_WHITE）
  返 回 值：
  说    明：窗口背景黑/前景白，点阵16*8，上下左右无额外行列，上下状态栏均有，无行号/列标，框架线型为双线，色块宽度2/高度1/无小边框，颜色略
***************************************************************************/
int gmw_init(CONSOLE_GRAPHICS_INFO *const pCGI, const int row, const int col, const int bgcolor, const int fgcolor)
{
	//memset(pCGI, 0, sizeof(_console_graphics_info_));
	pCGI->row_num = row;							//设置框架行列
	pCGI->col_num = col;
	gmw_set_color(pCGI, COLOR_BLACK, COLOR_WHITE, true);	//设置默认窗口颜色
	gmw_set_font(pCGI, "点阵字体", 16, 8);			//设置窗口字体
	pCGI->extern_up_lines = 0;						//设置主框架额外区域
	pCGI->extern_down_lines = 0;
	pCGI->extern_left_cols = 0;
	pCGI->extern_right_cols = 0;
	pCGI->top_status_line = true;					//设置上下状态栏
	pCGI->lower_status_line = true;
	gmw_set_status_line_color(pCGI, TOP_STATUS_LINE, COLOR_BLACK, COLOR_WHITE, COLOR_BLACK, COLOR_HYELLOW);		//设置上下状态栏颜色
	gmw_set_status_line_color(pCGI, LOWER_STATUS_LINE, COLOR_BLACK, COLOR_WHITE, COLOR_BLACK, COLOR_HYELLOW);
	pCGI->draw_frame_with_row_no = false;			//设置主框架行列号
	pCGI->draw_frame_with_col_no = false;
	gmw_set_frame_default_linetype(pCGI, 1);		//设置主框架线型
	gmw_set_block_default_linetype(pCGI, 1);		//设置色块线型
	pCGI->CFI.block_width = 2;						//设置色块长宽
	pCGI->CFI.block_high = 1;
	pCGI->CFI.separator = true;						//设置色块的分隔线
	pCGI->CBI.block_border = false;					//设置色块小边框
	gmw_set_delay(pCGI, DELAY_OF_BLOCK_MOVED, 0);	//设置延迟
	gmw_set_delay(pCGI, DELAY_OF_DRAW_BLOCK, 0);
	gmw_set_delay(pCGI, DELAY_OF_DRAW_FRAME, 0);
	pCGI->SLI.top_start_x = 0;						//设置上下状态栏的位置
	pCGI->SLI.top_start_y = 0;
	pCGI->SLI.lower_start_x = 0;
	pCGI->SLI.lower_start_y = 0;
	gmw_inner_update_rowcol(pCGI);					//更新各种其他数据
	pCGI->other_setting_enable_right_mouse = 0;
	pCGI->other_setting2 = 0;
	return 0; //此句可根据需要修改
}

/***************************************************************************
  函数名称：
  功    能：画主游戏框架
  输入参数：const CONSOLE_GRAPHICS_INFO *const pCGI	：整体结构指针
  返 回 值：
  说    明：具体可参考demo的效果
***************************************************************************/
int gmw_draw_frame(const CONSOLE_GRAPHICS_INFO *const pCGI)
{
	setfontsize(pCGI->CFT.font_type, pCGI->CFT.font_size_high, pCGI->CFT.font_size_width);	//修改窗口字体
	setconsoleborder(pCGI->cols, pCGI->lines);	//修改窗口大小
	const int gap = !!(pCGI->CFI.separator), rowMark = !!(pCGI->draw_frame_with_row_no), colMark = !!(pCGI->draw_frame_with_col_no);
	const int row = pCGI->row_num, col = pCGI->col_num, offsetX = pCGI->start_x, offsetY = pCGI->start_y, bgColor = pCGI->CFI.bgcolor, fgColor = pCGI->CFI.fgcolor, width = pCGI->CFI.block_width, high = pCGI->CFI.block_high;
	char buf[100] = "";
	int x = 0, y = 0;
	for (int i = 0; colMark && i < col; i++)	//输出列标
	{
		gmw_inner_get_col_number(i, buf);		//计算列标字符串，这两行不能写在一起
		showstr(gmw_inner_get_x_in_frame(pCGI, i) + (width - strlen(buf)) / 2, gmw_inner_get_y_in_frame(pCGI, 0) - 2, buf, pCGI->area_bgcolor, pCGI->area_fgcolor);	//输出列标字符串
	}
	showstr(rowMark * 2 + offsetX, gmw_inner_get_y_in_frame(pCGI, 0) - 1, pCGI->CFI.top_left, bgColor, fgColor);	//左上分隔符
	for (int j = 0; j < col; j++)
	{
		for (int k = 0; k < width; k += 2)	//中上连接符
			showstr(gmw_inner_get_x_in_frame(pCGI, j) + k, gmw_inner_get_y_in_frame(pCGI, 0) - 1, pCGI->CFI.h_normal, bgColor, fgColor);
		if (gap)	//中上分隔符
			showstr(gmw_inner_get_x_in_frame(pCGI, j) + width, gmw_inner_get_y_in_frame(pCGI, 0) - 1, pCGI->CFI.h_top_separator, bgColor, fgColor);
		Sleep(pCGI->delay_of_draw_frame);		//延迟
	}
	showstr(gmw_inner_get_x_in_frame(pCGI, col) - 2 * gap, gmw_inner_get_y_in_frame(pCGI, 0) - 1, pCGI->CFI.top_right, bgColor, fgColor);	//左下分隔符
	for (int i = 0; i < row; i++)	//循环每行
	{
		y = gmw_inner_get_y_in_frame(pCGI, i);
		if (rowMark)	//行标
			showch(offsetX, (high + gap) * i + colMark + offsetY + high / 2 + !!(pCGI->top_status_line), gmw_inner_get_row_number(i), pCGI->area_bgcolor, pCGI->area_fgcolor); //输出行标
		for (int m = 0; m < high; m++)	//循环每个色块的高度
		{
			showstr(rowMark * 2 + offsetX, y + m, pCGI->CFI.v_normal, bgColor, fgColor);	//输出左中连接符
			for (int j = 0; j < col; j++)	//循环每列
			{
				showch(gmw_inner_get_x_in_frame(pCGI, j), y + m, ' ', bgColor, fgColor, width);	//输出空格
				if (gap)
				{
					showstr(gmw_inner_get_x_in_frame(pCGI, j) + width, y + m, pCGI->CFI.v_normal, bgColor, fgColor);	//输出中间竖向连接符
					Sleep(pCGI->delay_of_draw_frame);		//延迟
				}
			}
			showstr(gmw_inner_get_x_in_frame(pCGI, col) - 2 * gap, y + m, pCGI->CFI.v_normal, bgColor, fgColor);	//输出右中连接符
		}
		if (gap)
			showstr(rowMark * 2 + offsetX, y + high, pCGI->CFI.v_left_separator, bgColor, fgColor);	//输出左中连接符
		for (int j = 0; gap && j < col && i < row - 1; j++)
		{
			for (int k = 0; k < width; k += 2)
			{
				showstr(gmw_inner_get_x_in_frame(pCGI, j) + k, y + high, pCGI->CFI.h_normal, bgColor, fgColor);	//输出中间横向连接符
				Sleep(pCGI->delay_of_draw_frame);		//延迟
			}
			showstr(gmw_inner_get_x_in_frame(pCGI, j) + width, y + high, pCGI->CFI.mid_separator, bgColor, fgColor);	//输出中间连接符
		}
		if (gap)
			showstr(gmw_inner_get_x_in_frame(pCGI, col) - 2 * gap, y + high, pCGI->CFI.v_right_separator, bgColor, fgColor);	//输出右中连接符
		Sleep(pCGI->delay_of_draw_frame);		//延迟
	}
	showstr(rowMark * 2 + offsetX, gmw_inner_get_y_in_frame(pCGI, row) - !!(pCGI->CFI.separator), pCGI->CFI.lower_left, bgColor, fgColor);	//输出左下分隔符
	for (int j = 0; j < col; j++)
	{
		for (int k = 0; k < width; k += 2)
			showstr(gmw_inner_get_x_in_frame(pCGI, j) + k, gmw_inner_get_y_in_frame(pCGI, row) - !!(pCGI->CFI.separator), pCGI->CFI.h_normal, bgColor, fgColor);	//输出中下连接符
		if (gap)
			showstr(gmw_inner_get_x_in_frame(pCGI, j) + width, gmw_inner_get_y_in_frame(pCGI, row) - !!(pCGI->CFI.separator), pCGI->CFI.h_lower_separator, bgColor, fgColor);	//输出中下分隔符
		Sleep(pCGI->delay_of_draw_frame);		//延迟
	}
	showstr(gmw_inner_get_x_in_frame(pCGI, col) - 2 * gap, gmw_inner_get_y_in_frame(pCGI, row) - !!(pCGI->CFI.separator), pCGI->CFI.lower_right, bgColor, fgColor);	//输出右下分隔符
	return 0; //此句可根据需要修改
}

/***************************************************************************
  函数名称：
  功    能：在状态栏上显示信息
  输入参数：const CONSOLE_GRAPHICS_INFO *const pCGI	：整体结构指针
		   const int type							：指定是上/下状态栏
		   const char *msg						：正常信息
		   const char *catchy_msg					：需要特别标注的信息（在正常信息前显示）
  返 回 值：
  说    明：1、输出宽度限定为主框架的宽度（含行号列标位置），超出则截去
            2、如果最后一个字符是某汉字的前半个，会导致后面乱码，要处理
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
	return 0; //此句可根据需要修改
}

/***************************************************************************
  函数名称：
  功    能：显示某一个色块(内容为字符串，坐标为row/col)
  输入参数：const CONSOLE_GRAPHICS_INFO *const pCGI	：整体结构指针
		   const int row_no						：行号（从0开始，人为保证正确性，程序不检查）
		   const int col_no						：列号（从0开始，人为保证正确性，程序不检查）
		   const int bdi_value						：需要显示的值
		   const BLOCK_DISPLAY_INFO *const bdi		：存放该值对应的显示信息的结构体数组
  返 回 值：
  说    明：1、BLOCK_DISPLAY_INFO 的含义见头文件，用法参考测试样例
            2、bdi_value为 BDI_VALUE_BLANK 表示空白块，要特殊处理
***************************************************************************/
int gmw_draw_block(const CONSOLE_GRAPHICS_INFO *const pCGI, const int row_no, const int col_no, const int bdi_value, const BLOCK_DISPLAY_INFO *const bdi)
{
	int index = -1;
	for (int i = 0; bdi[i].value != BDI_VALUE_END; i++)	//查找bdi_value对应的色块信息
		if (bdi[i].value == bdi_value)
			index = i;
	if (index == -1)	//没有找到对应的色块信息
		return -1;
	const int gap = !!(pCGI->CFI.separator), rowMark = !!(pCGI->draw_frame_with_row_no), colMark = !!(pCGI->draw_frame_with_row_no);
	const int row = pCGI->row_num, col = pCGI->col_num, offsetX = pCGI->start_x, offsetY = pCGI->start_y, bgColor = (bdi[index].bgcolor != -1 ? bdi[index].bgcolor : pCGI->CFI.bgcolor), fgColor = (bdi[index].fgcolor != -1 ? bdi[index].fgcolor : pCGI->CFI.fgcolor), width = pCGI->CFI.block_width, high = pCGI->CFI.block_high;
	const int x = gmw_inner_get_x_in_frame(pCGI, col_no), y = gmw_inner_get_y_in_frame(pCGI, row_no);
	gmw_inner_draw_block(pCGI, x, y, width, high, index, bgColor, fgColor, pCGI->delay_of_draw_block, bdi);
	setcolor(pCGI->area_bgcolor, pCGI->area_fgcolor);
	return 0; //此句可根据需要修改
}

/***************************************************************************
  函数名称：
  功    能：移动某一个色块
  输入参数：const CONSOLE_GRAPHICS_INFO *const pCGI	：整体结构指针
		   const int row_no						：行号（从0开始，人为保证正确性，程序不检查）
		   const int col_no						：列号（从0开始，人为保证正确性，程序不检查）
		   const int bdi_value						：需要显示的值
		   const int blank_bdi_value				：移动过程中用于动画效果显示时用于表示空白的值（一般为0，此处做为参数代入，是考虑到可能出现的特殊情况）
		   const BLOCK_DISPLAY_INFO *const bdi		：存放显示值/空白值对应的显示信息的结构体数组
		   const int direction						：移动方向，一共四种，具体见cmd_gmw_tools.h
		   const int distance						：移动距离（从1开始，人为保证正确性，程序不检查）
  返 回 值：
  说    明：
***************************************************************************/
int gmw_move_block(const CONSOLE_GRAPHICS_INFO *const pCGI, const int row_no, const int col_no, const int bdi_value, const int blank_bdi_value, const BLOCK_DISPLAY_INFO *const bdi, const int direction, const int distance)
{
	if (distance <= 0 || pCGI == NULL)
		return -1;
	int index = -1;
	for (int i = 0; bdi[i].value != BDI_VALUE_END; i++)	//查找bdi_value对应的色块信息
		if (bdi[i].value == bdi_value)
			index = i;
	if (index == -1)	//没有找到对应的色块信息
		return -1;
	const int bgColor = (bdi[index].bgcolor != -1) ? bdi[index].bgcolor : pCGI->CFI.bgcolor;
	const int fgColor = (bdi[index].fgcolor != -1) ? bdi[index].fgcolor : pCGI->CFI.fgcolor;
	const int blankBgColor = (bdi[blank_bdi_value].bgcolor != -1) ? bdi[blank_bdi_value].bgcolor : pCGI->CFI.bgcolor;
	const int blankFgColor = (bdi[blank_bdi_value].fgcolor != -1) ? bdi[blank_bdi_value].fgcolor : pCGI->CFI.fgcolor;
	const int width = pCGI->CFI.block_width, high = pCGI->CFI.block_high;
	const int x = gmw_inner_get_x_in_frame(pCGI, col_no), y = gmw_inner_get_y_in_frame(pCGI, row_no);
	const int symbol = (direction == UP_TO_DOWN || direction == LEFT_TO_RIGHT) ? 1 : -1;	//方向
	CONSOLE_GRAPHICS_INFO *tCGI = new CONSOLE_GRAPHICS_INFO;	//调整delay_of_draw_block的值，加快显示速度
	memcpy(tCGI, pCGI, sizeof(CONSOLE_GRAPHICS_INFO));
	tCGI->delay_of_draw_block = 0;
	if (direction == UP_TO_DOWN || direction == DOWN_TO_UP)						//纵向移动
	{
		for (int i = 0; i < distance * (high + !!(tCGI->CFI.separator)); i++)	//移动distance个格子
		{
			if (tCGI->CBI.block_border)				//带有边框的情况
			{
				if (direction == UP_TO_DOWN)		//下移
				{
					if (tCGI->CFI.separator && i % (high + 1) == 0)				//输出分隔符
					{
						showstr(x, y + (i - 1) * symbol, tCGI->CFI.h_normal, tCGI->CFI.bgcolor, tCGI->CFI.fgcolor, width / 2);
						showstr(x, y + (i - 0) * symbol, " ", blankBgColor, blankFgColor, width);
					}
					else if (!(i == 0 && !tCGI->CFI.separator))												//输出空格，擦除原来的图案，注意循环第一次时不能擦除
						showstr(x, y + (i - 0) * symbol, " ", blankBgColor, blankFgColor, width);
				}
				else								//上移
				{
					if (tCGI->CFI.separator && i % (high + 1) == 0)				//输出分隔符
					{
						showstr(x, y + (i - high) * symbol, tCGI->CFI.h_normal, tCGI->CFI.bgcolor, tCGI->CFI.fgcolor, width / 2);
						showstr(x, y + (i - high + 1) * symbol, " ", blankBgColor, blankFgColor, width);
					}
					else if(!(i == 0 && !tCGI->CFI.separator))												//输出空格，擦除原来的图案，注意循环第一次时不能擦除
						showstr(x, y + (i - high + 1) * symbol, " ", blankBgColor, blankFgColor, width);
				}
			}
			else									//不带边框的情况
			{
				if (tCGI->CFI.separator && i % (high + 1) == high - high / 2)	//输出分隔符
					showstr(x, y + (i)* symbol, tCGI->CFI.h_normal, tCGI->CFI.bgcolor, tCGI->CFI.fgcolor, width / 2);
				else															//输出空格，擦除原来的图案
					showstr(x, y + i * symbol + high / 2, " ", blankBgColor, blankFgColor, width);
			}
			gmw_inner_draw_block(tCGI, x, y + (i + 1) * symbol, width, high, index, bgColor, fgColor, tCGI->delay_of_draw_block, bdi);	//绘制色块
			Sleep(tCGI->delay_of_block_moved);						//延迟
		}
		showstr(x, y + (distance * (high + !!(tCGI->CFI.separator)) - ((direction == UP_TO_DOWN) ? 1 : high)) * symbol, (tCGI->CFI.separator ? tCGI->CFI.h_normal : "  "), tCGI->CFI.bgcolor, tCGI->CFI.fgcolor, width / 2);	//消除最后多余的图案，输出空格或分隔符
	}
	if (direction == LEFT_TO_RIGHT || direction == RIGHT_TO_LEFT)				//横向移动
	{
		for (int i = 0; i < distance * (width + 2 * !!(tCGI->CFI.separator)); i++)
		{
			if (tCGI->CBI.block_border)				//带有边框的情况
			{
				if (direction == LEFT_TO_RIGHT)		//右移
				{
					if (tCGI->CFI.separator && i % (width + 2) == 0)			//输出分隔符
						for (int k = 0; k < high; k++)
						{
							showstr(x + (i - 2) * symbol, y + k, tCGI->CFI.v_normal, tCGI->CFI.bgcolor, tCGI->CFI.fgcolor);
							showstr(x + (i - 0) * symbol, y + k, " ", blankBgColor, blankFgColor);
						}
					else if(!(i == 0 && !tCGI->CFI.separator))														//输出空格，擦除原来的图案，注意循环第一次时不能擦除
						for (int k = 0; k < high; k++)
							showstr(x + (i - 0) * symbol, y + k, " ", blankBgColor, blankFgColor);
				}
				else								//左移
				{
					if (tCGI->CFI.separator && i % (width + 2) == 0)			//输出分隔符
						for (int k = 0; k < high; k++)
						{
							showstr(x + (i - width + 0) * symbol, y + k, tCGI->CFI.v_normal, tCGI->CFI.bgcolor, tCGI->CFI.fgcolor);
							showstr(x + (i - width + 1) * symbol, y + k, " ", blankBgColor, blankFgColor);
						}
					else if (!(i == 0 && !tCGI->CFI.separator))														//输出空格，擦除原来的图案，注意循环第一次时不能擦除
						for (int k = 0; k < high; k++)
							showstr(x + (i - width + 1) * symbol, y + k, " ", blankBgColor, blankFgColor);
				}
			}
			else									//不带边框的情况
			{
				if (direction == LEFT_TO_RIGHT)
				{
					if (tCGI->CFI.separator && i % (width + 2) == width + 1)	//输出分隔符
						for (int k = 0; k < high; k++)
							showstr(x + (i - 1) * symbol, y + k, tCGI->CFI.v_normal, tCGI->CFI.bgcolor, tCGI->CFI.fgcolor);
					else														//输出空格，擦除原来的图案
						showstr(x + i * symbol, y + high / 2, " ", blankBgColor, blankFgColor);
				}
				else
				{
					if (tCGI->CFI.separator && i % (width + 2) == width + 1)	//输出分隔符
						for (int k = 0; k < high; k++)
							showstr(x + (i - 1) * symbol + width - 2, y + k, tCGI->CFI.v_normal, tCGI->CFI.bgcolor, tCGI->CFI.fgcolor);
					else														//输出空格，擦除原来的图案
						showstr(x + i * symbol + width - 1, y + high / 2, " ", blankBgColor, blankFgColor);
				}
			}
			gmw_inner_draw_block(tCGI, x + (i + 1) * symbol, y, width, high, index, bgColor, fgColor, tCGI->delay_of_draw_block, bdi);
			Sleep(tCGI->delay_of_block_moved / 2);						//延迟
		}
		for (int k = 0; k < high; k++)
			showstr(x + (distance * (width + 2 * !!(tCGI->CFI.separator)) - (direction == LEFT_TO_RIGHT ? 2 : width)) * symbol, y + k, (tCGI->CFI.separator ? tCGI->CFI.v_normal : "  "), tCGI->CFI.bgcolor, tCGI->CFI.fgcolor);	//消除最后多余的图案，输出空格或分隔符
	}
	delete tCGI;
	return 0; //此句可根据需要修改
}

/***************************************************************************
  函数名称：
  功    能：读键盘或鼠标
  输入参数：const CONSOLE_GRAPHICS_INFO *const pCGI	：整体结构指针
		   int &MAction							：如果返回 CCT_MOUSE_EVENT，则此值有效，为 MOUSE_ONLY_MOVED/MOUSE_LEFT_BUTTON_CLICK/MOUSE_RIGHT_BUTTON_CLICK 三者之一
													   如果返回 CCT_KEYBOARD_EVENT，则此值无效
		   int &MRow								：如果返回 CCT_MOUSE_EVENT 且 MAction = MOUSE_ONLY_MOVED/MOUSE_LEFT_BUTTON_CLICK，则此值有效，表示左键选择的游戏区域的行号（从0开始）
												  其余情况此值无效（如果访问无效值导致错误，不是本函数的错!!!）
		   int &MCol								：如果返回 CCT_MOUSE_EVENT 且 MAction = MOUSE_ONLY_MOVED/MOUSE_LEFT_BUTTON_CLICK，则此值有效，表示左键选择的游戏区域的列号（从0开始）
												  其余情况此值无效（如果访问无效值导致错误，不是本函数的错!!!）
		   int &KeyCode1							：如果返回 CCT_KEYBOARD_EVENT，则此值有效，为读到的键码（如果双键码，则为第一个）
												  其余情况此值无效（如果访问无效值导致错误，不是本函数的错!!!）
		   int &KeyCode2							：如果返回 CCT_KEYBOARD_EVENT，则此值有效，为读到的键码（如果双键码，则为第二个，如果是单键码，则为0）
												  其余情况此值无效（如果访问无效值导致错误，不是本函数的错!!!）
		   const bool update_lower_status_line		：鼠标移动时，是否要在本函数中显示"[当前光标] *行*列/位置非法"的信息（true=显示，false=不显示，缺省为true）
  返 回 值：函数返回约定
		   1、如果是鼠标移动，得到的MRow/MCol与传入的相同(鼠标指针微小的移动)，则不返回，继续读
							  得到行列非法位置，则不返回，根据 update_lower_status_line 的设置在下状态栏显示"[当前光标] 位置非法"
							  得到的MRow/MCol与传入的不同(行列至少一个变化)，根据 update_lower_status_line 的设置在下状态栏显示"[当前光标] *行*列"，再返回MOUSE_ONLY_MOVED（有些游戏返回后要处理色块的不同颜色显示）
		   2、如果是按下鼠标左键，且当前鼠标指针停留在主游戏区域的*行*列上，则返回 CCT_MOUSE_EVENT ，MAction 为 MOUSE_LEFT_BUTTON_CLICK, MRow 为行号，MCol 为列标
								  且当前鼠标指针停留在非法区域（非游戏区域，游戏区域中的分隔线），则不返回，根据 update_lower_status_line 的设置在下状态栏显示"[当前光标] 位置非法"
		   3、如果是按下鼠标右键，则不判断鼠标指针停留区域是否合法，直接返回 CCT_MOUSE_EVENT ，MAction 为 MOUSE_LEFT_BUTTON_CLICK, MRow、MCol不可信
		   4、如果按下键盘上的某键（含双键码按键），则直接返回 CCT_KEYBOARD_EVENT，KeyCode1/KeyCode2中为对应的键码值
 说    明：通过调用 cmd_console_tools.cpp 中的 read_keyboard_and_mouse 函数实现
***************************************************************************/
int gmw_read_keyboard_and_mouse(const CONSOLE_GRAPHICS_INFO *const pCGI, int &MAction, int &MRow, int &MCol, int &KeyCode1, int &KeyCode2, const bool update_lower_status_line)
{
	static const HANDLE __hin = GetStdHandle(STD_INPUT_HANDLE);		//取标准输入设备对应的句柄
	static int MX_old = -1, MY_old = -1, MAction_old = MOUSE_ONLY_MOVED, MX, MY, MRow_old = -1, MCol_old = -1;
	INPUT_RECORD InputRec;
	DWORD        res;
	COORD        crPos;
	enable_mouse();
	while (1) 
	{
		/* 从hin中读输入状态（包括鼠标、键盘等） */
		ReadConsoleInput(__hin, &InputRec, 1, &res);

		/* 键盘事件（要优于鼠标事件，否则如果鼠标放在目标区，无法读键） */
		if (InputRec.EventType == KEY_EVENT) {
			KeyCode1 = 0x00;
			KeyCode2 = 0x00;
			if (InputRec.Event.KeyEvent.bKeyDown)	//只在按下时判断，弹起时不判断
			{
				switch (InputRec.Event.KeyEvent.wVirtualKeyCode) {
					case VK_UP:
						KeyCode1 = 0xe0;
						KeyCode2 = KB_ARROW_UP;	//模拟 _getch()方式返回的两个键码，分别是224（0xE0）和72（0x48）
						break;
					case VK_DOWN:
						KeyCode1 = 0xe0;
						KeyCode2 = KB_ARROW_DOWN;	//模拟 _getch()方式返回的两个键码，分别是224（0xE0）和80（0x50）
						break;
					case VK_LEFT:
						KeyCode1 = 0xe0;
						KeyCode2 = KB_ARROW_LEFT;	//模拟 _getch()方式返回的两个键码，分别是224（0xE0）和75（0x4B）
						break;
					case VK_RIGHT:
						KeyCode1 = 0xe0;
						KeyCode2 = KB_ARROW_RIGHT;	//模拟 _getch()方式返回的两个键码，分别是224（0xE0）和77（0x4D）
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

				  /* 非箭头键直接返回ASCII形式（Fn、Insert、Delete等均未处理） */
				if (KeyCode1 == 0)
					KeyCode1 = InputRec.Event.KeyEvent.uChar.AsciiChar;

				return CCT_KEYBOARD_EVENT;
			} //end of if (KEYDOWN)
		} // end of if (键盘事件)

		/* 鼠标事件 */
		if (InputRec.EventType == MOUSE_EVENT)	/* 从返回中读鼠标指针当前的坐标 */
		{
			crPos = InputRec.Event.MouseEvent.dwMousePosition;
			MX = crPos.X;
			MY = crPos.Y;

			if (InputRec.Event.MouseEvent.dwEventFlags == MOUSE_MOVED)	//鼠标移动
			{
				/* 如果始终是MOUSE_MOVED事件且坐标不变，则不认为是MOUSE_MOVED */
				if (MX_old == MX && MY_old == MY && MAction_old == MOUSE_ONLY_MOVED)
					continue;
				if (pCGI && gmw_inner_check_rowcol_in_frame(pCGI, MX, MY, MRow, MCol) == true)	//如果鼠标落在框架的某个色块内
				{
					if (MRow_old != MRow || MCol_old != MCol)	//如果鼠标所指的色块有所变化
					{
						gmw_inner_update_lower_statue_line(pCGI, MRow, MCol);	//更新下状态栏信息
						MAction = MOUSE_ONLY_MOVED;
						disable_mouse();
						return CCT_MOUSE_EVENT;
					}
					MRow_old = MRow;	//记录旧色块
					MCol_old = MCol;
				}
				MX_old = MX;	//记录旧位置
				MY_old = MY;
				MAction = MOUSE_ONLY_MOVED;	//修改Maction
				MAction_old = MAction;		//记录旧Maction
			}
			MAction_old = MOUSE_NO_ACTION; //置非MOUSE_ONLY_MOVED值即可

			if (InputRec.Event.MouseEvent.dwButtonState == FROM_LEFT_1ST_BUTTON_PRESSED)		//按下左键
			{
				if (pCGI && gmw_inner_check_rowcol_in_frame(pCGI, MX, MY, MRow, MCol) == false)	//如果鼠标没有落在框架的某个色块内
				{
					gmw_status_line(pCGI, LOWER_STATUS_LINE, "[当前光标] 位置非法");	//更新下状态栏信息
				}
				else
				{
					MAction = MOUSE_LEFT_BUTTON_CLICK;	//修改Maction，退出函数
					disable_mouse();
					return CCT_MOUSE_EVENT;
				}
			}
			else if (InputRec.Event.MouseEvent.dwButtonState == RIGHTMOST_BUTTON_PRESSED)		//按下右键
			{
				if (pCGI && pCGI->other_setting_enable_right_mouse == 1 && gmw_inner_check_rowcol_in_frame(pCGI, MX, MY, MRow, MCol) == false)	//如果鼠标没有落在框架的某个色块内
				{
					gmw_status_line(pCGI, LOWER_STATUS_LINE, "[当前光标] 位置非法");	//更新下状态栏信息
				}
				else
				{
					MAction = MOUSE_RIGHT_BUTTON_CLICK;	//修改Maction，退出函数
					disable_mouse();
					return CCT_MOUSE_EVENT;
				}
			}
			else //忽略其它按键操作
				continue;
		} // end of if(鼠标事件)
	} //end of while(1)

	return CCT_MOUSE_EVENT;
}

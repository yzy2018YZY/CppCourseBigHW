//1850059 计1班 杨志远

#pragma once
#define _CRT_SECURE_NO_WARNINGS
#include "../common/cmd_console_base.h"
#include "../common/cmd_console_graphics.h"
#include "../common/cmd_hdc_tools.h"
#include <iomanip>
#include <fstream>
#include <string.h>
#include <io.h>
using namespace std;

//修改以下关于大小和位移的参数可能会导致不可预料的后果
const int TableToBlank = 4;					//定义一个tab会被转化为多少个空格（建议不超过8）
const int WindowXSize1 = 70;				//初始窗口横向大小（默认70）
const int WindowYSize1 = 30;				//初始窗口纵向大小（默认30）
const int WindowXSize2 = 145;				//阅读窗口横向大小（默认145）
const int WindowYSize2 = 35;				//阅读窗口纵向大小（默认35）
const int FontSize1 = 16;					//初始界面字体大小
const char Font1[] = "新宋体";				//初始界面字体
const int FontSize2 = 16;					//阅读界面字体大小
const char Font2[] = "新宋体";				//阅读界面字体
const int ChooseXOffset = 20;				//初始界面横向位移（默认20）
const int ChooseYOffset = 3;				//初始界面纵向位移（默认3）
const int TextBoxXSize = 85;				//阅读界面宽度（必须大于2）
const int TextBoxYSize = 25;				//阅读界面高度（必须大于0）
const int TextBoxXOffset = 25;				//阅读界面横向位移（默认25）
const int TextBoxYOffset = 1;				//阅读界面纵向位移（默认1）
const int TextBoxDelay1 = 25;				//阅读界面上下行延迟
const int TextBoxDelay2 = 25;				//阅读界面翻页延迟
const int TextBoxDelay3 = 200;				//阅读界面其他按键延迟
const int MaxBookListLength = 512;			//默认书本列表最大长度（即文件夹下书本的最大数目，必须大于等于MenuBookListLength）
const int MenuBookListLength = 15;			//菜单中的书本列表的纵向长度
const int MenuBookListStringLength = 13;	//菜单中的书本列表的横向长度
const long MaxBookSize = 20 * 1024 * 1024;	//书本最大大小
#if defined MAX_PATH	//Windows自带的宏，文件路径字符串的最大长度
const int DefaultStringLength = MAX_PATH + 10;//默认字符串最大长度（不小于windows最大路径长度）
#else
const int DefaultStringLength = 270;		//默认字符串最大长度（不小于windows最大路径长度）
#endif
const char ProjectName[] = "90-b3";								//当前项目名
const char BookFolderName[] = "book";							//子文件夹名称
const char SavingFileName[] = "position.saving";				//存档文件名称
const char ConsoleTitle[] = "文本阅读器 1850059 计1班 杨志远";	//标题内容
const char HelpInfo[] = "方向↑↓键移动一行，PgUp与PgDn翻页，S键跳转，F键切换显示模式，N键返回菜单，Q键退出";	//阅读界面下方的帮助信息
const char HelpInfoChooseMode[] = "方向键切换光标 回车选择文件";	//初始界面帮助信息

const char CorrectExtensiveName[][5] //允许的文件扩展名，必须以空字符串结尾
{
	"txt",
	//"pdf",
	"",
};

enum PrintMode //文本显示模式，不需要改动
{
	PrintModeNormal,
	PrintModeAutoEnter,
};

//90-b3-tools
int get_book_file_path(char bookPath[], const char bookName[]);
long get_file_size(const char filePath[]);
long read_index_of_book_file(const char bookFilePath[], const char bookName[], int bookSize);
bool save_index_of_book_file(const char bookFilePath[], const char bookName[], int index);

//90-b3-base
int get_book_list(char bookList[][DefaultStringLength]);
int choose_book(const char bookList[][DefaultStringLength], const int bookListLength, const int lines, const int length, const int startLine, int offsetX, const int offsetY);
int read_book_file(char bookStr[], const char bookFilePath[], const long fileSize);
int read_book(char bookStr[], const int bookSize, long &startIndex, int length, const int lines, const int offsetX, const int offsetY, const int mode);

//90-b3-graphics
void draw_graphics_init(int windowX, int windowY, int fontSize, const char font[]);
void draw_graphics();
void draw_graghics_up();
void draw_graghics_down();
void draw_graphics_F();
void draw_graphics_S();
void draw_graphics_PgUp();
void draw_graphics_PgDn();
void draw_rotate_ellipse(bool clockwise);

//1850059 计1班 杨志远

#include "90-b3.h"

static void highlight_one_line(const char preStr[], const char str[], const int strShowedLength, const int preCursorX, const int preCursorY, const int cursorX, const int cursorY);
static int print_book_list(const char bookList[][DefaultStringLength], const int bookListLength, const int lines, const int length, const int startLine, int offsetX, const int offsetY);
static int get_strLength_of_one_line(const char bookStr[], const int startIndex, int length);
static int convert_bookStr_to_autoenter(char buf[], char bookStr[]);
static char* convert_bookStr_to_normal(char bookStr[]);
static void clear_text(const int lines, const int length, const int offsetX, const int offsetY);
static int print_book_text(char bookStr[], const long strLengthOfLine[], int startLine, int length, int lines, int offsetX, int offsetY, int maxLine);
static void print_help_info(int x, int y);
static int jump_book_line(char bookStr[], char convertedBookStr[], const long strLengthOfLineNormal[], const long strLengthOfLineAutoEnter[], long &line, int length, int lines, int offsetX, int offsetY, int maxLineNormal, int maxLineAutoEnter, int mode);
static long get_str_length_each_line(const char bookStr[], const int bookSize, long strLengthOfLine[], const int length);
static void print_book_text_choose_mode(char bookStr[], char convertedBookStr[], const long strLengthOfLineNormal[], const long strLengthOfLineAutoEnter[], long line, int length, int lines, int offsetX, int offsetY, int maxLineNormal, int maxLineAutoEnter, int mode);
static long get_line_of_index_from_array(const long strLengthOfLineNormal[], const long strLengthOfLineAutoEnter[], const long maxLineNormal, const long maxLineAutoEnter, const long index, const int mode);
static void change_print_mode(long &line, int lines, int maxLineNormal, int maxLineAutoEnter, int &mode);
static int main_cycle_of_read_book(char bookStr[], char convertedBookStr[], const long strLengthOfLineNormal[], const long strLengthOfLineAutoEnter[], long &line, int length, int lines, int offsetX, int offsetY, int maxLineNormal, int maxLineAutoEnter, int mode);
static int main_cycle_of_choose_book(const char bookList[][DefaultStringLength], const char convertedBookList[][DefaultStringLength], int bookListLength, int lines, int length, int startLine, int offsetX, int offsetY);

//函数print_book_list的一部分，反色输出一行信息，正常输出另一行信息
static void highlight_one_line(const char preStr[], const char str[], const int strShowedLength, const int preCursorX, const int preCursorY, const int cursorX, const int cursorY)
{
	showstr(preCursorX, preCursorY, preStr, COLOR_BLACK, COLOR_WHITE, 1, strShowedLength * 2);//正常输出之前行的信息
	showstr(cursorX, cursorY, str, COLOR_WHITE, COLOR_BLACK, 1, strShowedLength * 2);//反色输出当前行信息
	gotoxy(cursorX + my_min((int)strlen(str) - 2, strShowedLength * 2 - 2) + 2, cursorY);//跳转到当前行末尾
}

//输出文本菜单
static int print_book_list(const char bookList[][DefaultStringLength], const int bookListLength, const int lines, const int length, const int startLine, int offsetX, const int offsetY)
{
	int cursorX, cursorY;
	getxy(cursorX, cursorY); //记录光标初始位置
	//showstr(offsetX, offsetY, helpInfo);//输出帮助信息
	//draw_framework(lines, length, false, true, false, COLOR_WHITE, COLOR_BLACK, offsetX - 2, offsetY); //输出外框架
	for (int i = startLine; i < startLine + lines && i < bookListLength; i++) //输出每行书本名称
	{
		showstr(offsetX, offsetY + i - startLine + 2, bookList[i], 0, 7, 1, length * 2);
	}
	gotoxy(cursorX, cursorY); //光标回到初始位置
	return 0;
}

//获取从startIndex开始的某行的长度.
static int get_strLength_of_one_line(const char bookStr[], const int startIndex, int length)
{
	length -= 1;//防止右边显示出界
	int cursorX = 0;
	for (int i = startIndex; bookStr[i] != '\0'; i++)
	{
		if (bookStr[i] != '\n')
		{
			if (is_Chinese_character(bookStr[i]))//遇到中文的第一个字符就跳过下一个字符
			{
				i++;
				cursorX++;
			}
			cursorX++;
		}
		if (bookStr[i] == '\n')//主动换行
		{
			cursorX++;
			break;
		}
		if (cursorX >= length)//被动换行
		{
			if (cursorX > length)
				cursorX -= 2;
			break;
		}
	}
	return cursorX;
}

//将bookStr转换为AutoEnter模式下的字符串
static int convert_bookStr_to_autoenter(char buf[], char bookStr[])
{
	char *b = buf, *p = bookStr;
	for (; *p != '\0'; p++)
	{
		if (*p != '\n' && *p != '\t') //普通字符
		{
			*b++ = *p;
		}
		if (*p == '\t') //tab
		{
			for(int i = 0; i < TableToBlank; i++)
				*b++ = ' ';
		}
		if ((*p == '\n' && (*(p + 1) == ' ' || *(p + 1) == '\t' || *(p + 1) == '\n' || (*(p + 1) == (char)0xA1 && *(p + 2) == (char)0xA1)))) //符合条件的回车字符
		{
			*b++ = '\n';
		}
	}
	*b = '\0'; //置尾零
	return 0;
}

//返回新的bookStr字符串，务必重新赋值！调整bookStr，添加空格，满足Update的要求
static char* convert_bookStr_to_normal(char bookStr[])
{
	char *bookFileStr = new char[int(strlen(bookStr) * (TableToBlank + 1))]; //重新建立一个存储整个文件内容的超大字符串
	char *p = bookStr, *b = bookFileStr;
	for (; *p != '\0'; p++)
	{
		if (*p == '\n' && !(*(p + 1) == ' ' || *(p + 1) == '\t' || *(p + 1) == '\n' || (*(p + 1) == (char)0xA1 && *(p + 2) == (char)0xA1))) //回车且下一个不为空格
		{
			*b++ = '\n';
			for (int i = 0; i < 4; i++)	//插入4个空格
				*b++ = ' ';
		}
		else if (*p == '\t') //替换tab
		{
			for (int i = 0; i < TableToBlank; i++)	//插入多个空格
				*b++ = ' ';
		}
		else //其他情况照常写入新数组
			*b++ = *p;
	}
	*b = '\0'; //补尾零
	delete[]bookStr; //释放之前的字符串
	return bookFileStr; //返回新字符串
}


//清除屏幕中的文字
static void clear_text(const int lines, const int length, const int offsetX, const int offsetY)
{	
	char temp[WindowXSize2] = "";
	for (int i = 0; i <= length - 2; i++)//创建足够长的空格组成的字符串
		temp[i] = ' ';
	for (long line = 0; line < lines; line++)//每行输出空行
	{
		gotoxy(offsetX + 2, offsetY + line + 2);
		cout << temp << endl;
	}
}

//输出从startLine开始的书本文本
static int print_book_text(char bookStr[], const long strLengthOfLine[], int startLine, int length, int lines, int offsetX, int offsetY, int maxLine)
{
	clear_text(lines, length, offsetX, offsetY); //清除文字
	for (long line = 0; line < lines && line < maxLine; line++) //输出每行的文字
	{
		gotoxy(offsetX + 2, offsetY + 2 + line);
		char tempCh = bookStr[strLengthOfLine[startLine + line + 1]]; //记录下一行第一个字符
		bookStr[strLengthOfLine[startLine + line + 1]] = '\0'; //添加尾零，方便输出
		cout << &bookStr[strLengthOfLine[startLine + line]];
		bookStr[strLengthOfLine[startLine + line + 1]] = tempCh; //还原
	}
	gotoxy(offsetX + 2, offsetY + 2);
	return 0;
}

//输出帮助信息
static void print_help_info(int x, int y)
{
	gotoxy(x, y);
	cout << HelpInfo;
}

//S键跳转功能的实现，以行为单位而非字节，返回跳转后的line
static int jump_book_line(char bookStr[], char convertedBookStr[], const long strLengthOfLineNormal[], const long strLengthOfLineAutoEnter[], long &line, int length, int lines, int offsetX, int offsetY, int maxLineNormal, int maxLineAutoEnter, int mode)
{
	const int maxStrLength = 6, innerOffsetY = 6;
	const char info[] = "要跳转到（0-100的浮点数，小数点后最多两位）：";
	char input[20] = "", tempCh = 0;
	double target;
	gotoxy(0, lines + offsetY + innerOffsetY); //输出帮助信息
	cout << info << "                   ";
	gotoxy((int)strlen(info), lines + offsetY + innerOffsetY);
	for (int i = 0; i < maxStrLength; i++) //输入数字
	{
		tempCh = _getch();
		if (tempCh == '\r')
			break;
		putchar(tempCh);
		input[i] = tempCh;
	}
	target = atof(input); //转换为浮点数
	if (target >= 0 && target <= 100)
	{
		if (mode == PrintModeNormal)
		{
			line = (long)(target / 100 * maxLineNormal); //计算line的大小
			line = min(line, maxLineNormal - lines + 1); //调整line范围
			line = max(line, 0);
		}
		if (mode == PrintModeAutoEnter)
		{
			line = (long)(target / 100 * maxLineAutoEnter); //计算line的大小
			line = min(line, maxLineAutoEnter - lines + 1); //调整line范围
			line = max(line, 0);
		}
		print_book_text_choose_mode(bookStr, convertedBookStr, strLengthOfLineNormal, strLengthOfLineAutoEnter, line, length, lines, offsetX, offsetY, maxLineNormal, maxLineAutoEnter, mode);
	}
	gotoxy(0, lines + offsetY + innerOffsetY);//清除信息
	cout << "                                                        " << endl;
	Sleep(TextBoxDelay3);
	return line;
}

//获取bookStr每行的长度信息
static long get_str_length_each_line(const char bookStr[], const int bookSize, long strLengthOfLine[], const int length)
{
	long strLength = 0, preStrLength = -1, res = -1; //必须小于0
	long line = 1;
	strLength = strLengthOfLine[0] = 0; //第一行必定为0
	for (line = 1; line < bookSize; line++) //从第二行开始累计计算长度信息
	{
		strLength += get_strLength_of_one_line(bookStr, strLength, length); 
		strLengthOfLine[line] = strLength;
		if (preStrLength == strLength && res < 0) //当连续两行记录返回值
		{
			res = line - 1;
			break;
		}
		else //存储当前值
			preStrLength = strLength;
	}
	return res;
}

//根据不同的mode输出从startIndex开始的文本
static void print_book_text_choose_mode(char bookStr[], char convertedBookStr[], const long strLengthOfLineNormal[], const long strLengthOfLineAutoEnter[], long line, int length, int lines, int offsetX, int offsetY, int maxLineNormal, int maxLineAutoEnter, int mode)
{
	if (mode == PrintModeNormal)
	{
		print_book_text(bookStr, strLengthOfLineNormal, line, length, lines, offsetX, offsetY, maxLineNormal);
		gotoxy(0, lines + offsetY + 4);
		cout << "当前进度：" << setiosflags(ios::fixed) << setprecision(2) << (double)(line) / maxLineNormal * 100 << "%"; //输出进度信息
		if (line >= (maxLineNormal - lines + 1))
			cout << " （已到达末尾）      ";
		else
			cout << "                     ";
	}
	if (mode == PrintModeAutoEnter)
	{
		print_book_text(convertedBookStr, strLengthOfLineAutoEnter, line, length, lines, offsetX, offsetY, maxLineAutoEnter);
		gotoxy(0, lines + offsetY + 4);
		cout << "当前进度：" << setiosflags(ios::fixed) << setprecision(2) << (double)(line) / maxLineAutoEnter * 100 << "%"; //输出进度信息
		if (line >= maxLineAutoEnter - lines + 1)
			cout << " （已到达末尾）      ";
		else
			cout << "                     ";
	}
}

//从字符串中获取某个位置对应的行数
static long get_line_of_index_from_array(const long strLengthOfLineNormal[], const long strLengthOfLineAutoEnter[], const long maxLineNormal, const long maxLineAutoEnter, const long index, const int mode)
{
	if (mode == PrintModeNormal)
	{
		for (long line = 0; line < maxLineNormal; line++)
		{
			if (strLengthOfLineNormal[line] > index)
				return line - 1;
		}
	}
	if (mode == PrintModeAutoEnter)
	{
		for (long line = 0; line < maxLineAutoEnter; line++)
		{
			if (strLengthOfLineAutoEnter[line] > index)
				return line - 1;
		}
	}
	return 0;
}

//改变输出模式
static void change_print_mode(long &line, int lines, int maxLineNormal, int maxLineAutoEnter, int &mode)
{
	if (mode == PrintModeNormal) //交换输出模式
	{
		mode = PrintModeAutoEnter;
		line = min(line, maxLineAutoEnter - lines);//处理超出范围的值
		line = max(line, 0);
	}
	else if (mode == PrintModeAutoEnter)
	{
		mode = PrintModeNormal;
		line = min(line, maxLineNormal - lines);
		line = max(line, 0);
	}
}

//read_book主循环函数，根据键盘按键做出反应
static int main_cycle_of_read_book(char bookStr[], char convertedBookStr[], const long strLengthOfLineNormal[], const long strLengthOfLineAutoEnter[], long &line, int length, int lines, int offsetX, int offsetY, int maxLineNormal, int maxLineAutoEnter, int mode)
{
	int mx, my, maction, keycode1, keycode2, ret; //记录键盘输入信息
	draw_graphics();
	for (; ; )
	{
		ret = gmw_read_keyboard_and_mouse(NULL, mx, my, maction, keycode1, keycode2); //获取键盘按键
		if (ret == CCT_KEYBOARD_EVENT)
		{
			if (keycode1 == 'N' || keycode1 == 'n') //N键
				return 1;
			if (keycode1 == 'Q' || keycode1 == 'q') //Q键
				return 0;
			if (keycode1 == 'F' || keycode1 == 'f') //F键
			{
				change_print_mode(line, lines, maxLineNormal, maxLineAutoEnter, mode);
				print_book_text_choose_mode(bookStr, convertedBookStr, strLengthOfLineNormal, strLengthOfLineAutoEnter, line, length, lines, offsetX, offsetY, maxLineNormal, maxLineAutoEnter, mode);
				draw_graphics_F();
			}
			if (keycode1 == 'S' || keycode1 == 's') //S键
			{
				draw_graphics_S();
				jump_book_line(bookStr, convertedBookStr, strLengthOfLineNormal, strLengthOfLineAutoEnter, line, length, lines, offsetX, offsetY, maxLineNormal, maxLineAutoEnter, mode);
			}
			if (keycode1 == 224) //其他按键
			{
				if (keycode2 == KB_ARROW_UP || keycode2 == KB_ARROW_DOWN) //方向上键与方向下键
				{
					if (keycode2 == KB_ARROW_UP && ((mode == PrintModeNormal && line > 0) || (mode == PrintModeAutoEnter && line > 0)))
						print_book_text_choose_mode(bookStr, convertedBookStr, strLengthOfLineNormal, strLengthOfLineAutoEnter, --line, length, lines, offsetX, offsetY, maxLineNormal, maxLineAutoEnter, mode);
					if (keycode2 == KB_ARROW_DOWN && ((mode == PrintModeNormal && line + lines < maxLineNormal + 1) || (mode == PrintModeAutoEnter && line + lines < maxLineAutoEnter + 1)))
						print_book_text_choose_mode(bookStr, convertedBookStr, strLengthOfLineNormal, strLengthOfLineAutoEnter, ++line, length, lines, offsetX, offsetY, maxLineNormal, maxLineAutoEnter, mode);
					if (keycode2 == KB_ARROW_UP)
						draw_graghics_up();
					if (keycode2 == KB_ARROW_DOWN)
						draw_graghics_down();
					draw_rotate_ellipse(keycode2 == KB_ARROW_DOWN);
				}
				if (keycode2 == VK_PRIOR || keycode2 == VK_NEXT) //Page Up 与 Page Down
				{
					for (int i = 0; i < lines; i++)//翻页到底
					{
						if (keycode2 == VK_PRIOR && line > 0) //Page Up
							line--;
						if (keycode2 == VK_NEXT && ((mode == PrintModeNormal && line + lines < maxLineNormal + 1) || (mode == PrintModeAutoEnter && line + lines < maxLineAutoEnter + 1))) //Page Down
							line++;
					}
					print_book_text_choose_mode(bookStr, convertedBookStr, strLengthOfLineNormal, strLengthOfLineAutoEnter, line, length, lines, offsetX, offsetY, maxLineNormal, maxLineAutoEnter, mode);
					if (keycode2 == VK_PRIOR)
						draw_graphics_PgUp();
					if (keycode2 == VK_NEXT)
						draw_graphics_PgDn();
					draw_rotate_ellipse(keycode2 == VK_NEXT);
				}
			}
		}
	}
}

static int main_cycle_of_choose_book(const char bookList[][DefaultStringLength], const char convertedBookList[][DefaultStringLength], int bookListLength, int lines, int length, int startLine, int offsetX, int offsetY)
{
	bool chosed = false;
	int bookChosed = 0, preBookChosed = 0, cursor = 0, preCursor = 0, mx, my, maction, keycode1, keycode2, ret;
	for (; chosed == false; )
	{
		ret = gmw_read_keyboard_and_mouse(NULL, mx, my, maction, keycode1, keycode2);
		if (ret == CCT_KEYBOARD_EVENT)
		{
			switch (keycode1)
			{
				case 13: //ENTER键
					showstr(0, WindowYSize1 - 3, "                                        ");
					gotoxy(0, WindowYSize1 - 3);
					cout << "选中了" << bookList[bookChosed];
					chosed = true;
					break;
				case 224: //方向键
					if (keycode2 == KB_ARROW_UP || keycode2 == KB_ARROW_DOWN)
					{
						if (keycode2 == KB_ARROW_UP) //方向上键
						{
							if (bookChosed > 0) //调整选择的书本和光标位置
								bookChosed--;
							if (cursor > 0)
								cursor--;
							if (cursor == 0 && preBookChosed != bookChosed) //满足条件才重新输出列表，减少闪烁
								print_book_list(convertedBookList, bookListLength, lines, length, startLine + bookChosed, offsetX, offsetY);
						}
						if (keycode2 == KB_ARROW_DOWN) //方向下键
						{
							if (bookChosed < bookListLength - 1) //调整选择的书本和光标位置
								bookChosed++;
							if (cursor < min(lines, bookListLength) - 1)
								cursor++;
							if ((cursor + 1 >= lines) && preBookChosed != bookChosed) //满足条件才重新输出列表，减少闪烁
								print_book_list(convertedBookList, bookListLength, lines, length, startLine + bookChosed - lines + 1, offsetX, offsetY);
						}
						if (preBookChosed != bookChosed) //满足前后两次选择的书本不同的条件才反色显示，减少闪烁
							highlight_one_line(convertedBookList[preBookChosed], convertedBookList[bookChosed], length, offsetX, offsetY + preCursor - startLine + 2, offsetX, offsetY + cursor - startLine + 2);
						preBookChosed = bookChosed; //记录选择的书本和光标位置
						preCursor = cursor;
					}
					break;
			}
		}
	}
	return bookChosed;
}

//获取书本列表信息，返回书本数目
int get_book_list(char bookList[][DefaultStringLength])
{
	struct _finddata_t file;
	int k, bookCount = 0, extensiveNameIndex = 0, i = 0, maxLength = MaxBookListLength;
	long HANDLE = 0;
	char bookPath[DefaultStringLength] = ".\\";
	while (CorrectExtensiveName[extensiveNameIndex][0] != '\0' && i < maxLength) // 以不同的扩展名循环读取信息
	{
		strcat(bookPath, BookFolderName);
		strcat(bookPath, "\\");
		strcat(bookPath, "*.");
		strcat(bookPath, CorrectExtensiveName[extensiveNameIndex++]);
		k = HANDLE = (long)_findfirst(bookPath, &file);
		while (k != -1 && i++ < maxLength) //循环读入文件夹内文件
		{
			strcpy(bookList[bookCount++], file.name);
			k = _findnext(HANDLE, &file);
		}
	}
	_findclose(HANDLE);
	return bookCount;
}

//输出信息并让用户选择一本书本，返回选择的书本的索引
int choose_book(const char bookList[][DefaultStringLength], const int bookListLength, const int lines, const int length, const int startLine, int offsetX, const int offsetY)
{
	if (bookList == NULL) //输入形参判断
		return -1;
	if (bookListLength == 0) //没有可以显示的文件文件
	{
		cout << "错误：未找到" << BookFolderName << "文件夹或" << BookFolderName << "文件夹中没有可以打开的文件" << endl << "回车退出" << endl;
		wait_enter();
		return -2;
	}
	bool chosed = false;
	setcursor(CURSOR_VISIBLE_HALF);
	char convertedBookList[MaxBookListLength][DefaultStringLength]; //处理半个汉字问题的字符串
	for (int i = 0; i < bookListLength; i++) //处理半个汉字问题
	{
		strcpy(convertedBookList[i], bookList[i]);
		convert_half_character(convertedBookList[i], length * 2);
	}
	CONSOLE_GRAPHICS_INFO CGI;
	gmw_init(&CGI, lines, length, COLOR_BLACK, COLOR_HWHITE);
	gmw_set_frame_style(&CGI, 2, 1, false);
	gmw_set_ext_rowcol(&CGI, ChooseYOffset, WindowYSize1 - ChooseYOffset - lines - 1, ChooseXOffset - 2, WindowXSize1 - ChooseXOffset - length - 4);
	gmw_draw_frame(&CGI);
	print_book_list(convertedBookList, bookListLength, lines, length, 0, offsetX, offsetY); //输出书本列表
	showstr(offsetX, offsetY, HelpInfoChooseMode); //输出帮助信息
	highlight_one_line(convertedBookList[0], convertedBookList[0], length, offsetX, offsetY + 0 - startLine + 2, offsetX, offsetY + 0 - startLine + 2); //反色显示当前文件
	return main_cycle_of_choose_book(bookList, convertedBookList, bookListLength, lines, length, startLine, offsetX, offsetY); //循环读入键盘按键并进行反应，返回选择的书本
}

//打开文件，读入书本内容
int read_book_file(char bookStr[], const char bookFilePath[], const long fileSize)
{
	if (bookStr == NULL) //形参输入判断
		return -1;
	ifstream in;
	in.open(bookFilePath, ios::in);
	if (!in.is_open())
		return -2;
	in.read(bookStr, fileSize); //一次读入整个文件
	if (in.gcount() > fileSize) //读入的字符大于文件大小，一般不会发生
	{
		in.close();
		return -3;
	}
	bookStr[in.gcount()] = '\0'; //置尾零
	in.close();
	return 0;
}

//读书功能主函数
int read_book(char bookStr[], const int bookSize, long &startIndex, int length, const int lines, const int offsetX, const int offsetY, int mode)
{
	if ((mode != PrintModeNormal && mode != PrintModeAutoEnter) || bookStr == NULL) //形参输入判断
		return -1;
	length = length - (1 - (length) % 2); //将长度强制转为偶数
	bool quit = false; //记录是否退出
	long line, maxLineNormal, maxLineAutoEnter;
	long *strLengthOfLineNormal = new long[bookSize + 5], *strLengthOfLineAutoEnter = new long[bookSize + 5]; //记录不同模式下每行的第一个字符的索引
	char *convertedBookStr = new char[int((bookSize + bookSize / length) * (TableToBlank + 1))]; //AutoEnter模式下转换后的bookStr

	CONSOLE_GRAPHICS_INFO CGI; //输出外框架
	gmw_init(&CGI, lines, length / 2, COLOR_BLACK, COLOR_HWHITE);
	gmw_set_frame_default_linetype(&CGI, 2);
	gmw_set_frame_style(&CGI, 2, 1, false);
	gmw_set_font(&CGI, Font2, FontSize2);
	gmw_set_ext_rowcol(&CGI, TextBoxYOffset, WindowYSize2 - TextBoxYOffset - lines - 2, TextBoxXOffset, WindowXSize2 - TextBoxXOffset - length - 4);
	gmw_draw_frame(&CGI);

	convert_bookStr_to_autoenter(convertedBookStr, bookStr); //转换bookStr，调整空格
	bookStr = convert_bookStr_to_normal(bookStr);//调整bookStr，添加空格，满足Update的要求
	maxLineNormal = get_str_length_each_line(bookStr, bookSize, strLengthOfLineNormal, length); //计算出每行的第一个字符的索引
	maxLineAutoEnter = get_str_length_each_line(convertedBookStr, bookSize, strLengthOfLineAutoEnter, length); //同上
	line = get_line_of_index_from_array(strLengthOfLineNormal, strLengthOfLineAutoEnter, maxLineNormal, maxLineAutoEnter, startIndex, mode); // 获取开始位置对应的行数
	print_book_text_choose_mode(bookStr, convertedBookStr, strLengthOfLineNormal, strLengthOfLineAutoEnter, line, length, lines, offsetX, offsetY, maxLineNormal, maxLineAutoEnter, mode); //输出一次文本
	print_help_info(0, lines + offsetY + 5); //输出帮助信息

	setcursor(CURSOR_INVISIBLE);
	
	int ret = main_cycle_of_read_book(bookStr, convertedBookStr, strLengthOfLineNormal, strLengthOfLineAutoEnter, line, length, lines, offsetX, offsetY, maxLineNormal, maxLineAutoEnter, mode);//开始主循环

	if (mode == PrintModeNormal) //记录退出时的位置
		startIndex = strLengthOfLineNormal[line];
	if (mode == PrintModeAutoEnter)
		startIndex = strLengthOfLineAutoEnter[line];

	delete[]convertedBookStr; //释放内存
	delete[]strLengthOfLineNormal;
	delete[]strLengthOfLineAutoEnter;
	delete[]bookStr;
	return ret;
}

//1850059 ��1�� ��־Զ

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

//����print_book_list��һ���֣���ɫ���һ����Ϣ�����������һ����Ϣ
static void highlight_one_line(const char preStr[], const char str[], const int strShowedLength, const int preCursorX, const int preCursorY, const int cursorX, const int cursorY)
{
	showstr(preCursorX, preCursorY, preStr, COLOR_BLACK, COLOR_WHITE, 1, strShowedLength * 2);//�������֮ǰ�е���Ϣ
	showstr(cursorX, cursorY, str, COLOR_WHITE, COLOR_BLACK, 1, strShowedLength * 2);//��ɫ�����ǰ����Ϣ
	gotoxy(cursorX + my_min((int)strlen(str) - 2, strShowedLength * 2 - 2) + 2, cursorY);//��ת����ǰ��ĩβ
}

//����ı��˵�
static int print_book_list(const char bookList[][DefaultStringLength], const int bookListLength, const int lines, const int length, const int startLine, int offsetX, const int offsetY)
{
	int cursorX, cursorY;
	getxy(cursorX, cursorY); //��¼����ʼλ��
	//showstr(offsetX, offsetY, helpInfo);//���������Ϣ
	//draw_framework(lines, length, false, true, false, COLOR_WHITE, COLOR_BLACK, offsetX - 2, offsetY); //�������
	for (int i = startLine; i < startLine + lines && i < bookListLength; i++) //���ÿ���鱾����
	{
		showstr(offsetX, offsetY + i - startLine + 2, bookList[i], 0, 7, 1, length * 2);
	}
	gotoxy(cursorX, cursorY); //���ص���ʼλ��
	return 0;
}

//��ȡ��startIndex��ʼ��ĳ�еĳ���.
static int get_strLength_of_one_line(const char bookStr[], const int startIndex, int length)
{
	length -= 1;//��ֹ�ұ���ʾ����
	int cursorX = 0;
	for (int i = startIndex; bookStr[i] != '\0'; i++)
	{
		if (bookStr[i] != '\n')
		{
			if (is_Chinese_character(bookStr[i]))//�������ĵĵ�һ���ַ���������һ���ַ�
			{
				i++;
				cursorX++;
			}
			cursorX++;
		}
		if (bookStr[i] == '\n')//��������
		{
			cursorX++;
			break;
		}
		if (cursorX >= length)//��������
		{
			if (cursorX > length)
				cursorX -= 2;
			break;
		}
	}
	return cursorX;
}

//��bookStrת��ΪAutoEnterģʽ�µ��ַ���
static int convert_bookStr_to_autoenter(char buf[], char bookStr[])
{
	char *b = buf, *p = bookStr;
	for (; *p != '\0'; p++)
	{
		if (*p != '\n' && *p != '\t') //��ͨ�ַ�
		{
			*b++ = *p;
		}
		if (*p == '\t') //tab
		{
			for(int i = 0; i < TableToBlank; i++)
				*b++ = ' ';
		}
		if ((*p == '\n' && (*(p + 1) == ' ' || *(p + 1) == '\t' || *(p + 1) == '\n' || (*(p + 1) == (char)0xA1 && *(p + 2) == (char)0xA1)))) //���������Ļس��ַ�
		{
			*b++ = '\n';
		}
	}
	*b = '\0'; //��β��
	return 0;
}

//�����µ�bookStr�ַ�����������¸�ֵ������bookStr����ӿո�����Update��Ҫ��
static char* convert_bookStr_to_normal(char bookStr[])
{
	char *bookFileStr = new char[int(strlen(bookStr) * (TableToBlank + 1))]; //���½���һ���洢�����ļ����ݵĳ����ַ���
	char *p = bookStr, *b = bookFileStr;
	for (; *p != '\0'; p++)
	{
		if (*p == '\n' && !(*(p + 1) == ' ' || *(p + 1) == '\t' || *(p + 1) == '\n' || (*(p + 1) == (char)0xA1 && *(p + 2) == (char)0xA1))) //�س�����һ����Ϊ�ո�
		{
			*b++ = '\n';
			for (int i = 0; i < 4; i++)	//����4���ո�
				*b++ = ' ';
		}
		else if (*p == '\t') //�滻tab
		{
			for (int i = 0; i < TableToBlank; i++)	//�������ո�
				*b++ = ' ';
		}
		else //��������ճ�д��������
			*b++ = *p;
	}
	*b = '\0'; //��β��
	delete[]bookStr; //�ͷ�֮ǰ���ַ���
	return bookFileStr; //�������ַ���
}


//�����Ļ�е�����
static void clear_text(const int lines, const int length, const int offsetX, const int offsetY)
{	
	char temp[WindowXSize2] = "";
	for (int i = 0; i <= length - 2; i++)//�����㹻���Ŀո���ɵ��ַ���
		temp[i] = ' ';
	for (long line = 0; line < lines; line++)//ÿ���������
	{
		gotoxy(offsetX + 2, offsetY + line + 2);
		cout << temp << endl;
	}
}

//�����startLine��ʼ���鱾�ı�
static int print_book_text(char bookStr[], const long strLengthOfLine[], int startLine, int length, int lines, int offsetX, int offsetY, int maxLine)
{
	clear_text(lines, length, offsetX, offsetY); //�������
	for (long line = 0; line < lines && line < maxLine; line++) //���ÿ�е�����
	{
		gotoxy(offsetX + 2, offsetY + 2 + line);
		char tempCh = bookStr[strLengthOfLine[startLine + line + 1]]; //��¼��һ�е�һ���ַ�
		bookStr[strLengthOfLine[startLine + line + 1]] = '\0'; //���β�㣬�������
		cout << &bookStr[strLengthOfLine[startLine + line]];
		bookStr[strLengthOfLine[startLine + line + 1]] = tempCh; //��ԭ
	}
	gotoxy(offsetX + 2, offsetY + 2);
	return 0;
}

//���������Ϣ
static void print_help_info(int x, int y)
{
	gotoxy(x, y);
	cout << HelpInfo;
}

//S����ת���ܵ�ʵ�֣�����Ϊ��λ�����ֽڣ�������ת���line
static int jump_book_line(char bookStr[], char convertedBookStr[], const long strLengthOfLineNormal[], const long strLengthOfLineAutoEnter[], long &line, int length, int lines, int offsetX, int offsetY, int maxLineNormal, int maxLineAutoEnter, int mode)
{
	const int maxStrLength = 6, innerOffsetY = 6;
	const char info[] = "Ҫ��ת����0-100�ĸ�������С����������λ����";
	char input[20] = "", tempCh = 0;
	double target;
	gotoxy(0, lines + offsetY + innerOffsetY); //���������Ϣ
	cout << info << "                   ";
	gotoxy((int)strlen(info), lines + offsetY + innerOffsetY);
	for (int i = 0; i < maxStrLength; i++) //��������
	{
		tempCh = _getch();
		if (tempCh == '\r')
			break;
		putchar(tempCh);
		input[i] = tempCh;
	}
	target = atof(input); //ת��Ϊ������
	if (target >= 0 && target <= 100)
	{
		if (mode == PrintModeNormal)
		{
			line = (long)(target / 100 * maxLineNormal); //����line�Ĵ�С
			line = min(line, maxLineNormal - lines + 1); //����line��Χ
			line = max(line, 0);
		}
		if (mode == PrintModeAutoEnter)
		{
			line = (long)(target / 100 * maxLineAutoEnter); //����line�Ĵ�С
			line = min(line, maxLineAutoEnter - lines + 1); //����line��Χ
			line = max(line, 0);
		}
		print_book_text_choose_mode(bookStr, convertedBookStr, strLengthOfLineNormal, strLengthOfLineAutoEnter, line, length, lines, offsetX, offsetY, maxLineNormal, maxLineAutoEnter, mode);
	}
	gotoxy(0, lines + offsetY + innerOffsetY);//�����Ϣ
	cout << "                                                        " << endl;
	Sleep(TextBoxDelay3);
	return line;
}

//��ȡbookStrÿ�еĳ�����Ϣ
static long get_str_length_each_line(const char bookStr[], const int bookSize, long strLengthOfLine[], const int length)
{
	long strLength = 0, preStrLength = -1, res = -1; //����С��0
	long line = 1;
	strLength = strLengthOfLine[0] = 0; //��һ�бض�Ϊ0
	for (line = 1; line < bookSize; line++) //�ӵڶ��п�ʼ�ۼƼ��㳤����Ϣ
	{
		strLength += get_strLength_of_one_line(bookStr, strLength, length); 
		strLengthOfLine[line] = strLength;
		if (preStrLength == strLength && res < 0) //���������м�¼����ֵ
		{
			res = line - 1;
			break;
		}
		else //�洢��ǰֵ
			preStrLength = strLength;
	}
	return res;
}

//���ݲ�ͬ��mode�����startIndex��ʼ���ı�
static void print_book_text_choose_mode(char bookStr[], char convertedBookStr[], const long strLengthOfLineNormal[], const long strLengthOfLineAutoEnter[], long line, int length, int lines, int offsetX, int offsetY, int maxLineNormal, int maxLineAutoEnter, int mode)
{
	if (mode == PrintModeNormal)
	{
		print_book_text(bookStr, strLengthOfLineNormal, line, length, lines, offsetX, offsetY, maxLineNormal);
		gotoxy(0, lines + offsetY + 4);
		cout << "��ǰ���ȣ�" << setiosflags(ios::fixed) << setprecision(2) << (double)(line) / maxLineNormal * 100 << "%"; //���������Ϣ
		if (line >= (maxLineNormal - lines + 1))
			cout << " ���ѵ���ĩβ��      ";
		else
			cout << "                     ";
	}
	if (mode == PrintModeAutoEnter)
	{
		print_book_text(convertedBookStr, strLengthOfLineAutoEnter, line, length, lines, offsetX, offsetY, maxLineAutoEnter);
		gotoxy(0, lines + offsetY + 4);
		cout << "��ǰ���ȣ�" << setiosflags(ios::fixed) << setprecision(2) << (double)(line) / maxLineAutoEnter * 100 << "%"; //���������Ϣ
		if (line >= maxLineAutoEnter - lines + 1)
			cout << " ���ѵ���ĩβ��      ";
		else
			cout << "                     ";
	}
}

//���ַ����л�ȡĳ��λ�ö�Ӧ������
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

//�ı����ģʽ
static void change_print_mode(long &line, int lines, int maxLineNormal, int maxLineAutoEnter, int &mode)
{
	if (mode == PrintModeNormal) //�������ģʽ
	{
		mode = PrintModeAutoEnter;
		line = min(line, maxLineAutoEnter - lines);//��������Χ��ֵ
		line = max(line, 0);
	}
	else if (mode == PrintModeAutoEnter)
	{
		mode = PrintModeNormal;
		line = min(line, maxLineNormal - lines);
		line = max(line, 0);
	}
}

//read_book��ѭ�����������ݼ��̰���������Ӧ
static int main_cycle_of_read_book(char bookStr[], char convertedBookStr[], const long strLengthOfLineNormal[], const long strLengthOfLineAutoEnter[], long &line, int length, int lines, int offsetX, int offsetY, int maxLineNormal, int maxLineAutoEnter, int mode)
{
	int mx, my, maction, keycode1, keycode2, ret; //��¼����������Ϣ
	draw_graphics();
	for (; ; )
	{
		ret = gmw_read_keyboard_and_mouse(NULL, mx, my, maction, keycode1, keycode2); //��ȡ���̰���
		if (ret == CCT_KEYBOARD_EVENT)
		{
			if (keycode1 == 'N' || keycode1 == 'n') //N��
				return 1;
			if (keycode1 == 'Q' || keycode1 == 'q') //Q��
				return 0;
			if (keycode1 == 'F' || keycode1 == 'f') //F��
			{
				change_print_mode(line, lines, maxLineNormal, maxLineAutoEnter, mode);
				print_book_text_choose_mode(bookStr, convertedBookStr, strLengthOfLineNormal, strLengthOfLineAutoEnter, line, length, lines, offsetX, offsetY, maxLineNormal, maxLineAutoEnter, mode);
				draw_graphics_F();
			}
			if (keycode1 == 'S' || keycode1 == 's') //S��
			{
				draw_graphics_S();
				jump_book_line(bookStr, convertedBookStr, strLengthOfLineNormal, strLengthOfLineAutoEnter, line, length, lines, offsetX, offsetY, maxLineNormal, maxLineAutoEnter, mode);
			}
			if (keycode1 == 224) //��������
			{
				if (keycode2 == KB_ARROW_UP || keycode2 == KB_ARROW_DOWN) //�����ϼ��뷽���¼�
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
				if (keycode2 == VK_PRIOR || keycode2 == VK_NEXT) //Page Up �� Page Down
				{
					for (int i = 0; i < lines; i++)//��ҳ����
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
				case 13: //ENTER��
					showstr(0, WindowYSize1 - 3, "                                        ");
					gotoxy(0, WindowYSize1 - 3);
					cout << "ѡ����" << bookList[bookChosed];
					chosed = true;
					break;
				case 224: //�����
					if (keycode2 == KB_ARROW_UP || keycode2 == KB_ARROW_DOWN)
					{
						if (keycode2 == KB_ARROW_UP) //�����ϼ�
						{
							if (bookChosed > 0) //����ѡ����鱾�͹��λ��
								bookChosed--;
							if (cursor > 0)
								cursor--;
							if (cursor == 0 && preBookChosed != bookChosed) //������������������б�������˸
								print_book_list(convertedBookList, bookListLength, lines, length, startLine + bookChosed, offsetX, offsetY);
						}
						if (keycode2 == KB_ARROW_DOWN) //�����¼�
						{
							if (bookChosed < bookListLength - 1) //����ѡ����鱾�͹��λ��
								bookChosed++;
							if (cursor < min(lines, bookListLength) - 1)
								cursor++;
							if ((cursor + 1 >= lines) && preBookChosed != bookChosed) //������������������б�������˸
								print_book_list(convertedBookList, bookListLength, lines, length, startLine + bookChosed - lines + 1, offsetX, offsetY);
						}
						if (preBookChosed != bookChosed) //����ǰ������ѡ����鱾��ͬ�������ŷ�ɫ��ʾ��������˸
							highlight_one_line(convertedBookList[preBookChosed], convertedBookList[bookChosed], length, offsetX, offsetY + preCursor - startLine + 2, offsetX, offsetY + cursor - startLine + 2);
						preBookChosed = bookChosed; //��¼ѡ����鱾�͹��λ��
						preCursor = cursor;
					}
					break;
			}
		}
	}
	return bookChosed;
}

//��ȡ�鱾�б���Ϣ�������鱾��Ŀ
int get_book_list(char bookList[][DefaultStringLength])
{
	struct _finddata_t file;
	int k, bookCount = 0, extensiveNameIndex = 0, i = 0, maxLength = MaxBookListLength;
	long HANDLE = 0;
	char bookPath[DefaultStringLength] = ".\\";
	while (CorrectExtensiveName[extensiveNameIndex][0] != '\0' && i < maxLength) // �Բ�ͬ����չ��ѭ����ȡ��Ϣ
	{
		strcat(bookPath, BookFolderName);
		strcat(bookPath, "\\");
		strcat(bookPath, "*.");
		strcat(bookPath, CorrectExtensiveName[extensiveNameIndex++]);
		k = HANDLE = (long)_findfirst(bookPath, &file);
		while (k != -1 && i++ < maxLength) //ѭ�������ļ������ļ�
		{
			strcpy(bookList[bookCount++], file.name);
			k = _findnext(HANDLE, &file);
		}
	}
	_findclose(HANDLE);
	return bookCount;
}

//�����Ϣ�����û�ѡ��һ���鱾������ѡ����鱾������
int choose_book(const char bookList[][DefaultStringLength], const int bookListLength, const int lines, const int length, const int startLine, int offsetX, const int offsetY)
{
	if (bookList == NULL) //�����β��ж�
		return -1;
	if (bookListLength == 0) //û�п�����ʾ���ļ��ļ�
	{
		cout << "����δ�ҵ�" << BookFolderName << "�ļ��л�" << BookFolderName << "�ļ�����û�п��Դ򿪵��ļ�" << endl << "�س��˳�" << endl;
		wait_enter();
		return -2;
	}
	bool chosed = false;
	setcursor(CURSOR_VISIBLE_HALF);
	char convertedBookList[MaxBookListLength][DefaultStringLength]; //����������������ַ���
	for (int i = 0; i < bookListLength; i++) //��������������
	{
		strcpy(convertedBookList[i], bookList[i]);
		convert_half_character(convertedBookList[i], length * 2);
	}
	CONSOLE_GRAPHICS_INFO CGI;
	gmw_init(&CGI, lines, length, COLOR_BLACK, COLOR_HWHITE);
	gmw_set_frame_style(&CGI, 2, 1, false);
	gmw_set_ext_rowcol(&CGI, ChooseYOffset, WindowYSize1 - ChooseYOffset - lines - 1, ChooseXOffset - 2, WindowXSize1 - ChooseXOffset - length - 4);
	gmw_draw_frame(&CGI);
	print_book_list(convertedBookList, bookListLength, lines, length, 0, offsetX, offsetY); //����鱾�б�
	showstr(offsetX, offsetY, HelpInfoChooseMode); //���������Ϣ
	highlight_one_line(convertedBookList[0], convertedBookList[0], length, offsetX, offsetY + 0 - startLine + 2, offsetX, offsetY + 0 - startLine + 2); //��ɫ��ʾ��ǰ�ļ�
	return main_cycle_of_choose_book(bookList, convertedBookList, bookListLength, lines, length, startLine, offsetX, offsetY); //ѭ��������̰��������з�Ӧ������ѡ����鱾
}

//���ļ��������鱾����
int read_book_file(char bookStr[], const char bookFilePath[], const long fileSize)
{
	if (bookStr == NULL) //�β������ж�
		return -1;
	ifstream in;
	in.open(bookFilePath, ios::in);
	if (!in.is_open())
		return -2;
	in.read(bookStr, fileSize); //һ�ζ��������ļ�
	if (in.gcount() > fileSize) //������ַ������ļ���С��һ�㲻�ᷢ��
	{
		in.close();
		return -3;
	}
	bookStr[in.gcount()] = '\0'; //��β��
	in.close();
	return 0;
}

//���鹦��������
int read_book(char bookStr[], const int bookSize, long &startIndex, int length, const int lines, const int offsetX, const int offsetY, int mode)
{
	if ((mode != PrintModeNormal && mode != PrintModeAutoEnter) || bookStr == NULL) //�β������ж�
		return -1;
	length = length - (1 - (length) % 2); //������ǿ��תΪż��
	bool quit = false; //��¼�Ƿ��˳�
	long line, maxLineNormal, maxLineAutoEnter;
	long *strLengthOfLineNormal = new long[bookSize + 5], *strLengthOfLineAutoEnter = new long[bookSize + 5]; //��¼��ͬģʽ��ÿ�еĵ�һ���ַ�������
	char *convertedBookStr = new char[int((bookSize + bookSize / length) * (TableToBlank + 1))]; //AutoEnterģʽ��ת�����bookStr

	CONSOLE_GRAPHICS_INFO CGI; //�������
	gmw_init(&CGI, lines, length / 2, COLOR_BLACK, COLOR_HWHITE);
	gmw_set_frame_default_linetype(&CGI, 2);
	gmw_set_frame_style(&CGI, 2, 1, false);
	gmw_set_font(&CGI, Font2, FontSize2);
	gmw_set_ext_rowcol(&CGI, TextBoxYOffset, WindowYSize2 - TextBoxYOffset - lines - 2, TextBoxXOffset, WindowXSize2 - TextBoxXOffset - length - 4);
	gmw_draw_frame(&CGI);

	convert_bookStr_to_autoenter(convertedBookStr, bookStr); //ת��bookStr�������ո�
	bookStr = convert_bookStr_to_normal(bookStr);//����bookStr����ӿո�����Update��Ҫ��
	maxLineNormal = get_str_length_each_line(bookStr, bookSize, strLengthOfLineNormal, length); //�����ÿ�еĵ�һ���ַ�������
	maxLineAutoEnter = get_str_length_each_line(convertedBookStr, bookSize, strLengthOfLineAutoEnter, length); //ͬ��
	line = get_line_of_index_from_array(strLengthOfLineNormal, strLengthOfLineAutoEnter, maxLineNormal, maxLineAutoEnter, startIndex, mode); // ��ȡ��ʼλ�ö�Ӧ������
	print_book_text_choose_mode(bookStr, convertedBookStr, strLengthOfLineNormal, strLengthOfLineAutoEnter, line, length, lines, offsetX, offsetY, maxLineNormal, maxLineAutoEnter, mode); //���һ���ı�
	print_help_info(0, lines + offsetY + 5); //���������Ϣ

	setcursor(CURSOR_INVISIBLE);
	
	int ret = main_cycle_of_read_book(bookStr, convertedBookStr, strLengthOfLineNormal, strLengthOfLineAutoEnter, line, length, lines, offsetX, offsetY, maxLineNormal, maxLineAutoEnter, mode);//��ʼ��ѭ��

	if (mode == PrintModeNormal) //��¼�˳�ʱ��λ��
		startIndex = strLengthOfLineNormal[line];
	if (mode == PrintModeAutoEnter)
		startIndex = strLengthOfLineAutoEnter[line];

	delete[]convertedBookStr; //�ͷ��ڴ�
	delete[]strLengthOfLineNormal;
	delete[]strLengthOfLineAutoEnter;
	delete[]bookStr;
	return ret;
}

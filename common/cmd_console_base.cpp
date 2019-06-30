//1850059 计1班 杨志远

#include "cmd_console_base.h"

//输出提示信息并且选择模式
char welcome_and_select_mode(const char *str[], const char modeInfo[])
{
	int maxLength = 0, lines = (int)strlen(modeInfo);
	char choices[100] = "", ch;
	bool flag = false;
	for (int i = 0; i < lines; i++)
	{
		maxLength = max(maxLength, (int)strlen(str[i]));
		choices[i] = tolower(modeInfo[i]);
	}
	for (int i = 0; i < maxLength; i++)
		cout << "-";
	cout << endl;
	for (int i = 0; i < lines; i++)
		cout << str[i] << endl;
	for (int i = 0; i < maxLength; i++)
		cout << "-";
	cout << endl << "[请选择] ";
	while (flag == false)
	{
		ch = tolower(_getch());
		for (int i = 0; i < lines; i++)
		{
			if (tolower(ch) == choices[i])
			{
				cout << ch;
				flag = true;
				break;
			}
		}
	}
	if (ch == choices[lines - 1])
		exit(0);
	return tolower(ch);
}

//printMode: -1->单色  0->彩色  1->特殊标记为*（必须传入tempArray）
//2->特殊标记为彩色（蓝色，必须传入tempArray）  3->反显（只用彩色标识0）
void print_map(int map[][DefaultMaxCol + 1], int row, int col, int printMode, int tempArray[][DefaultMaxCol + 1])
{
	cout << "  |";
	for (int i = 0; i < col; i++)
		cout << "  " << i + 1;
	cout << endl << "--+";
	for (int i = 0; i < col; i++)
		cout << "---";
	cout << "-" << endl;
	for (int i = 0; i < row; i++)
	{
		cout << char('A' + i) << " |";
		for (int j = 0; j < col; j++)
		{
			if (printMode == 1)
				cout << ((tempArray[i][j] == -1) ? "  *" : "  0");
			else
			{
				cout << "  ";
				if (printMode == 0 && map[i][j] || printMode == 2 && tempArray[i][j] || printMode == 3 && map[i][j] == 0)
					setcolor(COLOR_HYELLOW, (printMode == 2 ? COLOR_BLUE : map[i][j]));
				cout << map[i][j];
				setcolor();
			}
		}
		cout << endl;
	}
	cout << endl;
}

//输出提示信息message，并调用print_map函数
void print_map_with_info(int map[][DefaultMaxCol + 1], int row, int col, const char message[], int printMode, int tempArray[][DefaultMaxCol + 1])
{
	cout << message << endl;
	print_map(map, row, col, printMode, tempArray);
}

//mode: 行数 -> 1   列数 -> 2    End -> 3    起点 -> 4    终点 -> 5    根据需求增加后续参数
char *input_information(int winX, int inputMode, int mode, const char HelpInformation[][DefaultMaxInfoLength], char tempStr[], int *row, int *col, char instruction[])
{
	bool secondInput = false;
	int x, y;
	getxy(x, y);
	x += winX;
	while (1)
	{
		gotoxy(0, y + secondInput);
		cout << HelpInformation[inputMode];
		cout << "                        ";
		gotoxy(x, y + secondInput);
		cin >> tempStr;
		cin.ignore(99999, '\n');
		_strlwr(tempStr);
		if (inputMode == 1)
			*col = tempStr[0] - '0';
		if (inputMode == 2)
			*row = tempStr[0] - '0';
		if (inputMode == 4 || inputMode == 5)
			strcpy(instruction, tempStr);
		break;
	}
	return tempStr;
}

//检查输入的tempStr是否符合规范
static bool check_input_information(int inputMode, char tempStr[], int realRow, int realCol, int minRow, int maxRow, int minCol, int maxCol)
{
	if (inputMode == 1)
	{
		if (strlen(tempStr) != 1 || tempStr[0] < minRow + '0' || tempStr[0] > maxRow + '0')
			return false;
	}
	if (inputMode == 2)
	{
		if (strlen(tempStr) != 1 || tempStr[0] < minCol + '0' || tempStr[0] > maxCol + '0')
			return false;
	}
	if (inputMode == 3)
	{
		if (strlen(tempStr) != 3 || _stricmp(tempStr, "end") != 0)
			return false;
	}
	if (inputMode == 4 || inputMode == 5)
	{
		if (strlen(tempStr) != 2 || tempStr[0] < 'a' || tempStr[0] > 'a' + realRow - 1 || tempStr[1] < '1' || tempStr[1] > '1' + realCol - 1)
			return false;
	}
	return true;
}

//输入并检查信息
void input_checked_infomation(int winX, int inputMode, int mode, const char HelpInformation[][DefaultMaxInfoLength], char tempStr[], int minRow, int maxRow, int minCol, int maxCol, int *row, int *col, char instruction[], int realRow, int realCol)
{
	int x = 0, y = 0;
	getxy(x, y);
	while (1)
	{
		input_information(winX, inputMode, mode, HelpInformation, tempStr, row, col, instruction);
		if (!check_input_information(inputMode, tempStr, realRow, realCol, minRow, maxRow, minCol, maxCol))
		{
			gotoxy(0, y + 1);
			cout << HelpInformation[0];
			gotoxy(0, y);
			continue;
		}
		break;
	}
	gotoxy(0, y + 1);
	for (int i = 0, k = (int)strlen(HelpInformation[0]); i < k + 5; i++)
		cout << " ";
	gotoxy(0, y + 1);
}

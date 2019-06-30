//1850059 计1班 杨志远

#include "90-b4.h"

//开始界面中输入一个数字，offsetX和offsetY代表显示的帮助信息的
int input_number(const int helpInfoLength, const int offsetX, const int offsetY)
{
	const int maxLength = 10;
	while (1)
	{
		char ch, temp[maxLength + 5] = "";
		int pos = 0, res;
		showch(offsetX + helpInfoLength, offsetY, ' ', COLOR_WHITE, COLOR_BLACK, maxLength);	//用空格擦除一行
		gotoxy(offsetX + helpInfoLength, offsetY);	//输出提示信息
		while ((ch = _getch()))	//输入字符
		{
			if (ch != '\b')
			{
				temp[pos++] = ch;
				putchar(ch);
			}
			if (pos >= maxLength || ch == '\n' || ch == '\r')
			{
				break;
			}
		}
		if (temp[0] > '0' || temp[0] < '9')	//判断是否是数字
		{
			res = atoi(temp);	//转换数字
			return res;			//返回转换的数字
		}
	}
}

//添加新数字
int create_new_number(int map[][MaxCol], const int row, const int col, int &x, int &y)
{
	const int num[] = { 2, 4 };		//可以添加的数字
	do
	{
		x = rand() % col;
		y = rand() % row;
	} while (map[y][x] != 0);		//找到一个空格位置
	map[y][x] = num[rand() % (sizeof(num) / sizeof(num[0]))];	//添加数字
	return map[y][x];
}

//以某个方向移动整个地图，返回移动次数
int move_map(const CONSOLE_GRAPHICS_INFO * const pCGI, int map[][MaxCol], const int row, const int col, const int keycode, int &score)
{
	int res = 0;
	if (keycode == KB_ARROW_UP)				//向上移动
	{
		for (int j = 0; j < col; j++)		//遍历每一列
		{
			bool flag = false;		//保证每一行数字最多只合并一次
			for (int i = 0; i < row; i++)	//遍历每一行
			{
				if (map[i][j] != 0)			//找到有数字的位置
				{
					int k, distance = 0, value = map[i][j];	//记录当前的值和移动的距离
					for (k = i - 1; k >= 0; k--)			//遍历从当前行到起点每个数字
					{
						if (map[k][j] == 0 || (map[k][j] == map[k + 1][j] && !flag))	//如果可以移动或合并
						{
							if (map[k][j] == map[k + 1][j])		//如果可以合并
							{
								flag = true;			//记录已合并
								score += (2 * value);	//计入分数
							}
							map[k][j] += map[k + 1][j];	//合并数字
							map[k + 1][j] = 0;			//当前位置记为0
							distance++;					//记录距离
						}
					}
					gmw_move_block(pCGI, i, j, value, 0, BDI, DOWN_TO_UP, distance);	//移动的动画
					gmw_draw_block(pCGI, i - distance, j, map[i - distance][j], BDI);	//绘制当前位置的色块
					if (distance > 0)	//如果发生了移动，则记录下来
						res++;
				}
			}
		}
	}
	//以下的步骤类似，区别在于循环范围和顺序，很难写成同样的函数
	if (keycode == KB_ARROW_DOWN)//
	{
		for (int j = 0; j < col; j++)
		{
			bool flag = false;
			for (int i = row - 1; i >= 0; i--)
			{
				if (map[i][j] != 0)
				{
					int k, distance = 0, value = map[i][j];
					for (k = i + 1; k < row; k++)
					{
						if (map[k][j] == 0 || (map[k][j] == map[k - 1][j] && !flag))
						{
							if (map[k][j] == map[k - 1][j])
							{
								flag = true;
								score += (2 * value);
							}
							map[k][j] += map[k - 1][j];
							map[k - 1][j] = 0;
							distance++;
						}
					}
					gmw_move_block(pCGI, i, j, value, 0, BDI, UP_TO_DOWN, distance);
					gmw_draw_block(pCGI, i + distance, j, map[i + distance][j], BDI);
					if (distance > 0)
						res++;
				}
			}
		}
	}
	if (keycode == KB_ARROW_LEFT)
	{
		for (int i = 0; i < row; i++)
		{
			bool flag = false;
			for (int j = 0; j < col; j++)
			{
				if (map[i][j] != 0)
				{
					int k, distance = 0, value = map[i][j];
					for (k = j - 1; k >= 0; k--)
					{
						if (map[i][k] == 0 || (map[i][k] == map[i][k + 1] && !flag))
						{
							if (map[i][k] == map[i][k + 1])
							{
								flag = true;
								score += (2 * value);
							}
							map[i][k] += map[i][k + 1];
							map[i][k + 1] = 0;
							distance++;
						}
					}
					gmw_move_block(pCGI, i, j, value, 0, BDI, RIGHT_TO_LEFT, distance);
					gmw_draw_block(pCGI, i, j - distance, map[i][j - distance], BDI);
					if (distance > 0)
						res++;
				}
			}
		}
	}
	if (keycode == KB_ARROW_RIGHT)
	{
		for (int i = 0; i < row; i++)
		{
			bool flag = false;
			for (int j = col - 1; j >= 0; j--)
			{
				if (map[i][j] != 0)
				{
					int k, distance = 0, value = map[i][j];
					for (k = j + 1; k < col; k++)
					{
						if (map[i][k] == 0 || (map[i][k] == map[i][k - 1] && !flag))
						{
							if (map[i][k] == map[i][k - 1])
							{
								flag = true;
								score += (2 * value);
							}
							map[i][k] += map[i][k - 1];
							map[i][k - 1] = 0;
							distance++;
						}
					}
					gmw_move_block(pCGI, i, j, value, 0, BDI, LEFT_TO_RIGHT, distance);
					gmw_draw_block(pCGI, i, j + distance, map[i][j + distance], BDI);
					if (distance > 0)
						res++;
				}
			}
		}
	}
	return res;
}

//绘制所有的色块
int draw_numbers(const CONSOLE_GRAPHICS_INFO * const pCGI, const int map[][MaxCol], const int row, const int col)
{
	for (int i = 0; i < row; i++)
	{
		for (int j = 0; j < row; j++)
		{
			gmw_draw_block(pCGI, i, j, map[i][j], BDI);
		}
	}
	return 0;
}

//判断游戏失败
bool game_fail(const int map[][MaxCol], const int row, const int col)
{
	for (int i = 0; i < row - 1; i++)
	{
		for (int j = 0; j < col; j++)
		{
			if (map[i][j] == 0)		//有空格
				return false;
			if (i < row - 1 && map[i][j] == map[i + 1][j])	//上下数字相同
				return false;
			if (j < col - 1 && map[i][j] == map[i][j + 1])	//左右数字相同
				return false;
		}
	}
	return true;
}

//判断游戏胜利
bool game_win(const int map[][MaxCol], const int row, const int col, const int targetScore)
{
	for (int i = 0; i < row; i++)
	{
		for (int j = 0; j < col; j++)
		{
			if (map[i][j] == targetScore)
				return true;
		}
	}
	return false;
}

//输出游戏胜利或失败的信息
void after_game_win_of_false(const CONSOLE_GRAPHICS_INFO * const pCGI, const bool win)
{
	const char str[2][5] = { "失败", "成功" };
	showstr(pCGI->cols / 2 - 6, pCGI->lines / 2 - 4, "╔════╗", COLOR_PINK, COLOR_WHITE);	//第一行
	showstr(pCGI->cols / 2 - 6, pCGI->lines / 2 - 3, "║游戏", COLOR_PINK, COLOR_WHITE);		//第二行
	showstr(pCGI->cols / 2 - 6 + 6, pCGI->lines / 2 - 3, str[!!win], COLOR_PINK, COLOR_WHITE);	//成功或失败
	showstr(pCGI->cols / 2 - 6 + 6 + sizeof(str[!!win]) - 1, pCGI->lines / 2 - 3, "║", COLOR_PINK, COLOR_WHITE);
	showstr(pCGI->cols / 2 - 6, pCGI->lines / 2 - 2, "║", COLOR_PINK, COLOR_WHITE);			//第三行
	showch (pCGI->cols / 2 - 6 + 2, pCGI->lines / 2 - 2, ButtonRestart, COLOR_PINK, COLOR_WHITE);
	showstr(pCGI->cols / 2 - 6 + 3, pCGI->lines / 2 - 2, "：再玩 ║", COLOR_PINK, COLOR_WHITE);
	showstr(pCGI->cols / 2 - 6, pCGI->lines / 2 - 1, "║", COLOR_PINK, COLOR_WHITE);			//第四行
	showch (pCGI->cols / 2 - 6 + 2, pCGI->lines / 2 - 1, ButtonQuit, COLOR_PINK, COLOR_WHITE);
	showstr(pCGI->cols / 2 - 6 + 3, pCGI->lines / 2 - 1, "：退出 ║", COLOR_PINK, COLOR_WHITE);
	showstr(pCGI->cols / 2 - 6, pCGI->lines / 2, "╚════╝", COLOR_PINK, COLOR_WHITE);		//第五行
}

//更新上状态栏的信息
int update_top_status_message(char message[], const int blockLength, const int targetScore, const int score, const int hiScore, const int time)
{
	int cur = 0;
	for (int i = 0; i < blockLength; i++)	//添加blockLength个空格
		message[cur++] = ' ';
	cur += sprintf(&message[cur], "目标:%d 分数:%d 最高:%d 时间:%d (%c:重玩 %c:退出)", targetScore, score, hiScore, time, ButtonRestart, ButtonQuit);	//添加信息
	message[cur] = '\0';					//添加尾零
	return cur;
}

//记录最高分信息
int load_hiScore(int hiScoreList[][MaxCol - MinCol + 1], const int row, const int col)
{
	memset(hiScoreList, 0, sizeof(int) * (MaxRow - MinRow + 1) * (MaxCol - MinCol + 1));	//重置hiScoreList
	ifstream in;
	in.open(SaveFileName, ios::in);
	if (!in.is_open())
		return 0;
	int num, ret = 0, i = 0, j = 0;
	while (!in.eof())	//循环到文件尾
	{
		in >> i >> j >> num;	//输入三个数字
		if (in.eof())
			break;
		if (!in.good())			//如果有非法输入，则放弃这一行信息
		{
			in.clear();
			in.ignore(9999, '\n');
			continue;
		}
		if (i >= MinRow && i <= MaxRow && j >= MinCol && j <= MaxCol)	//如果行数和列数在范围之内
		{
			hiScoreList[i - MinRow][j - MinCol] = max(num, 0);
		}
		if (i == row && j == col)	//如果找到了需要的行列地图，记录最高分数
			ret = num;
	}
	in.close();
	return ret;
}

//存储最高分信息
bool save_hiScore(const int hiScoreList[][MaxCol - MinCol + 1])
{
	ofstream out;
	out.open(SaveFileName, ios::out);
	if (!out.is_open())
		return false;
	for (int i = MinRow; i <= MaxRow; i++)			//循环最小行到最大行
	{
		for (int j = MinCol; j <= MaxCol; j++)		//循环最小列到最大列
		{
			out << i << " " << j << " " << hiScoreList[i - MinRow][j - MinCol] << endl;		//写入信息
		}
	}
	out.close();
	return true;
}
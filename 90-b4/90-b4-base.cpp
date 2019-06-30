//1850059 ��1�� ��־Զ

#include "90-b4.h"

//��ʼ����������һ�����֣�offsetX��offsetY������ʾ�İ�����Ϣ��
int input_number(const int helpInfoLength, const int offsetX, const int offsetY)
{
	const int maxLength = 10;
	while (1)
	{
		char ch, temp[maxLength + 5] = "";
		int pos = 0, res;
		showch(offsetX + helpInfoLength, offsetY, ' ', COLOR_WHITE, COLOR_BLACK, maxLength);	//�ÿո����һ��
		gotoxy(offsetX + helpInfoLength, offsetY);	//�����ʾ��Ϣ
		while ((ch = _getch()))	//�����ַ�
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
		if (temp[0] > '0' || temp[0] < '9')	//�ж��Ƿ�������
		{
			res = atoi(temp);	//ת������
			return res;			//����ת��������
		}
	}
}

//���������
int create_new_number(int map[][MaxCol], const int row, const int col, int &x, int &y)
{
	const int num[] = { 2, 4 };		//������ӵ�����
	do
	{
		x = rand() % col;
		y = rand() % row;
	} while (map[y][x] != 0);		//�ҵ�һ���ո�λ��
	map[y][x] = num[rand() % (sizeof(num) / sizeof(num[0]))];	//�������
	return map[y][x];
}

//��ĳ�������ƶ�������ͼ�������ƶ�����
int move_map(const CONSOLE_GRAPHICS_INFO * const pCGI, int map[][MaxCol], const int row, const int col, const int keycode, int &score)
{
	int res = 0;
	if (keycode == KB_ARROW_UP)				//�����ƶ�
	{
		for (int j = 0; j < col; j++)		//����ÿһ��
		{
			bool flag = false;		//��֤ÿһ���������ֻ�ϲ�һ��
			for (int i = 0; i < row; i++)	//����ÿһ��
			{
				if (map[i][j] != 0)			//�ҵ������ֵ�λ��
				{
					int k, distance = 0, value = map[i][j];	//��¼��ǰ��ֵ���ƶ��ľ���
					for (k = i - 1; k >= 0; k--)			//�����ӵ�ǰ�е����ÿ������
					{
						if (map[k][j] == 0 || (map[k][j] == map[k + 1][j] && !flag))	//��������ƶ���ϲ�
						{
							if (map[k][j] == map[k + 1][j])		//������Ժϲ�
							{
								flag = true;			//��¼�Ѻϲ�
								score += (2 * value);	//�������
							}
							map[k][j] += map[k + 1][j];	//�ϲ�����
							map[k + 1][j] = 0;			//��ǰλ�ü�Ϊ0
							distance++;					//��¼����
						}
					}
					gmw_move_block(pCGI, i, j, value, 0, BDI, DOWN_TO_UP, distance);	//�ƶ��Ķ���
					gmw_draw_block(pCGI, i - distance, j, map[i - distance][j], BDI);	//���Ƶ�ǰλ�õ�ɫ��
					if (distance > 0)	//����������ƶ������¼����
						res++;
				}
			}
		}
	}
	//���µĲ������ƣ���������ѭ����Χ��˳�򣬺���д��ͬ���ĺ���
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

//�������е�ɫ��
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

//�ж���Ϸʧ��
bool game_fail(const int map[][MaxCol], const int row, const int col)
{
	for (int i = 0; i < row - 1; i++)
	{
		for (int j = 0; j < col; j++)
		{
			if (map[i][j] == 0)		//�пո�
				return false;
			if (i < row - 1 && map[i][j] == map[i + 1][j])	//����������ͬ
				return false;
			if (j < col - 1 && map[i][j] == map[i][j + 1])	//����������ͬ
				return false;
		}
	}
	return true;
}

//�ж���Ϸʤ��
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

//�����Ϸʤ����ʧ�ܵ���Ϣ
void after_game_win_of_false(const CONSOLE_GRAPHICS_INFO * const pCGI, const bool win)
{
	const char str[2][5] = { "ʧ��", "�ɹ�" };
	showstr(pCGI->cols / 2 - 6, pCGI->lines / 2 - 4, "�X�T�T�T�T�[", COLOR_PINK, COLOR_WHITE);	//��һ��
	showstr(pCGI->cols / 2 - 6, pCGI->lines / 2 - 3, "�U��Ϸ", COLOR_PINK, COLOR_WHITE);		//�ڶ���
	showstr(pCGI->cols / 2 - 6 + 6, pCGI->lines / 2 - 3, str[!!win], COLOR_PINK, COLOR_WHITE);	//�ɹ���ʧ��
	showstr(pCGI->cols / 2 - 6 + 6 + sizeof(str[!!win]) - 1, pCGI->lines / 2 - 3, "�U", COLOR_PINK, COLOR_WHITE);
	showstr(pCGI->cols / 2 - 6, pCGI->lines / 2 - 2, "�U", COLOR_PINK, COLOR_WHITE);			//������
	showch (pCGI->cols / 2 - 6 + 2, pCGI->lines / 2 - 2, ButtonRestart, COLOR_PINK, COLOR_WHITE);
	showstr(pCGI->cols / 2 - 6 + 3, pCGI->lines / 2 - 2, "������ �U", COLOR_PINK, COLOR_WHITE);
	showstr(pCGI->cols / 2 - 6, pCGI->lines / 2 - 1, "�U", COLOR_PINK, COLOR_WHITE);			//������
	showch (pCGI->cols / 2 - 6 + 2, pCGI->lines / 2 - 1, ButtonQuit, COLOR_PINK, COLOR_WHITE);
	showstr(pCGI->cols / 2 - 6 + 3, pCGI->lines / 2 - 1, "���˳� �U", COLOR_PINK, COLOR_WHITE);
	showstr(pCGI->cols / 2 - 6, pCGI->lines / 2, "�^�T�T�T�T�a", COLOR_PINK, COLOR_WHITE);		//������
}

//������״̬������Ϣ
int update_top_status_message(char message[], const int blockLength, const int targetScore, const int score, const int hiScore, const int time)
{
	int cur = 0;
	for (int i = 0; i < blockLength; i++)	//���blockLength���ո�
		message[cur++] = ' ';
	cur += sprintf(&message[cur], "Ŀ��:%d ����:%d ���:%d ʱ��:%d (%c:���� %c:�˳�)", targetScore, score, hiScore, time, ButtonRestart, ButtonQuit);	//�����Ϣ
	message[cur] = '\0';					//���β��
	return cur;
}

//��¼��߷���Ϣ
int load_hiScore(int hiScoreList[][MaxCol - MinCol + 1], const int row, const int col)
{
	memset(hiScoreList, 0, sizeof(int) * (MaxRow - MinRow + 1) * (MaxCol - MinCol + 1));	//����hiScoreList
	ifstream in;
	in.open(SaveFileName, ios::in);
	if (!in.is_open())
		return 0;
	int num, ret = 0, i = 0, j = 0;
	while (!in.eof())	//ѭ�����ļ�β
	{
		in >> i >> j >> num;	//������������
		if (in.eof())
			break;
		if (!in.good())			//����зǷ����룬�������һ����Ϣ
		{
			in.clear();
			in.ignore(9999, '\n');
			continue;
		}
		if (i >= MinRow && i <= MaxRow && j >= MinCol && j <= MaxCol)	//��������������ڷ�Χ֮��
		{
			hiScoreList[i - MinRow][j - MinCol] = max(num, 0);
		}
		if (i == row && j == col)	//����ҵ�����Ҫ�����е�ͼ����¼��߷���
			ret = num;
	}
	in.close();
	return ret;
}

//�洢��߷���Ϣ
bool save_hiScore(const int hiScoreList[][MaxCol - MinCol + 1])
{
	ofstream out;
	out.open(SaveFileName, ios::out);
	if (!out.is_open())
		return false;
	for (int i = MinRow; i <= MaxRow; i++)			//ѭ����С�е������
	{
		for (int j = MinCol; j <= MaxCol; j++)		//ѭ����С�е������
		{
			out << i << " " << j << " " << hiScoreList[i - MinRow][j - MinCol] << endl;		//д����Ϣ
		}
	}
	out.close();
	return true;
}
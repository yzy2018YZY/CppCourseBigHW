//1850059 ��1�� ��־Զ

#include "90-b1.h"

void mode_1_2_3(int map[][DefaultMaxCol + 1], int row, int col, int erasableArray[][DefaultMaxCol + 1], int &erasableBalls, int mode)
{
	cout << endl;
	print_map_with_info(map, row, col, "��ʼ���飺", -1);
	cout << "���س�������Ѱ�ҳ�ʼ��������Ĳ���..." << endl;
	wait_enter();
	erasableBalls = erasable_count(map, row, col, erasableArray);
	while (erasableBalls)
	{
		cout << endl << "��ʼ���������ͬɫ��ʶ����" << endl;
		print_map(map, row, col, 2, erasableArray);
		if (mode == tolower(ModeInfomation[0]))
			return;
		cout << "���س����������������0����..." << endl;
		wait_enter();
		cout << endl << "�����0�������(��ͬɫ��ʶ)��" << endl;
		erase_balls(NULL, map, row, col, erasableArray, false);
		print_map(map, row, col, 3);
		cout << "���س���������ֵ���..." << endl;
		wait_enter();
		cout << endl << "��ֵ���������(��ͬɫ��ʶ)��" << endl;
		fill_blanks(NULL, map, row, col, erasableBalls, false);
		print_map(map, row, col, 2, erasableArray);
		erasableBalls = erasable_count(map, row, col, erasableArray);
	}
	cout << "��ʼ���޿�������" << endl;
	if (mode == tolower(ModeInfomation[2]))
	{
		cout << endl << "��ѡ���������ʾ����ͬɫ��ʶ����" << endl;
		mark_exchangable_balls(NULL, map, row, col, erasableArray, false);
		print_map(map, row, col, 2, erasableArray);
	}
}

void mode_9(int map[][DefaultMaxCol + 1], int erasableArray[][DefaultMaxCol + 1])
{
	setconsoleborder(80, 45, 80, 5000);
	int no, row, col;
	char fileName1[50] = "test\\", fileName2[50] = "test\\", tempStr[50];
	ifstream fin1, fin2;
	cout << "������ѧ��(���� 1859999 ��Ӧ��� test\\1859999.dat �ļ�)" << endl;
	while (!(cin >> no) || no < 1000000 || no > 1859999)
	{
		cout << HelpInformation[0] << endl;
		cin.clear();
		cin.ignore(9999, '\n');
	}
	strcat(fileName1, _itoa(no, tempStr, 10));
	strcpy(fileName2, fileName1);
	strcat(fileName1, ".dat");
	strcat(fileName2, ".ans");
	fin1.open(fileName1, ios::in);
	fin2.open(fileName2, ios::in);
	if (fin1.is_open() == 0 || fin2.is_open() == 0)
	{
		cout << "���ļ�ʧ��" << endl;
		return;
	}
	fin1 >> row >> col;
	cout << endl;
	for (int i = 0; i < row; i++)
		for (int j = 0; j < col; j++)
			fin1 >> map[i][j];
	print_map_with_info(map, row, col, "��ʼ���飺", -1);
	mark_exchangable_balls(NULL, map, row, col, erasableArray, false);
	cout << endl;
	print_map_with_info(map, row, col, "��ѡ���������ʾ����ͬɫ��ʶ����", 2, erasableArray);
	memset(erasableArray, 0, sizeof(int) * (DefaultMaxCol + 1) * (DefaultMaxCol + 1));
	for (int i = 0; i < row; i++)
		for (int j = 0; j < col; j++)
		{
			int temp;
			fin2 >> temp;
			if (temp > 90)
				erasableArray[i][j] = temp - 90;
			map[i][j] = (temp > 90) ? temp - 90 : temp;
		}
	print_map_with_info(map, row, col, "���ļ������ݣ�", 2, erasableArray);
	fin1.close();
	fin2.close();
}
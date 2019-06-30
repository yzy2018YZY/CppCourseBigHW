//1850059 计1班 杨志远

#include "90-b1.h"

void mode_1_2_3(int map[][DefaultMaxCol + 1], int row, int col, int erasableArray[][DefaultMaxCol + 1], int &erasableBalls, int mode)
{
	cout << endl;
	print_map_with_info(map, row, col, "初始数组：", -1);
	cout << "按回车键进行寻找初始可消除项的操作..." << endl;
	wait_enter();
	erasableBalls = erasable_count(map, row, col, erasableArray);
	while (erasableBalls)
	{
		cout << endl << "初始可消除项（不同色标识）：" << endl;
		print_map(map, row, col, 2, erasableArray);
		if (mode == tolower(ModeInfomation[0]))
			return;
		cout << "按回车键进行数组下落除0操作..." << endl;
		wait_enter();
		cout << endl << "下落除0后的数组(不同色标识)：" << endl;
		erase_balls(NULL, map, row, col, erasableArray, false);
		print_map(map, row, col, 3);
		cout << "按回车键进行新值填充..." << endl;
		wait_enter();
		cout << endl << "新值填充后的数组(不同色标识)：" << endl;
		fill_blanks(NULL, map, row, col, erasableBalls, false);
		print_map(map, row, col, 2, erasableArray);
		erasableBalls = erasable_count(map, row, col, erasableArray);
	}
	cout << "初始已无可消除项" << endl;
	if (mode == tolower(ModeInfomation[2]))
	{
		cout << endl << "可选择的消除提示（不同色标识）：" << endl;
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
	cout << "请输入学号(输入 1859999 对应会打开 test\\1859999.dat 文件)" << endl;
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
		cout << "打开文件失败" << endl;
		return;
	}
	fin1 >> row >> col;
	cout << endl;
	for (int i = 0; i < row; i++)
		for (int j = 0; j < col; j++)
			fin1 >> map[i][j];
	print_map_with_info(map, row, col, "初始数组：", -1);
	mark_exchangable_balls(NULL, map, row, col, erasableArray, false);
	cout << endl;
	print_map_with_info(map, row, col, "可选择的消除提示（不同色标识）：", 2, erasableArray);
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
	print_map_with_info(map, row, col, "答案文件的内容：", 2, erasableArray);
	fin1.close();
	fin2.close();
}
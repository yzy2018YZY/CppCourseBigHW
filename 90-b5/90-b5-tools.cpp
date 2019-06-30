//1850059 计1班 杨志远

#include "90-b5.h"

//static void input_row_col(const char *message, char &row, int &col)
//{
//	while (1)
//	{
//		cout << message;
//		cin >> row >> col;
//		if (row >= 'a' && row <= 'j')
//			row -= 32;
//		if (!cin.good())
//		{
//			cin.clear();
//			cin.ignore(999999, '\n');
//			continue;
//		}
//		break;
//	}
//}

/***************************************************************************
  函数名称：
  功    能：将学号+MD5(password)再用security_str按位异或后形成要发送的“报道”数据
  输入参数：char *reg_str				：异或后需要发送的串（返回）
			const char *stu_no			：学号
			const char *stu_password	：交作业系统中的口令（明码），无论口令多长，转换为MD5后为32字节的hex串
			const char *security_str	：从Server端收到的异或加密串
  返 回 值：
  说    明：以学号 : 1859999 , 密码 : Password，异或串 : f272a9b7422ee1ddec6c4b1abe758cadefc658c2 为例
			1、Password     ： => MD5 => “dc647eb65e6711e155375218212b3964”
			2、认证串(原始) ：1859999+dc647eb65e6711e155375218212b3964
			3、认证串(原始)和异或串进行按位异或
				   1859999+dc647eb65e6711e155375218212b3964
				   f272a9b7422ee1ddec6c4b1abe758cadefc658c2
			   进行按位异或，结果不保证一定是图形ASCII字符，所以再转为hex
			   认证串(Hex发送) ：570a020b58005b1c50510451525406525006005405535450575004020d51505c5757515406015506
			   解释：第1个字节 1和f，即 0x31^0x66 = 0x57
					 第2个字节 8和2，即 0x38^0x32 = 0x0a
					 ...(略)
***************************************************************************/
int cmd_tcp_socket::make_register_string(char *send_regstr, const char *stu_no, const char *stu_password, const char *security_str)
{
	static int count = 0;	//52次循环的计数
	char passwordMD5[65] = "", buf[65] = "", dest[65] = "", connector[2] = "";
	connector[0] = (Connection[0] == ' ' ? ConnectorString[count] : Connection[0]);
	count = (count + 1) % strlen(ConnectorString);
	MD5(passwordMD5, stu_password);
	strcpy(buf, stu_no);
	strcat(buf, connector);
	strcat(buf, passwordMD5);
	strcpy(dest, buf);
	for (int i = 0; i < 40; i++)
	{
		dest[i] = dest[i] ^ security_str[i];
		send_regstr[2 * i] = hex_to_char((dest[i] & 0xf0) >> 4);
		send_regstr[2 * i + 1] = hex_to_char(dest[i] & 0x0f);
	}
	send_regstr[80] = '\0';
	if (debug_switch)
	{
		cout << "口令(加密后)：" << passwordMD5 << endl;
		cout << "认证串(原始)：" << buf << endl;
		cout << "异或串      ：" << security_str << endl;
		cout << "认证串(Hex发送) ：" << send_regstr << endl;
	}
	return 0;
}

int get_gameprogress_string_single(cmd_tcp_socket &client, string &s)
{
	int ret = 0;
	client.get_gameprogress_string(s);
	if (s == "InvalidCoordinate")
	{
		ret = -10;
	}
	if (s == "HitFail")
	{
		ret = -1;
	}
	if (s == "HitBody")
	{
		ret = 1;
	}
	if (s == "HitHead")
	{
		ret = 2;
	}
	if (s == "HitRepeat")
	{
		ret = -10;
	}
	if (s == "StartGame")
	{
		ret = 0;
	}
	return ret;
}

int get_gameprogress_string_double(cmd_tcp_socket &client, string &s)
{
	int ret = 0;
	client.recv_from_server(s);
	if ((int)s.find("InvalidCoordinate") >= 0)
	{
		s = "InvalidCoordinate";
		ret = -10;
	}
	if ((int)s.find("HitFail") >= 0)
	{
		s = "HitFail";
		ret = -3;
	}
	if ((int)s.find("HitPlane") >= 0)
	{
		s = "HitPlane";
		ret = 9;
	}
	if ((int)s.find("HitRepeat") >= 0)
	{
		s = "HitRepeat";
		ret = -10;
	}
	if ((int)s.find("StartGame") >= 0)
	{
		s = "StartGame";
		ret = 0;
	}
	if ((int)s.find("GameOver") >= 0)
	{
		client.score = atoi(&s.c_str()[(int)s.find("FinalScore") + strlen("FinalScore = ")]);
		strcpy(client.gameid, (&s.c_str()[(int)s.find("GameID") + strlen("GameID = ")]));
		s = "GameOver";
		ret = 9;
	}
	return ret;
}

//demo测试专用
//int cmd_tcp_socket::send_checked_coordinate(const char row, const int col)
//{
//	if (row < 'A' || row >= 'A' + RowNum || col < 0 || col >= ColNum)
//	{
//		gotoxy(0, 25);
//		cout << "  " << (int)row << "  " << col << "  ";
//		//system("pause");
//	}
//	int innerRow = row;
//	if (innerRow >= 0 && innerRow < RowNum)
//		innerRow = row + 'A';
//	return send_coordinate(innerRow, col);
//}

//demo测试专用
//int game_progress(cmd_tcp_socket &client)
//{
//	char sel;
//	char row, head_row, tail_row;
//	int col, head_col, tail_col, score = 0;
//	bool recv_startgame = false;
//
//	while (1)
//	{
//		/* 等待Server端的gameprogress */
//		string spack;
//		score += client.get_gameprogress_string_single(spack);
//		cout << "Server应答 : " << spack << endl;
//		if (spack == "StartGame")
//			recv_startgame = true;
//
//		/* 没收到StartGame前所有其他信息均认为错误 */
//		if (!recv_startgame)
//			return -1;
//
//		if (spack == "GameOver")
//			return 0;
//
//		cout << "1.传送一个坐标" << endl;
//		cout << "2.传送整架飞机坐标" << endl;
//		while (1)
//		{
//			sel = _getch();
//			if (sel == '1' || sel == '2')
//				break;
//		}
//		switch (sel)
//		{
//			case '1':
//				input_row_col("请输入行(A-J)列(0-9)坐标 : ", row, col);
//				client.send_coordinate(row, col);
//				break;
//			case '2':
//				input_row_col("请输入机头行(A-J)列(0-9)坐标 : ", head_row, head_col);
//				input_row_col("请输入机尾正中行(A-J)列(0-9)坐标 : ", tail_row, tail_col);
//				client.send_plane_coordinates(head_row, head_col, tail_row, tail_col);
//				break;
//		}//end of switch
//	}//end of while(1)
//}

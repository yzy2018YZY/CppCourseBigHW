//1850059 ��1�� ��־Զ

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
  �������ƣ�
  ��    �ܣ���ѧ��+MD5(password)����security_str��λ�����γ�Ҫ���͵ġ�����������
  ���������char *reg_str				��������Ҫ���͵Ĵ������أ�
			const char *stu_no			��ѧ��
			const char *stu_password	������ҵϵͳ�еĿ�����룩�����ۿ���೤��ת��ΪMD5��Ϊ32�ֽڵ�hex��
			const char *security_str	����Server���յ��������ܴ�
  �� �� ֵ��
  ˵    ������ѧ�� : 1859999 , ���� : Password����� : f272a9b7422ee1ddec6c4b1abe758cadefc658c2 Ϊ��
			1��Password     �� => MD5 => ��dc647eb65e6711e155375218212b3964��
			2����֤��(ԭʼ) ��1859999+dc647eb65e6711e155375218212b3964
			3����֤��(ԭʼ)����򴮽��а�λ���
				   1859999+dc647eb65e6711e155375218212b3964
				   f272a9b7422ee1ddec6c4b1abe758cadefc658c2
			   ���а�λ��򣬽������֤һ����ͼ��ASCII�ַ���������תΪhex
			   ��֤��(Hex����) ��570a020b58005b1c50510451525406525006005405535450575004020d51505c5757515406015506
			   ���ͣ���1���ֽ� 1��f���� 0x31^0x66 = 0x57
					 ��2���ֽ� 8��2���� 0x38^0x32 = 0x0a
					 ...(��)
***************************************************************************/
int cmd_tcp_socket::make_register_string(char *send_regstr, const char *stu_no, const char *stu_password, const char *security_str)
{
	static int count = 0;	//52��ѭ���ļ���
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
		cout << "����(���ܺ�)��" << passwordMD5 << endl;
		cout << "��֤��(ԭʼ)��" << buf << endl;
		cout << "���      ��" << security_str << endl;
		cout << "��֤��(Hex����) ��" << send_regstr << endl;
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

//demo����ר��
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

//demo����ר��
//int game_progress(cmd_tcp_socket &client)
//{
//	char sel;
//	char row, head_row, tail_row;
//	int col, head_col, tail_col, score = 0;
//	bool recv_startgame = false;
//
//	while (1)
//	{
//		/* �ȴ�Server�˵�gameprogress */
//		string spack;
//		score += client.get_gameprogress_string_single(spack);
//		cout << "ServerӦ�� : " << spack << endl;
//		if (spack == "StartGame")
//			recv_startgame = true;
//
//		/* û�յ�StartGameǰ����������Ϣ����Ϊ���� */
//		if (!recv_startgame)
//			return -1;
//
//		if (spack == "GameOver")
//			return 0;
//
//		cout << "1.����һ������" << endl;
//		cout << "2.�������ܷɻ�����" << endl;
//		while (1)
//		{
//			sel = _getch();
//			if (sel == '1' || sel == '2')
//				break;
//		}
//		switch (sel)
//		{
//			case '1':
//				input_row_col("��������(A-J)��(0-9)���� : ", row, col);
//				client.send_coordinate(row, col);
//				break;
//			case '2':
//				input_row_col("�������ͷ��(A-J)��(0-9)���� : ", head_row, head_col);
//				input_row_col("�������β������(A-J)��(0-9)���� : ", tail_row, tail_col);
//				client.send_plane_coordinates(head_row, head_col, tail_row, tail_col);
//				break;
//		}//end of switch
//	}//end of while(1)
//}

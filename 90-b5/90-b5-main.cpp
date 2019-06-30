//1850059 ��1�� ��־Զ

#include "90-b5.h"

static void game(bool autoMode, const char *strNo, const char *strPassword)
{
	bool first = true;
	cmd_tcp_socket client;
	client.set_debug_switch(false);	//��client������е�debug���أ�����ʱ�ɴ򿪣���ͼ�ν���ʱ��رգ�
	while (1)
	{
		if (!first)		//���κδ�����ʱ5����������������һ�Σ�
		{
			cout << "������������ӶϿ�!" << endl;
			Sleep(DelayRetry);
		}
		else
			first = false;
		if (client.connect() < 0)	//����ʧ����Ϣ����û��debug_switch����ӡ
		{
			cout << "���ӷ�����ʧ��!" << endl;
			continue;
		}
		string s1;
		if (client.get_security_string(s1) < 0)	//��ȡ���������͹����ļ��ܴ���֤��(��40���ַ�)
		{
			client.close();
			continue;
		}
		char reg_str[81];
		client.make_register_string(reg_str, strNo, strPassword, s1.c_str()); //���Լ���ѧ�ź�����
		client.send_register_string(reg_str);	//����֤�����͹�ȥ
		/* ������Ϸ��������
		   1�������յ�Server��StartGame�������շ�����
		   2���յ�Server��GameOver�򷵻�0����Ϸ����
		   3���������������-1��������Ϣ����ȷ�ȴ��󣩣��������ٴ��ظ�	*/
		if (game_start(client, autoMode) < 0)
		{
			client.close();
			continue;
		}
		else //game_progressֻ���յ�GameOver�ŷ���0
		{
			if (game_over(autoMode) > 0)
			{
				first = true;
				continue;
			}
			break;
		}
	};
	client.close();
}

int main(int argc, char **argv)
{
	srand((unsigned int)time(0));
	if (argc != 4) 
	{
		cout << "Usage : " << argv[0] << " -auto|-manual  stu_no  stu_pwd" << endl;
		return -1;
	}
	bool autoMode = (strcmp(argv[1], "-auto") == 0), manualMode = (strcmp(argv[1], "-manual") == 0);
	if (!is_stu_no(argv[2]))
	{
		cout << "ѧ��" << argv[2] << "����" << endl;
		return -1;
	}
	else if (manualMode)
	{
		game(autoMode, argv[2], argv[3]);
	}
	else if (autoMode)
	{
		int autoModeRepeat = (Connection[0] == ' ' ? strlen(ConnectorString) : 1);
		for (int i = 0; i < autoModeRepeat; i++)
		{
			game(autoMode, argv[2], argv[3]);
		}
	}
	else
	{
		cout << "Usage : " << argv[0] << " -auto|-manual  stu_no  stu_pwd" << endl;
		return -1;
	}
	return 0;
}

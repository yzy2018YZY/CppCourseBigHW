//1850059 计1班 杨志远

#include "90-b5.h"

static void game(bool autoMode, const char *strNo, const char *strPassword)
{
	bool first = true;
	cmd_tcp_socket client;
	client.set_debug_switch(false);	//打开client类对象中的debug开关（调试时可打开，到图形界面时需关闭）
	while (1)
	{
		if (!first)		//出任何错误，延时5秒重连（不包括第一次）
		{
			cout << "与服务器的连接断开!" << endl;
			Sleep(DelayRetry);
		}
		else
			first = false;
		if (client.connect() < 0)	//连接失败信息，有没有debug_switch都打印
		{
			cout << "连接服务器失败!" << endl;
			continue;
		}
		string s1;
		if (client.get_security_string(s1) < 0)	//读取服务器发送过来的加密串认证串(共40个字符)
		{
			client.close();
			continue;
		}
		char reg_str[81];
		client.make_register_string(reg_str, strNo, strPassword, s1.c_str()); //传自己的学号和密码
		client.send_register_string(reg_str);	//将认证串发送过去
		/* 进入游戏交互环节
		   1、必须收到Server的StartGame，才能收发后续
		   2、收到Server的GameOver则返回0，游戏结束
		   3、其它错误均返回-1（报文信息不正确等错误），重连，再次重复	*/
		if (game_start(client, autoMode) < 0)
		{
			client.close();
			continue;
		}
		else //game_progress只有收到GameOver才返回0
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
		cout << "学号" << argv[2] << "错误" << endl;
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

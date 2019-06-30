//1850059 ��1�� ��־Զ

#pragma once
#define _CRT_SECURE_NO_WARNINGS
#include "../common/cmd_console_base.h"
#include "../common/cmd_console_graphics.h"
#include "../common/cmd_hdc_tools.h"
#include <iomanip>
#include <fstream>
#include <string.h>
#include <io.h>
using namespace std;

//�޸����¹��ڴ�С��λ�ƵĲ������ܻᵼ�²���Ԥ�ϵĺ��
const int TableToBlank = 4;					//����һ��tab�ᱻת��Ϊ���ٸ��ո񣨽��鲻����8��
const int WindowXSize1 = 70;				//��ʼ���ں����С��Ĭ��70��
const int WindowYSize1 = 30;				//��ʼ���������С��Ĭ��30��
const int WindowXSize2 = 145;				//�Ķ����ں����С��Ĭ��145��
const int WindowYSize2 = 35;				//�Ķ����������С��Ĭ��35��
const int FontSize1 = 16;					//��ʼ���������С
const char Font1[] = "������";				//��ʼ��������
const int FontSize2 = 16;					//�Ķ����������С
const char Font2[] = "������";				//�Ķ���������
const int ChooseXOffset = 20;				//��ʼ�������λ�ƣ�Ĭ��20��
const int ChooseYOffset = 3;				//��ʼ��������λ�ƣ�Ĭ��3��
const int TextBoxXSize = 85;				//�Ķ������ȣ��������2��
const int TextBoxYSize = 25;				//�Ķ�����߶ȣ��������0��
const int TextBoxXOffset = 25;				//�Ķ��������λ�ƣ�Ĭ��25��
const int TextBoxYOffset = 1;				//�Ķ���������λ�ƣ�Ĭ��1��
const int TextBoxDelay1 = 25;				//�Ķ������������ӳ�
const int TextBoxDelay2 = 25;				//�Ķ����淭ҳ�ӳ�
const int TextBoxDelay3 = 200;				//�Ķ��������������ӳ�
const int MaxBookListLength = 512;			//Ĭ���鱾�б���󳤶ȣ����ļ������鱾�������Ŀ��������ڵ���MenuBookListLength��
const int MenuBookListLength = 15;			//�˵��е��鱾�б�����򳤶�
const int MenuBookListStringLength = 13;	//�˵��е��鱾�б�ĺ��򳤶�
const long MaxBookSize = 20 * 1024 * 1024;	//�鱾����С
#if defined MAX_PATH	//Windows�Դ��ĺ꣬�ļ�·���ַ�������󳤶�
const int DefaultStringLength = MAX_PATH + 10;//Ĭ���ַ�����󳤶ȣ���С��windows���·�����ȣ�
#else
const int DefaultStringLength = 270;		//Ĭ���ַ�����󳤶ȣ���С��windows���·�����ȣ�
#endif
const char ProjectName[] = "90-b3";								//��ǰ��Ŀ��
const char BookFolderName[] = "book";							//���ļ�������
const char SavingFileName[] = "position.saving";				//�浵�ļ�����
const char ConsoleTitle[] = "�ı��Ķ��� 1850059 ��1�� ��־Զ";	//��������
const char HelpInfo[] = "����������ƶ�һ�У�PgUp��PgDn��ҳ��S����ת��F���л���ʾģʽ��N�����ز˵���Q���˳�";	//�Ķ������·��İ�����Ϣ
const char HelpInfoChooseMode[] = "������л���� �س�ѡ���ļ�";	//��ʼ���������Ϣ

const char CorrectExtensiveName[][5] //������ļ���չ���������Կ��ַ�����β
{
	"txt",
	//"pdf",
	"",
};

enum PrintMode //�ı���ʾģʽ������Ҫ�Ķ�
{
	PrintModeNormal,
	PrintModeAutoEnter,
};

//90-b3-tools
int get_book_file_path(char bookPath[], const char bookName[]);
long get_file_size(const char filePath[]);
long read_index_of_book_file(const char bookFilePath[], const char bookName[], int bookSize);
bool save_index_of_book_file(const char bookFilePath[], const char bookName[], int index);

//90-b3-base
int get_book_list(char bookList[][DefaultStringLength]);
int choose_book(const char bookList[][DefaultStringLength], const int bookListLength, const int lines, const int length, const int startLine, int offsetX, const int offsetY);
int read_book_file(char bookStr[], const char bookFilePath[], const long fileSize);
int read_book(char bookStr[], const int bookSize, long &startIndex, int length, const int lines, const int offsetX, const int offsetY, const int mode);

//90-b3-graphics
void draw_graphics_init(int windowX, int windowY, int fontSize, const char font[]);
void draw_graphics();
void draw_graghics_up();
void draw_graghics_down();
void draw_graphics_F();
void draw_graphics_S();
void draw_graphics_PgUp();
void draw_graphics_PgDn();
void draw_rotate_ellipse(bool clockwise);

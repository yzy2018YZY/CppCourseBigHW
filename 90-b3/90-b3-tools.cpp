//1850059 ��1�� ��־Զ

#include "90-b3.h"

//read_index_of_book_file��save_index_of_book_file��һ���֣���ȡ�浵�ļ���·���;���ת�����ļ���
static void get_saving_file_path(const char bookFilePath[], const char bookName[], char path[], char fixedBookName[])
{
	strcpy(path, bookFilePath);
	path[strrchr(path, '\\') - path] = '\0';
	strcat(path, "\\");
	strcat(path, SavingFileName);

	strcpy(fixedBookName, "[");
	strcat(fixedBookName, bookName);
	strcat(fixedBookName, "]");
}

//��ȡ�鱾�ļ���·��
int get_book_file_path(char bookPath[], const char bookName[])
{
	if (bookPath == NULL || bookName == NULL)
		return -1;
	bookPath[0] = '\0';
	strcat(bookPath, ".\\");
	strcat(bookPath, BookFolderName);
	strcat(bookPath, "\\");
	strcat(bookPath, bookName);
	return (int)strlen(bookPath);
}

//��ȡ�ļ���Ӧ����ʼλ��
long read_index_of_book_file(const char bookFilePath[], const char bookName[], int bookSize)
{
	char path[DefaultStringLength] = "", fixedBookName[DefaultStringLength] = "", tempStr[DefaultStringLength] = "";
	long index = 0;
	get_saving_file_path(bookFilePath, bookName, path, fixedBookName);
	ifstream in;
	in.open(path, ios::in);
	if (!in.is_open()) //�ļ���ʧ��
		return 0;
	for (; !in.eof();)
	{
		in >> tempStr;
		if (strcmp(tempStr, fixedBookName) == 0) //�ҵ��˶�Ӧ���ļ�����
		{
			in >> tempStr;
			index = atol(tempStr); //��ֱ�Ӷ��룬��ֹ���ֹ�������
		}
	}
	if (index < 0 || index >= bookSize) //���ֳ�����Χ
		index = 0;
	in.close();
	return (long)index;
}

//�����ļ���Ӧ����ʼλ��
bool save_index_of_book_file(const char bookFilePath[], const char bookName[], int index)
{
	char path[DefaultStringLength] = "", fixedBookName[DefaultStringLength] = "", tempStr[DefaultStringLength] = "";
	char bookList[MaxBookListLength][DefaultStringLength];
	long indexList[MaxBookListLength] = { 0 }, bookCount = 0; //�洢�ļ���Ӧ������
	bool flag = false; //��¼�Ƿ�洢�����ļ�������
	get_saving_file_path(bookFilePath, bookName, path, fixedBookName); //�û��Զ��庯������ȡ�浵�ļ���·��path�;���ת�����ļ���fixedBookName
	fstream f(path, ios::in | ios::out); //��ȡ�ļ�
	if (f.is_open())
	{
		for (int i = 0; i < MaxBookListLength; i++)
		{ //��ֹ�洢���ļ�����
			f >> bookList[bookCount];  //��ȡ�ļ���
			f >> tempStr;
			indexList[bookCount] = atol(tempStr); //��ȡ�ļ���Ӧ������
			if (strcmp(bookList[bookCount], fixedBookName) == 0)
			{ //�ҵ��˱��ļ���Ӧ������
				indexList[bookCount] = index;
				flag = true;
			}
			if (f.eof())
				break;
			bookCount++;
		}
	}
	f.close();
	ofstream out(path, ios::out); //д���ļ�
	if (!out.is_open())
		return false;
	for (int i = 0; i < bookCount; i++) //���ļ��������ļ�����Ϣ���¼�¼
		out << bookList[i] << endl << indexList[i] << endl << endl;
	if (!flag) //���û�м�¼������ļ����Ǿͼ�¼��ĩβ
		out << fixedBookName << endl << index << endl << endl;
	out.close();
	return true;
}

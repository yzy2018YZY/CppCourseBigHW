//1850059 计1班 杨志远

#include "90-b3.h"

//read_index_of_book_file和save_index_of_book_file的一部分，获取存档文件的路径和经过转化的文件名
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

//获取书本文件的路径
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

//读取文件对应的起始位置
long read_index_of_book_file(const char bookFilePath[], const char bookName[], int bookSize)
{
	char path[DefaultStringLength] = "", fixedBookName[DefaultStringLength] = "", tempStr[DefaultStringLength] = "";
	long index = 0;
	get_saving_file_path(bookFilePath, bookName, path, fixedBookName);
	ifstream in;
	in.open(path, ios::in);
	if (!in.is_open()) //文件打开失败
		return 0;
	for (; !in.eof();)
	{
		in >> tempStr;
		if (strcmp(tempStr, fixedBookName) == 0) //找到了对应的文件数据
		{
			in >> tempStr;
			index = atol(tempStr); //不直接读入，防止数字过大而溢出
		}
	}
	if (index < 0 || index >= bookSize) //数字超出范围
		index = 0;
	in.close();
	return (long)index;
}

//保存文件对应的起始位置
bool save_index_of_book_file(const char bookFilePath[], const char bookName[], int index)
{
	char path[DefaultStringLength] = "", fixedBookName[DefaultStringLength] = "", tempStr[DefaultStringLength] = "";
	char bookList[MaxBookListLength][DefaultStringLength];
	long indexList[MaxBookListLength] = { 0 }, bookCount = 0; //存储文件对应的索引
	bool flag = false; //记录是否存储过本文件的索引
	get_saving_file_path(bookFilePath, bookName, path, fixedBookName); //用户自定义函数，获取存档文件的路径path和经过转化的文件名fixedBookName
	fstream f(path, ios::in | ios::out); //读取文件
	if (f.is_open())
	{
		for (int i = 0; i < MaxBookListLength; i++)
		{ //防止存储的文件过多
			f >> bookList[bookCount];  //获取文件名
			f >> tempStr;
			indexList[bookCount] = atol(tempStr); //获取文件对应的索引
			if (strcmp(bookList[bookCount], fixedBookName) == 0)
			{ //找到了本文件对应的数据
				indexList[bookCount] = index;
				flag = true;
			}
			if (f.eof())
				break;
			bookCount++;
		}
	}
	f.close();
	ofstream out(path, ios::out); //写入文件
	if (!out.is_open())
		return false;
	for (int i = 0; i < bookCount; i++) //将文件中其他文件的信息重新记录
		out << bookList[i] << endl << indexList[i] << endl << endl;
	if (!flag) //如果没有记录过这个文件，那就记录在末尾
		out << fixedBookName << endl << index << endl << endl;
	out.close();
	return true;
}

//1850059 ��1�� ��־Զ

#include "90-b3.h"

//main��������ѭ�����ڲ˵����Ķ�����֮��ѭ���л�
bool main_cycle()
{
	bool quit = false;
	char bookList[MaxBookListLength][DefaultStringLength], bookFilePath[DefaultStringLength] = "";
	while (!quit)
	{
		draw_graphics_init(WindowXSize1, WindowYSize1, FontSize1, Font1); //�����˵������С������
		memset(bookList, 0, sizeof(char) * MaxBookListLength * DefaultStringLength);
		int bookListLength = get_book_list(bookList); //��ȡ�鱾�б���Ϣ����¼�鱾��Ŀ
		int bookChosed = choose_book(bookList, bookListLength, MenuBookListLength, MenuBookListStringLength, 0, ChooseXOffset, ChooseYOffset); //ѡ���鱾
		if (bookChosed < 0) //��ȡ�鱾�б�������
			return false;
		get_book_file_path(bookFilePath, bookList[bookChosed]); //��ȡ��ǰ�ļ���·��
		long fileSize = get_file_size(bookFilePath); //��ȡ��ǰ�ļ��Ĵ�С
		char * bookFileStr = new char[fileSize + 50]; //����һ���洢�����ļ����ݵĳ����ַ�������read_book�������ͷ�
		gotoxy(0, 0); //�����ǰ�ļ���Ϣ
		draw_graphics_init(WindowXSize2, WindowYSize2, FontSize2, Font2); //�����Ķ������С������
		cout << "��ǰ�����Ķ���" << bookList[bookChosed] << "  �ļ���С��" << setiosflags(ios::fixed) << setprecision(4) << (double)(fileSize) / 1024 / 1024 << "MB";
		read_book_file(bookFileStr, bookFilePath, fileSize + 10);
		long startIndex = read_index_of_book_file(bookFilePath, bookList[bookChosed], fileSize);
		if (read_book(bookFileStr, fileSize, startIndex, TextBoxXSize, TextBoxYSize, TextBoxXOffset, TextBoxYOffset, PrintModeNormal) == 0)
			quit = true;
		if (!save_index_of_book_file(bookFilePath, bookList[bookChosed], startIndex))
			cout << "����ʧ��" << endl;
	}
	return true;
}

int main()
{
	setconsoletitle(ConsoleTitle);
	main_cycle();
	gotoxy(0, 0);
	return 0;
}

//1850059 计1班 杨志远

#include "90-b3.h"

//main函数的主循环，在菜单和阅读界面之间循环切换
bool main_cycle()
{
	bool quit = false;
	char bookList[MaxBookListLength][DefaultStringLength], bookFilePath[DefaultStringLength] = "";
	while (!quit)
	{
		draw_graphics_init(WindowXSize1, WindowYSize1, FontSize1, Font1); //调整菜单界面大小与字体
		memset(bookList, 0, sizeof(char) * MaxBookListLength * DefaultStringLength);
		int bookListLength = get_book_list(bookList); //获取书本列表信息，记录书本数目
		int bookChosed = choose_book(bookList, bookListLength, MenuBookListLength, MenuBookListStringLength, 0, ChooseXOffset, ChooseYOffset); //选择书本
		if (bookChosed < 0) //读取书本列表发生错误
			return false;
		get_book_file_path(bookFilePath, bookList[bookChosed]); //获取当前文件的路径
		long fileSize = get_file_size(bookFilePath); //获取当前文件的大小
		char * bookFileStr = new char[fileSize + 50]; //建立一个存储整个文件内容的超大字符串，在read_book函数中释放
		gotoxy(0, 0); //输出当前文件信息
		draw_graphics_init(WindowXSize2, WindowYSize2, FontSize2, Font2); //调整阅读界面大小与字体
		cout << "当前正在阅读：" << bookList[bookChosed] << "  文件大小：" << setiosflags(ios::fixed) << setprecision(4) << (double)(fileSize) / 1024 / 1024 << "MB";
		read_book_file(bookFileStr, bookFilePath, fileSize + 10);
		long startIndex = read_index_of_book_file(bookFilePath, bookList[bookChosed], fileSize);
		if (read_book(bookFileStr, fileSize, startIndex, TextBoxXSize, TextBoxYSize, TextBoxXOffset, TextBoxYOffset, PrintModeNormal) == 0)
			quit = true;
		if (!save_index_of_book_file(bookFilePath, bookList[bookChosed], startIndex))
			cout << "保存失败" << endl;
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

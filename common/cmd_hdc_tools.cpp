//1850059 计1班 杨志远

#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <Windows.h>

/* 允许添加需要的头文件 */

#include "cmd_hdc_tools.h"
using namespace std;

extern "C" WINBASEAPI HWND WINAPI GetConsoleWindow();

/* 已定义的静态全局变量 */
static HWND hWnd = GetConsoleWindow();
static HDC hdc = NULL;
static const double PI = 3.14159;
static int _BgColor_, _FgColor_, _Width_, _High;

/* 允许适度使用静态全局变量，但仅限static，即外部不可见 */

/* 此处允许添加自定义函数，但仅限static，即外部不可见 */

static inline void hdc_base_line(const int x1, const int y1, const int x2, const int y2);

template <typename T>
static T my_max(T a, T b)
{
	return (a > b) ? a : b;
}

template <typename T>
static T my_min(T a, T b)
{
	return (a < b) ? a : b;
}

template <typename T>
static void my_exchange(T &a, T &b)
{
	T temp = a;
	a = b;
	b = temp;
}

//画水平直线，比直接hdc_line效率更高
static void hdc_horizonal_line(const int x1, const int x2, const int y, const int thickness = 2, const int RGB_value = INVALID_RGB)
{
	int th = max(thickness / 2, 1);

	if (RGB_value != INVALID_RGB)
		hdc_set_pencolor(RGB_value);

	for (int i = -th; i <= th; i++)
	{
		hdc_base_line(x1 - 1, y + i - 1, x2 - 1, y + i - 1);
	}
}

//计算二维空间的叉积的正负（计算数值容易溢出且意义不大）
static int cross_product_2D(int x_a, int y_a, int x_b, int y_b)
{
	double temp = x_a * y_b - x_b * y_a;
	if (fabs(temp) < 1e-6)
		return 0;
	else if (temp > 0)
		return 1;
	else
		return -1;
}

//计算二维空间的点积的正负（计算数值容易溢出且意义不大）
static int dot_product_2D(int x_a, int y_a, int x_b, int y_b)
{
	double temp = x_a * x_b + y_b * y_a;
	if (fabs(temp) < 1e-6)
		return 0;
	else if (temp > 0)
		return 1;
	else
		return -1;
}

//自定义角度转换为系统角度 mode: 0->默认  1->矩形  2->弧形  3->扇形  4->椭圆
enum MyAngleMode
{
	MyAngleModeRect = 1,
	MyAngleModeArc,
	MyAngleModeSector,
	MyAngleModeEllipse,
	MyAngleModeMathematicalCurve,
};
static double my_angle(double angle, int mode)
{
	switch (mode)
	{
		case MyAngleModeArc:
			return angle - 90;
		case MyAngleModeSector:
			return 90 - angle;
		case MyAngleModeEllipse:
			return 0 + angle;
		default:
			return angle;
	}
}

//坐标值转换为数学直角坐标系下的角度
static double coor_to_angle(double x, double y)
{
	if (y == 0)
		return (x >= 0) ? 0 : 180;
	if (x == 0)
		return (y >= 0) ? 90 : 270;
	if (x > 0)
		return atan(y / x) * 180 / PI;
	if (x < 0)
		return atan(y / x) * 180 / PI + 180;
	return 0;
}

//计算绕圆心旋转一定角度后的坐标，x与y是相对于圆心的坐标，changeAngleMode是my_angle函数的参数，tranverse代表是否要反向求值
static void rotate_coordinate(const int x, const int y, const int rotationAngles, int &resultX, int &resultY, const int changeAngleMode = 0, const bool tranverse = false)
{
	double distance = sqrt(x * x + y * y);
	double angle = my_angle((!tranverse) ? (atan((double)y / x) * 180 / PI + rotationAngles) : (atan(-(double)y / x) * 180 / PI + 180 + rotationAngles), changeAngleMode);
	resultX = (int)(distance * cos(angle * PI / 180));
	resultY = (int)(distance * sin(angle * PI / 180));
}

//画椭圆函数的一部分，负责根据椭圆上的坐标值计算旋转之后的坐标值
static void part_of_ellipse(int &x, int &y, const int radius_a, const int radius_b, const int rotation_angles, int &resultX1, int &resultY1, int &resultX2, int &resultY2, bool calcX)
{
	if (calcX)
		x = (int)(radius_a * sqrt(1 - (double)(y * y) / double(radius_b * radius_b)));
	else
		y = (int)(radius_b * sqrt(1 - (double)(x * x) / double(radius_a * radius_a)));
	double distance = sqrt(x * x + y * y);
	rotate_coordinate(x, y, rotation_angles, resultX1, resultY1, MyAngleModeEllipse, false);
	rotate_coordinate(x, y, rotation_angles, resultX2, resultY2, MyAngleModeEllipse, true);
}

/* 下面给出了几个基本函数的完整实现，不要改动 */
/***************************************************************************
  函数名称：
  功    能：初始化
  输入参数：const int bgcolor：背景色
			const int fgcolor：前景色
			const int width  ：屏幕宽度（点阵）
			const int high   ：屏幕高度（点阵）
  返 回 值：
  说    明：
***************************************************************************/
void hdc_init(const int bgcolor, const int fgcolor, const int width, const int high, const int fontSize, const char font[])
{
	/* 先释放，防止不release而再次init（hdc_release可重入） */
	hdc_release();

	/* 窗口init后，用一个静态全局量记录，后续hdc_cls()会用到 */
	_BgColor_ = bgcolor;
	_FgColor_ = fgcolor;
	_Width_ = width;
	_High = high;

	hdc = GetDC(hWnd);

	setcursor(CURSOR_INVISIBLE);
	setcolor(bgcolor, fgcolor);
	setfontsize("点阵字体", 16);
	setconsoleborder(width / 8 + !!(width % 8), high / 16 + !!(high % 16)); //将点阵的宽度及高度转换为特定字体的行列数，!!的含义：如果不是8/16的倍数，行列多+1
	cls();
}

/***************************************************************************
  函数名称：
  功    能：释放画图资源
  输入参数：
  返 回 值：
  说    明：可重入
***************************************************************************/
void hdc_release()
{
	if (hdc) {
		ReleaseDC(hWnd, hdc);
		hdc = NULL;
		setcursor(CURSOR_VISIBLE_NORMAL);
	}
}

/***************************************************************************
  函数名称：
  功    能：设置画笔颜色，传入RGB值
  输入参数：
  返 回 值：
  说    明：
***************************************************************************/
void hdc_set_pencolor(const int RGB_value)
{
	SelectObject(hdc, GetStockObject(DC_PEN));
	SetDCPenColor(hdc, RGB_value);
}

/***************************************************************************
  函数名称：
  功    能：设置画笔颜色，传入RGB三色，值0-255
  输入参数：
  返 回 值：
  说    明：
***************************************************************************/
void hdc_set_pencolor(const unsigned char red, const unsigned char green, const unsigned char blue)
{
	hdc_set_pencolor(RGB(red, green, blue));
}

/***************************************************************************
  函数名称：
  功    能：清除屏幕上现有的图形
  输入参数：
  返 回 值：
  说    明：
***************************************************************************/
void hdc_cls()
{
	/* 发现一定要换一种颜色初始化才能清除像素点，找到更简便方法的同学可以通知我 */
	hdc_init(_BgColor_, (_FgColor_ + 1) % 16, _Width_, _High);

	hdc_init(_BgColor_, _FgColor_, _Width_, _High);
}
/***************************************************************************
  函数名称：
  功    能：在(x,y)位置处用指定颜色画出一个像素点
  输入参数：const int x：x坐标，左上角为(0,0)
			const int y：y坐标，左上角为(0,0)
  返 回 值：
  说    明：颜色直接用当前设定，可以在调用处指定
***************************************************************************/
static inline void hdc_base_point(const int x, const int y)
{
	MoveToEx(hdc, x - 1, y - 1, NULL);
	LineTo(hdc, x, y);
}

/***************************************************************************
  函数名称：
  功    能：在(x,y)位置处用指定颜色画出一个像素点
  输入参数：const int x：x坐标，左上角为(0,0)
			const int y：y坐标，左上角为(0,0)
  返 回 值：
  说    明：颜色直接用当前设定，可以在调用处指定
***************************************************************************/
static inline void hdc_base_line(const int x1, const int y1, const int x2, const int y2)
{
	MoveToEx(hdc, x1, y1, NULL);
	LineTo(hdc, x2, y2);
}

/***************************************************************************
  函数名称：
  功    能：在(x,y)位置处用指定颜色画一个指定粗细的点(用画实心圆来模拟)
  输入参数：const int x			：x坐标，左上角为(0,0)
			const int y			：y坐标，左上角为(0,0)
			const int thickness	：点的粗细，下限位1，上限不限(如过大会导致不完全填充)（有缺省值）
			const int RGB_value	：点的颜色（有缺省值）
  返 回 值：
  说    明：改进运行效率
***************************************************************************/
void hdc_point(const int x, const int y, const int thickness, const int RGB_value)
{
	int tn = thickness;
	if (tn < 1)
		tn = 1;		//处理下限
	/* 不卡点直径的上限，但是单上限超过一定大小是，画出的圆部分位置未实心 */

	const int tn_end = (tn <= 1) ? 1 : tn / 2;

	int angle, level;
	int old_x1 = INT_MAX, old_y1 = INT_MAX, x1, y1;
	int count = 0;

	if (RGB_value != INVALID_RGB)
		hdc_set_pencolor(RGB_value);

	/* 用画圆的方式模拟粗点 */
	for (level = 1; level <= tn_end; level++) {
		for (angle = 0; angle <= 360; angle++) {
			/* 注意，系统的坐标轴，0°在圆的最下方 */
			x1 = x + (int)(level * sin(angle * PI / 180));
			y1 = y - (int)(level * cos(angle * PI / 180));

			/* 当半径很小时，角度变化不会引起int型的x1/y1变化，因此加判断语句，避免重复画同一像素点 */
			if (x1 != old_x1 || y1 != old_y1) {
				old_x1 = x1;
				old_y1 = y1;
				hdc_base_point(x1, y1);
				++count;
			}
		}
	}
#if 0
	/* 放开此注释，可以看到一个粗点用了几个像素点组成 */
	gotoxy(0, 41);
	printf("count=%d           ", count);
	getchar();
#endif
}

/* -------------------------------------------------------------------------
    给出下列函数的实现，函数名及参数表不准改动
    1、不需要调用系统的其他有关图形操作的函数
    2、下列函数的实现都基于hdc_point函数的组合
    3、想一想，是不是所有函数都需要给出独立的实现过程？应该先实现哪些函数？
    4、填充是实现中最复杂的部分
    5、系统的角度与函数参数中的角度含义相差180°
   ------------------------------------------------------------------------- */

/***************************************************************************
  函数名称：
  功    能：画线段
  输入参数：const int x1		：起点的x
			const int y1		：起点的y
			const int x2		：终点的x
			const int y2		：终点的y
			const int thickness	：线段的粗细（有缺省值）
			const int RGB_value	：线段的颜色（有缺省值）
  返 回 值：
  说    明：
***************************************************************************/
void hdc_line(const int x1, const int y1, const int x2, const int y2, const int thickness, const int RGB_value)
{
	int th = max(thickness / 2, 1);
	int x, y, level, angle, old_x = INT_MAX, old_y = INT_MAX;
	
	if (RGB_value != INVALID_RGB)
		hdc_set_pencolor(RGB_value);

	hdc_base_line(x1, y1, x2, y2);
	for (level = 1; level <= th; level++) 
	{
		for (angle = 0; angle < 360; angle += 5) 
		{
			x = (int)(level * sin(angle * PI / 180) + 0.5);
			y = (int)(level * cos(angle * PI / 180) + 0.5);
			if (x != old_x || y != old_y) 
			{
				old_x = x;
				old_y = y;
				hdc_base_line(x1 + x - 1, y1 + y - 1, x2 + x - 1, y2 + y - 1);
			}
		}
	}
}

/***************************************************************************
  函数名称：
  功    能：给出三点的坐标，画一个三角形
  输入参数：const int x1		：第1个点的x
			const int y1		：第1个点的y
			const int x2		：第2个点的x
			const int y2		：第2个点的y
			const int x3		：第3个点的x
			const int y3		：第3个点的y
			bool filled			：是否需要填充（有缺省值）
			const int thickness	：边的粗细（有缺省值）
			const int RGB_value	：颜色（有缺省值）
  返 回 值：
  说    明：不判断三点是否共线，如果共线，划出一条直线即可
***************************************************************************/
void hdc_triangle(const int x1, const int y1, const int x2, const int y2, const int x3, const int y3, bool filled, const int thickness, const int RGB_value)
{
	int th = max(thickness, 1);

	if (RGB_value != INVALID_RGB)
		hdc_set_pencolor(RGB_value);

	//画线
	hdc_line(x1, y1, x2, y2, th, RGB_value);
	hdc_line(x1, y1, x3, y3, th, RGB_value);
	hdc_line(x2, y2, x3, y3, th, RGB_value);

	if (filled)
	{
		//要涂色的图形所在的大矩形
		int startX = min(x1, min(x2, x3)), startY = min(y1, min(y2, y3));
		int endX = max(x1, max(x2, x3)), endY = max(y1, max(y2, y3));
		int x, y;
		for (y = startY; y <= endY; y++)
		{
			//记录填充横线的起始x坐标
			int xStartLine = INT_MAX;
			for (x = startX; x <= endX; x++)
			{
				//计算向量AM、BM、CM的叉积的正负
				int c1 = cross_product_2D(x2 - x, y2 - y, x3 - x, y3 - y);
				int c2 = cross_product_2D(x3 - x, y3 - y, x1 - x, y1 - y);
				int c3 = cross_product_2D(x1 - x, y1 - y, x2 - x, y2 - y);
				//如果三个叉积都同号，则点(x,y)在三角形内
				bool inTriangle = ((c1 > 0 && c2 > 0 && c3 > 0) || (c1 < 0 && c2 < 0 && c3 < 0));
				//填充横线的起始x坐标
				if (inTriangle && xStartLine == INT_MAX)
					xStartLine = x;
				//此时x的值为填充横线的终点x坐标
				if (!inTriangle && xStartLine != INT_MAX)
					break;
			}
			if (xStartLine != -1)
			{
				//用画线的方式取代画点，极大提高效率
				hdc_horizonal_line(xStartLine, x, y, 1);
			}
		}
	}
}

/***************************************************************************
  函数名称：
  功    能：给出左上角坐标及宽度、高度，画出一个长方形
  输入参数：const int left_up_x			：左上角x
			const int left_up_y			：左上角y
			const int width				：宽度
			const int high				：高度
			const int rotation_angles	：以左上角为支点，与x轴的旋转倾角（向下转为正）
			bool filled					：是否需要填充（有缺省值）
			const int thickness			：边的粗细（有缺省值）
			const int RGB_value			：颜色（有缺省值）
  返 回 值：
  说    明：
***************************************************************************/
void hdc_rectangle(const int left_up_x, const int left_up_y, const int width, const int high, const int rotation_angles, const bool filled, const int thickness, const int RGB_value)
{
	struct Point
	{
		int x;
		int y;
	};

	int th = max(thickness, 1);

	if (RGB_value != INVALID_RGB)
		hdc_set_pencolor(RGB_value);

	//先处理特殊情况，提高性能
	if (filled && rotation_angles % 360 == 0)
	{
		for (int i = 0; i <= high; i++)
		{
			hdc_horizonal_line(left_up_x, left_up_x + width, left_up_y + i, 1);
		}
		return;
	}

	//计算四个点的坐标
	Point left_up    = { left_up_x , left_up_y };
	Point left_down  = { left_up.x   + (int)(high * cos((my_angle(rotation_angles, MyAngleModeRect) + 90) * PI / 180)) , 
		                 left_up.y   + (int)(high * sin((my_angle(rotation_angles, MyAngleModeRect) + 90) * PI / 180)) };
	Point right_up   = { left_up.x   + (int)(width * cos(my_angle(rotation_angles, MyAngleModeRect) * PI / 180)) ,
		                 left_up.y   + (int)(width * sin(my_angle(rotation_angles, MyAngleModeRect) * PI / 180)) };
	Point right_down = { left_down.x + (int)(width * cos(my_angle(rotation_angles, MyAngleModeRect) * PI / 180)) ,
		                 left_down.y + (int)(width * sin(my_angle(rotation_angles, MyAngleModeRect) * PI / 180)) };
	
	if (filled)
	{
		////要涂色的图形所在的大矩形
		//int startX = min(left_up.x, min(left_down.x, min(right_up.x, right_down.x))), startY = min(left_up.y, min(left_down.y, min(right_up.y, right_down.y)));
		//int endX = max(left_up.x, max(left_down.x, max(right_up.x, right_down.x))), endY = max(left_up.y, max(left_down.y, max(right_up.y, right_down.y)));	
		//int x, y;
		//for (y = startY; y <= endY; y++)
		//{
		//	int xStartLine = INT_MAX;
		//	for (x = startX; x <= endX; x++)
		//	{
		//		//求向量AB、BC、CD、DA
		//		Point AB = { right_up.x - left_up.x , right_up.y - left_up.y };
		//		Point BC = { right_down.x - right_up.x , right_down.y - right_up.y };
		//		Point CD = { left_down.x - right_down.x , left_down.y - right_down.y };
		//		Point DA = { left_up.x - left_down.x , left_up.y - left_down.y };

		//		//计算向量AB与向量AM的点积的正负
		//		int c1 = dot_product_2D(AB.x, AB.y, x - left_up.x, y - left_up.y);
		//		int c2 = dot_product_2D(BC.x, BC.y, x - right_up.x, y - right_up.y);
		//		int c3 = dot_product_2D(CD.x, CD.y, x - right_down.x, y - right_down.y);
		//		int c4 = dot_product_2D(DA.x, DA.y, x - left_down.x, y - left_down.y);

		//		//如果四个点积都同号，则点(x,y)在三角形内
		//		bool inRectangle = ((c1 > 0 && c2 > 0 && c3 > 0 && c4 > 0) || (c1 < 0 && c2 < 0 && c3 < 0 && c4 < 0));
		//		//填充横线的起始x坐标
		//		if (inRectangle && xStartLine == INT_MAX)
		//			xStartLine = x;
		//		//此时x的值为填充横线的终点x坐标
		//		if (!inRectangle && xStartLine != INT_MAX)
		//			break;
		//	}
		//	if (xStartLine != -1)
		//	{
		//		//用画线的方式取代画点，极大提高效率
		//		//hdc_line(xStartLine, y, x, y, 1);
		//		hdc_horizonal_line(xStartLine, x, y);
		//	}
		//}

		//将矩形分解为两个三角形来涂色
		hdc_triangle(left_up.x, left_up.y, left_down.x, left_down.y, right_up.x, right_up.y, true);
		hdc_triangle(right_down.x, right_down.y, left_down.x, left_down.y, right_up.x, right_up.y, true);
	}
	else
	{
		//画线
		hdc_line(left_up.x, left_up.y, right_up.x, right_up.y, th);
		hdc_line(left_up.x, left_up.y, left_down.x, left_down.y, th);
		hdc_line(right_up.x, right_up.y, right_down.x, right_down.y, th);
		hdc_line(left_down.x, left_down.y, right_down.x, right_down.y, th);
	}
}

/***************************************************************************
  函数名称：
  功    能：给出左上角坐标及边长，画出一个长方形
  输入参数：const int left_up_x			：左上角x
			const int left_up_y			：左上角y
			const int length			：边长
			const int rotation_angles	：以左上角为支点，与x轴的旋转倾角（向下转为正）（有缺省值）
			const bool filled			：是否需要填充（有缺省值）
			const int thickness			：边的粗细（有缺省值）
			const int RGB_value			：颜色（有缺省值）
  输入参数：
  返 回 值：
  说    明：
***************************************************************************/
void hdc_square(const int left_up_x, const int left_up_y, const int length, const int rotation_angles, const bool filled, const int thickness, const int RGB_value)
{
	hdc_rectangle(left_up_x, left_up_y, length, length, rotation_angles, filled, thickness, RGB_value);
}

/***************************************************************************
  函数名称：
  功    能：按照给出的参数画一段圆弧
  输入参数：const int point_x		：圆心的x
			const int point_y		：圆心的y
			const int radius		：半径
			const int angle_begin	：起始角度（以圆的最上方为0°，顺时针为正，注意：与系统坐标差了180°!!!）（有缺省值）
			const int angle_end		：结束角度（同上）（有缺省值）
			const int thickness		：粗细（有缺省值）
			const int RGB_value		：颜色（有缺省值）
  返 回 值：
  说    明：
***************************************************************************/
void hdc_arc(const int point_x, const int point_y, const int radius, const int angle_begin, const int angle_end, const int thickness, const int RGB_value)
{
	//填充系数，值越小越精细，但更慢
	const double step = min((180.0 / (PI * radius) * 0.45), 0.8);
	if (RGB_value != INVALID_RGB)
		hdc_set_pencolor(RGB_value);

	//处理角度
	double begin = angle_begin % 360;
	double end = angle_end % 360;
	while (begin >= end)
		end += 360;

	//初始化
	int th = max(thickness, 1);
	int x, y, oldX = INT_MAX, oldY = INT_MAX;
	double tempCos, tempSin;
	for (double angle = begin; angle <= end; angle += step)
	{
		//提取出计算式中的cos和sin的值，防止重复计算
		tempCos = cos((my_angle(angle, MyAngleModeArc) * PI / 180));
		tempSin = sin((my_angle(angle, MyAngleModeArc) * PI / 180));
		//改用hdc_base_point，提高性能，但还是较慢
		for (int i = -th / 2; i <= th / 2; i++)
		{
			x = point_x + (int)((radius + i) * tempCos);
			y = point_y + (int)((radius + i) * tempSin);
			if (x != oldX || y != oldY)
			{
				oldX = x;
				oldY = y;
				hdc_base_point(x, y);
			}
		}
	}
}

/***************************************************************************
  函数名称：
  功    能：按照给出的参数画一个扇形
  输入参数：const int point_x		：圆心的x
			const int point_y		：圆心的y
			const int radius		：半径
			const int angle_begin	：起始角度（以圆的最上方为0°，顺时针为正，注意：与系统坐标差了180°!!!）（有缺省值）
			const int angle_end		：结束角度（同上）（有缺省值）
			const bool filled		：是否需要填充（有缺省值）
			const int thickness		：粗细（有缺省值）
			const int RGB_value		：颜色（有缺省值）
  输入参数：
  返 回 值：
  说    明：当起始/结束角度差值为360的倍数时，不画两条边
***************************************************************************/
void hdc_sector(const int point_x, const int point_y, const int radius, const int angle_begin, const int angle_end, const bool filled, const int thickness, const int RGB_value)
{
	if (RGB_value != INVALID_RGB)
		hdc_set_pencolor(RGB_value);

	//处理角度
	int begin = angle_begin, end = angle_end;
	while (begin < 0)
	{
		begin += 360;
		end += 360;
	}
	while (begin >= end)
		end += 360;

	int startX = point_x + (int)(radius * cos(my_angle(angle_begin, MyAngleModeArc) * PI / 180));
	int startY = point_y + (int)(radius * sin(my_angle(angle_begin, MyAngleModeArc) * PI / 180));
	int endX = point_x + (int)(radius * cos(my_angle(angle_end, MyAngleModeArc) * PI / 180));
	int endY = point_y + (int)(radius * sin(my_angle(angle_end, MyAngleModeArc) * PI / 180));
	//画两条直线
	if (begin % 360 != end % 360)
	{
		hdc_line(point_x, point_y, startX, startY, thickness);
		hdc_line(point_x, point_y, endX, endY, thickness);
	}
	
	if (filled)
	{
		int x, y, curAngle;
		for (y = point_y - radius; y <= point_y + radius; y += 1)
		{
			//记录填充横线的起始x坐标
			int xStartLine = INT_MAX;
			for (x = point_x - radius; x <= point_x + radius; x += 1)
			{
				//使用-(y - point_y))，将数学坐标系转换为计算机内坐标系
				curAngle = (int)my_angle(coor_to_angle((x - point_x), -(y - point_y)), MyAngleModeSector);
				//调整角度
				while (curAngle < begin)
					curAngle += 360;
				//判断该点是否在扇形内 要同时满足在圆内、在起始角度和终止角度与中心连成的两条直线内，或者在360°的地方
				bool inSector = ((x - point_x) * (x - point_x) + (y - point_y) * (y - point_y) <= radius * radius)
					&& curAngle > begin && curAngle <= end || end % 360 == 0 && curAngle % 360 == 0;
				
				//填充横线的起始x坐标
				if (inSector && xStartLine == INT_MAX)
					xStartLine = x;
				//此时x的值为填充横线的终点x坐标
				if (!inSector && xStartLine != INT_MAX)
				{
					//用画线的方式取代画点，极大提高效率，此处不能直接break，因为可能左右都要画线
					hdc_horizonal_line(xStartLine, x, y);
					xStartLine = INT_MAX;
				}
			}
			if (xStartLine != INT_MAX)
			{
				//用画线的方式取代画点，极大提高效率
				hdc_horizonal_line(xStartLine, x, y, 1);
			}
		}
	}
	else
	{
		//画弧形
		hdc_arc(point_x, point_y, radius, begin, end, thickness);
	}
}

/***************************************************************************
  函数名称：
  功    能：按照给出的参数画一个圆
  输入参数：const int point_x		：圆心的x
			const int point_y		：圆心的y
			const int radius		：半径
			const bool filled		：是否需要填充（有缺省值）
			const int thickness		：粗细（有缺省值）
			const int RGB_value		：颜色（有缺省值）
  返 回 值：
  说    明：
***************************************************************************/
void hdc_circle(const int point_x, const int point_y, const int radius, const bool filled, const int thickness, const int RGB_value)
{
	if (RGB_value != INVALID_RGB)
		hdc_set_pencolor(RGB_value);

	hdc_arc(point_x, point_y, radius, 0, 360, thickness);

	if (filled)
	{
		int startX, endX;
		for (int i = - radius; i <= radius; i += 2)
		{
			//通过垂径定理计算填充横线的起始x坐标和终止x坐标
			startX = point_x - (int)sqrt(radius * radius - i * i);
			endX   = point_x + (int)sqrt(radius * radius - i * i);
			hdc_line(startX, point_y + i, endX, point_y + i);
		}
	}
}

/***************************************************************************
  函数名称：
  功    能：按照给出的参数画一个椭圆
  输入参数：const int point_x			：圆心的x
			const int point_y			：圆心的y
			const int radius_a			：平行于X轴的半径
			const int radius_b			：平行于Y轴的半径
			const int rotation_angles	：以圆心为支点，与x轴的旋转倾角（向下转为正）（有缺省值）
			const bool filled			：是否需要填充（有缺省值）
			const int thickness			：粗细（有缺省值）
			const int RGB_value			：颜色（有缺省值）
  返 回 值：
  说    明：
***************************************************************************/
void hdc_ellipse(const int point_x, const int point_y, const int radius_a, const int radius_b, const int rotation_angles, const bool filled, const int thickness, const int RGB_value)
{
	if (RGB_value != INVALID_RGB)
		hdc_set_pencolor(RGB_value);

	int x, y, tempX1, tempX2, tempY1, tempY2;

	if (filled)
	{
		//用画线的方式取代画点，极大提高效率
		for (y = -radius_b; y <= radius_b; y++)
		{
			part_of_ellipse(x, y, radius_a, radius_b, rotation_angles, tempX1, tempY1, tempX2, tempY2, true);
			hdc_line(point_x + tempX1, point_y + tempY1, point_x + tempX2, point_y + tempY2, 1);
		}
	}
	else
	{
		int oldX1 = INT_MAX, oldY1 = INT_MAX, oldX2 = INT_MAX, oldY2 = INT_MAX;
		//int oldX = INT_MAX, oldY = INT_MAX;
		//分别沿x轴和y轴方向画椭圆的外边框，使用4/5半径来提高效率
		for (y = -radius_b * 4 / 5; y <= radius_b * 4 / 5; y++)
		{
			part_of_ellipse(x, y, radius_a, radius_b, rotation_angles, tempX1, tempY1, tempX2, tempY2, true);
			if (oldX1 != tempX1 || oldY1 != tempY1)
			{
				oldX1 = tempX1;
				oldY1 = tempY1;
				hdc_point(point_x + tempX1, point_y + tempY1, thickness);
			}
			if (oldX2 != tempX2 || oldY2 != tempY2)
			{
				oldX2 = tempX2;
				oldY2 = tempY2;
				hdc_point(point_x + tempX2, point_y + tempY2, thickness);
			}
		}
		for (x = -radius_a * 4 / 5; x <= radius_a * 4 / 5; x++)
		{
			part_of_ellipse(x, y, radius_a, radius_b, rotation_angles, tempX1, tempY1, tempX2, tempY2, false);
			if (oldX1 != tempX1 || oldY1 != tempY1)
			{
				oldX1 = tempX1;
				oldY1 = tempY1;
				hdc_point(point_x + tempX1, point_y + tempY1, thickness);
			}
			if (oldX2 != tempX2 || oldY2 != tempY2)
			{
				oldX2 = tempX2;
				oldY2 = tempY2;
				hdc_point(point_x + tempX2, point_y + tempY2, thickness);
			}
		}

		//类似方法
		/*for (y = -radius_a; y <= radius_a; y++)
		{
			double A = radius_b * radius_b * cos(angle) * cos(angle) + radius_a * radius_a * sin(angle) * sin(angle);
			double B = (radius_b * radius_b - radius_a * radius_a) * sin(2 * angle) * y;
			double C = (radius_b * radius_b * sin(angle) * sin(angle) + radius_a * radius_a * cos(angle) * cos(angle)) * y * y - (double)radius_a * radius_a * radius_b * radius_b;;
			double x1 = ((-B + sqrt(B * B - 4 * A * C)) / (2 * A));
			double x2 = ((-B - sqrt(B * B - 4 * A * C)) / (2 * A));
			if (oldX1 != x1)
			{
				oldX1 = x1;
				hdc_point(point_x + x1, point_y + y, thickness);
			}
			if (oldX2 != x2)
			{
				oldX2 = x2;
				hdc_point(point_x + x2, point_y + y, thickness);
			}
		}*/

		//double angle = (double)rotation_angles * PI / 180.0;
		//for (y = -radius_a; y <= radius_a; y++)
		//{
		//	for (x = -radius_a; x <= radius_a; x++)
		//	{
		//		//https://blog.csdn.net/pengpengblog/article/details/53213716
		//		if (fabs(pow((x*cos(angle) + y * sin(angle)), 2) / pow(radius_a, 2) + pow((x*sin(angle) - y * cos(angle)), 2) / pow(radius_b, 2) - 1) < 1e-2)
		//		{
		//			if (abs(oldX - x) > 0 || abs(oldY - y) > 0)
		//			{
		//				oldX = x;
		//				oldY = y;
		//				hdc_point(point_x + x, point_y + y, thickness);
		//			}
		//		}
		//	}
		//}

	}

	//另一种方法，画出来的椭圆粗细很不均匀，弃用
	/*int x, y;
	for (int level = radius_b; level <= radius_a; level++)
	{
		for (double angle = 0; angle <= 90; angle += 0.2)
		{
			x = (int)(level * cos(angle * PI / 180));
			y = (int)(level * sin(angle * PI / 180));
			//if (abs(radius_b * radius_b * x * x + radius_a * radius_a * y * y - radius_a * radius_a * radius_b * radius_b) <= 1000)
			if(fabs(double(x * x) / (radius_a * radius_a) + double(y * y) / (radius_b * radius_b) - 1) <= 0.005)
			{
				hdc_point(point_x + x, point_y + y, thickness);
				hdc_point(point_x - x, point_y + y, thickness);
				hdc_point(point_x + x, point_y - y, thickness);
				hdc_point(point_x - x, point_y - y, thickness);
			}
		}
	}*/
}

//绘制阿基米德螺线  r = a + b * angle
void hdc_draw_Mathematical_curve(const int point_x, const int point_y, const int startAngle, const int endAngle, const double a, const double b, const int thickness, const int RGB_value)
{
	const int tn = max(thickness, 1);
	//const double step = (tn >= 5) ? (tn >= 10 ? 4 : 2) : 1;
	const double step = 0.7 + 0.3 * tn;
	const int ignore = (tn >= 6) ? 2 : 1;
	const int start = min(startAngle, endAngle);
	const int end = max(startAngle, endAngle);

	if (RGB_value != INVALID_RGB)
		hdc_set_pencolor(RGB_value);
	
	int x, y, oldX = INT_MAX, oldY = INT_MAX;
	double angle, traversedAngle;
	int count = 0;
	Sleep(100);
	for (angle = start; angle <= end; angle += (step / ((pow(angle - start, 1.5) + 360.0) / 360.0)))
	{
		traversedAngle = my_angle(angle, MyAngleModeMathematicalCurve) * PI / 180;
		x = point_x + (int)((a + b * angle) * cos(traversedAngle));
		y = point_y + (int)((a + b * angle) * sin(traversedAngle));
		if (abs(x - oldX) >= ignore || abs(y - oldY) >= ignore)
		{
			oldX = x;
			oldY = y;
			hdc_point(x, y, tn);
			count++;
		}
	}
}


//1850059 ��1�� ��־Զ

#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <Windows.h>

/* ���������Ҫ��ͷ�ļ� */

#include "cmd_hdc_tools.h"
using namespace std;

extern "C" WINBASEAPI HWND WINAPI GetConsoleWindow();

/* �Ѷ���ľ�̬ȫ�ֱ��� */
static HWND hWnd = GetConsoleWindow();
static HDC hdc = NULL;
static const double PI = 3.14159;
static int _BgColor_, _FgColor_, _Width_, _High;

/* �����ʶ�ʹ�þ�̬ȫ�ֱ�����������static�����ⲿ���ɼ� */

/* �˴���������Զ��庯����������static�����ⲿ���ɼ� */

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

//��ˮƽֱ�ߣ���ֱ��hdc_lineЧ�ʸ���
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

//�����ά�ռ�Ĳ����������������ֵ������������岻��
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

//�����ά�ռ�ĵ����������������ֵ������������岻��
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

//�Զ���Ƕ�ת��Ϊϵͳ�Ƕ� mode: 0->Ĭ��  1->����  2->����  3->����  4->��Բ
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

//����ֵת��Ϊ��ѧֱ������ϵ�µĽǶ�
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

//������Բ����תһ���ǶȺ�����꣬x��y�������Բ�ĵ����꣬changeAngleMode��my_angle�����Ĳ�����tranverse�����Ƿ�Ҫ������ֵ
static void rotate_coordinate(const int x, const int y, const int rotationAngles, int &resultX, int &resultY, const int changeAngleMode = 0, const bool tranverse = false)
{
	double distance = sqrt(x * x + y * y);
	double angle = my_angle((!tranverse) ? (atan((double)y / x) * 180 / PI + rotationAngles) : (atan(-(double)y / x) * 180 / PI + 180 + rotationAngles), changeAngleMode);
	resultX = (int)(distance * cos(angle * PI / 180));
	resultY = (int)(distance * sin(angle * PI / 180));
}

//����Բ������һ���֣����������Բ�ϵ�����ֵ������ת֮�������ֵ
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

/* ��������˼�����������������ʵ�֣���Ҫ�Ķ� */
/***************************************************************************
  �������ƣ�
  ��    �ܣ���ʼ��
  ���������const int bgcolor������ɫ
			const int fgcolor��ǰ��ɫ
			const int width  ����Ļ��ȣ�����
			const int high   ����Ļ�߶ȣ�����
  �� �� ֵ��
  ˵    ����
***************************************************************************/
void hdc_init(const int bgcolor, const int fgcolor, const int width, const int high, const int fontSize, const char font[])
{
	/* ���ͷţ���ֹ��release���ٴ�init��hdc_release�����룩 */
	hdc_release();

	/* ����init����һ����̬ȫ������¼������hdc_cls()���õ� */
	_BgColor_ = bgcolor;
	_FgColor_ = fgcolor;
	_Width_ = width;
	_High = high;

	hdc = GetDC(hWnd);

	setcursor(CURSOR_INVISIBLE);
	setcolor(bgcolor, fgcolor);
	setfontsize("��������", 16);
	setconsoleborder(width / 8 + !!(width % 8), high / 16 + !!(high % 16)); //������Ŀ�ȼ��߶�ת��Ϊ�ض��������������!!�ĺ��壺�������8/16�ı��������ж�+1
	cls();
}

/***************************************************************************
  �������ƣ�
  ��    �ܣ��ͷŻ�ͼ��Դ
  ���������
  �� �� ֵ��
  ˵    ����������
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
  �������ƣ�
  ��    �ܣ����û�����ɫ������RGBֵ
  ���������
  �� �� ֵ��
  ˵    ����
***************************************************************************/
void hdc_set_pencolor(const int RGB_value)
{
	SelectObject(hdc, GetStockObject(DC_PEN));
	SetDCPenColor(hdc, RGB_value);
}

/***************************************************************************
  �������ƣ�
  ��    �ܣ����û�����ɫ������RGB��ɫ��ֵ0-255
  ���������
  �� �� ֵ��
  ˵    ����
***************************************************************************/
void hdc_set_pencolor(const unsigned char red, const unsigned char green, const unsigned char blue)
{
	hdc_set_pencolor(RGB(red, green, blue));
}

/***************************************************************************
  �������ƣ�
  ��    �ܣ������Ļ�����е�ͼ��
  ���������
  �� �� ֵ��
  ˵    ����
***************************************************************************/
void hdc_cls()
{
	/* ����һ��Ҫ��һ����ɫ��ʼ������������ص㣬�ҵ�����㷽����ͬѧ����֪ͨ�� */
	hdc_init(_BgColor_, (_FgColor_ + 1) % 16, _Width_, _High);

	hdc_init(_BgColor_, _FgColor_, _Width_, _High);
}
/***************************************************************************
  �������ƣ�
  ��    �ܣ���(x,y)λ�ô���ָ����ɫ����һ�����ص�
  ���������const int x��x���꣬���Ͻ�Ϊ(0,0)
			const int y��y���꣬���Ͻ�Ϊ(0,0)
  �� �� ֵ��
  ˵    ������ɫֱ���õ�ǰ�趨�������ڵ��ô�ָ��
***************************************************************************/
static inline void hdc_base_point(const int x, const int y)
{
	MoveToEx(hdc, x - 1, y - 1, NULL);
	LineTo(hdc, x, y);
}

/***************************************************************************
  �������ƣ�
  ��    �ܣ���(x,y)λ�ô���ָ����ɫ����һ�����ص�
  ���������const int x��x���꣬���Ͻ�Ϊ(0,0)
			const int y��y���꣬���Ͻ�Ϊ(0,0)
  �� �� ֵ��
  ˵    ������ɫֱ���õ�ǰ�趨�������ڵ��ô�ָ��
***************************************************************************/
static inline void hdc_base_line(const int x1, const int y1, const int x2, const int y2)
{
	MoveToEx(hdc, x1, y1, NULL);
	LineTo(hdc, x2, y2);
}

/***************************************************************************
  �������ƣ�
  ��    �ܣ���(x,y)λ�ô���ָ����ɫ��һ��ָ����ϸ�ĵ�(�û�ʵ��Բ��ģ��)
  ���������const int x			��x���꣬���Ͻ�Ϊ(0,0)
			const int y			��y���꣬���Ͻ�Ϊ(0,0)
			const int thickness	����Ĵ�ϸ������λ1�����޲���(�����ᵼ�²���ȫ���)����ȱʡֵ��
			const int RGB_value	�������ɫ����ȱʡֵ��
  �� �� ֵ��
  ˵    �����Ľ�����Ч��
***************************************************************************/
void hdc_point(const int x, const int y, const int thickness, const int RGB_value)
{
	int tn = thickness;
	if (tn < 1)
		tn = 1;		//��������
	/* ������ֱ�������ޣ����ǵ����޳���һ����С�ǣ�������Բ����λ��δʵ�� */

	const int tn_end = (tn <= 1) ? 1 : tn / 2;

	int angle, level;
	int old_x1 = INT_MAX, old_y1 = INT_MAX, x1, y1;
	int count = 0;

	if (RGB_value != INVALID_RGB)
		hdc_set_pencolor(RGB_value);

	/* �û�Բ�ķ�ʽģ��ֵ� */
	for (level = 1; level <= tn_end; level++) {
		for (angle = 0; angle <= 360; angle++) {
			/* ע�⣬ϵͳ�������ᣬ0����Բ�����·� */
			x1 = x + (int)(level * sin(angle * PI / 180));
			y1 = y - (int)(level * cos(angle * PI / 180));

			/* ���뾶��Сʱ���Ƕȱ仯��������int�͵�x1/y1�仯����˼��ж���䣬�����ظ���ͬһ���ص� */
			if (x1 != old_x1 || y1 != old_y1) {
				old_x1 = x1;
				old_y1 = y1;
				hdc_base_point(x1, y1);
				++count;
			}
		}
	}
#if 0
	/* �ſ���ע�ͣ����Կ���һ���ֵ����˼������ص���� */
	gotoxy(0, 41);
	printf("count=%d           ", count);
	getchar();
#endif
}

/* -------------------------------------------------------------------------
    �������к�����ʵ�֣���������������׼�Ķ�
    1������Ҫ����ϵͳ�������й�ͼ�β����ĺ���
    2�����к�����ʵ�ֶ�����hdc_point���������
    3����һ�룬�ǲ������к�������Ҫ����������ʵ�ֹ��̣�Ӧ����ʵ����Щ������
    4�������ʵ������ӵĲ���
    5��ϵͳ�ĽǶ��뺯�������еĽǶȺ������180��
   ------------------------------------------------------------------------- */

/***************************************************************************
  �������ƣ�
  ��    �ܣ����߶�
  ���������const int x1		������x
			const int y1		������y
			const int x2		���յ��x
			const int y2		���յ��y
			const int thickness	���߶εĴ�ϸ����ȱʡֵ��
			const int RGB_value	���߶ε���ɫ����ȱʡֵ��
  �� �� ֵ��
  ˵    ����
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
  �������ƣ�
  ��    �ܣ�������������꣬��һ��������
  ���������const int x1		����1�����x
			const int y1		����1�����y
			const int x2		����2�����x
			const int y2		����2�����y
			const int x3		����3�����x
			const int y3		����3�����y
			bool filled			���Ƿ���Ҫ��䣨��ȱʡֵ��
			const int thickness	���ߵĴ�ϸ����ȱʡֵ��
			const int RGB_value	����ɫ����ȱʡֵ��
  �� �� ֵ��
  ˵    �������ж������Ƿ��ߣ�������ߣ�����һ��ֱ�߼���
***************************************************************************/
void hdc_triangle(const int x1, const int y1, const int x2, const int y2, const int x3, const int y3, bool filled, const int thickness, const int RGB_value)
{
	int th = max(thickness, 1);

	if (RGB_value != INVALID_RGB)
		hdc_set_pencolor(RGB_value);

	//����
	hdc_line(x1, y1, x2, y2, th, RGB_value);
	hdc_line(x1, y1, x3, y3, th, RGB_value);
	hdc_line(x2, y2, x3, y3, th, RGB_value);

	if (filled)
	{
		//ҪͿɫ��ͼ�����ڵĴ����
		int startX = min(x1, min(x2, x3)), startY = min(y1, min(y2, y3));
		int endX = max(x1, max(x2, x3)), endY = max(y1, max(y2, y3));
		int x, y;
		for (y = startY; y <= endY; y++)
		{
			//��¼�����ߵ���ʼx����
			int xStartLine = INT_MAX;
			for (x = startX; x <= endX; x++)
			{
				//��������AM��BM��CM�Ĳ��������
				int c1 = cross_product_2D(x2 - x, y2 - y, x3 - x, y3 - y);
				int c2 = cross_product_2D(x3 - x, y3 - y, x1 - x, y1 - y);
				int c3 = cross_product_2D(x1 - x, y1 - y, x2 - x, y2 - y);
				//������������ͬ�ţ����(x,y)����������
				bool inTriangle = ((c1 > 0 && c2 > 0 && c3 > 0) || (c1 < 0 && c2 < 0 && c3 < 0));
				//�����ߵ���ʼx����
				if (inTriangle && xStartLine == INT_MAX)
					xStartLine = x;
				//��ʱx��ֵΪ�����ߵ��յ�x����
				if (!inTriangle && xStartLine != INT_MAX)
					break;
			}
			if (xStartLine != -1)
			{
				//�û��ߵķ�ʽȡ�����㣬�������Ч��
				hdc_horizonal_line(xStartLine, x, y, 1);
			}
		}
	}
}

/***************************************************************************
  �������ƣ�
  ��    �ܣ��������Ͻ����꼰��ȡ��߶ȣ�����һ��������
  ���������const int left_up_x			�����Ͻ�x
			const int left_up_y			�����Ͻ�y
			const int width				�����
			const int high				���߶�
			const int rotation_angles	�������Ͻ�Ϊ֧�㣬��x�����ת��ǣ�����תΪ����
			bool filled					���Ƿ���Ҫ��䣨��ȱʡֵ��
			const int thickness			���ߵĴ�ϸ����ȱʡֵ��
			const int RGB_value			����ɫ����ȱʡֵ��
  �� �� ֵ��
  ˵    ����
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

	//�ȴ�������������������
	if (filled && rotation_angles % 360 == 0)
	{
		for (int i = 0; i <= high; i++)
		{
			hdc_horizonal_line(left_up_x, left_up_x + width, left_up_y + i, 1);
		}
		return;
	}

	//�����ĸ��������
	Point left_up    = { left_up_x , left_up_y };
	Point left_down  = { left_up.x   + (int)(high * cos((my_angle(rotation_angles, MyAngleModeRect) + 90) * PI / 180)) , 
		                 left_up.y   + (int)(high * sin((my_angle(rotation_angles, MyAngleModeRect) + 90) * PI / 180)) };
	Point right_up   = { left_up.x   + (int)(width * cos(my_angle(rotation_angles, MyAngleModeRect) * PI / 180)) ,
		                 left_up.y   + (int)(width * sin(my_angle(rotation_angles, MyAngleModeRect) * PI / 180)) };
	Point right_down = { left_down.x + (int)(width * cos(my_angle(rotation_angles, MyAngleModeRect) * PI / 180)) ,
		                 left_down.y + (int)(width * sin(my_angle(rotation_angles, MyAngleModeRect) * PI / 180)) };
	
	if (filled)
	{
		////ҪͿɫ��ͼ�����ڵĴ����
		//int startX = min(left_up.x, min(left_down.x, min(right_up.x, right_down.x))), startY = min(left_up.y, min(left_down.y, min(right_up.y, right_down.y)));
		//int endX = max(left_up.x, max(left_down.x, max(right_up.x, right_down.x))), endY = max(left_up.y, max(left_down.y, max(right_up.y, right_down.y)));	
		//int x, y;
		//for (y = startY; y <= endY; y++)
		//{
		//	int xStartLine = INT_MAX;
		//	for (x = startX; x <= endX; x++)
		//	{
		//		//������AB��BC��CD��DA
		//		Point AB = { right_up.x - left_up.x , right_up.y - left_up.y };
		//		Point BC = { right_down.x - right_up.x , right_down.y - right_up.y };
		//		Point CD = { left_down.x - right_down.x , left_down.y - right_down.y };
		//		Point DA = { left_up.x - left_down.x , left_up.y - left_down.y };

		//		//��������AB������AM�ĵ��������
		//		int c1 = dot_product_2D(AB.x, AB.y, x - left_up.x, y - left_up.y);
		//		int c2 = dot_product_2D(BC.x, BC.y, x - right_up.x, y - right_up.y);
		//		int c3 = dot_product_2D(CD.x, CD.y, x - right_down.x, y - right_down.y);
		//		int c4 = dot_product_2D(DA.x, DA.y, x - left_down.x, y - left_down.y);

		//		//����ĸ������ͬ�ţ����(x,y)����������
		//		bool inRectangle = ((c1 > 0 && c2 > 0 && c3 > 0 && c4 > 0) || (c1 < 0 && c2 < 0 && c3 < 0 && c4 < 0));
		//		//�����ߵ���ʼx����
		//		if (inRectangle && xStartLine == INT_MAX)
		//			xStartLine = x;
		//		//��ʱx��ֵΪ�����ߵ��յ�x����
		//		if (!inRectangle && xStartLine != INT_MAX)
		//			break;
		//	}
		//	if (xStartLine != -1)
		//	{
		//		//�û��ߵķ�ʽȡ�����㣬�������Ч��
		//		//hdc_line(xStartLine, y, x, y, 1);
		//		hdc_horizonal_line(xStartLine, x, y);
		//	}
		//}

		//�����ηֽ�Ϊ������������Ϳɫ
		hdc_triangle(left_up.x, left_up.y, left_down.x, left_down.y, right_up.x, right_up.y, true);
		hdc_triangle(right_down.x, right_down.y, left_down.x, left_down.y, right_up.x, right_up.y, true);
	}
	else
	{
		//����
		hdc_line(left_up.x, left_up.y, right_up.x, right_up.y, th);
		hdc_line(left_up.x, left_up.y, left_down.x, left_down.y, th);
		hdc_line(right_up.x, right_up.y, right_down.x, right_down.y, th);
		hdc_line(left_down.x, left_down.y, right_down.x, right_down.y, th);
	}
}

/***************************************************************************
  �������ƣ�
  ��    �ܣ��������Ͻ����꼰�߳�������һ��������
  ���������const int left_up_x			�����Ͻ�x
			const int left_up_y			�����Ͻ�y
			const int length			���߳�
			const int rotation_angles	�������Ͻ�Ϊ֧�㣬��x�����ת��ǣ�����תΪ��������ȱʡֵ��
			const bool filled			���Ƿ���Ҫ��䣨��ȱʡֵ��
			const int thickness			���ߵĴ�ϸ����ȱʡֵ��
			const int RGB_value			����ɫ����ȱʡֵ��
  ���������
  �� �� ֵ��
  ˵    ����
***************************************************************************/
void hdc_square(const int left_up_x, const int left_up_y, const int length, const int rotation_angles, const bool filled, const int thickness, const int RGB_value)
{
	hdc_rectangle(left_up_x, left_up_y, length, length, rotation_angles, filled, thickness, RGB_value);
}

/***************************************************************************
  �������ƣ�
  ��    �ܣ����ո����Ĳ�����һ��Բ��
  ���������const int point_x		��Բ�ĵ�x
			const int point_y		��Բ�ĵ�y
			const int radius		���뾶
			const int angle_begin	����ʼ�Ƕȣ���Բ�����Ϸ�Ϊ0�㣬˳ʱ��Ϊ����ע�⣺��ϵͳ�������180��!!!������ȱʡֵ��
			const int angle_end		�������Ƕȣ�ͬ�ϣ�����ȱʡֵ��
			const int thickness		����ϸ����ȱʡֵ��
			const int RGB_value		����ɫ����ȱʡֵ��
  �� �� ֵ��
  ˵    ����
***************************************************************************/
void hdc_arc(const int point_x, const int point_y, const int radius, const int angle_begin, const int angle_end, const int thickness, const int RGB_value)
{
	//���ϵ����ֵԽСԽ��ϸ��������
	const double step = min((180.0 / (PI * radius) * 0.45), 0.8);
	if (RGB_value != INVALID_RGB)
		hdc_set_pencolor(RGB_value);

	//����Ƕ�
	double begin = angle_begin % 360;
	double end = angle_end % 360;
	while (begin >= end)
		end += 360;

	//��ʼ��
	int th = max(thickness, 1);
	int x, y, oldX = INT_MAX, oldY = INT_MAX;
	double tempCos, tempSin;
	for (double angle = begin; angle <= end; angle += step)
	{
		//��ȡ������ʽ�е�cos��sin��ֵ����ֹ�ظ�����
		tempCos = cos((my_angle(angle, MyAngleModeArc) * PI / 180));
		tempSin = sin((my_angle(angle, MyAngleModeArc) * PI / 180));
		//����hdc_base_point��������ܣ������ǽ���
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
  �������ƣ�
  ��    �ܣ����ո����Ĳ�����һ������
  ���������const int point_x		��Բ�ĵ�x
			const int point_y		��Բ�ĵ�y
			const int radius		���뾶
			const int angle_begin	����ʼ�Ƕȣ���Բ�����Ϸ�Ϊ0�㣬˳ʱ��Ϊ����ע�⣺��ϵͳ�������180��!!!������ȱʡֵ��
			const int angle_end		�������Ƕȣ�ͬ�ϣ�����ȱʡֵ��
			const bool filled		���Ƿ���Ҫ��䣨��ȱʡֵ��
			const int thickness		����ϸ����ȱʡֵ��
			const int RGB_value		����ɫ����ȱʡֵ��
  ���������
  �� �� ֵ��
  ˵    ��������ʼ/�����ǶȲ�ֵΪ360�ı���ʱ������������
***************************************************************************/
void hdc_sector(const int point_x, const int point_y, const int radius, const int angle_begin, const int angle_end, const bool filled, const int thickness, const int RGB_value)
{
	if (RGB_value != INVALID_RGB)
		hdc_set_pencolor(RGB_value);

	//����Ƕ�
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
	//������ֱ��
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
			//��¼�����ߵ���ʼx����
			int xStartLine = INT_MAX;
			for (x = point_x - radius; x <= point_x + radius; x += 1)
			{
				//ʹ��-(y - point_y))������ѧ����ϵת��Ϊ�����������ϵ
				curAngle = (int)my_angle(coor_to_angle((x - point_x), -(y - point_y)), MyAngleModeSector);
				//�����Ƕ�
				while (curAngle < begin)
					curAngle += 360;
				//�жϸõ��Ƿ��������� Ҫͬʱ������Բ�ڡ�����ʼ�ǶȺ���ֹ�Ƕ����������ɵ�����ֱ���ڣ�������360��ĵط�
				bool inSector = ((x - point_x) * (x - point_x) + (y - point_y) * (y - point_y) <= radius * radius)
					&& curAngle > begin && curAngle <= end || end % 360 == 0 && curAngle % 360 == 0;
				
				//�����ߵ���ʼx����
				if (inSector && xStartLine == INT_MAX)
					xStartLine = x;
				//��ʱx��ֵΪ�����ߵ��յ�x����
				if (!inSector && xStartLine != INT_MAX)
				{
					//�û��ߵķ�ʽȡ�����㣬�������Ч�ʣ��˴�����ֱ��break����Ϊ�������Ҷ�Ҫ����
					hdc_horizonal_line(xStartLine, x, y);
					xStartLine = INT_MAX;
				}
			}
			if (xStartLine != INT_MAX)
			{
				//�û��ߵķ�ʽȡ�����㣬�������Ч��
				hdc_horizonal_line(xStartLine, x, y, 1);
			}
		}
	}
	else
	{
		//������
		hdc_arc(point_x, point_y, radius, begin, end, thickness);
	}
}

/***************************************************************************
  �������ƣ�
  ��    �ܣ����ո����Ĳ�����һ��Բ
  ���������const int point_x		��Բ�ĵ�x
			const int point_y		��Բ�ĵ�y
			const int radius		���뾶
			const bool filled		���Ƿ���Ҫ��䣨��ȱʡֵ��
			const int thickness		����ϸ����ȱʡֵ��
			const int RGB_value		����ɫ����ȱʡֵ��
  �� �� ֵ��
  ˵    ����
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
			//ͨ������������������ߵ���ʼx�������ֹx����
			startX = point_x - (int)sqrt(radius * radius - i * i);
			endX   = point_x + (int)sqrt(radius * radius - i * i);
			hdc_line(startX, point_y + i, endX, point_y + i);
		}
	}
}

/***************************************************************************
  �������ƣ�
  ��    �ܣ����ո����Ĳ�����һ����Բ
  ���������const int point_x			��Բ�ĵ�x
			const int point_y			��Բ�ĵ�y
			const int radius_a			��ƽ����X��İ뾶
			const int radius_b			��ƽ����Y��İ뾶
			const int rotation_angles	����Բ��Ϊ֧�㣬��x�����ת��ǣ�����תΪ��������ȱʡֵ��
			const bool filled			���Ƿ���Ҫ��䣨��ȱʡֵ��
			const int thickness			����ϸ����ȱʡֵ��
			const int RGB_value			����ɫ����ȱʡֵ��
  �� �� ֵ��
  ˵    ����
***************************************************************************/
void hdc_ellipse(const int point_x, const int point_y, const int radius_a, const int radius_b, const int rotation_angles, const bool filled, const int thickness, const int RGB_value)
{
	if (RGB_value != INVALID_RGB)
		hdc_set_pencolor(RGB_value);

	int x, y, tempX1, tempX2, tempY1, tempY2;

	if (filled)
	{
		//�û��ߵķ�ʽȡ�����㣬�������Ч��
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
		//�ֱ���x���y�᷽����Բ����߿�ʹ��4/5�뾶�����Ч��
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

		//���Ʒ���
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

	//��һ�ַ���������������Բ��ϸ�ܲ����ȣ�����
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

//���ư����׵�����  r = a + b * angle
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


//1850059 ��1�� ��־Զ

/**********************************

ͼ�������Դ��������NDS�ƻ��İ����Ų���ʽ
https://baike.baidu.com/pic/nds/10846/0/7af40ad162d9f2d3c7b48044acec8a136227ccf0?fr=lemma&ct=single#aid=0&pic=7af40ad162d9f2d3c7b48044acec8a136227ccf0
���¼��̰���ʱ��һ���Ķ���Ч����F��������
ͼ�λ����Ĭ��145*35�Ĵ��ڳߴ�������ţ���֤ÿ��ͼ�ε�λ�ò���������

***********************************/

#include "90-b3.h"

const int COLOR_NORMAL = RGB(150, 175, 25);
const int COLOR_CLICKED = RGB(75, 75, 0);
const int COLOR_ELLIPSE = RGB(200, 200, 50);

//��ʼ�����趨������С
void draw_graphics_init(int windowX, int windowY, int fontSize, const char font[])
{
	hdc_init(0, 7, windowX * 8, windowY * 16, FontSize2, Font2);
}

//���»��Ʒ������ÿ�μ��̲�������������������Ҫ�ػ���
static void update_arrow()
{
	double xScale = WindowXSize2 / 145.0, yScale = WindowYSize2 / 35.0;
	hdc_rectangle(int(75 * xScale), int(200 * yScale), int(50 * xScale), int(150 * yScale), 0, true, 2, COLOR_NORMAL);
	hdc_rectangle(int(25 * xScale), int(250 * yScale), int(150 * xScale), int(50 * yScale), 0, true, 2, COLOR_NORMAL);
	hdc_line(int(100 * xScale), int(205 * yScale), int(100 * xScale), int(230 * yScale), 4, RGB(0, 0, 0));	//��
	hdc_line(int(100 * xScale), int(320 * yScale), int(100 * xScale), int(345 * yScale), 4, RGB(0, 0, 0));	//��
	hdc_line(int(30 * xScale), int(275 * yScale), int(55 * xScale), int(275 * yScale), 4, RGB(0, 0, 0));	//��
	hdc_line(int(145 * xScale), int(275 * yScale), int(170 * xScale), int(275 * yScale), 4, RGB(0, 0, 0));	//��
}

//������ɵ�ȫ��ͼ��
void draw_graphics()
{
	double xScale = WindowXSize2 / 145.0, yScale = WindowYSize2 / 35.0;
	update_arrow();
	hdc_circle(int( 950 * xScale), int(350 * yScale), int(25 * (xScale + yScale) / 2), true, 2, COLOR_NORMAL);	//��N
	hdc_circle(int(1100 * xScale), int(350 * yScale), int(25 * (xScale + yScale) / 2), true, 2, COLOR_NORMAL);	//��S
	hdc_circle(int(1025 * xScale), int(300 * yScale), int(25 * (xScale + yScale) / 2), true, 2, COLOR_NORMAL);	//��Q
	hdc_circle(int(1025 * xScale), int(400 * yScale), int(25 * (xScale + yScale) / 2), true, 2, COLOR_NORMAL);	//��F
	hdc_rectangle(int( 925 * xScale), int(150 * yScale), int(75 * xScale), int(50 * yScale), 0, true, 2, COLOR_NORMAL);	//����PgUp
	hdc_rectangle(int(1050 * xScale), int(150 * yScale), int(75 * xScale), int(50 * yScale), 0, true, 2, COLOR_NORMAL);	//����PgDn
	draw_rotate_ellipse(true); //��ת����Բ
}

//�����Ϸ�����Ķ���
void draw_graghics_up()
{
	double xScale = WindowXSize2 / 145.0, yScale = WindowYSize2 / 35.0;
	//Sleep(15); //���ҵĵ����ϵ��°����̨����Ҫ���������䣬����ͼ�λ���ȫ��ԭ���������ɰ����̨��������������
	update_arrow();
	hdc_rectangle(int(75 * xScale), int(200 * yScale), int(50 * xScale), int(47 * yScale), 0, true, 2, COLOR_CLICKED);
	hdc_line(int(100 * xScale), int(205 * yScale), int(100 * xScale), int(230 * yScale), 4, RGB(0, 0, 0));
	Sleep(TextBoxDelay1);
	hdc_rectangle(int(75 * xScale), int(200 * yScale), int(50 * xScale), int(47 * yScale), 0, true, 2, COLOR_NORMAL);
	hdc_line(int(100 * xScale), int(205 * yScale), int(100 * xScale), int(230 * yScale), 4, RGB(0, 0, 0));
}

//�����·�����Ķ���
void draw_graghics_down()
{
	double xScale = WindowXSize2 / 145.0, yScale = WindowYSize2 / 35.0;
	//Sleep(15); //���ҵĵ����ϵ��°����̨����Ҫ���������䣬����ͼ�λ���ȫ��ԭ���������ɰ����̨��������������
	update_arrow();
	hdc_rectangle(int(75 * xScale), int(303 * yScale), int(50 * xScale), int(50 * yScale), 0, true, 2, COLOR_CLICKED);
	hdc_line(int(100 * xScale), int(320 * yScale), int(100 * xScale), int(345 * yScale), 4, RGB(0, 0, 0));
	Sleep(TextBoxDelay1);
	hdc_rectangle(int(75 * xScale), int(303 * yScale), int(50 * xScale), int(50 * yScale), 0, true, 2, COLOR_NORMAL);
	hdc_line(int(100 * xScale), int(320 * yScale), int(100 * xScale), int(345 * yScale), 4, RGB(0, 0, 0));
}

//����F���Ķ���
void draw_graphics_F()
{
	double xScale = WindowXSize2 / 145.0, yScale = WindowYSize2 / 35.0;
	hdc_circle(int(1025 * xScale), int(400 * yScale), int(25 * (xScale + yScale) / 2), true, 2, COLOR_CLICKED); //��F
	update_arrow();
	Sleep(TextBoxDelay3);
	hdc_circle(int(1025 * xScale), int(400 * yScale), int(25 * (xScale + yScale) / 2), true, 2, COLOR_NORMAL); //��F
}

//����S���Ķ���
void draw_graphics_S()
{
	double xScale = WindowXSize2 / 145.0, yScale = WindowYSize2 / 35.0;
	hdc_circle(int(1100 * xScale), int(350 * yScale), int(25 * (xScale + yScale) / 2), true, 2, COLOR_CLICKED); //��S
	update_arrow();
	Sleep(TextBoxDelay3 / 2);
	hdc_circle(int(1100 * xScale), int(350 * yScale), int(25 * (xScale + yScale) / 2), true, 2, COLOR_NORMAL); //��S
}

//����PageUp���Ķ���
void draw_graphics_PgUp()
{
	double xScale = WindowXSize2 / 145.0, yScale = WindowYSize2 / 35.0;
	hdc_rectangle(int( 925 * xScale), int(150 * yScale), int(75 * xScale), int(50 * yScale), 0, true, 2, COLOR_CLICKED);	//����PgUp
	Sleep(TextBoxDelay2);
	update_arrow();
	hdc_rectangle(int( 925 * xScale), int(150 * yScale), int(75 * xScale), int(50 * yScale), 0, true, 2, COLOR_NORMAL);		//����PgUp
}

//����PageDown���Ķ���
void draw_graphics_PgDn()
{
	double xScale = WindowXSize2 / 145.0, yScale = WindowYSize2 / 35.0;
	hdc_rectangle(int(1050 * xScale), int(150 * yScale), int(75 * xScale), int(50 * yScale), 0, true, 2, COLOR_CLICKED);	//����PgDn
	Sleep(TextBoxDelay2);
	update_arrow();
	hdc_rectangle(int(1050 * xScale), int(150 * yScale), int(75 * xScale), int(50 * yScale), 0, true, 2, COLOR_NORMAL);		//����PgDn
}

//������ת����Բ��clockwiseΪtrue��ʾ˳ʱ����ת��false��ʾ��ʱ��
void draw_rotate_ellipse(bool clockwise)
{
	const int RotateAngle = 18; //����ÿ����ת�ĽǶȱ仯
	static int rotateCount = 0; //���徲̬�ֲ���������¼��ת�ĽǶ�
	double xScale = WindowXSize2 / 145.0, yScale = WindowYSize2 / 35.0;
	//hdc_ellipse(900, 550, 30, 15, rotateCount, false, 2, RGB(0, 0, 0));
	hdc_square(950 - 32, 550 - 32, 64, 0, true, 2, RGB(12, 12, 12)); //������һ�λ��Ƶ���Բ����΢�Ӵ�ߴ��ֹ��������ȫ
	if (clockwise)	//˳ʱ����ת
		rotateCount += RotateAngle;
	else			//��ʱ����ת
		rotateCount -= RotateAngle;
	if (rotateCount % 360 == 0) //��һȦ����Ϊ0����ֹ�������
		rotateCount = 0;
	hdc_ellipse(950, 550, 30, 15, rotateCount, false, 2, COLOR_ELLIPSE);
}
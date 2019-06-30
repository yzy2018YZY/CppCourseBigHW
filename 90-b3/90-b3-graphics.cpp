//1850059 计1班 杨志远

/**********************************

图案的灵感源自任天堂NDS掌机的按键排布方式
https://baike.baidu.com/pic/nds/10846/0/7af40ad162d9f2d3c7b48044acec8a136227ccf0?fr=lemma&ct=single#aid=0&pic=7af40ad162d9f2d3c7b48044acec8a136227ccf0
按下键盘按键时有一定的动画效果，F键最明显
图形会根据默认145*35的窗口尺寸进行缩放，保证每个图形的位置不发生错误

***********************************/

#include "90-b3.h"

const int COLOR_NORMAL = RGB(150, 175, 25);
const int COLOR_CLICKED = RGB(75, 75, 0);
const int COLOR_ELLIPSE = RGB(200, 200, 50);

//初始化，设定画布大小
void draw_graphics_init(int windowX, int windowY, int fontSize, const char font[])
{
	hdc_init(0, 7, windowX * 8, windowY * 16, FontSize2, Font2);
}

//重新绘制方向键（每次键盘操作都会擦除方向键，需要重画）
static void update_arrow()
{
	double xScale = WindowXSize2 / 145.0, yScale = WindowYSize2 / 35.0;
	hdc_rectangle(int(75 * xScale), int(200 * yScale), int(50 * xScale), int(150 * yScale), 0, true, 2, COLOR_NORMAL);
	hdc_rectangle(int(25 * xScale), int(250 * yScale), int(150 * xScale), int(50 * yScale), 0, true, 2, COLOR_NORMAL);
	hdc_line(int(100 * xScale), int(205 * yScale), int(100 * xScale), int(230 * yScale), 4, RGB(0, 0, 0));	//上
	hdc_line(int(100 * xScale), int(320 * yScale), int(100 * xScale), int(345 * yScale), 4, RGB(0, 0, 0));	//下
	hdc_line(int(30 * xScale), int(275 * yScale), int(55 * xScale), int(275 * yScale), 4, RGB(0, 0, 0));	//坐
	hdc_line(int(145 * xScale), int(275 * yScale), int(170 * xScale), int(275 * yScale), 4, RGB(0, 0, 0));	//右
}

//绘制完成的全部图形
void draw_graphics()
{
	double xScale = WindowXSize2 / 145.0, yScale = WindowYSize2 / 35.0;
	update_arrow();
	hdc_circle(int( 950 * xScale), int(350 * yScale), int(25 * (xScale + yScale) / 2), true, 2, COLOR_NORMAL);	//左N
	hdc_circle(int(1100 * xScale), int(350 * yScale), int(25 * (xScale + yScale) / 2), true, 2, COLOR_NORMAL);	//右S
	hdc_circle(int(1025 * xScale), int(300 * yScale), int(25 * (xScale + yScale) / 2), true, 2, COLOR_NORMAL);	//上Q
	hdc_circle(int(1025 * xScale), int(400 * yScale), int(25 * (xScale + yScale) / 2), true, 2, COLOR_NORMAL);	//下F
	hdc_rectangle(int( 925 * xScale), int(150 * yScale), int(75 * xScale), int(50 * yScale), 0, true, 2, COLOR_NORMAL);	//左上PgUp
	hdc_rectangle(int(1050 * xScale), int(150 * yScale), int(75 * xScale), int(50 * yScale), 0, true, 2, COLOR_NORMAL);	//左下PgDn
	draw_rotate_ellipse(true); //旋转的椭圆
}

//按下上方向键的动画
void draw_graghics_up()
{
	double xScale = WindowXSize2 / 145.0, yScale = WindowYSize2 / 35.0;
	//Sleep(15); //在我的电脑上的新版控制台必须要加上这句语句，否则图形画不全，原因不明，但旧版控制台不会出现这个现象
	update_arrow();
	hdc_rectangle(int(75 * xScale), int(200 * yScale), int(50 * xScale), int(47 * yScale), 0, true, 2, COLOR_CLICKED);
	hdc_line(int(100 * xScale), int(205 * yScale), int(100 * xScale), int(230 * yScale), 4, RGB(0, 0, 0));
	Sleep(TextBoxDelay1);
	hdc_rectangle(int(75 * xScale), int(200 * yScale), int(50 * xScale), int(47 * yScale), 0, true, 2, COLOR_NORMAL);
	hdc_line(int(100 * xScale), int(205 * yScale), int(100 * xScale), int(230 * yScale), 4, RGB(0, 0, 0));
}

//按下下方向键的动画
void draw_graghics_down()
{
	double xScale = WindowXSize2 / 145.0, yScale = WindowYSize2 / 35.0;
	//Sleep(15); //在我的电脑上的新版控制台必须要加上这句语句，否则图形画不全，原因不明，但旧版控制台不会出现这个现象
	update_arrow();
	hdc_rectangle(int(75 * xScale), int(303 * yScale), int(50 * xScale), int(50 * yScale), 0, true, 2, COLOR_CLICKED);
	hdc_line(int(100 * xScale), int(320 * yScale), int(100 * xScale), int(345 * yScale), 4, RGB(0, 0, 0));
	Sleep(TextBoxDelay1);
	hdc_rectangle(int(75 * xScale), int(303 * yScale), int(50 * xScale), int(50 * yScale), 0, true, 2, COLOR_NORMAL);
	hdc_line(int(100 * xScale), int(320 * yScale), int(100 * xScale), int(345 * yScale), 4, RGB(0, 0, 0));
}

//按下F键的动画
void draw_graphics_F()
{
	double xScale = WindowXSize2 / 145.0, yScale = WindowYSize2 / 35.0;
	hdc_circle(int(1025 * xScale), int(400 * yScale), int(25 * (xScale + yScale) / 2), true, 2, COLOR_CLICKED); //下F
	update_arrow();
	Sleep(TextBoxDelay3);
	hdc_circle(int(1025 * xScale), int(400 * yScale), int(25 * (xScale + yScale) / 2), true, 2, COLOR_NORMAL); //下F
}

//按下S键的动画
void draw_graphics_S()
{
	double xScale = WindowXSize2 / 145.0, yScale = WindowYSize2 / 35.0;
	hdc_circle(int(1100 * xScale), int(350 * yScale), int(25 * (xScale + yScale) / 2), true, 2, COLOR_CLICKED); //右S
	update_arrow();
	Sleep(TextBoxDelay3 / 2);
	hdc_circle(int(1100 * xScale), int(350 * yScale), int(25 * (xScale + yScale) / 2), true, 2, COLOR_NORMAL); //右S
}

//按下PageUp键的动画
void draw_graphics_PgUp()
{
	double xScale = WindowXSize2 / 145.0, yScale = WindowYSize2 / 35.0;
	hdc_rectangle(int( 925 * xScale), int(150 * yScale), int(75 * xScale), int(50 * yScale), 0, true, 2, COLOR_CLICKED);	//左上PgUp
	Sleep(TextBoxDelay2);
	update_arrow();
	hdc_rectangle(int( 925 * xScale), int(150 * yScale), int(75 * xScale), int(50 * yScale), 0, true, 2, COLOR_NORMAL);		//左上PgUp
}

//按下PageDown键的动画
void draw_graphics_PgDn()
{
	double xScale = WindowXSize2 / 145.0, yScale = WindowYSize2 / 35.0;
	hdc_rectangle(int(1050 * xScale), int(150 * yScale), int(75 * xScale), int(50 * yScale), 0, true, 2, COLOR_CLICKED);	//左下PgDn
	Sleep(TextBoxDelay2);
	update_arrow();
	hdc_rectangle(int(1050 * xScale), int(150 * yScale), int(75 * xScale), int(50 * yScale), 0, true, 2, COLOR_NORMAL);		//左下PgDn
}

//绘制旋转的椭圆，clockwise为true表示顺时针旋转，false表示逆时针
void draw_rotate_ellipse(bool clockwise)
{
	const int RotateAngle = 18; //定义每次旋转的角度变化
	static int rotateCount = 0; //定义静态局部变量来记录旋转的角度
	double xScale = WindowXSize2 / 145.0, yScale = WindowYSize2 / 35.0;
	//hdc_ellipse(900, 550, 30, 15, rotateCount, false, 2, RGB(0, 0, 0));
	hdc_square(950 - 32, 550 - 32, 64, 0, true, 2, RGB(12, 12, 12)); //消除上一次绘制的椭圆，略微加大尺寸防止擦除不完全
	if (clockwise)	//顺时针旋转
		rotateCount += RotateAngle;
	else			//逆时针旋转
		rotateCount -= RotateAngle;
	if (rotateCount % 360 == 0) //满一圈消除为0，防止数据溢出
		rotateCount = 0;
	hdc_ellipse(950, 550, 30, 15, rotateCount, false, 2, COLOR_ELLIPSE);
}
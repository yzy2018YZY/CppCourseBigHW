//1850059 计1班 杨志远

#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <Windows.h>
#include <stdio.h>  
#include <stdlib.h>  
#include <conio.h>  
#include <math.h>
#include <time.h>
#include "../common/cmd_hdc_tools.h"
using namespace std;

const int ECOLORLINE = RGB(0, 0, 0);
const int ECOLORBODY = RGB(195, 140, 90);
const int ECOLORBODYBACK = RGB(102, 78, 77);
const int ECOLORFURY = RGB(247, 228, 188);
const int ECOLORFURYDARK = RGB(204, 187, 155);
const int ECOLOREAR = RGB(200, 150, 100);
const int ECOLOREARINNER = RGB(55, 35, 20);
const int ECOLORHEAD = RGB(195, 145, 95);
const int ECOLORHAIR = RGB(246, 228, 187);
const int ECOLOREYE = RGB(35, 20, 15);
const int ECOLOREYEINNER = RGB(0, 0, 0);
const int ECOLOREYEINNERROUND = RGB(100, 100, 100);
const int ECOLOREYEBELOW = RGB(94, 56, 21);
const int ECOLOREYETOP = RGB(255, 255, 255);
const int ECOLORNOSEINNER = RGB(134, 134, 134);
const int ECOLORTAIL = RGB(155, 117, 80);

static int base_x = 500;
static int base_y = 400;

static void head()
{
	hdc_triangle(base_x - 160, base_y - 155, base_x - 160, base_y - 140, base_x - 140, base_y - 140, true, 2, ECOLORHAIR);//头上的毛轮廓
	hdc_triangle(base_x - 155, base_y - 160, base_x - 150, base_y - 140, base_x - 130, base_y - 140, true, 2, ECOLORHAIR);
	hdc_triangle(base_x - 145, base_y - 160, base_x - 150, base_y - 145, base_x - 125, base_y - 145, true, 2, ECOLORHAIR);
	hdc_triangle(base_x - 125, base_y - 165, base_x - 130, base_y - 140, base_x - 110, base_y - 140, true, 2, ECOLORHAIR);
	hdc_triangle(base_x - 110, base_y - 160, base_x - 110, base_y - 130, base_x - 90, base_y - 130, true, 2, ECOLORHAIR);
	hdc_triangle(base_x - 160, base_y - 155, base_x - 160, base_y - 140, base_x - 140, base_y - 140, false, 2, ECOLORLINE);//头上的毛
	hdc_triangle(base_x - 155, base_y - 160, base_x - 150, base_y - 140, base_x - 130, base_y - 140, false, 2, ECOLORLINE);
	hdc_triangle(base_x - 125, base_y - 165, base_x - 130, base_y - 140, base_x - 110, base_y - 140, false, 2, ECOLORLINE);
	hdc_triangle(base_x - 110, base_y - 160, base_x - 110, base_y - 130, base_x - 90, base_y - 130, false, 2, ECOLORLINE);
	hdc_triangle(base_x - 145, base_y - 160, base_x - 150, base_y - 145, base_x - 125, base_y - 145, false, 2, ECOLORLINE);
	hdc_arc(base_x - 150, base_y - 50, 100 + 2, 63, 320, 2, ECOLORLINE);//脸轮廓
	hdc_arc(base_x - 150, base_y - 50, 100 + 2, 35, 40, 2, ECOLORLINE);
	hdc_arc(base_x - 150, base_y - 50, 100 + 2, 348, 355, 2, ECOLORLINE);
	hdc_ellipse(base_x - 154, base_y - 2, 93 + 1, 50 + 1, 3, false, 4, ECOLORLINE);//脸颊轮廓
	hdc_circle(base_x - 150, base_y - 50, 100, true, 2, ECOLORHEAD);//脸
	hdc_ellipse(base_x - 154, base_y - 2, 93, 50, 3, true, 2, ECOLORHEAD);//脸颊
}

static void ears()
{
	Sleep(50);
	hdc_ellipse(base_x - 212, base_y - 220, 100, 30, 78, true, 2, ECOLOREARINNER);//左耳填充
	hdc_arc(base_x - 50, base_y - 255, 200, 225, 294, 15, ECOLOREAR);//左耳左半边
	hdc_arc(base_x - 375, base_y - 190, 200, 43, 114, 15, ECOLOREAR);//左耳右半边
	hdc_arc(base_x - 50, base_y - 255, 200 + 8, 225, 296, 2, ECOLORLINE);//左耳左半边轮廓
	hdc_arc(base_x - 375, base_y - 190, 200 + 8, 42, 114, 2, ECOLORLINE);//左耳右半边轮廓
	
	Sleep(50);
	hdc_ellipse(base_x + 19, base_y - 140, 100, 30, -12, true, 2, ECOLOREARINNER);//右耳填充
	hdc_arc(base_x + 60, base_y + 20, 200, -50, 22, 15, ECOLOREAR);//右耳上半边
	hdc_arc(base_x - 20, base_y - 297, 200, 129, 201, 15, ECOLOREAR);//右耳下半边
	hdc_arc(base_x + 60, base_y + 20, 200 + 8, -50, 24, 2, ECOLORLINE);//右耳上半边轮廓
	hdc_arc(base_x - 20, base_y - 297, 200 + 8, 128, 201, 2, ECOLORLINE);//右耳下半边轮廓
}


static void eyes()
{
	hdc_ellipse(base_x - 190, base_y - 65, 30, 15, -80, true, 2, ECOLOREYE);//左眼眼底
	hdc_ellipse(base_x - 193, base_y - 52, 16, 12, -80, true, 2, ECOLOREYEBELOW);//左眼眼底
	hdc_ellipse(base_x - 190, base_y - 65, 16, 8, -80, true, 2, ECOLOREYEINNER);//左眼瞳孔
	hdc_ellipse(base_x - 190, base_y - 85, 8, 5, -80, true, 2, ECOLOREYETOP);//左眼高光
	hdc_ellipse(base_x - 190, base_y - 85, 9, 6, -80, false, 1, ECOLOREYEINNERROUND);//左眼高光外轮廓
	hdc_ellipse(base_x - 190, base_y - 65, 30, 15, -80, false, 4, ECOLORLINE);//左眼轮廓
	hdc_arc(base_x - 205, base_y - 80, 40, 55, 80, 1, ECOLORLINE);//左眉毛

	hdc_ellipse(base_x - 110, base_y - 52, 30, 15, -80, true, 2, ECOLOREYE);//右眼眼底
	hdc_ellipse(base_x - 113, base_y - 39, 16, 12, -80, true, 2, ECOLOREYEBELOW);//右眼眼底
	hdc_ellipse(base_x - 110, base_y - 52, 16, 8, -80, true, 2, ECOLOREYEINNER);//右眼瞳孔
	hdc_ellipse(base_x - 110, base_y - 72, 8, 5, -80, true, 2, ECOLOREYETOP);//右眼高光
	hdc_ellipse(base_x - 110, base_y - 72, 9, 6, -80, false, 1, ECOLOREYEINNERROUND);//右眼高光外轮廓
	hdc_arc(base_x - 89, base_y - 61, 40, -60, -35, 1, ECOLORLINE);//右眉毛
	hdc_ellipse(base_x - 110, base_y - 52, 30, 15, -80, false, 4, ECOLORLINE);//右眼轮廓
}

static void nose()
{
	hdc_triangle(base_x - 162, base_y - 21, base_x - 152, base_y - 20, base_x - 158, base_y - 17, true, 1, ECOLORNOSEINNER);//鼻子填充
	hdc_triangle(base_x - 162, base_y - 21, base_x - 152, base_y - 20, base_x - 158, base_y - 17, false, 1, ECOLORLINE);//鼻子轮廓
}

static void mouth()
{
	hdc_set_pencolor(ECOLORLINE);//设置画笔为线条模式
	hdc_arc(base_x - 165, base_y + 60, 50, -5, 22, 1);//中间
	hdc_arc(base_x - 170, base_y - 13, 25, 180, 220, 1);//左边
	hdc_arc(base_x - 147, base_y - 10, 25, 155, 180, 1);//右边
}


static void body()
{
	hdc_set_pencolor(ECOLORBODY);//设置画笔为填充模式

	hdc_rectangle(base_x - 230, base_y + 110, 200, 70, -9, true, 2);//前腿填充
	hdc_rectangle(base_x - 220, base_y + 180, 198, 10, -12, true, 2);
	hdc_sector(base_x - 211, base_y + 225, 30, -15, 220, true, 2);//前脚填充
	hdc_sector(base_x - 169, base_y + 230, 30, 150, 450, true, 2);
	hdc_rectangle(base_x - 215, base_y + 220, 69, 15, 20, true, 2);
	hdc_triangle(base_x - 218, base_y + 191, base_x - 152, base_y + 250, base_x - 122, base_y + 170, true, 2);
	hdc_sector(base_x - 58, base_y + 215, 30, 150, 220, true, 2);//后脚填充
	hdc_rectangle(base_x - 73, base_y + 150, 33, 88, 0, true, 2);
	hdc_triangle(base_x - 78, base_y + 160, base_x - 25, base_y + 150, base_x - 73, base_y + 170, true, 2);
	hdc_triangle(base_x - 25, base_y + 150, base_x - 38, base_y + 230, base_x - 75, base_y + 170, true, 2);
	hdc_triangle(base_x - 144, base_y + 230, base_x - 123, base_y + 230, base_x - 91, base_y + 164, true, 2, ECOLORBODYBACK);//后腿填充
	hdc_triangle(base_x - 123, base_y + 230, base_x - 90, base_y + 190, base_x - 91, base_y + 164, true, 2, ECOLORBODYBACK);
	hdc_triangle(base_x - 122, base_y + 170, base_x - 144, base_y + 230, base_x - 91, base_y + 164, true, 2, ECOLORBODYBACK);

	hdc_set_pencolor(ECOLORLINE);//设置画笔为线条模式
	
	hdc_line(base_x - 230, base_y + 110, base_x - 215, base_y + 210, 2);//前右腿轮廓
	hdc_arc(base_x - 274, base_y + 210, 60, 85, 130, 2);//前右腿左轮廓
	hdc_arc(base_x - 210, base_y + 225, 30, 150, 220, 2);//前右脚轮廓
	hdc_arc(base_x - 432, base_y + 180, 250, 75, 107, 2);//前腿中轮廓
	hdc_arc(base_x - 169, base_y + 230, 30, 150, 227, 2);//前左脚轮廓
	hdc_line(base_x - 156, base_y + 257, base_x - 115, base_y + 150, 2);//前左腿轮廓
	hdc_line(base_x - 122, base_y + 170, base_x - 75, base_y + 160, 2);//身躯轮廓
	hdc_line(base_x - 143, base_y + 230, base_x - 123, base_y + 230, 2);//后右腿轮廓
	hdc_line(base_x - 123, base_y + 230, base_x - 90, base_y + 190, 2);
	hdc_line(base_x - 90, base_y + 190, base_x - 91, base_y + 164, 2);
	hdc_arc(base_x - 220, base_y + 200, 150, 73, 106, 2);//后右腿轮廓
	hdc_arc(base_x - 58, base_y + 215, 30, 150, 220, 2);//后右脚轮廓
	hdc_arc(base_x - 325, base_y + 140, 300, 77, 110, 2);//屁股轮廓

	hdc_circle(base_x - 230, base_y + 50, 50 + 2, false, 2);//左上的毛轮廓
	hdc_circle(base_x - 90, base_y + 60, 60 + 2, false, 2);//右上的毛轮廓
	hdc_circle(base_x - 170, base_y + 60, 80 + 2, false, 2);//正上的毛轮廓
	hdc_triangle(base_x - 120, base_y + 110, base_x - 220, base_y + 110, base_x - 180, base_y + 160, false, 4);//三角形的毛轮廓
	hdc_circle(base_x - 230, base_y + 50, 50, true, 2, ECOLORFURY);//左上的毛
	hdc_circle(base_x - 90, base_y + 60, 60, true, 2, ECOLORFURY);//右上的毛
	hdc_circle(base_x - 170, base_y + 60, 80, true, 2, ECOLORFURY);//正上的毛
	hdc_triangle(base_x - 120, base_y + 110, base_x - 220, base_y + 110, base_x - 180, base_y + 160, true, 2, ECOLORFURY);//三角形的毛
	hdc_arc(base_x - 170, base_y + 60, 80 + 2, -120, 135, 1, ECOLORLINE);//毛的纹路
	hdc_arc(base_x - 170, base_y + 70, 50 + 2, -150, -60, 1, ECOLORLINE);//毛的纹路
	hdc_arc(base_x - 170, base_y + 70, 50 + 2, 90, 150, 1, ECOLORLINE);//毛的纹路
}


static void tail()
{
	Sleep(100);//在我的电脑上必须要加上，否则就不能显示填充内容，原因不明
	
	//填充，带渐变色
	hdc_triangle(base_x + 140, base_y - 78, base_x + 155, base_y - 78, base_x + 145, base_y - 95, true, 2, ECOLORTAIL + RGB(50, 40, 40));
	hdc_rectangle(base_x + 140, base_y -  78,  14, 4, 0, true, 2, ECOLORTAIL + RGB(45, 45, 45));
	hdc_rectangle(base_x + 138, base_y -  74,  17, 4, 0, true, 2, ECOLORTAIL + RGB(40, 40, 40));
	hdc_rectangle(base_x + 136, base_y -  70,  20, 4, 0, true, 2, ECOLORTAIL + RGB(40, 40, 40));
	hdc_rectangle(base_x + 133, base_y -  66,  24, 4, 0, true, 2, ECOLORTAIL + RGB(35, 35, 35));
	hdc_rectangle(base_x + 130, base_y -  62,  27, 4, 0, true, 2, ECOLORTAIL + RGB(35, 35, 35));
	hdc_rectangle(base_x + 128, base_y -  58,  29, 4, 0, true, 2, ECOLORTAIL + RGB(30, 30, 30));
	hdc_rectangle(base_x + 125, base_y -  54,  34, 4, 0, true, 2, ECOLORTAIL + RGB(30, 30, 30));
	hdc_rectangle(base_x + 121, base_y -  50,  38, 4, 0, true, 2, ECOLORTAIL + RGB(25, 25, 25));
	hdc_rectangle(base_x + 117, base_y -  46,  43, 4, 0, true, 2, ECOLORTAIL + RGB(25, 25, 25));
	hdc_rectangle(base_x + 112, base_y -  42,  47, 4, 0, true, 2, ECOLORTAIL + RGB(20, 20, 20));
	hdc_rectangle(base_x + 107, base_y -  38,  51, 4, 0, true, 2, ECOLORTAIL + RGB(20, 20, 20));
	hdc_rectangle(base_x + 100, base_y -  34,  58, 4, 0, true, 2, ECOLORTAIL + RGB(15, 15, 15));
	hdc_rectangle(base_x +  92, base_y -  30,  66, 4, 0, true, 2, ECOLORTAIL + RGB(15, 15, 15));
	hdc_rectangle(base_x +  84, base_y -  26,  75, 4, 0, true, 2, ECOLORTAIL + RGB(10, 10, 10));
	hdc_rectangle(base_x +  69, base_y -  22,  89, 4, 0, true, 2, ECOLORTAIL + RGB(10, 10, 10));
	hdc_rectangle(base_x +  28, base_y -  18, 132, 4, 0, true, 2, ECOLORTAIL + RGB(5, 4, 4));
	hdc_rectangle(base_x +  13, base_y -  14, 144, 4, 0, true, 2, ECOLORTAIL + RGB(5, 4, 4));
	hdc_set_pencolor(ECOLORTAIL);
	hdc_rectangle(base_x +   5, base_y -  10, 152, 4, 0, true, 2);
	hdc_rectangle(base_x -   2, base_y -   6, 160, 4, 0, true, 2);
	hdc_rectangle(base_x -   8, base_y -   2, 164, 4, 0, true, 2);
	hdc_rectangle(base_x -  12, base_y +   2, 165, 4, 0, true, 2);
	hdc_rectangle(base_x -  18, base_y +   6, 173, 4, 0, true, 2);
	hdc_rectangle(base_x -  22, base_y +  10, 175, 4, 0, true, 2);
	hdc_rectangle(base_x -  25, base_y +  14, 176, 4, 0, true, 2);
	hdc_rectangle(base_x -  30, base_y +  18, 180, 4, 0, true, 2);
	hdc_rectangle(base_x -  33, base_y +  22, 181, 4, 0, true, 2);
	hdc_rectangle(base_x -  36, base_y +  26, 180, 4, 0, true, 2);
	hdc_rectangle(base_x -  38, base_y +  30, 181, 4, 0, true, 2);
	hdc_rectangle(base_x -  38, base_y +  34, 180, 4, 0, true, 2);
	hdc_rectangle(base_x -  40, base_y +  38, 180, 4, 0, true, 2);
	hdc_rectangle(base_x -  40, base_y +  42, 178, 4, 0, true, 2);
	hdc_rectangle(base_x -  42, base_y +  44, 178, 4, 0, true, 2);
	hdc_rectangle(base_x -  44, base_y +  48, 178, 4, 0, true, 2);
	hdc_rectangle(base_x -  46, base_y +  52, 176, 4, 0, true, 2);
	hdc_rectangle(base_x -  48, base_y +  56, 174, 4, 0, true, 2);
	hdc_rectangle(base_x -  48, base_y +  60, 171, 4, 0, true, 2);
	hdc_rectangle(base_x -  48, base_y +  64, 166, 4, 0, true, 2);
	hdc_rectangle(base_x -  50, base_y +  68, 164, 4, 0, true, 2);
	hdc_rectangle(base_x -  50, base_y +  72, 159, 4, 0, true, 2);
	hdc_rectangle(base_x -  50, base_y +  76, 156, 4, 0, true, 2);
	hdc_rectangle(base_x -  50, base_y +  80, 153, 4, 0, true, 2);
	hdc_rectangle(base_x -  50, base_y +  84, 148, 4, 0, true, 2);
	hdc_rectangle(base_x -  50, base_y +  88, 139, 4, 0, true, 2);
	hdc_rectangle(base_x -  48, base_y +  92, 131, 4, 0, true, 2);
	hdc_rectangle(base_x -  48, base_y +  96, 126, 4, 0, true, 2);
	hdc_rectangle(base_x -  46, base_y + 100, 112, 4, 0, true, 2);
	hdc_rectangle(base_x -  34, base_y + 104,  92, 4, 0, true, 2);
	hdc_rectangle(base_x -  20, base_y + 108,  60, 4, 0, true, 2);

	hdc_set_pencolor(ECOLORLINE);//设置画笔为线条模式
	hdc_arc(base_x + 50, base_y + 80, 100, -105, 0, 2);//上轮廓
	hdc_arc(base_x + 50, base_y - 118, 100, 105, 180, 2);
	hdc_arc(base_x + 10, base_y - 35, 150, 65, 205, 2);//下轮廓
}

//绘制伊布  图片：https://media.52poke.com/wiki/e/e2/133Eevee.png
void hdc_draw_Eevee(int x, int y)
{
	base_x = x;
	base_y = y;
	
	tail();
	ears();
	body();
	head();
	nose();
	eyes();
	mouth();
}
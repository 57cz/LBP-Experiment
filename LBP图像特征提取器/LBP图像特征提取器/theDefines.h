#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <math.h>
#include <cstring>
#include <io.h>
#include <direct.h>

#define PI 3.1415926

typedef unsigned short UINT16;
typedef unsigned char BYTE;
typedef int DWORD; 
typedef long LONG; 
typedef short WORD;

//文件头信息
typedef struct tagBITMAPFILEHEADER {
	/* bmfh */
	UINT16 bfType;
	DWORD bfSize;
	UINT16 bfReserved1;
	UINT16 bfReserved2;
	DWORD bfOffBits;
} BITMAPFILEHEADER;
//位图头信息
typedef struct tagBITMAPINFOHEADER {
	/* bmih */
	DWORD biSize;
	LONG biWidth;
	LONG biHeight;
	WORD biPlanes;
	WORD biBitCount;
	DWORD biCompression;
	DWORD biSizeImage;
	LONG biXPelsPerMeter;
	LONG biYPelsPerMeter;
	DWORD biClrUsed;
	DWORD biClrImportant;
} BITMAPINFOHEADER;
//24位像素
typedef struct pixel {
	unsigned char r;
	unsigned char g;
	unsigned char b;
}PIXEL;
//链表用来存文件
typedef struct Node {
	FILE* fp;
	char name[260];
	Node* next;
}Node, * node;
//链表操作节点和头节点
node n, h;
//全局变量文件头
BITMAPFILEHEADER H1;
//全局变量位图头
BITMAPINFOHEADER H2;
//用来补零的全局变量
int temp = 0;
//用来判断是否要补零的变量
int zeroFlag;
//像素结构体
PIXEL P;
//fp1是原始文件，fp2是灰度文件，fp3是输出文件
FILE* fp1, * fp2, * fp3;
//接收文件名
char str1[1000];
//用来给文件名加后缀
char str2[] = "_LBP.bmp";
//程序的开始运行时间
clock_t Time1;
//子程序的开始运行时间
clock_t Time2;
//程序当前运行时间
clock_t Time3;
//子程序累计时间
clock_t TimeCount = 0;
//全程序运行时间
clock_t time_all_count = 0;


#include "theDefines.h"

//求3者最大值
BYTE MAX3(BYTE a, BYTE b, BYTE c)
{
	if (a <= b)
		if (b <= c)
			return c;
		else
			return b;
	else
		return a;
}
//求3者最小值
BYTE MIN3(BYTE a, BYTE b, BYTE c)
{
	if (a >= b)
		if (b >= c)
			return c;
		else
			return b;
	else
		return a;
}

//创建链表节点并返回
node creat() {
	node n = (node)malloc(sizeof(Node));
	n->fp = 0;
	n->next = 0;
	return n;
}
//给指定节点加上后继元
void addnode(node n) {
	node n2 = creat();
	n->next = n2;
}
//摧毁链表
void Freeall(node n)
{
	while (n->next)
	{
		node p = n->next;
		free(n);
		n = p;
	}
}
//用来查找BMP文件的函数
void listFiles(const char* dir)
{
	intptr_t handle;
	_finddata_t findData;

	handle = _findfirst(dir, &findData);    // 查找目录中的第一个文件
	if (handle == -1)
	{
		printf("Failed to find first file!\n");
		return;
	}
	do
	{
		if (findData.attrib & _A_SUBDIR
			|| strcmp(findData.name, ".") == 0
			|| strcmp(findData.name, "..") == 0
			);
		else
		{
			if (strcmp(findData.name + strlen(findData.name) - 4, ".bmp") == 0)
			{
				printf("%s\n", findData.name);
				n->fp = fopen(findData.name, "rb");
				strcpy(n->name, findData.name);
				addnode(n);
				n = n->next;
			}
		}
	} while (_findnext(handle, &findData) == 0);    // 查找目录中的下一个文件
	printf("已经找到所有bmp文件\n\n");
	_findclose(handle);    // 关闭搜索句柄
	n = h;
}
//3种不同的模式，将彩色像素变为灰度像素
void pixel_trans(PIXEL* P, int type)
{
	BYTE avg;
	if (type == 1)
	{
		avg = (P->r + P->g + P->b) / 3;
		P->r = avg;
		P->g = avg;
		P->b = avg;
	}
	if (type == 2)
	{
		avg = (MAX3(P->r, P->g, P->b) + MIN3(P->r, P->g, P->b)) / 2;
		P->r = avg;
		P->g = avg;
		P->b = avg;
	}
	if (type == 3)
	{
		avg = 0.3 * P->r + 0.59 * P->g + 0.11 * P->b;
		P->r = avg;
		P->g = avg;
		P->b = avg;
	}
}

//将BMP变成灰度图
int BMP_color_to_gray(FILE* fp, int type)
{
	Time2 = clock();
	char c[2];	//临时数组用来读取BM
	fread(c, 1, 2, fp);	//读取前两个字符

	//检查文件是否为BMP
	if (!(c[0] == 'B' && c[1] == 'M'))
	{
		printf("这不是BMP文件，或者文件已经损坏。 -------- ");
		return 0;
	}
	else
		printf("已确认为BMP文件。\n");	//即使这样，也可能某些文件前两个字符刚好是BM
	rewind(fp);	//检查完毕，文件指针归位

	FILE* fp_out = fopen("grayTemp.bmp", "wb");		//临时文件，作为转换中间体，但不知道怎么删除
	fread(&H1, sizeof(BITMAPFILEHEADER), 1, fp);	//这里要加保护
	fread(&H2, sizeof(BITMAPINFOHEADER), 1, fp);
	if (feof(fp)) 
	{
		printf("文件格式不正确，或者文件已经损坏。 -------- ");	//明明是BMP为什么到这里就结束了？损坏？
		return 0;
	}
	if (H2.biBitCount != 24)
	{
		printf("这不是24位BMP文件，或者文件已经损坏。 -------- "); //也可能是其他BMP，或根本不是BMP
		return 0;
	}
	else
		printf("已确认为24位BMP文件。\n");

	zeroFlag = H2.biWidth * 3 % 4;	//文件正确读取，判断要不要补零（注意，这是个全局变量）
	fwrite(&H1, sizeof(BITMAPFILEHEADER), 1, fp_out);	//写入头信息
	fwrite(&H2, sizeof(BITMAPINFOHEADER), 1, fp_out);
	printf("正在转换为灰度图……\n");

	for (int i = 0; i < H2.biHeight; i++)
	{
		for (int j = 0; j < H2.biWidth; j++)
		{
			fread(&P, sizeof(PIXEL), 1, fp);
			if (feof(fp))
			{
				printf("文件格式不正确，或者文件已经损坏。 -------- ");	//明明是BMP为什么到这里就结束了？损坏？
				return 0;
			}
			pixel_trans(&P, type);
			fwrite(&P, sizeof(PIXEL), 1, fp_out);
		}
		if (zeroFlag)	//看看要不要补零
		{
			fread(&temp, 1, 4-zeroFlag, fp);
			fwrite(&temp, 1, 4-zeroFlag, fp_out);
		}
	}
	fclose(fp);
	fclose(fp_out);	//关闭2号中间体文件
	Time3 = clock();
	printf("灰度图转换完成。		耗时：%dms\n", Time3 - Time2);
	TimeCount += Time3 - Time2;
	return 1;	//程序正确执行完毕

	//执行完以后，全局变量中存着fp的信息，接下来的操作直接用
}

//按图片大小建立一个动态矩阵
BYTE** BYTE_matrix_create()
{
	BYTE** pm = (BYTE**)malloc(H2.biHeight * sizeof(BYTE*));	//为每一层首元素分配一个地址（连续的）
	if (!pm)
	{
		printf("可用内存不足\n");
		system("pause");
		exit(-1);
	}
	for (int i = 0; i < H2.biHeight; i++)
	{
		pm[i] = (BYTE*)malloc(H2.biWidth * sizeof(BYTE));	//为每个首元素分配这一列的存储空间（不连续的）
		if (!pm[i])
		{
			printf("可用内存不足\n"); 
			system("pause");
			exit(-1);
		}
	}
	return pm;	//返回首元素地址
}

//由灰度BMP创建单像素矩阵							
BYTE** BMP_pixel_matrix_create(FILE* fp)
{
	Time2 = clock();
	fseek(fp, 54L, 0);	//直接位移到像素部分
	
	printf("正在创建像素矩阵……\n");
	BYTE** pm = BYTE_matrix_create();	//建立一个动态二维数组

	printf("正在将文件写入矩阵……\n");
	for (int i = 0; i < H2.biHeight; i++)	//将文件中的像素写入数组
	{
		for (int j = 0; j < H2.biWidth; j++)
		{
			fread(&pm[i][j], 1, 1, fp);	//读第一个字节
			fseek(fp, 2L, 1);	//跳过后两个字节
		}
		if (zeroFlag)
			fread(&temp, 1, 4-zeroFlag, fp); //补零
	}
	Time3 = clock();
	printf("文件写入完成。			耗时：%dms\n", Time3 - Time2);
	TimeCount += Time3 - Time2;
	return pm;
}

//将单像素矩阵转为灰度BMP							 
int pixel_matrix_to_BMP(BYTE** pm, FILE* fp)	
{
	Time2 = clock();
	fwrite(&H1, sizeof(BITMAPFILEHEADER), 1, fp);	//写入文件头信息
	fwrite(&H2, sizeof(BITMAPINFOHEADER), 1, fp);

	printf("正在将LBP矩阵转换为LBP图谱……\n");
	for (int i = 0; i < H2.biHeight; i++)
	{
		for (int j = 0; j < H2.biWidth; j++)
		{
			P.r = P.g = P.b = pm[i][j];	//使像素为灰度像素
			fwrite(&P, sizeof(PIXEL), 1, fp);	//写入像素
		}
		if (zeroFlag)
			fwrite(&temp, 1, 4-zeroFlag, fp);	//补零
	}
	fclose(fp);	//关闭3号最终输出文件
	Time3 = clock();
	printf("LBP图谱转换完成。		耗时：%dms\n", Time3 - Time2);
	TimeCount += Time3 - Time2;
	return 1;
}

//原始LBP算法 (别用了）
BYTE** Origin_LBP(BYTE** pm)
{
	Time2 = clock();
	printf("正在创建LBP矩阵……\n");
	BYTE** pm_LBP = BYTE_matrix_create();
	int LBP_bit_array[8];
	int LBP_value;
	int power;
	printf("正在将像素矩阵转换为LBP矩阵……\n");
	
	for(int i=0;i<H2.biHeight;i++)	//对于每个像素，判断它的周围是否在数组内，如果不在，就恒为0
		for (int j = 0; j < H2.biWidth; j++)
		{
			//每次循环开始的归零化
			LBP_value = 0;
			for (int k = 0; k < 8; k++)
				LBP_bit_array[k] = 0;
			//下
			if (i != 0) 
				if (pm[i][j] < pm[i - 1][j]) LBP_bit_array[0] = 1;
			//右下
			if (i !=0 && j!=H2.biWidth - 1)
				if(pm[i][j]< pm[i - 1][j + 1]) LBP_bit_array[1] = 1;
			//右
			if (j != H2.biWidth - 1)
				if (pm[i][j] < pm[i][j + 1]) LBP_bit_array[2] = 1;
			//右上
			if (i != H2.biHeight - 1 && j != H2.biWidth - 1)
				if (pm[i][j] < pm[i + 1][j + 1]) LBP_bit_array[3] = 1;
			//上
			if (i != H2.biHeight - 1)
				if (pm[i][j] < pm[i + 1][j]) LBP_bit_array[4] = 1;
			//左上
			if (i != H2.biHeight - 1 && j != 0)
				if (pm[i][j] < pm[i + 1][j - 1]) LBP_bit_array[5] = 1;
			//左
			if (j != 0)
				if (pm[i][j] < pm[i][j - 1]) LBP_bit_array[6] = 1;
			//左下
			if (i != 0 && j != 0)
				if (pm[i][j] < pm[i - 1][j - 1]) LBP_bit_array[7] = 1;

			
			power = 1;
			for (int k = 0; k < 8; k++)	//计算LBP值
			{
				LBP_value += power * LBP_bit_array[k];
				power *= 2;
			}
			pm_LBP[i][j] = LBP_value;	//LBP值计入新的矩阵
		}
	Time3 = clock();
	printf("LBP矩阵转换完成。		耗时：%dms\n", Time3 - Time2);
	TimeCount += Time3 - Time2;
	return pm_LBP;	//返回LBP矩阵，还要转换为BMP
}

//LBP旋转不变算法 （不要用了）
BYTE** Rotate_LBP(BYTE** pm)
{
	Time2 = clock();
	printf("正在创建旋转不变LBP矩阵……\n");
	BYTE** pm_LBP = BYTE_matrix_create();
	int LBP_bit_array[8];
	int LBP_value;
	int power;
	int count;	//当count==8就停下，归零
	int min;	//LBP的最小值存在这里
	printf("正在将像素矩阵转换为LBP矩阵……\n");

	for (int i = 0; i < H2.biHeight; i++)	//对于每个像素，判断它的周围是否在数组内，如果不在，就恒为0
		for (int j = 0; j < H2.biWidth; j++)
		{
			//每次循环开始的归零化
			for (int k = 0; k < 8; k++)
				LBP_bit_array[k] = 0;
			//下
			if (i != 0)
				if (pm[i][j] < pm[i - 1][j]) LBP_bit_array[0] = 1;
			//右下
			if (i != 0 && j != H2.biWidth - 1)
				if (pm[i][j] < pm[i - 1][j + 1]) LBP_bit_array[1] = 1;
			//右
			if (j != H2.biWidth - 1)
				if (pm[i][j] < pm[i][j + 1]) LBP_bit_array[2] = 1;
			//右上
			if (i != H2.biHeight - 1 && j != H2.biWidth - 1)
				if (pm[i][j] < pm[i + 1][j + 1]) LBP_bit_array[3] = 1;
			//上
			if (i != H2.biHeight - 1)
				if (pm[i][j] < pm[i + 1][j]) LBP_bit_array[4] = 1;
			//左上
			if (i != H2.biHeight - 1 && j != 0)
				if (pm[i][j] < pm[i + 1][j - 1]) LBP_bit_array[5] = 1;
			//左
			if (j != 0)
				if (pm[i][j] < pm[i][j - 1]) LBP_bit_array[6] = 1;
			//左下
			if (i != 0 && j != 0)
				if (pm[i][j] < pm[i - 1][j - 1]) LBP_bit_array[7] = 1;

			min = 255;
			for (int k = 0; k < 8; k++)
			{
				power = 1;
				LBP_value = 0;
				for (int m = k, count = 0; count < 8; m++, count++)
				{
					LBP_value += power * LBP_bit_array[m % 8];
					power *= 2;
				}
				if (LBP_value < min)
					min = LBP_value;
			}
			pm_LBP[i][j] = min*3;
		}
	Time3 = clock();
	printf("LBP矩阵转换完成。		耗时：%dms\n", Time3 - Time2);
	TimeCount += Time3 - Time2;
	return pm_LBP;	//返回LBP矩阵，还要转换为BMP
}

//LBP等价模式算法 （求求你别用了）
BYTE** Equal_LBP(BYTE** pm)
{
	Time2 = clock();
	printf("正在创建等价LBP矩阵……\n");
	BYTE** pm_LBP = BYTE_matrix_create();
	int LBP_bit_array[8];
	int LBP_value;
	int power;
	printf("正在将像素矩阵转换为LBP矩阵……\n");

	for (int i = 0; i < H2.biHeight; i++)	//对于每个像素，判断它的周围是否在数组内，如果不在，就恒为0
		for (int j = 0; j < H2.biWidth; j++)
		{
			//每次循环开始的归零化
			LBP_value = 0;
			for (int k = 0; k < 8; k++)
				LBP_bit_array[k] = 0;
			//下
			if (i != 0)
				if (pm[i][j] < pm[i - 1][j]) LBP_bit_array[0] = 1;
			//右下
			if (i != 0 && j != H2.biWidth - 1)
				if (pm[i][j] < pm[i - 1][j + 1]) LBP_bit_array[1] = 1;
			//右
			if (j != H2.biWidth - 1)
				if (pm[i][j] < pm[i][j + 1]) LBP_bit_array[2] = 1;
			//右上
			if (i != H2.biHeight - 1 && j != H2.biWidth - 1)
				if (pm[i][j] < pm[i + 1][j + 1]) LBP_bit_array[3] = 1;
			//上
			if (i != H2.biHeight - 1)
				if (pm[i][j] < pm[i + 1][j]) LBP_bit_array[4] = 1;
			//左上
			if (i != H2.biHeight - 1 && j != 0)
				if (pm[i][j] < pm[i + 1][j - 1]) LBP_bit_array[5] = 1;
			//左
			if (j != 0)
				if (pm[i][j] < pm[i][j - 1]) LBP_bit_array[6] = 1;
			//左下
			if (i != 0 && j != 0)
				if (pm[i][j] < pm[i - 1][j - 1]) LBP_bit_array[7] = 1;

			for (int k = 0; k < 8; k++)
			{
				if (LBP_bit_array[k] != LBP_bit_array[(k + 1) % 8])
					LBP_value+=31;
			}
			pm_LBP[i][j] = LBP_value;	//LBP值计入新的矩阵
		}
	Time3 = clock();
	printf("LBP矩阵转换完成。		耗时：%dms\n", Time3 - Time2);
	TimeCount += Time3 - Time2;
	return pm_LBP;	//返回LBP矩阵，还要转换为BMP
}

//圆形LBP （就用这个）
BYTE** Circle_LBP(BYTE** pm, float r, int n, int choice)
{
	//条件判断
	{		
		if (!(choice >= 1 && choice <= 3))
		{
			printf("\n******** 输入的模式有误，模式已修改：3.等价模式 ********\n\n");
			choice = 3;
		}
		if (r < 1.0)
		{
			printf("\n******** 半径不得小于1.000000，半径已修改：1.000000 ********\n\n");
			r = 1;
		}
		if (n < 8)
		{
			printf("\n******** 取样点数不得小于8，取样点数已修改：8 ********\n\n");
			n = 8;
		}
		if (n > 8 && choice != 3)
		{
			printf("\n******** 只有等价模式下取样点可以大于8，取样点数已修改：8 ********\n\n");
			n = 8;
		}
		if (n > 255)
		{
			printf("\n******** 取样点数不得大于255，取样点数已修改：255 ********\n\n");
			n = 255;
		}
	}
	Time2 = clock();
	printf("正在创建圆形LBP矩阵……\n");
	BYTE** pm_LBP = BYTE_matrix_create();

	int* LBP_bit_array=(int*)malloc(4*n);
	int LBP_value;
	int power;	//2的power-1次幂
	float x, y;	//取样点坐标
	float value; //取样点值	
	float a, b, c, d; //线性插值参数
	int count;	//旋转不变的计数
	int min;	//旋转不变最小值

	printf("正在将像素矩阵转换为LBP矩阵……\n");

	for (int i = 0; i < H2.biHeight; i++)	//对于每个像素，判断它的周围是否在数组内，如果不在，就恒为0
		for (int j = 0; j < H2.biWidth; j++)
		{
			//每次循环开始的归零化
			for (int k = 0; k < n; k++)
				LBP_bit_array[k] = 0;

			//对n个取样点一一处理
			for (int k = 0; k < n; k++)
			{
				//确立取样点坐标
				x = j + r * cos(2 * PI * k / n);
				y = i + r * sin(2 * PI * k / n);
				
				//判断点在不在区域内
				if (!(x >= 0 && y >= 0 && x < H2.biWidth - 1 && y < H2.biHeight - 1))
					;	//什么都不做
				else //在里面
				{	
					//获得参数
					a = x - (int)x; b = 1 - a; c = y - (int)y; d = 1 - c; 
					//进行线性插值算取样点的值
					value = d * (a * pm[(int)y][(int)x + 1] + b * pm[(int)y][(int)x]) + c * (a * pm[(int)y + 1][(int)x + 1] + b * pm[(int)y + 1][(int)x]);
					//比较取样点和中心点
					if (pm[i][j] < value)
						LBP_bit_array[k] = 1;
				}
			}
			//根据模式计算LBP值
			if (choice == 1)	//原始LBP
			{
				LBP_value = 0;
				power = 1;
				for (int k = 0; k < n; k++)	//计算LBP值
				{
					LBP_value += power * LBP_bit_array[k];
					power *= 2;
				}
				pm_LBP[i][j] = LBP_value;	//LBP值计入新的矩阵
			}
			if (choice == 2)	//LBP旋转不变
			{
				min = 255;
				for (int k = 0; k < n; k++)
				{
					power = 1;
					LBP_value = 0;
					for (int m = k, count = 0; count < n; m++, count++)
					{
						LBP_value += power * LBP_bit_array[m % n];
						power *= 2;
					}
					if (LBP_value < min)
						min = LBP_value;
				}
				pm_LBP[i][j] = min;
			}
			if (choice == 3)	//LBP等价
			{
				LBP_value = 0;
				for (int k = 0; k < n; k++)
				{
					if (LBP_bit_array[k] != LBP_bit_array[(k + 1) % n])
						LBP_value += (int)(255/n);
				}
				pm_LBP[i][j] = LBP_value;	//LBP值计入新的矩阵
			}
		}
	Time3 = clock();
	printf("LBP矩阵转换完成。		耗时：%dms\n", Time3 - Time2);
	TimeCount += Time3 - Time2;
	return pm_LBP;	//返回LBP矩阵，还要转换为BMP
}
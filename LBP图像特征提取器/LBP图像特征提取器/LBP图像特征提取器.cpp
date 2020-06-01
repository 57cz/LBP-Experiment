#pragma pack(1)	//用来阻止结构体对齐
#include "functions.h"

//结构体定义和全局变量在theDefines.h中
//函数在functions.h中

BYTE** (*pointer[3])(BYTE**) = { Origin_LBP, Rotate_LBP, Equal_LBP };
int choice, pointNumbers;
float radius;
int main()
{
START:
	remove("grayTemp.bmp");		//清空临时文件
	if(h)Freeall(h);			//清空并新建链表
	h = n = creat();	
	Time1 = Time2 = Time3 = TimeCount = time_all_count = 0;
	printf("LBP图谱生成器，输入图像格式为24位BMP文件\n将所有要转换的BMP文件放在程序文件夹内\n完成后 ");
	system("pause");	//给用户时间以将文件放进去
	listFiles(".\\*.*");

	printf("请输入操作类型（1.普通圆形LBP 2.旋转不变圆形LBP 3.等价模式圆形LBP）：");
	scanf("%d", &choice);
	printf("请输入取样半径（不小于1的浮点数）：");
	scanf("%f", &radius);
	printf("请输入取样点数（不小于8的整数）：");
	scanf("%d", &pointNumbers);
	printf("\n\n");
	mkdir(".\\LBP_Process_OutPut");	//新建文件夹，存储输出文件
	Time1 = clock();

	//-------------------------------输入输出结束----------------------------- 分割线 -------------------------文件操作开始----------------------

	while (n->next)
	{
		fp1 = n->fp;	
		//将文件转换为灰度图，文件名：grayTemp.bmp
		if (!BMP_color_to_gray(fp1, 3))
		{
			printf("%s\n\n", n->name);	//标出有问题的文件
			fcloseall();				//关闭所有文件，以便用户不关掉程序就可以删除有问题的文件
			goto START;
		}

		//打开转换后的灰度图文件
		if (!(fp2 = fopen("grayTemp.bmp", "rb"))) 
		{
			printf("发生未知错误\n");	//数据结构C上这么写意味着有时发生这种错误？
			system("pause");
			exit(-1);
		} 
		
		//建立矩阵用于存储像素点以方便读取（也许也可以用fp2进行随机读取）
		BYTE** pm = BMP_pixel_matrix_create(fp2);  

		//对这个矩阵进行LBP操作得到一个新矩阵
		//BYTE** pm_LBP = pointer[choice-1](pm);	//3种方形LBP算子
		BYTE** pm_LBP = Circle_LBP(pm, radius, pointNumbers, choice);	//3种圆形LBP算子

		//给文件名改名为：文件名_LBP.bmp
		n->name[strlen(n->name) - 4] = 0; //去掉.bmp
		strcat(n->name, "_LBP.bmp");	//加上_LBP.bmp

		char direct[260]="LBP_Process_OutPut\\";	//把输出文件名和文件夹联系起来
		strcat(direct, n->name);	

		//创建最终输出文件
		if (!(fp3 = fopen(direct, "wb")))
		{
			printf("发生未知错误\n");
			system("pause"); 
			exit(-1);
		}

		//将LBP矩阵转换为BMP文件
		pixel_matrix_to_BMP(pm_LBP, fp3); 

		Time3 = clock();
		printf("\n输出的LBP图谱名为：\t\t%s\n\n\t\t      子程序运行时间：%dms\n\t\t\t    杂余时间：%dms\n\t\t\t  总运行时间：%dms\n\n", n->name, TimeCount, Time3-Time1-TimeCount, Time3 - Time1);
	
		time_all_count += Time3 - Time1;
		n = n->next;
	}
	fcloseall();	//先确保所有的文件被关闭，再删除中间文件,才能删掉
	remove("grayTemp.bmp");

	printf("全部图像处理完毕，总运行时间：%dms\n", time_all_count);
	system("pause");
	return 0;
}





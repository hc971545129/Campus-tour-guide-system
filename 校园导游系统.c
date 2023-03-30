#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<conio.h>

#define MAXVEX 30       //最大顶点个数
#define INFINITY 32768  //无穷大

typedef struct
{
	int No;           //地点序号
	char name[MAXVEX];//地点名称
}Vextype;//顶点类型
typedef struct
{
	int arcs[MAXVEX][MAXVEX];  //边集
	Vextype vex[MAXVEX];       //顶点集
	int vexnum;                //顶点数目
	int arcnum;                //边数目
}AdjMatrix;     //邻接矩阵



/* 函数调用超前声明 */
int Login(char mima[]);		/* 登录 */
void Content(void);			/* 目录 */
void Map();                 /* 平面图显示 */
void Introduce();           /* 地点介绍 */
void Create(AdjMatrix *G);  /* 创建 */
int Locate(AdjMatrix *G,char name[]);/* 根据地点名生成地点序号 */
int Read_Check(AdjMatrix *G);  /*检测函数,判断文件是否为空;也可作为读取文件操作函数*/
int Write_Check(AdjMatrix *G);  /*写入文件操作函数*/
int Display(AdjMatrix *G);  /* 打印现有顶点关联信息 */
void Add(AdjMatrix *G);     /* 添加路线信息 */
int Serach(AdjMatrix *G);   /* 查询任意地点的相关信息 */
void Shortcut(AdjMatrix *G);/* 最短路线 */
void Dijkstra(AdjMatrix *G,int start,int end,int dist[],int path[][MAXVEX]);/* 最短路线 */
void Double_All(AdjMatrix *G);  //任意两个地点之间所有简单路径
void DFS(AdjMatrix *G,int num1,int num2);


int main()		/* 主函数 */
{
	char mima[15] =  {"161210"};	/* 设置初始密码 */
	if(Login(mima))
	{
		printf("\t登录成功!按任意键继续...");
		getch();
		Content();  /* 主界面 */
	}
	else
		printf("\t密码错误!\n");
	return 0;
}

int Login(char mima[] /*密码*/)	/* 登录 */
{
	int i;
	char ch;
	char jiami[15];		/* 加密 */
	char input[15];		/* 输入 */
	char change[15];	/* 更改 */
	FILE *fp;
	fp = fopen("mima.txt","wt");	/* 打开只写文件 */
	if(fp==NULL)
	{
		printf("\n无法打开mima.txt!");
		getch();
		exit(1);
	}
	strcpy(jiami,mima);
	for(i=0; jiami[i]!='\0' && i<15; i++)
		jiami[i] += 12;
	fputs(jiami,fp);		/* 将简单加密后的密码存入文件 */
	fclose(fp);		/* 关闭文件 */
	printf("\n\n\n");
	printf("\t★登录★\n");
	printf("\t☆密码☆<初始为161210>:");
	scanf("%s",input);
	if(!strcmp(mima,input))
	{
		printf("\t登录成功!\n\n\t是否更改密码?\n\t是=Y,否=N(不必区分大小写):");
		getchar();
		scanf("%c",&ch);
		if(ch=='Y'||ch=='y')	/* 需更改密码 */
		{
			printf("\t原密码:");
			scanf("%s",change);
			while(strcmp(change,input))	/* 原密码输入错误 */
			{
				printf("\t原密码输入错误,请重试!\n");
				printf("\t原密码:");
				scanf("%s",change);
			}
			if(!strcmp(change,input))	/* 原密码输入正确 */
			{
				printf("\t新密码:");
				scanf("%s",mima);
				printf("\t密码修改成功!\n");
				printf("\t任意键重新登录...\n");
				getch();
				system("cls");
				Login(mima);
			}
		}
		else if(ch=='N'||ch=='n')	/* 无需更改密码 */
			return 1;
		else
		{
			printf("\t输入出错!\n");
			exit(1);
		}
	}
	else
		return 0;
}

void Content()	/* 目录 */
{
	int i,j;
	AdjMatrix G;
	int order;
	
	Create(&G);
	do
	{
		system("cls");
		printf("\n\n\n");
		printf("\t\t\t     \001 --- 您好!欢迎使用西安邮电大学校园导游系统! --- \001\n\n");
		printf("\t\t\t---                ☆   0.退出系统!     ☆                ---\n");
		printf("\t\t\t---             ☆   1.添加新的路线!       ☆             ---\n");
		printf("\t\t\t---          ☆   2.显示校园的基本信息!       ☆          ---\n");
		printf("\t\t\t---       ☆   3.查询某个地点的路线相关信息!     ☆       ---\n");
		printf("\t\t\t---    ☆   4.查询任意两个地点之间所有简单路径!     ☆    ---\n");
		printf("\t\t\t--- ☆   5.查询从某个地点出发到另外一个地点的最短路线! ☆ ---\n");
		printf("\t请选择0-5:");
		scanf("%d", &order);
		switch (order)
		{
		case 0: printf("\t☆感谢您的使用,再见!\n"); break;
		case 1: Add(&G); break;
		case 2: Display(&G); break;
		case 3: Serach(&G); break;
		case 4: Double_All(&G); break;
		case 5: Shortcut(&G); break;
		default: printf("\t请输入0-5的整数！\n");break;
		}
	}while(order!=0);
}
void Map()
{
	printf("\n\n\t\t\t\t\t西安邮电大学长安校区俯视图\n");
	printf("\n\n\t↑南\t\t\t\t\t\t\t\t\t\t单位：m\n\n");
	printf("\t\t                                ┏━━━━━━━━━━━━━━━━━━━━┓ \n");
	printf("\t\t                                ┃西区宿舍B     学生超市   西区宿舍A      ┃ \n");
	printf("\t\t                                ┃      ╲         │80   ╱88            ┃ \n");
	printf("\t\t┏━━━━━━━━━━━┓  子  ┃     126╲   300旭日苑                  ┃ \n");
	printf("\t\t┃      教师公寓        ┃      ┃       体育馆━━┛   ╲200             ┃ \n");
	printf("\t\t┃  40 ╱   50  ╲ 60   ┃  午  ┃ 300╱     ┗━━━━━足球场  美食广场 ┃ \n");
	printf("\t\t┃ 东区浴室━━ 东升苑  ┣━━━┫  ╱           200                  │50┃ \n");
	printf("\t\t┃ 50 │     ╱100    ╲┃ 天桥 ┃               100               卫生所 ┃ \n");
	printf("\t\t┃东区宿舍楼         150┣━━━┫━━━━情人湖━━图书馆        200╱   ┃ \n");
	printf("\t\t┃    │                ┃  大  ┃ 300      │80       │       西区实验楼┃ \n");
	printf("\t\t┃    │240             ┃      ┃    大学生活动中心   │300   ╱     │  ┃ \n");
	printf("\t\t┃    │                ┃  道  ┃           180 ╲    │    ╱200    │80┃ \n");
	printf("\t\t┃ 东区实验━━东区正门 ┃      ┃                 水煮鸽子╱         │  ┃ \n");
	printf("\t\t┃  教学楼  180   180 ╲┃      ┃                  100│         B教学楼 ┃ \n");
	printf("\t\t┗━━━━━━━━━━━┛红绿灯┃           200       │    200      │50┃ \n");
	printf("\t\t                            ┃  ┃   行政楼 ━━━  西区正门 ━━ A教学楼 ┃ \n");
	printf("\t\t                            ┃  ┗━━━━━━━━━━━━━━━━━━━━┛ \n");
    printf("\t\t                        400 ┗━━━━━━━━━━━┛                       \n");
	getch();
}

void Introduce()           /* 地点介绍 */
{
	char introduce[300];
	FILE *fp;
	fp = fopen("introduce.txt","rt");	/* 只读打开顶点边数信息 */
	if(fp==NULL)
	{
		printf("\t无法打开introduce.txt!\n");
		getch();
		return;
	}
	printf("\n\t以下部分地点介绍转自西安邮电大学吧!\n");
	while(fscanf(fp,"%s\n",introduce)!=EOF)
		printf("\t%s\n",introduce);
//	printf("\n\t旭日苑：旭日苑学生餐厅，在西邮人的眼里，她既不是普通的餐厅，也不是普通的食堂，我们爱叫她'旭日苑大酒店'。");
//	printf("\n\t基础教学楼A B：A楼和B楼都不是学校华丽的存在，但你们却永远朴实的存在于我的脑海。");
//	printf("\n\t西区正门：西安邮电大学霸气的大门，大门后面是一个有着悠久历史的瞭望台，抗日战争期间鬼子就是在距西安几百公里处被我瞭望台率先发现，成功狙击的。为了纪念那段历史，这座瞭望塔被完整的保存了下来，并且加以现代化的装饰，如今已成为我西邮一大特色建筑。");
//	printf("\n\t水煮鸽子：大雁塔广场的喷泉好看？哼哼！你拿个MP3，在重要的节日里站在这里，音乐，喷泉，应有尽有！");
//	printf("\n\t情人湖：有人说北方学校没水？看看！看看这是什么！这是一滩绝望的死水，清风吹啊吹，有时候其实可以吹出涟漪");
//	printf("\n\t体育馆：有没有发现世博中国馆其实是在我西邮体育馆的基础上又加了几层？对头！中国馆原型就在这里！我大西邮威武！");
//	printf("\n\t浴室：站在吃饭还是看病的十字路口，我选择了右行。这是一个值得我们感恩的建筑，是它，让我们在西邮的四年干干净净的做人！ ---送给澡堂的水塔");
//	printf("\n\t东区实验教学楼：东区最标志性建筑 最宏伟最气势磅礴的就是教学楼。");
//	printf("\n\t东升苑：东区食堂是限时的 所以你们要抓紧时间去次饭不要墨迹 不然只能吃空气了～");
//	printf("\n\t卫生所：最有用的我觉得也就是可以给你打打针打打疫苗，有病还是不要去的好，去外边看吧……理由不解释");
//	printf("\n\t美食广场：饭还是挺香的类型很多，可以供大家选择，只是环境不是特别好……");
}

int Locate(AdjMatrix *G,char name[])    //根据地点名生成地点序号
{
	int i;
	for(i=1; i<=G->vexnum; i++)
		if(!strcmp(name,G->vex[i].name))
			return i;
	return -1;
}


int Read_Check(AdjMatrix *G)     //检测函数,判断文件是否为空;也可作为读取文件操作函数
{
	int i=1,j,temp=0;
	char temp1[30],temp2[30];
	FILE *fp;
	
	fp = fopen("vex_arc_num.txt","rt");	/* 只读打开顶点边数信息 */
	if(fp==NULL)
	{
		printf("\t无法打开vex_arc_num.txt!\n");
		getch();
		return 0;
	}
	fscanf(fp,"%d %d",&G->vexnum,&G->arcnum);  //读取顶点数和边数
	fclose(fp);
	
	fp = fopen("place_data.txt","rt");	/* 只读打开地点序号信息 */
	if(fp==NULL)
	{
		printf("\t无法打开place_data.txt!\n");
		getch();
		return 0;
	}
	while(fscanf(fp,"%d %s",&G->vex[i].No,G->vex[i].name)!=EOF)  //读取地点和序号
		i++;
	fclose(fp);
	
	fp = fopen("line_data.txt","rt");	/* 只读打开路线信息 */
	if(fp==NULL)
	{
		printf("\t无法打开line_data.txt!\n");
		getch();
		return 0;
	}
	while(fscanf(fp,"%s %s %d",temp1,temp2,&temp)!=EOF)
	{
		G->arcs[Locate(G,temp1)][Locate(G,temp2)]=temp;
		G->arcs[Locate(G,temp2)][Locate(G,temp1)]=temp;
	}
	fclose(fp);
	return 1;
}

int Write_Check(AdjMatrix *G)  /*写入文件操作函数*/
{
	int i;
	FILE *fp;

	fp = fopen("vex_arc_num.txt","wt");	/* 只写打开顶点边数信息 */
	if(fp==NULL)
	{
		printf("\t无法打开vex_arc_num.txt!\n");
		getch();
		return 0;
	}
	fprintf(fp,"%d %d",G->vexnum,G->arcnum);  //写入顶点数和边数
	fclose(fp);

	fp = fopen("place_data.txt","wt");	/* 只写打开地点序号信息 */
	if(fp==NULL)
	{
		printf("\t无法打开place_data.txt!\n");
		getch();
		return 0;
	}
	
	for(i=1; i<=G->vexnum; i++)
		fprintf(fp,"%d %s\n",G->vex[i].No,G->vex[i].name);  //写入地点和序号
	
	fclose(fp);
	return 1;
}

void Create(AdjMatrix *G)	/* 邻接矩阵创建无向图 */
{
	int i,j,k,weight;
	char place[30];     //地点
	FILE *fp;
	system("cls");
	
	if(Read_Check(G)==0)   //读取文件，如果为空，新建；
	{
		
		printf("\t无法打开vex_arc_num.txt!\n");
		printf("\t暂无任何地点信息!按任意键开始添加...\n");
		getch();
		
		fp = fopen("vex_arc_num.txt","at");	/* 追加打开顶点边数信息 */
		if(fp==NULL)
		{
			printf("\t无法打开vex_arc_num.txt!\n");
			getch();
			exit(1);
		}

		printf("\t请输入学校的地点数目和路线数目(空格分开)：");
		scanf("%d %d",&G->vexnum,&G->arcnum);
		fprintf(fp,"%d %d\n",G->vexnum,G->arcnum);      /* 将数目存入文件 */
		fclose(fp);

		fp = fopen("place_data.txt","at");	/* 追加打开地点信息 */
		if(fp==NULL)
		{
			printf("\t无法打开place_data.txt!\n");
			getch();
			exit(1);
		}
		
		for(i=1; i<G->vexnum; i++)     /* 初始化邻接矩阵全都置为无穷大 */
			for(j=1; j<G->vexnum; j++)
				G->arcs[i][j]=INFINITY;

		printf("\t请输入学校中的%d个地点及地点名：\n",G->vexnum);
		for(i=1; i<=G->vexnum; i++)
		{
			printf("\tNo.%d个地点：",i);
			G->vex[i].No=i;
			_flushall();
			scanf("%s",G->vex[i].name);
			fprintf(fp,"%d %s\n",G->vex[i].No,G->vex[i].name);      /* 将地点,序号存入文件 */
		}
		fclose(fp); /* 关闭文件 */

		fp = fopen("line_data.txt","at");	/* 追加打开路线信息 */
		if(fp==NULL)
		{
			printf("\t无法打开line_data.txt!\n");
			getch();
			exit(1);
		}

		printf("\t请输入校园图中的%d条路线：\n",G->arcnum);
		for(k=0; k<G->arcnum; k++)
		{
			printf("\tNo.%d条路线：\n",k+1);
			printf("\t起点：");
			scanf("%s",place);
			fprintf(fp,"%s ",place);    /* 将起点存入文件 */
			i=Locate(G,place);
			printf("\t终点：");
			scanf("%s",place);
			fprintf(fp,"%s ",place);    /* 将终点存入文件 */
			j=Locate(G,place);
			printf("\t距离：");
			scanf("%d",&weight);
			G->arcs[i][j]=weight;
			G->arcs[j][i]=weight;
			fprintf(fp,"%d\n",G->arcs[i][j]);    /* 将路线(权值)存入文件 */
		}
		fclose(fp); /* 关闭文件 */
		printf("\t成功录入%d个地点信息!\n\t按任意键返回...",G->vexnum);
		getch();
	}
	
	Read_Check(G);
	for(i=1; i<=G->vexnum; i++)     /* 初始化邻接矩阵全都置为无穷大 */
			for(j=1; j<=G->vexnum; j++)
				G->arcs[i][j]=INFINITY;
	Read_Check(G);
/*检测矩阵是否读入
	for(i=1; i<=G->vexnum; i++)
		for(j=1; j<=G->vexnum; j++)
		{
			printf("%6d",G->arcs[i][j]);
			if(j%G->vexnum==0)printf("\n");
		}
	getch();
*/
}

int Display(AdjMatrix *G)
{
	int i,j=1,k=0;
	system("cls");
	Map();
	printf("\n\n\n");
	printf("\t图中暂有地点%d个，边数%d条.\n",G->vexnum,G->arcnum);
	printf("\t以下是图中所有地点及其编号：\n");
	while(j<=G->vexnum)
	{
		printf("\t%-3d %-14s ",G->vex[j].No,G->vex[j].name);
		if(j%4==0)printf("\n");
		j++;
	}
	Introduce();
	printf("\n\n\t校园图路线信息如下：\n"); /* 打开成功，开始读取 */
	for(i=1; i<=G->vexnum; i++)
	{
		for(j=1; j<=i; j++)
		{
			if(G->arcs[i][j]!=INFINITY)
			{
				printf("\t%14s<-->%-14s:%5dm\t",G->vex[i].name,G->vex[j].name,G->arcs[i][j]);
				k++;
				if(k%2==0)printf("\n");
			}
		}
	}
	printf("\t按任意键继续...");
	getch();
	return 0;
}

int Serach(AdjMatrix *G)
{
	char place[30];
	char temp1[30],temp2[30],temp3[30];  //临时字符数组，用于比较
	int No,no,weight;
	FILE *fp1,*fp2;
	Map();
	printf("\t请输入要查询的地点：");
	scanf("%s",place);
	No=Locate(G,place);
	fp1 = fopen("place_data.txt","rt");	/* 只读打开地点信息 */
	if(fp1==NULL)
	{
		printf("\n\t无法打开place_data.txt!\n");
		getch();
		return 0;	/* 打开错误,无文件或无信息 */
	}
	while(fscanf(fp1,"%d %s",&no,temp3)!=EOF)
	{
		if(No==no)      //文件里存在待查地点信息
		{
			fp2 = fopen("line_data.txt","rt");	/* 只读打开路线信息 */
			if(fp2==NULL)
			{
				printf("\n\t无法打开line_data.txt!\n");
				getch();
				return 0;	/* 打开错误,无文件或无信息 */
			}
			while(fscanf(fp2,"%s %s %d",temp1,temp2,&weight)!=EOF)
				if(!strcmp(temp1,place)||!strcmp(temp2,place))  //当有顶点与待查顶点一致时，输出打印
					printf("\t%14s<-->%-14s:%5dm\n",temp1,temp2,weight);
			fclose(fp2);
		}
	}
	fclose(fp1);
	printf("\t按任意键继续...");
	getch();
	return 0;

}


void Add(AdjMatrix *G)
{
	char place[30];
	int start,end,weight;
	FILE *fp;
	fp = fopen("line_data.txt","at");	/* 追加打开路线信息 */
	if(fp==NULL)
	{
		printf("\t无法打开line_data.txt!\n");
		getch();
		return;
	}
	Map();
	printf("\t请输入添加路线的起点：");
	scanf("%s",place);
	fprintf(fp,"%s ",place);
 	start=Locate(G,place);
 	
	if(start==-1)     //生成序号大于最大，说明为新地点
	{
		G->vexnum++;
		start=G->vexnum;
		G->vex[G->vexnum].No=G->vexnum;
		strcpy(G->vex[G->vexnum].name,place);
	}
	printf("\t终点：");
	scanf("%s",place);
	fprintf(fp,"%s ",place);
	end=Locate(G,place);

	if(end==-1)       //生成序号大于最大，说明为新地点
	{
		G->vexnum++;
		end=G->vexnum;
		G->vex[G->vexnum].No=G->vexnum;
		strcpy(G->vex[G->vexnum].name,place);
	}
	G->arcnum++;
	printf("\t距离(权值)：");
	scanf("%d",&weight);
	G->arcs[start][end]=weight;
	G->arcs[end][start]=weight;
	fprintf(fp,"%d\n",weight);
	fclose(fp);
	Write_Check(G);     //保存添加信息到文件
	
	printf("\t成功添加信息!\n\t任意键返回...");
	getch();
}



void Shortcut(AdjMatrix *G)     //最短路线
{
	char place[30];
	int start,end;
	int dist[MAXVEX],path[MAXVEX][MAXVEX]={0};
	Map();
	printf("\t请输入起点：");
	scanf("%s",place);
	start = Locate(G,place);
	printf("\t请输入终点：");
	scanf("%s",place);
	end = Locate(G,place);
	Dijkstra(G,start,end,dist,path);
	
}

void Dijkstra(AdjMatrix *G,int start,int end,int dist[],int path[][MAXVEX])
{
	int mindist,i,j,k,t=1;
	for(i=1; i<=G->vexnum; i++)      //初始化
	{
		dist[i] = G->arcs[start][i];
		if(G->arcs[start][i]!=INFINITY)
			path[i][1] = start;
	}
	path[start][0]=1;
	for(i=2; i<=G->vexnum; i++)
	{
		mindist = INFINITY;
		for(j=1; j<=G->vexnum; j++)
			if(!path[j][0]&&dist[j]<mindist)
			{
				k=j;
				mindist=dist[j];
			}
		if(mindist==INFINITY) return;
		path[k][0]=1;
		for(j=1; j<=G->vexnum; j++)
		{
			if(!path[j][0]&&G->arcs[k][j]<INFINITY&&dist[k]+G->arcs[k][j]<dist[j])
			{
				dist[j]=dist[k]+G->arcs[k][j];
				t=1;
				while(path[k][t]!=0)
				{
					path[j][t]=path[k][t];
					t++;
				}
				path[j][t]=k;
				path[j][t+1]=0;
			}
		}
	}
	for(i=1; i<=G->vexnum; i++)
		if(i==end) break;
	printf("\t%s→%s 的最短路径为：\n\t从%s",G->vex[start].name,G->vex[end].name,G->vex[start].name);
	for(j=2; path[i][j]!=0; j++)
		printf("→%s",G->vex[path[i][j]].name);
	printf("→%s,距离为%dm!\n\t任意键继续...",G->vex[end].name,dist[i]);
	getch();
}
int path[100];
int visit[100];
int top=0;
void DFS(AdjMatrix *G,int num1,int num2)
{
	int i,j,count=0;
	top++;
	path[top]=num1;
	visit[num1]=1;
	if (num1==num2)
	{
		for (i=0;i<=top-1;i++)
		{
			printf("%s→",G->vex[path[i]].name);
			count++;
		}
		printf("%s",G->vex[path[top]].name);
		printf("共中转%d次\n",count);
		visit[num1]=0;
		top--;
		return;
	}
	for(j=1;j<= G->vexnum;j++)
	{
		if (G->arcs[num1][j]<INFINITY&&!visit[j])
			DFS(G,j,num2);
	}
	visit[num1]=0;
	top--;
}
void Double_All(AdjMatrix *G)
{
	int i,num1,num2;
	char a[50],b[50];
	Map();
	printf("\t请输入起点:");
	scanf("%s",a);
	printf("\t请输入终点:");
	scanf("%s",b);
	num1=Locate(G,a);
	num2=Locate(G,b);
	top=-1;
	for(i=0;i<G->vexnum;i++)
		visit[i]=0;
	DFS(G,num1,num2);
	printf("\t按任意键继续...");
	getch();
	system("cls");
}


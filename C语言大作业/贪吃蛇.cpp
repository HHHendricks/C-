#include<stdio.h>
#include<time.h>
#include<windows.h>
#include<stdlib.h>

#define U 1
#define D 2
#define L 3
#define R 4  //定义方向:上：U,下：D,左：L,右：R

typedef struct snake{
	int x;
	int y;
	struct snake *next;
}SNAKE;

/*定义的全局变量*/
int endgameways=0;  //结束游戏的方式：1:撞墙，2:咬到自己，3:按Esc键 
SNAKE *head,*fooda,*foodb;  //蛇头指针，蛇尾指针，食物指针
SNAKE *sq;  //打印蛇时用到指针 
int score=0,adda=10,addb=20,speed=5;  //总分、当前两种食物得分、速度
int status,sleeptime=200;  //当前按键状态 
int numa=0,numb=0;  //当前吃的两种食物个数 
int esc=0;

//声明全部函数//
void gotoxy(int x,int y);
void hidecursor();
void map();
void createsnake();
void biteself();
void initfood1();
void createfood1();
void createfood2();
void crosswall();
void endgame();
void movesnake();
void keystatus();
void welcome();

int main()
{
	hidecursor();
	welcome();
	map();
	createsnake();
	initfood1();
	createfood2();
	keystatus();
	
	system("pause");
	return 0;
}

void gotoxy(int x,int y)//函数gotoxy:定义光标位置
{
	COORD coord;//坐标系coord 
	HANDLE handle;
	
	coord.X=x;//横坐标X 
	coord.Y=y;//纵坐标 y
	handle=GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleCursorPosition(handle,coord);
}//end gotoxy 

void hidecursor()//函数hidecursor：隐藏光标
{
	CONSOLE_CURSOR_INFO curinfo;
	HANDLE Out;
	
	Out=GetStdHandle(STD_OUTPUT_HANDLE);
	curinfo.dwSize=1;//光标百分比厚度：1~100
	curinfo.bVisible=0;//是否可见
	SetConsoleCursorInfo(Out,&curinfo);
} //end hidecursor 

void welcome()//函数welcome：欢迎界面
{
	int i;
	HANDLE consolehwnd;//创建句柄
	consolehwnd=GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(consolehwnd,FOREGROUND_INTENSITY|FOREGROUND_RED|FOREGROUND_GREEN|FOREGROUND_BLUE);
	//设置为亮白色 
	
	for(i=14;i<39;i++)
	{
		gotoxy(2*i,4);
		printf("■");
	 } 
	 
	for(i=5;i<21;i++)
	{
		gotoxy(28,i);
		printf("■");
		gotoxy(76,i);
		printf("■");
	}

	
	for(i=15;i<38;i++)
	{
		gotoxy(2*i,6);
		printf("■");
	}
	
	for(i=15;i<38;i++)
	{
		gotoxy(2*i,8);
		printf("■");
	}
	
	for(i=15;i<38;i++)
	{
		gotoxy(2*i,17);
		printf("■");
	}
	
	for(i=15;i<38;i++)
	{
		gotoxy(2*i,20);
		printf("■");
	}
	
	gotoxy(45,5);
	printf("欢迎来到贪吃蛇！");
	
	gotoxy(46,7);
	printf("贪吃蛇控制台");
	
	gotoxy(40,9);
	printf("按↑.↓.←.→控制蛇移动");
	
	gotoxy(40,10);
	printf("1 加速，2 减速，空格键暂停");
	
	gotoxy(35,11);
	printf("食物分为蓝色食物和红色食物两种"); 
	gotoxy(35,12);
	printf("每个红色食物的得分是蓝色食物的两倍");
	
	gotoxy(35,13);
	printf("蛇撞墙、咬到自己或按Esc键时结束游戏");  
	
	gotoxy(33,14);
	printf("蛇的速度分1~10十个等级，1级最慢，10级最快");
	
	gotoxy(33,15);
	printf("速度越快时吃每个食物得分越高，反之亦然");
	
	gotoxy(33,16);
	printf("按1速度不再增加时,速度已达上限,反之亦然");
	
	gotoxy(39,18);
	printf("请在游戏开始前关闭中文输入法"); 
	
	gotoxy(45,19);
	printf("回车键开始游戏"); 
	
	gotoxy(60,21);
	printf("蔡昊 季祥昊联合出品"); 
	gotoxy(60,22);
	printf("此游戏最终解释权归蔡昊，季祥昊所有"); 
	
	getchar();
	system("cls");
}

void map() //函数map：画地图
{
	int i;
	HANDLE consolehwnd;//创建句柄
	consolehwnd=GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(consolehwnd,FOREGROUND_INTENSITY|FOREGROUND_RED|FOREGROUND_GREEN|FOREGROUND_BLUE);
	//设置地图为亮白色 
	
	for(i=0;i<29;i++)
	{
		gotoxy(2*i,0);
		printf("■");
		gotoxy(2*i,26);
		printf("■");
	}
	
	for(i=1;i<26;i++)
	{
		gotoxy(0,i);
		printf("■");
		gotoxy(56,i);
		printf("■");
	}
}//end map

void createsnake()//函数creatsnake：初始化蛇，长度为5（头插法）
{
	SNAKE *new1;			
	int i;
	
	head=NULL;
	new1=(SNAKE *)malloc(sizeof(SNAKE));
	
	if(!new1)//判断分配内存是否成功 
	{
		printf("内存分配失败!\n");
		exit(1);
	}
	
	new1->x=20;
	new1->y=5;
	new1->next=head;
	head=new1;
	
	for(i=1;i<=4;i++)
    {
        new1=(snake*)malloc(sizeof(snake));
        
        if(!new1)//判断分配内存是否成功 
	    {
		    printf("内存分配失败!\n");
		    exit(1);
        }
	
        new1->x=20+2*i;
        new1->y=5;
        new1->next=head;
        head=new1;
    }
    
    gotoxy(new1->x,new1->y);
    printf("⊙");
    new1=new1->next;
    
    while(new1!=NULL)//遍历蛇身
	{
		gotoxy(new1->x,new1->y);
		printf("■");
		new1=new1->next;
	} 
} //end creasnake

void biteself()//函数：判断是否咬到自己
{
	SNAKE *self;
	
	self=head->next;
	
	while(self!=NULL)
	{
		if(self->x==head->x&&self->y==head->y)
		{
			endgameways=2;
			endgame();
		}
		
		self=self->next;
	}
	
} //end biteself

void initfood1()//函数initfood：初始食物1 
{
	SNAKE *food1;
	time_t t;
	HANDLE consolehwnd;//创建句柄
	consolehwnd=GetStdHandle(STD_OUTPUT_HANDLE); 
	
	srand((unsigned int)time(&t));
	food1=(SNAKE *)malloc(sizeof(SNAKE));
	
	if(!food1)//判断分配内存是否成功 
	{
		printf("内存分配失败!\n");
		exit(1);
	}
	
	food1->x=rand()%54+2;
	food1->y=rand()%25+1;
	
	sq=head;
	
	while(sq!=NULL)
	{
		while((food1->x==sq->x&&food1->y==sq->y)||food1->x%2!=0)//保证食物与蛇头对其同时蛇神与食物不重合 
		{
			food1->x=rand()%54+2;
			food1->y=rand()%25+1;
		}
		
		sq=sq->next;
	}
	
	SetConsoleTextAttribute(consolehwnd,FOREGROUND_INTENSITY|FOREGROUND_BLUE); //设置食物1为高亮蓝色 
	gotoxy(food1->x,food1->y);
	printf("■");
	
	fooda=food1;
}

void createfood1()//随机产生食物1
{
	SNAKE *food1;
	time_t t;
	HANDLE consolehwnd;//创建句柄
	consolehwnd=GetStdHandle(STD_OUTPUT_HANDLE); 
	
	srand((unsigned int)time(&t));
	food1=(SNAKE *)malloc(sizeof(SNAKE));
	
	if(!food1)//判断分配内存是否成功 
	{
		printf("内存分配失败!\n");
		exit(1);
	}
	
	food1->x=rand()%54+2;
	food1->y=rand()%25+1;
	
	while(food1->x%2!=0||(food1->x==foodb->x&&food1->y==foodb->y))//保证食物与蛇头对齐和两个食物不重合
	{
		food1->x=rand()%54+2;
	}
	
	sq=head;
	
	while(sq!=NULL)
	{
		while((food1->x==sq->x&&food1->y==sq->y)||food1->x%2!=0)//保证食物与蛇头对其同时蛇神与食物不重合 
		{
			food1->x=rand()%54+2;
			food1->y=rand()%25+1;
		}
		
		sq=sq->next;
	}
	
	SetConsoleTextAttribute(consolehwnd,FOREGROUND_INTENSITY|FOREGROUND_BLUE); //设置食物1为高亮蓝色 
	gotoxy(food1->x,food1->y);
	printf("■");
	
	fooda=food1;
 } 
 
void createfood2()//随机产生食物2
{
	SNAKE *food2;
	time_t t;
	HANDLE consolehwnd;//创建句柄
	consolehwnd=GetStdHandle(STD_OUTPUT_HANDLE); 
	
	srand((unsigned int)time(&t));
	food2=(SNAKE *)malloc(sizeof(SNAKE));
	
	if(!food2)//判断分配内存是否成功 
	{
		printf("Memory request failed!\n");
		exit(1);
	}
	
	food2->x=rand()%54+2;
	food2->y=1;
	
	while(food2->x%2!=0||(food2->x==fooda->x&&food2->y&&fooda->y))//保证两个食物不重合 
	{
		food2->y=25;
		food2->x=rand()%54+2;
	}
	
	sq=head;
	
	while(sq!=NULL)
	{
		while((sq->x==food2->x&&sq->y==food2->y)||food2->x%2!=0) //判断蛇身是否与食物重合
		{
			food2->x=rand()%54+2;
			food2->y=1;
		}
		
		sq=sq->next;
	}
	
	SetConsoleTextAttribute(consolehwnd,FOREGROUND_INTENSITY|FOREGROUND_RED);//设置食物2为高亮红色  
	gotoxy(food2->x,food2->y);
	printf("■");
	
	foodb=food2;
 }  
 void crosswall()
{
	if(head->x==0||head->x==56||head->y==0||head->y==26)
	{
		endgameways=1;
		endgame();
	}
 } 
 
 void endgame()
 {
 	system("cls");
 	HANDLE consolehwnd;//创建句柄
	consolehwnd=GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(consolehwnd,FOREGROUND_INTENSITY|FOREGROUND_GREEN);//设置字体为高亮红色
	gotoxy(35,8);
	switch(endgameways)
	{
		case 1:printf("对不起，您撞到墙了!"); break;
		case 2:printf("对不起，您咬到自己了！"); break;
		case 3:printf("您已经结束游戏了！"); break;
	}
	gotoxy(35,10);
	printf("总共吃了%d个蓝色食物，%d个红色食物",numa,numb);
	gotoxy(35,12);
	printf("最终得分是：%d\n",score);
	esc=1;
 }
 void movesnake()
 {
 	SNAKE *nexthead;
 	nexthead=(SNAKE*)malloc(sizeof(SNAKE));
 	if(!nexthead){
 		printf("Memory request failed!\n");
		exit(1);
	 }
	 switch(status){
	 	case 1:nexthead->x=head->x;nexthead->y=head->y-1; break;
	 	case 2:nexthead->x=head->x;nexthead->y=head->y+1; break;
	 	case 3:nexthead->x=head->x-2;nexthead->y=head->y; break;
	 	case 4:nexthead->x=head->x+2;nexthead->y=head->y; break;
	 }
	 if(nexthead->x==fooda->x&&nexthead->y==fooda->y)
	 {
	 	nexthead->next=head;
	 	head=nexthead;
	 	sq=head;
	 	
	 	gotoxy(sq->x,sq->y);
	 	printf("⊙");
	 	sq=sq->next;
	 	
	 	while(sq!=NULL)
	 	{
	 		gotoxy(sq->x,sq->y);
	 		printf("■");
            sq=sq->next;
		 }
		 score+=adda;
		 numa++;
		 createfood1();
	 }
	else  if(nexthead->x==foodb->x&&nexthead->y==foodb->y)
	 {
	 	nexthead->next=head;
	 	head=nexthead;
	 	sq=head;
	 	
	 	gotoxy(sq->x,sq->y);
	 	printf("⊙");
	 	sq=sq->next;
	 	
	 	while(sq!=NULL)
	 	{
	 		gotoxy(sq->x,sq->y);
	 		printf("■");
            sq=sq->next;
		 }
		 score+=addb;
		 numb++;
		 createfood2();
	 }
	 else{nexthead->next=head;
	 	head=nexthead;
	 	sq=head;
	 	
	 	gotoxy(sq->x,sq->y);
	 	printf("⊙");
	 	sq=sq->next;
	 	
	 	while(sq->next->next!=NULL)
	 	{
	 		gotoxy(sq->x,sq->y);
	 		printf("■");
            sq=sq->next;
		 }
		 gotoxy(sq->next->x,sq->next->y);
		 printf("  ");
		 free(sq->next);
	 	sq->next=NULL;
	 }
 }
void keystatus()
{
	status=R;
	while(1)
	{
		HANDLE consolehwnd;//创建句柄
	    consolehwnd=GetStdHandle(STD_OUTPUT_HANDLE);
	    SetConsoleTextAttribute(consolehwnd,FOREGROUND_INTENSITY|FOREGROUND_RED);
	    //设置字体为高亮红色 
	    
		gotoxy(75,6);
		printf("当前总得分：%d",score);
		gotoxy(75,8);
		printf("每个蓝色食物得分：%d分",adda);
		gotoxy(75,10);
		printf("每个红色食物得分：%d分",addb);
		gotoxy(75,12);
		printf("当前吃的蓝色食物个数：%d个",numa);
		gotoxy(75,14);
		printf("当前吃的红色食物个数：%d个",numb); 
		gotoxy(75,16);
		printf("当前速度等级：%2d",speed);
		SetConsoleTextAttribute(consolehwnd,FOREGROUND_INTENSITY|FOREGROUND_GREEN);
		if(GetAsyncKeyState(VK_UP)&&status!=D)
			status=U;
		else if(GetAsyncKeyState(VK_DOWN)&&status!=U)
			status=D;
		else if(GetAsyncKeyState(VK_LEFT)&&status!=R)
			status=L;
		else if(GetAsyncKeyState(VK_RIGHT)&&status!=L)
			status=R;
		else if(GetAsyncKeyState(VK_SPACE))
		{
			while(1)
			{
				Sleep(300);
				if(GetAsyncKeyState(VK_SPACE))
				break;
			}
		}
		else if(GetAsyncKeyState(VK_ESCAPE))
		{
			endgameways=3;
			endgame(); 
			break;
		}
		else if(GetAsyncKeyState('1'))
		{
			if(sleeptime>50)
			{
				sleeptime-=20;
				adda+=2;
				addb+=2;
				speed+=1;
			}
		}
		else if(GetAsyncKeyState('2'))
		{
			if(sleeptime<320)
			{
				sleeptime+=30;
				adda-=2;
				addb-=2;
				speed--;
			}
		}
		movesnake();
		crosswall();
		biteself();
		Sleep(sleeptime);
		if(esc==1)
		break;
		
	}
}

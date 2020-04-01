#include<stdio.h>
#include<time.h>
#include<windows.h>
#include<stdlib.h>

#define U 1
#define D 2
#define L 3
#define R 4  //���巽��:�ϣ�U,�£�D,��L,�ң�R

typedef struct snake{
	int x;
	int y;
	struct snake *next;
}SNAKE;

/*�����ȫ�ֱ���*/
int endgameways=0;  //������Ϸ�ķ�ʽ��1:ײǽ��2:ҧ���Լ���3:��Esc�� 
SNAKE *head,*fooda,*foodb;  //��ͷָ�룬��βָ�룬ʳ��ָ��
SNAKE *sq;  //��ӡ��ʱ�õ�ָ�� 
int score=0,adda=10,addb=20,speed=5;  //�ܷ֡���ǰ����ʳ��÷֡��ٶ�
int status,sleeptime=200;  //��ǰ����״̬ 
int numa=0,numb=0;  //��ǰ�Ե�����ʳ����� 
int esc=0;

//����ȫ������//
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

void gotoxy(int x,int y)//����gotoxy:������λ��
{
	COORD coord;//����ϵcoord 
	HANDLE handle;
	
	coord.X=x;//������X 
	coord.Y=y;//������ y
	handle=GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleCursorPosition(handle,coord);
}//end gotoxy 

void hidecursor()//����hidecursor�����ع��
{
	CONSOLE_CURSOR_INFO curinfo;
	HANDLE Out;
	
	Out=GetStdHandle(STD_OUTPUT_HANDLE);
	curinfo.dwSize=1;//���ٷֱȺ�ȣ�1~100
	curinfo.bVisible=0;//�Ƿ�ɼ�
	SetConsoleCursorInfo(Out,&curinfo);
} //end hidecursor 

void welcome()//����welcome����ӭ����
{
	int i;
	HANDLE consolehwnd;//�������
	consolehwnd=GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(consolehwnd,FOREGROUND_INTENSITY|FOREGROUND_RED|FOREGROUND_GREEN|FOREGROUND_BLUE);
	//����Ϊ����ɫ 
	
	for(i=14;i<39;i++)
	{
		gotoxy(2*i,4);
		printf("��");
	 } 
	 
	for(i=5;i<21;i++)
	{
		gotoxy(28,i);
		printf("��");
		gotoxy(76,i);
		printf("��");
	}

	
	for(i=15;i<38;i++)
	{
		gotoxy(2*i,6);
		printf("��");
	}
	
	for(i=15;i<38;i++)
	{
		gotoxy(2*i,8);
		printf("��");
	}
	
	for(i=15;i<38;i++)
	{
		gotoxy(2*i,17);
		printf("��");
	}
	
	for(i=15;i<38;i++)
	{
		gotoxy(2*i,20);
		printf("��");
	}
	
	gotoxy(45,5);
	printf("��ӭ����̰���ߣ�");
	
	gotoxy(46,7);
	printf("̰���߿���̨");
	
	gotoxy(40,9);
	printf("����.��.��.���������ƶ�");
	
	gotoxy(40,10);
	printf("1 ���٣�2 ���٣��ո����ͣ");
	
	gotoxy(35,11);
	printf("ʳ���Ϊ��ɫʳ��ͺ�ɫʳ������"); 
	gotoxy(35,12);
	printf("ÿ����ɫʳ��ĵ÷�����ɫʳ�������");
	
	gotoxy(35,13);
	printf("��ײǽ��ҧ���Լ���Esc��ʱ������Ϸ");  
	
	gotoxy(33,14);
	printf("�ߵ��ٶȷ�1~10ʮ���ȼ���1��������10�����");
	
	gotoxy(33,15);
	printf("�ٶ�Խ��ʱ��ÿ��ʳ��÷�Խ�ߣ���֮��Ȼ");
	
	gotoxy(33,16);
	printf("��1�ٶȲ�������ʱ,�ٶ��Ѵ�����,��֮��Ȼ");
	
	gotoxy(39,18);
	printf("������Ϸ��ʼǰ�ر��������뷨"); 
	
	gotoxy(45,19);
	printf("�س�����ʼ��Ϸ"); 
	
	gotoxy(60,21);
	printf("��� ��������ϳ�Ʒ"); 
	gotoxy(60,22);
	printf("����Ϸ���ս���Ȩ���껣����������"); 
	
	getchar();
	system("cls");
}

void map() //����map������ͼ
{
	int i;
	HANDLE consolehwnd;//�������
	consolehwnd=GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(consolehwnd,FOREGROUND_INTENSITY|FOREGROUND_RED|FOREGROUND_GREEN|FOREGROUND_BLUE);
	//���õ�ͼΪ����ɫ 
	
	for(i=0;i<29;i++)
	{
		gotoxy(2*i,0);
		printf("��");
		gotoxy(2*i,26);
		printf("��");
	}
	
	for(i=1;i<26;i++)
	{
		gotoxy(0,i);
		printf("��");
		gotoxy(56,i);
		printf("��");
	}
}//end map

void createsnake()//����creatsnake����ʼ���ߣ�����Ϊ5��ͷ�巨��
{
	SNAKE *new1;			
	int i;
	
	head=NULL;
	new1=(SNAKE *)malloc(sizeof(SNAKE));
	
	if(!new1)//�жϷ����ڴ��Ƿ�ɹ� 
	{
		printf("�ڴ����ʧ��!\n");
		exit(1);
	}
	
	new1->x=20;
	new1->y=5;
	new1->next=head;
	head=new1;
	
	for(i=1;i<=4;i++)
    {
        new1=(snake*)malloc(sizeof(snake));
        
        if(!new1)//�жϷ����ڴ��Ƿ�ɹ� 
	    {
		    printf("�ڴ����ʧ��!\n");
		    exit(1);
        }
	
        new1->x=20+2*i;
        new1->y=5;
        new1->next=head;
        head=new1;
    }
    
    gotoxy(new1->x,new1->y);
    printf("��");
    new1=new1->next;
    
    while(new1!=NULL)//��������
	{
		gotoxy(new1->x,new1->y);
		printf("��");
		new1=new1->next;
	} 
} //end creasnake

void biteself()//�������ж��Ƿ�ҧ���Լ�
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

void initfood1()//����initfood����ʼʳ��1 
{
	SNAKE *food1;
	time_t t;
	HANDLE consolehwnd;//�������
	consolehwnd=GetStdHandle(STD_OUTPUT_HANDLE); 
	
	srand((unsigned int)time(&t));
	food1=(SNAKE *)malloc(sizeof(SNAKE));
	
	if(!food1)//�жϷ����ڴ��Ƿ�ɹ� 
	{
		printf("�ڴ����ʧ��!\n");
		exit(1);
	}
	
	food1->x=rand()%54+2;
	food1->y=rand()%25+1;
	
	sq=head;
	
	while(sq!=NULL)
	{
		while((food1->x==sq->x&&food1->y==sq->y)||food1->x%2!=0)//��֤ʳ������ͷ����ͬʱ������ʳ�ﲻ�غ� 
		{
			food1->x=rand()%54+2;
			food1->y=rand()%25+1;
		}
		
		sq=sq->next;
	}
	
	SetConsoleTextAttribute(consolehwnd,FOREGROUND_INTENSITY|FOREGROUND_BLUE); //����ʳ��1Ϊ������ɫ 
	gotoxy(food1->x,food1->y);
	printf("��");
	
	fooda=food1;
}

void createfood1()//�������ʳ��1
{
	SNAKE *food1;
	time_t t;
	HANDLE consolehwnd;//�������
	consolehwnd=GetStdHandle(STD_OUTPUT_HANDLE); 
	
	srand((unsigned int)time(&t));
	food1=(SNAKE *)malloc(sizeof(SNAKE));
	
	if(!food1)//�жϷ����ڴ��Ƿ�ɹ� 
	{
		printf("�ڴ����ʧ��!\n");
		exit(1);
	}
	
	food1->x=rand()%54+2;
	food1->y=rand()%25+1;
	
	while(food1->x%2!=0||(food1->x==foodb->x&&food1->y==foodb->y))//��֤ʳ������ͷ���������ʳ�ﲻ�غ�
	{
		food1->x=rand()%54+2;
	}
	
	sq=head;
	
	while(sq!=NULL)
	{
		while((food1->x==sq->x&&food1->y==sq->y)||food1->x%2!=0)//��֤ʳ������ͷ����ͬʱ������ʳ�ﲻ�غ� 
		{
			food1->x=rand()%54+2;
			food1->y=rand()%25+1;
		}
		
		sq=sq->next;
	}
	
	SetConsoleTextAttribute(consolehwnd,FOREGROUND_INTENSITY|FOREGROUND_BLUE); //����ʳ��1Ϊ������ɫ 
	gotoxy(food1->x,food1->y);
	printf("��");
	
	fooda=food1;
 } 
 
void createfood2()//�������ʳ��2
{
	SNAKE *food2;
	time_t t;
	HANDLE consolehwnd;//�������
	consolehwnd=GetStdHandle(STD_OUTPUT_HANDLE); 
	
	srand((unsigned int)time(&t));
	food2=(SNAKE *)malloc(sizeof(SNAKE));
	
	if(!food2)//�жϷ����ڴ��Ƿ�ɹ� 
	{
		printf("Memory request failed!\n");
		exit(1);
	}
	
	food2->x=rand()%54+2;
	food2->y=1;
	
	while(food2->x%2!=0||(food2->x==fooda->x&&food2->y&&fooda->y))//��֤����ʳ�ﲻ�غ� 
	{
		food2->y=25;
		food2->x=rand()%54+2;
	}
	
	sq=head;
	
	while(sq!=NULL)
	{
		while((sq->x==food2->x&&sq->y==food2->y)||food2->x%2!=0) //�ж������Ƿ���ʳ���غ�
		{
			food2->x=rand()%54+2;
			food2->y=1;
		}
		
		sq=sq->next;
	}
	
	SetConsoleTextAttribute(consolehwnd,FOREGROUND_INTENSITY|FOREGROUND_RED);//����ʳ��2Ϊ������ɫ  
	gotoxy(food2->x,food2->y);
	printf("��");
	
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
 	HANDLE consolehwnd;//�������
	consolehwnd=GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(consolehwnd,FOREGROUND_INTENSITY|FOREGROUND_GREEN);//��������Ϊ������ɫ
	gotoxy(35,8);
	switch(endgameways)
	{
		case 1:printf("�Բ�����ײ��ǽ��!"); break;
		case 2:printf("�Բ�����ҧ���Լ��ˣ�"); break;
		case 3:printf("���Ѿ�������Ϸ�ˣ�"); break;
	}
	gotoxy(35,10);
	printf("�ܹ�����%d����ɫʳ�%d����ɫʳ��",numa,numb);
	gotoxy(35,12);
	printf("���յ÷��ǣ�%d\n",score);
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
	 	printf("��");
	 	sq=sq->next;
	 	
	 	while(sq!=NULL)
	 	{
	 		gotoxy(sq->x,sq->y);
	 		printf("��");
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
	 	printf("��");
	 	sq=sq->next;
	 	
	 	while(sq!=NULL)
	 	{
	 		gotoxy(sq->x,sq->y);
	 		printf("��");
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
	 	printf("��");
	 	sq=sq->next;
	 	
	 	while(sq->next->next!=NULL)
	 	{
	 		gotoxy(sq->x,sq->y);
	 		printf("��");
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
		HANDLE consolehwnd;//�������
	    consolehwnd=GetStdHandle(STD_OUTPUT_HANDLE);
	    SetConsoleTextAttribute(consolehwnd,FOREGROUND_INTENSITY|FOREGROUND_RED);
	    //��������Ϊ������ɫ 
	    
		gotoxy(75,6);
		printf("��ǰ�ܵ÷֣�%d",score);
		gotoxy(75,8);
		printf("ÿ����ɫʳ��÷֣�%d��",adda);
		gotoxy(75,10);
		printf("ÿ����ɫʳ��÷֣�%d��",addb);
		gotoxy(75,12);
		printf("��ǰ�Ե���ɫʳ�������%d��",numa);
		gotoxy(75,14);
		printf("��ǰ�Եĺ�ɫʳ�������%d��",numb); 
		gotoxy(75,16);
		printf("��ǰ�ٶȵȼ���%2d",speed);
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

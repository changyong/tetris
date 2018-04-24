/*************************
<Russia Diamonds Ver 1.0>
Copyright by forever_chang
forever_chang@163.com
2001.11.1
*************************/
/*****************************************************************************************/
#include "graphics.h" /*ͷ�ļ�*/
#include "time.h"
#include "stdlib.h"
#include "bios.h"
#include "dos.h"
#include "stdio.h"
#define ESC 0x11b /*����ɨ����*/
#define UP 0x4800
#define DOWN 0x5000
#define LEFT 0x4b00
#define F1 0x3b00
#define RIGHT 0x4d00
#define YES 0x1579
#define NO 0x316e
#define RESTART 0x1372
/*****************************************************************************************/
struct diamond /*��¼ÿ�ַ���ÿ��״̬����Ϣ*/
{
 int x[4];
 int y[4];
 int start_x;
 int start_y;
 int color;
 struct diamond *next;
};
int grid[12][23]; /*��¼���и��ӵ�״̬ (0)û�з��� (1)�й̶����� (2)�л����*/
int x; /*���������λ��*/
int y;
int level; /*��Ϸ�Ѷ�*/
int count; /*������*/
int score;/*�÷�*/
struct diamond *nowinfo; /*��ǰ�����*/
struct diamond *nextinfo; /*��һ������*/
int color;/*�����ӵ���ɫ*/
int backcolor;/*��Ϸ���򱳾�ɫ*/
/*****************************************************************************************/
void show_interface (int mode);/*��ʾ��Ϸ����*/
void show_copsign (int topx,int topy,int size,int color);/*��ʾ��˾��־--���ΰҵ*/
void show_intro (int xs,int ys);/*��ʾ���������*/
/*void print(); �����ú���*/
void scandel();/*ɨ�����и��ӿ��Ƿ��п�������*/
void show_down ();/*������������һ��״̬*/
void show_next ();/*���鷭ת�����һ��״̬*/
void show_left ();/*���������ƶ������һ��״̬*/
void show_right ();/*���������ƶ������һ��״̬*/
void interrupt (*oldtimer)();/*ָ��δ��װǰ���ж�����,������ָ��,ָ��һ�ο�ִ�еĴ���*/
void install();/*��װ�µ��ж�����,�����жϷ������װ���ж���������*/
void interrupt newtimer();/*�жϷ������*/
struct diamond *get_diamond();/*����õ�һ������*/
struct diamond *create_I();/*������������������η���Ļ�������*/
struct diamond *create_T();/*�������������һ��״̬��ָ��*/
struct diamond *create_L();
struct diamond *create_J();
struct diamond *create_Z();
struct diamond *create_N();
struct diamond *create_H();
void delinfo (struct diamond *info);/*�ͷŵ�ǰ������ռ�õĿռ�*/
void addtobuffer(int c);/*����̻�����������һ��DOWN*/
/*void clrkey();����dos�ж���ռ��̻�����,δʹ�ô˷���.*/
void showsubwin(struct diamond *next);/*��С������ʾ��һ������*/
void showscore(int scoreget);/*��ʾ����*/
void startset();/*��ʼ����Ϸ*/
/*****************************************************************************************/
main ()
{
 int driver=VGA;
 int mode=VGAHI;
 int i;/*������*/
 int j;
 int key;/*��¼����ɨ����*/
 initgraph (&driver,&mode,"");/*��ʼ��*/
 color=8;
 backcolor=7;
 level=1;
 score=298;
 count=0;
 show_interface (9);/*��ʾ����*/
 setfillstyle(SOLID_FILL,1);
 bar(0,465,640,480);
 outtextxy(5,469,"Press any key to start...");
 getch();
 setfillstyle(SOLID_FILL,9);
 bar(0,465,640,480);
 showscore(0);/*��ʾ����*/
 randomize();
 nowinfo=get_diamond ();/*�õ�һ����ǰ����*/
 oldtimer=getvect(0x1c);
 install(newtimer);
 for(i=0;i<=21;i++)/*��ʼ��grid[12][23]*/
 {
  for(j=1;j<=10;j++)
  grid[j][i]=0;
 }
 for(i=0;i<=22;i++)
 {
  grid[0][i]=1;
  grid[11][i]=1;
 }
 for(i=0;i<=11;i++)
 {
  grid[i][22]=1;
 }

 x=nowinfo->start_x;/*��ʼ������λ��*/
 y=nowinfo->start_y;
 nextinfo=get_diamond ();/*�õ���һ������*/
 showsubwin(nextinfo);

 for (;;)
 {
  key=bioskey(0);/*�õ�����ɨ����*/
  if (key)
  {
   switch(key)
   {

	case DOWN:{
               show_down ();
			   break;
		      }
	case UP:{
	         show_next ();
			 break;
			}
	case LEFT:{
		       show_left();
			   break;
			  }
	case RIGHT:{
   	            show_right();
		        break;
			   }
    case RESTART:{
			      install(oldtimer);
				  setfillstyle(SOLID_FILL,1);
			      bar(0,465,640,480);
			      outtextxy(5,469,"Are you sure to restart (Y/N)...");
				  for (;;)
				  {
         	       key=bioskey(0);/*�õ�����ɨ����*/
                   if (key==YES)
				   {
                    startset();
				    setfillstyle(SOLID_FILL,9);
			        bar(0,465,640,480);
					break;
				   }
                   if (key==NO)
				   {
			       setfillstyle(SOLID_FILL,9);
			       bar(0,465,640,480);
				   install(newtimer);
				   break;
				   }
				 }
				 break;
				}
/*	case F1:{
		     print();
			 break;
			}
*/
	case ESC:{
		      install(oldtimer);
		      setfillstyle(SOLID_FILL,1);
			  bar(0,465,640,480);
			  outtextxy(5,469,"Are you sure to exit (Y/N)...");
			  for (;;)
			  {
         	   key=bioskey(0);/*�õ�����ɨ����*/
               if (key==YES)
			   {
				closegraph();
		        exit(0);
			   }
               if (key==NO)
			   {
			    setfillstyle(SOLID_FILL,9);
			    bar(0,465,640,480);
				install(newtimer);
				break;
			   }
			  }
		      
			 }
   }
  }
 }
}
/*****************************************************************************************/
struct diamond *get_diamond()
{
 struct diamond *now;
 switch (random(7)+1)
 {
  case 1:{
	       now=create_I();
		   return now;
		  }
   case 2:{
	   	   now=create_T();
		   return now;
	      }
   case 3:{
	       now=create_L();
		   return now;
		  }
   case 4:{
	       now=create_J();
		   return now;
		  }
   case 5:{
	       now=create_Z();
		   return now;
		  }
   case 6:{
	       now=create_N();
		   return now;
		  }
   case 7:{
	       now=create_H();
		   return now;
		  }
 }
}
/*****************************************************************************************/
void show_interface (int fill_mode)
{
 int i;
 setbkcolor (9);
 setcolor (color);
 setfillstyle (SOLID_FILL,backcolor);
 bar (100,60,300,420);
 bar (360,60,440,140);
 rectangle (100,60,300,420);
 rectangle (96,56,304,424);

 rectangle (360,60,440,140);
 rectangle (356,56,444,144);
 setfillstyle (fill_mode,14);
 floodfill (97,57,color);
 floodfill (397,57,color);
 setcolor(1);
 rectangle(96,56,304,424);
 setcolor (color);
 for (i=80;i<=400;i+=20)
  {
   line (100,i,300,i);
  }
 for (i=120;i<=280;i+=20)
  {
   line (i,60,i,420);
  }
 for (i=80;i<=120;i+=20)
 {
  line (360,i,440,i);
 }
 for (i=380;i<=420;i+=20)
 {
  line (i,60,i,140);
 }
 show_intro (360,180);
 show_copsign (475,320,40,1);
 setfillstyle(SOLID_FILL,1);
 setcolor(14);
 rectangle(420,405,534,417);
 floodfill (421,406,14);
 outtextxy(422,408,"HI-TECH WEALTH");
}
/*****************************************************************************************/
void show_copsign (int topx,int topy,int size,int color)
{
 int halfsize,qtrsize;
 int xadd,xdel,yadd1,yadd2;
 halfsize=0.5*size;
 qtrsize=0.25*size;

 xadd=topx+size;
 xdel=topx-size;
 yadd1=topy+size;
 yadd2=topy+2*size;

 setcolor(color);
 line (topx,topy,xdel,yadd1);
 line (xdel,yadd1,topx,yadd2);
 line (topx,yadd2,xadd,yadd1);
 line (xadd,yadd1,topx,topy);
 rectangle (topx-halfsize,topy+halfsize,topx+halfsize,yadd1+halfsize);
 setfillstyle (SOLID_FILL,color);
 floodfill (topx,topy+1,color);
 floodfill (xdel+1,yadd1,color);
 floodfill (topx,yadd2-1,color);
 floodfill (xadd-1,yadd1,color);
 rectangle (topx-halfsize,yadd1-qtrsize,topx-0.75*halfsize,yadd1+qtrsize);
 floodfill (topx-halfsize+1,yadd1-qtrsize+1,color);
 rectangle (topx-qtrsize,yadd1-halfsize,topx+qtrsize,yadd1-0.25*halfsize);
 floodfill (topx-qtrsize+1,yadd1-halfsize+1,color);
 rectangle (topx+0.75*halfsize,yadd1-qtrsize,topx+halfsize,yadd1+qtrsize);
 floodfill (topx+0.75*halfsize+1,yadd1-qtrsize+1,color);
 rectangle (topx-qtrsize,yadd1+0.25*halfsize,topx+qtrsize,yadd2-halfsize);
 floodfill (topx-qtrsize+1,yadd1+0.25*halfsize+1,color);
 setcolor(14);
 line (topx,topy-1,xdel-1,yadd1);
 line (xdel-1,yadd1,topx,yadd2+1);
 line (topx,yadd2+1,xadd+1,yadd1);
 line (xadd+1,yadd1,topx,topy-1);
 setfillstyle (SOLID_FILL,14);
 floodfill (topx,yadd1,color);
}
/*****************************************************************************************/
 void show_intro (int xs,int ys)
{
 char stemp[20];
 setcolor (15);
 rectangle(xs-3,ys-3,xs+239,ys+115);
 line (xs-3,ys+26,xs+239,ys+26);
 line (xs-3,ys+72,xs+239,ys+72);
 outtextxy(xs,ys,"Level:");
 outtextxy(xs,ys+15,"Score:");
 sprintf(stemp," -Roll  -Downwards");
 stemp[0]=24;
 stemp[7]=25;
 outtextxy(xs,ys+30,"Help:");
 setcolor(14);
 outtextxy(xs+40,ys+30,stemp);
 outtextxy(xs+40,ys+45,"<-Turn Left >-Turn Right");
 outtextxy(xs+40,ys+60,"Esc-Exit R-Restart");
 outtextxy(xs,ys+75,"Russia Diamonds Ver 1.0");
 outtextxy(xs,ys+90,"CopyRight By ChangYong.01-11-1");
 outtextxy(xs,ys+105,"Email:forever_chang@163.com");
}
/*****************************************************************************************/
struct diamond *create_I()
{
 struct diamond *info;
 struct diamond *first;
 first=(struct diamond *)malloc(sizeof(struct diamond));
 info=(struct diamond *)malloc(sizeof(struct diamond));
 first->next=info;
 info->next=first;
 first->x[0]=0;
 first->y[0]=0;
 first->x[1]=-1;
 first->y[1]=0;
 first->x[2]=1;
 first->y[2]=0;
 first->x[3]=2;
 first->y[3]=0;
 first->start_x=5;
 first->start_y=3;
 first->color=2;

 info->x[0]=0;
 info->y[0]=0;
 info->x[1]=0;
 info->y[1]=-1;
 info->x[2]=0;
 info->y[2]=1;
 info->x[3]=0;
 info->y[3]=2;
 info->start_x=5;
 info->start_y=1;
 info->color=2;

 if(random(2)==0) {return first;}
  else {return first->next;}
}
/*****************************************************************************************/
struct diamond *create_T()
{
 struct diamond *info;
 struct diamond *first;
 struct diamond *last;
 int i;

 info=(struct diamond *)malloc(sizeof(struct diamond));
 info->x[0]=0;
 info->y[0]=0;
 info->x[1]=-1;
 info->y[1]=0;
 info->x[2]=0;
 info->y[2]=-1;
 info->x[3]=1;
 info->y[3]=0;
 info->start_x=5;
 info->start_y=3;
 info->color=4;
 first=info;
 last=info;

 info=(struct diamond *)malloc(sizeof(struct diamond));
 info->x[0]=0;
 info->y[0]=0;
 info->x[1]=0;
 info->y[1]=1;
 info->x[2]=0;
 info->y[2]=-1;
 info->x[3]=1;
 info->y[3]=0;
 info->start_x=5;
 info->start_y=2;
 info->color=4;
 last->next=info;
 last=info;

 info=(struct diamond *)malloc(sizeof(struct diamond));
 info->x[0]=0;
 info->y[0]=0;
 info->x[1]=-1;
 info->y[1]=0;
 info->x[2]=1;
 info->y[2]=0;
 info->x[3]=0;
 info->y[3]=1;
 info->start_x=5;
 info->start_y=2;
 info->color=4;
 last->next=info;
 last=info;

 info=(struct diamond *)malloc(sizeof(struct diamond));
 info->x[0]=0;
 info->y[0]=0;
 info->x[1]=0;
 info->y[1]=1;
 info->x[2]=0;
 info->y[2]=-1;
 info->x[3]=-1;
 info->y[3]=0;
 info->start_x=5;
 info->start_y=2;
 info->color=4;
 last->next=info;
 last=info;
 last->next=first;

 for (i=0;i<=random(4);i++)
 {
  first=first->next;
 }
 return first;
}
/*****************************************************************************************/
struct diamond *create_L()
{
 struct diamond *info;
 struct diamond *first;
 struct diamond *last;
 int i;

 info=(struct diamond *)malloc(sizeof(struct diamond));
 info->x[0]=0;
 info->y[0]=0;
 info->x[1]=0;
 info->y[1]=1;
 info->x[2]=0;
 info->y[2]=-1;
 info->x[3]=1;
 info->y[3]=1;
 info->start_x=5;
 info->start_y=2;
 info->color=5;
 first=info;
 last=info;

 info=(struct diamond *)malloc(sizeof(struct diamond));
 info->x[0]=0;
 info->y[0]=0;
 info->x[1]=-1;
 info->y[1]=0;
 info->x[2]=1;
 info->y[2]=0;
 info->x[3]=-1;
 info->y[3]=1;
 info->start_x=5;
 info->start_y=2;
 info->color=5;
 last->next=info;
 last=info;

 info=(struct diamond *)malloc(sizeof(struct diamond));
 info->x[0]=0;
 info->y[0]=0;
 info->x[1]=0;
 info->y[1]=1;
 info->x[2]=0;
 info->y[2]=-1;
 info->x[3]=-1;
 info->y[3]=-1;
 info->start_x=5;
 info->start_y=2;
 info->color=5;
 last->next=info;
 last=info;

 info=(struct diamond *)malloc(sizeof(struct diamond));
 info->x[0]=0;
 info->y[0]=0;
 info->x[1]=-1;
 info->y[1]=0;
 info->x[2]=1;
 info->y[2]=0;
 info->x[3]=1;
 info->y[3]=-1;
 info->start_x=5;
 info->start_y=2;
 info->color=5;
 last->next=info;
 last=info;
 last->next=first;

 for (i=0;i<=random(4);i++)
 {
  first=first->next;
 }
 return first;
}
/*****************************************************************************************/
struct diamond *create_J()
{
 struct diamond *info;
 struct diamond *first;
 struct diamond *last;
 int i;

 info=(struct diamond *)malloc(sizeof(struct diamond));
 info->x[0]=0;
 info->y[0]=0;
 info->x[1]=0;
 info->y[1]=1;
 info->x[2]=0;
 info->y[2]=-1;
 info->x[3]=-1;
 info->y[3]=1;
 info->start_x=5;
 info->start_y=2;
 info->color=6;
 first=info;
 last=info;

 info=(struct diamond *)malloc(sizeof(struct diamond));
 info->x[0]=0;
 info->y[0]=0;
 info->x[1]=-1;
 info->y[1]=0;
 info->x[2]=1;
 info->y[2]=0;
 info->x[3]=-1;
 info->y[3]=-1;
 info->start_x=5;
 info->start_y=2;
 info->color=6;
 last->next=info;
 last=info;

 info=(struct diamond *)malloc(sizeof(struct diamond));
 info->x[0]=0;
 info->y[0]=0;
 info->x[1]=0;
 info->y[1]=1;
 info->x[2]=0;
 info->y[2]=-1;
 info->x[3]=1;
 info->y[3]=-1;
 info->start_x=5;
 info->start_y=2;
 info->color=6;
 last->next=info;
 last=info;

 info=(struct diamond *)malloc(sizeof(struct diamond));
 info->x[0]=0;
 info->y[0]=0;
 info->x[1]=-1;
 info->y[1]=0;
 info->x[2]=1;
 info->y[2]=0;
 info->x[3]=1;
 info->y[3]=1;
 info->start_x=5;
 info->start_y=2;
 info->color=6;
 last->next=info;
 last=info;
 last->next=first;

 for (i=0;i<=random(4);i++)
 {
  first=first->next;
 }
 return first;
}
/*****************************************************************************************/
struct diamond *create_Z()
{
 struct diamond *info;
 struct diamond *first;
 first=(struct diamond *)malloc(sizeof(struct diamond));
 info=(struct diamond *)malloc(sizeof(struct diamond));
 first->next=info;
 info->next=first;
 first->x[0]=0;
 first->y[0]=0;
 first->x[1]=-1;
 first->y[1]=0;
 first->x[2]=0;
 first->y[2]=1;
 first->x[3]=1;
 first->y[3]=1;
 first->start_x=5;
 first->start_y=2;
 first->color=9;

 info->x[0]=0;
 info->y[0]=0;
 info->x[1]=0;
 info->y[1]=1;
 info->x[2]=1;
 info->y[2]=0;
 info->x[3]=1;
 info->y[3]=-1;
 info->start_x=5;
 info->start_y=2;
 info->color=9;

 if(random(2)==0) {return first;}
  else {return first->next;}
}
/*****************************************************************************************/
struct diamond *create_N()
{
 struct diamond *info;
 struct diamond *first;
 first=(struct diamond *)malloc(sizeof(struct diamond));
 info=(struct diamond *)malloc(sizeof(struct diamond));
 first->next=info;
 info->next=first;
 first->x[0]=0;
 first->y[0]=0;
 first->x[1]=0;
 first->y[1]=1;
 first->x[2]=-1;
 first->y[2]=1;
 first->x[3]=1;
 first->y[3]=0;
 first->start_x=5;
 first->start_y=2;
 first->color=14;

 info->x[0]=0;
 info->y[0]=0;
 info->x[1]=0;
 info->y[1]=-1;
 info->x[2]=1;
 info->y[2]=0;
 info->x[3]=1;
 info->y[3]=1;
 info->start_x=5;
 info->start_y=2;
 info->color=14;

 if(random(2)==0) {return first;}
  else {return first->next;}
}
/*****************************************************************************************/
struct diamond *create_H()
{
 struct diamond *first;
 first=(struct diamond *)malloc(sizeof(struct diamond));
 first->next=first;
 first->x[0]=0;
 first->y[0]=0;
 first->x[1]=0;
 first->y[1]=1;
 first->x[2]=1;
 first->y[2]=0;
 first->x[3]=1;
 first->y[3]=1;
 first->start_x=5;
 first->start_y=2;
 first->color=1;
 return first;
}
/*****************************************************************************************/
void show_next ()
{
 int nowx;/*��¼��ǰÿ�����ӵ�λ��*/
 int nowy;
 int i;/*������*/
 int j;
 int haveit;/*��ǰ�����Ƿ��Ѿ���ʾ*/
 struct diamond *next;/*��ǰ����ķ�ת�����һ��״̬*/
 next=nowinfo->next;
 if (next==NULL) {gotoxy(1,1);printf("null");}
 for (i=0;i<=3;i++)/*�ж��Ƿ��ܹ���ת,������,��ֱ���˳��ú���*/
 {
  if (grid[x+next->x[i]][y+next->y[i]]==1)
  {
   return;
  }
 }

 setfillstyle(SOLID_FILL,backcolor);/*���ñ���ɫ����������Ҫ�ĸ���*/
 for (i=0;i<=3;i++)
 {
  haveit=0;
  for (j=0;j<=3;j++)
  {
   if (next->x[j]==nowinfo->x[i]&&next->y[j]==nowinfo->y[i]) {haveit=1;break;}
  }
  if (haveit==0) /*�жϷ�ת��ø����Ƿ���ʾ,������ʾ,���ø�����Ϊ����ɫ*/
  {
  grid[x+nowinfo->x[i]][y+nowinfo->y[i]]=0;
  if (y+nowinfo->y[i]>=4)/*�жϸø����Ƿ��˿�����ʾ������*/
   floodfill(80+(nowinfo->x[i]+x)*20+1,-20+(nowinfo->y[i]+y)*20+1,color);
  }
 }

 nowinfo=next;
 nowx=x;
 nowy=y;
 setfillstyle(SOLID_FILL,nowinfo->color);/*�������ɫΪ�������ɫ*/
 for (i=0;i<=3;i++)
 {
  if (grid[x+nowinfo->x[i]][y+nowinfo->y[i]]!=2)/*����ø��ӻ�û�б���ʾ*/
  {
   nowx=x+nowinfo->x[i];
   nowy=y+nowinfo->y[i];
   if (y+nowinfo->y[i]>=4)
   floodfill(80+nowx*20+1,-20+nowy*20+1,color);
   grid[nowx][nowy]=2; /*���øø��ӵ�ǰ�л����*/
  }
 }
 return;
}
/*****************************************************************************************/
void show_left ()
{
 int i;/*������*/
 int j;
 int haveit;/*��ǰ�����Ƿ��Ѿ���ʾ*/
 int nowx;/*��¼��ǰÿ�����ӵ�λ��*/
 int nowy;
 for (i=0;i<=3;i++)/*�ж��Ƿ���������ƶ�*/
 {
  if (grid[x-1+nowinfo->x[i]][y+nowinfo->y[i]]==1)
  {
   return;
  }
 }

 setfillstyle(SOLID_FILL,backcolor);/*���ñ���ɫ����������Ҫ�ĸ���*/
 for (i=0;i<=3;i++)
 {
  haveit=0;
  for (j=0;j<=3;j++)
  {
   if (nowinfo->x[i]==nowinfo->x[j]-1&&nowinfo->y[i]==nowinfo->y[j]) {haveit=1;break;}
  }
   if (haveit==0) /*�жϷ�ת��ø����Ƿ���ʾ,������ʾ,���ø�����Ϊ����ɫ*/
  {
   grid[x+nowinfo->x[i]][y+nowinfo->y[i]]=0;
   if (y+nowinfo->y[i]>=4)/*�жϸø����Ƿ��˿�����ʾ������*/
   floodfill(80+(nowinfo->x[i]+x)*20+1,-20+(nowinfo->y[i]+y)*20+1,color);
  }
 }

 setfillstyle(SOLID_FILL,nowinfo->color);/*�������ɫΪ�������ɫ*/
 for (i=0;i<=3;i++)
 {
  nowx=x+nowinfo->x[i]-1;
  nowy=y+nowinfo->y[i];
  if (grid[nowx][nowy]!=2)/*����ø��ӻ�û�б���ʾ*/
  {
   if (nowy>=4)
   floodfill(80+nowx*20+1,-20+nowy*20+1,color);
   grid[nowx][nowy]=2;
  }
 }
 x=x-1;
 return;
}
/*****************************************************************************************/
void show_right ()
{
 int i;/*������*/
 int j;
 int haveit;/*��ǰ�����Ƿ��Ѿ���ʾ*/
 int nowx;/*��¼��ǰÿ�����ӵ�λ��*/
 int nowy;
 for (i=0;i<=3;i++)
 {
  if (grid[x+1+nowinfo->x[i]][y+nowinfo->y[i]]==1)
  {
   return;/*�ж��Ƿ���������ƶ�*/
  }
 }

 setfillstyle(SOLID_FILL,backcolor);/*���ñ���ɫ����������Ҫ�ĸ���*/
 for (i=0;i<=3;i++)
 {
  haveit=0;
  for (j=0;j<=3;j++)
  {
   if (nowinfo->x[i]==nowinfo->x[j]+1&&nowinfo->y[i]==nowinfo->y[j]) {haveit=1;break;}
  }
   if (haveit==0)/*�жϷ�ת��ø����Ƿ���ʾ,������ʾ,���ø�����Ϊ����ɫ*/
  {
   grid[x+nowinfo->x[i]][y+nowinfo->y[i]]=0;
   if (y+nowinfo->y[i]>=4)/*�жϸø����Ƿ��˿�����ʾ������*/
   floodfill(80+(nowinfo->x[i]+x)*20+1,-20+(nowinfo->y[i]+y)*20+1,color);
  }
 }


 setfillstyle(SOLID_FILL,nowinfo->color);/*�������ɫΪ�������ɫ*/
 for (i=0;i<=3;i++)
 {
  nowx=x+nowinfo->x[i]+1;
  nowy=y+nowinfo->y[i];
  if (grid[nowx][nowy]!=2)
  {
   if (nowy>=4)/*�жϸø����Ƿ��˿�����ʾ������*/
   floodfill(80+nowx*20+1,-20+nowy*20+1,color);
   grid[nowx][nowy]=2;
  }
 }
 x=x+1;
 return;
}
/*****************************************************************************************/
void show_down ()
{

 int i;/*������*/
 int j;
 int haveit;/*��ǰ�����Ƿ��Ѿ���ʾ*/
 int nowx;/*��¼��ǰÿ�����ӵ�λ��*/
 int nowy;
 int key;
 for (i=0;i<=3;i++)
 {
  if (grid[x+nowinfo->x[i]][y+nowinfo->y[i]+1]==1)/*�жϷ����Ƿ��ܹ�����*/
  {
   for (j=0;j<=3;j++)
   {
    grid[x+nowinfo->x[j]][y+nowinfo->y[j]]=1;
	if (y+nowinfo->y[j]<=3)
	{/*�ж���Ϸ�Ƿ��Ѿ�����*/
	 install(oldtimer);
     setfillstyle(SOLID_FILL,1);
	 bar(0,465,640,480);
	 outtextxy(5,469,"Do you want to restart (Y/N)...");
	 for (;;)
	 {
      key=bioskey(0);
      if (key==YES)
	  {
       startset();
	   setfillstyle(SOLID_FILL,9);
	   bar(0,465,640,480);
	   return;
	  }
      if (key==NO)
	  {
	   closegraph();
	   exit (0);}
	  }
	 }
	}

   delinfo(nowinfo);
   scandel();/*ɨ��,ɾ��*/
   delay(2500);
   while(bioskey(1)) bioskey(0);/*������̻�����*/
  /* clrkey();*/
   nowinfo=nextinfo;/*�õ��µķ���*/
   nextinfo=get_diamond();/*�õ���һ������*/
   showsubwin(nextinfo);
   x=nowinfo->start_x;/*�������÷���λ��*/
   y=nowinfo->start_y;
   return;
  }
 }

 setfillstyle(SOLID_FILL,backcolor);/*���ñ���ɫ����������Ҫ�ĸ���*/

 for (i=0;i<=3;i++)
 {
  haveit=0;
  for (j=0;j<=3;j++)
  {
   if (nowinfo->x[i]==nowinfo->x[j]&&nowinfo->y[i]==nowinfo->y[j]+1) {haveit=1;break;}
  }
   if (haveit==0)/*�жϷ�ת��ø����Ƿ���ʾ,������ʾ,���ø�����Ϊ����ɫ*/
  {
   grid[x+nowinfo->x[i]][y+nowinfo->y[i]]=0;
   if (y+nowinfo->y[i]>=4)/*�жϸø����Ƿ��˿�����ʾ������*/
    floodfill(80+(nowinfo->x[i]+x)*20+1,-20+(nowinfo->y[i]+y)*20+1,color);
  }
 }
 setfillstyle(SOLID_FILL,nowinfo->color);/*�������ɫΪ�������ɫ*/
 for (i=0;i<=3;i++)
 {
  nowx=x+nowinfo->x[i];
  nowy=y+nowinfo->y[i]+1;
  if (grid[nowx][nowy]!=2)/*����ø��ӻ�û�б���ʾ*/
  {
   if (nowy>=4)
   floodfill(80+nowx*20+1,-20+nowy*20+1,color);
   grid[nowx][nowy]=2;
  }
 }
 y=y+1;
 return;
}
/*****************************************************************************************/
/*void print()�����ú���
{
int i;
int j;
gotoxy(1,1);
for (j=0;j<=22;j++)
{for (i=0;i<=11;i++)
{
printf ("%d",grid[i][j]);}
printf ("\n");
}
}*/
/*****************************************************************************************/
void scandel()
{
 int i;
 int k;
 int j;
 int num;
 int scoreadd;
 scoreadd=0;
 for (k=21;k>=4;k--)
 {
  num=0;
  for (i=1;i<=10;i++)
  {
   if (grid[i][k]==1) num++;
  }
  if (num==10)
  {
   scoreadd++;
   for (i=1;i<=10;i++)
   {
    setfillstyle(SOLID_FILL,backcolor);
    floodfill(80+i*20+1,-20+k*20+1,color);
   }

   for (j=k;j>=5;j--)
   {
	for (i=1;i<=10;i++)
	{
	 setfillstyle(SOLID_FILL,getpixel(80+i*20+1,-40+j*20+1));
     floodfill(80+i*20+1,-20+j*20+1,color);
	 grid[i][j]=grid[i][j-1];
	}
   }
   for (i=1;i<=10;i++)
   {
    setfillstyle(SOLID_FILL,backcolor);
    floodfill(80+i*20+1,61,color);
	grid[i][4]=0;
   }
   k++;
  }
 }
if (scoreadd!=0) showscore(2*scoreadd-1);
}
/*****************************************************************************************/
void interrupt newtimer()/*�µĶ�ʱ�жϵ���*/
{
  int leveltemp;
  leveltemp=11-level;
  count++;
  if(count>=leveltemp)
  {
  addtobuffer(DOWN);
  count=0;
  }
 (*oldtimer)();
}
/*****************************************************************************************/
void install (void interrupt (*paddr)())
{
 disable();
 setvect(0x1c,paddr);
 enable();
}
/******************************************************************************************/
void delinfo (struct diamond *info)/*�ͷſ��ٵĿռ�*/
{
 struct diamond *now;
 struct diamond *next;
 now=info->next;
 next=info->next;
 while(next!=info)
 {
  next=now->next;
  free(now);
  now=next;
 }
 free(info);
 }
/******************************************************************************************/
void addtobuffer(int c)/*����̻�����������һ��DOWN*/
{
 unsigned i;
 i=*(unsigned far *)0x0040001CL;
 *(unsigned far *)(0x00400000L | i)=c;
 i+=2;
 if (i>=*(unsigned far *)0x00400082L) i=*(unsigned far *)0x00400080L;
 *(unsigned far *)0x0040001CL=i;
}
/******************************************************************************************/
/*void clrkey()����dos�ж���ռ��̻�����,δʹ�ô˷���.
{
  union REGS in ;
  in.h.ah = 0x0c ;
  in.h.al = 0x00 ;
  intdos(&in , &in) ;
}
*/
/******************************************************************************************/
void showsubwin(struct diamond *next)
{
 int i;/*������*/
 int j;
 setfillstyle (SOLID_FILL,backcolor);
 for (i=0;i<=3;i++)
 {
  for(j=0;j<=3;j++)
  {
   floodfill(361+20*i,61+20*j,color);
  }
 }
 setfillstyle (SOLID_FILL,next->color);
 for (i=0;i<=3;i++)
 {
  floodfill(381+next->x[i]*20,81+next->y[i]*20,color);
 }
}
/******************************************************************************************/
void showscore(int scoreget)
{
 char sscore[6];
 char slevel[2];
 score+=scoreget;
 if (score<1000) level=score/100+1;
 sprintf(sscore,"%d",score);
 sprintf(slevel,"%d",level);
 setfillstyle(SOLID_FILL,9);
 bar(406,179,490,203);
 setcolor(14);
 outtextxy (408,180,slevel);
 outtextxy (408,195,sscore);
}
/*******************************************************************************************/
void startset()
{
 int i;
 int j;
 setfillstyle(SOLID_FILL,backcolor);
 for(i=0;i<=21;i++)/*���³�ʼ��*/
 {
  for(j=1;j<=10;j++)
  if (grid[j][i]!=0)
  {
  grid[j][i]=0;
  if (i>=4) floodfill(80+j*20+1,-20+i*20+1,color);
  }
 }
 score=0;
 showscore(0);
 nowinfo=get_diamond ();/*�õ�һ����ǰ����*/
 x=nowinfo->start_x;/*��ʼ������λ��*/
 y=nowinfo->start_y;
 nextinfo=get_diamond ();/*�õ���һ������*/
 showsubwin(nextinfo);
 install(newtimer);
}
/*************************
<Russia Diamonds Ver 1.0>
Copyright by forever_chang
forever_chang@163.com
2001.11.1
*************************/
/*****************************************************************************************/
#include "graphics.h" /*头文件*/
#include "time.h"
#include "stdlib.h"
#include "bios.h"
#include "dos.h"
#include "stdio.h"
#define ESC 0x11b /*键盘扫描码*/
#define UP 0x4800
#define DOWN 0x5000
#define LEFT 0x4b00
#define F1 0x3b00
#define RIGHT 0x4d00
#define YES 0x1579
#define NO 0x316e
#define RESTART 0x1372
/*****************************************************************************************/
struct diamond /*记录每种方块每种状态的信息*/
{
 int x[4];
 int y[4];
 int start_x;
 int start_y;
 int color;
 struct diamond *next;
};
int grid[12][23]; /*记录所有格子的状态 (0)没有方块 (1)有固定方块 (2)有活动方块*/
int x; /*活动方块所在位置*/
int y;
int level; /*游戏难度*/
int count; /*计数器*/
int score;/*得分*/
struct diamond *nowinfo; /*当前活动方块*/
struct diamond *nextinfo; /*下一个方块*/
int color;/*画格子的颜色*/
int backcolor;/*游戏区域背景色*/
/*****************************************************************************************/
void show_interface (int mode);/*显示游戏界面*/
void show_copsign (int topx,int topy,int size,int color);/*显示公司标志--恒基伟业*/
void show_intro (int xs,int ys);/*显示软件介绍区*/
/*void print(); 测试用函数*/
void scandel();/*扫描所有格子看是否有可消除行*/
void show_down ();/*方块下落后的下一个状态*/
void show_next ();/*方块翻转后的下一个状态*/
void show_left ();/*方块向左移动后的下一个状态*/
void show_right ();/*方块向右移动后的下一个状态*/
void interrupt (*oldtimer)();/*指向未安装前的中断向量,即函数指针,指向一段可执行的代码*/
void install();/*安装新的中断向量,即将中断服务程序安装到中断向量表中*/
void interrupt newtimer();/*中断服务程序*/
struct diamond *get_diamond();/*随机得到一个方块*/
struct diamond *create_I();/*函数用来构造各种类形方块的环形链表*/
struct diamond *create_T();/*返回链表中随机一个状态的指针*/
struct diamond *create_L();
struct diamond *create_J();
struct diamond *create_Z();
struct diamond *create_N();
struct diamond *create_H();
void delinfo (struct diamond *info);/*释放当前方块所占用的空间*/
void addtobuffer(int c);/*向键盘缓冲区中增加一个DOWN*/
/*void clrkey();调用dos中断清空键盘缓冲区,未使用此方法.*/
void showsubwin(struct diamond *next);/*在小窗口显示下一个方块*/
void showscore(int scoreget);/*显示分数*/
void startset();/*初始化游戏*/
/*****************************************************************************************/
main ()
{
 int driver=VGA;
 int mode=VGAHI;
 int i;/*计数器*/
 int j;
 int key;/*记录键盘扫描码*/
 initgraph (&driver,&mode,"");/*初始化*/
 color=8;
 backcolor=7;
 level=1;
 score=298;
 count=0;
 show_interface (9);/*显示界面*/
 setfillstyle(SOLID_FILL,1);
 bar(0,465,640,480);
 outtextxy(5,469,"Press any key to start...");
 getch();
 setfillstyle(SOLID_FILL,9);
 bar(0,465,640,480);
 showscore(0);/*显示分数*/
 randomize();
 nowinfo=get_diamond ();/*得到一个当前方块*/
 oldtimer=getvect(0x1c);
 install(newtimer);
 for(i=0;i<=21;i++)/*初始化grid[12][23]*/
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

 x=nowinfo->start_x;/*初始化方块位置*/
 y=nowinfo->start_y;
 nextinfo=get_diamond ();/*得到下一个方块*/
 showsubwin(nextinfo);

 for (;;)
 {
  key=bioskey(0);/*得到键盘扫描码*/
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
         	       key=bioskey(0);/*得到键盘扫描码*/
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
         	   key=bioskey(0);/*得到键盘扫描码*/
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
 int nowx;/*记录当前每个格子的位置*/
 int nowy;
 int i;/*计数器*/
 int j;
 int haveit;/*当前格子是否已经显示*/
 struct diamond *next;/*当前方块的翻转后的下一个状态*/
 next=nowinfo->next;
 if (next==NULL) {gotoxy(1,1);printf("null");}
 for (i=0;i<=3;i++)/*判断是否能够翻转,若不能,就直接退出该函数*/
 {
  if (grid[x+next->x[i]][y+next->y[i]]==1)
  {
   return;
  }
 }

 setfillstyle(SOLID_FILL,backcolor);/*设置背景色以消除不需要的格子*/
 for (i=0;i<=3;i++)
 {
  haveit=0;
  for (j=0;j<=3;j++)
  {
   if (next->x[j]==nowinfo->x[i]&&next->y[j]==nowinfo->y[i]) {haveit=1;break;}
  }
  if (haveit==0) /*判断翻转后该格子是否显示,若不显示,将该格子设为背景色*/
  {
  grid[x+nowinfo->x[i]][y+nowinfo->y[i]]=0;
  if (y+nowinfo->y[i]>=4)/*判断该格子是否到了可以显示的区域*/
   floodfill(80+(nowinfo->x[i]+x)*20+1,-20+(nowinfo->y[i]+y)*20+1,color);
  }
 }

 nowinfo=next;
 nowx=x;
 nowy=y;
 setfillstyle(SOLID_FILL,nowinfo->color);/*设置填冲色为方块的颜色*/
 for (i=0;i<=3;i++)
 {
  if (grid[x+nowinfo->x[i]][y+nowinfo->y[i]]!=2)/*如果该格子还没有被显示*/
  {
   nowx=x+nowinfo->x[i];
   nowy=y+nowinfo->y[i];
   if (y+nowinfo->y[i]>=4)
   floodfill(80+nowx*20+1,-20+nowy*20+1,color);
   grid[nowx][nowy]=2; /*设置该格子当前有活动方块*/
  }
 }
 return;
}
/*****************************************************************************************/
void show_left ()
{
 int i;/*计数器*/
 int j;
 int haveit;/*当前格子是否已经显示*/
 int nowx;/*记录当前每个格子的位置*/
 int nowy;
 for (i=0;i<=3;i++)/*判断是否可以向左移动*/
 {
  if (grid[x-1+nowinfo->x[i]][y+nowinfo->y[i]]==1)
  {
   return;
  }
 }

 setfillstyle(SOLID_FILL,backcolor);/*设置背景色以消除不需要的格子*/
 for (i=0;i<=3;i++)
 {
  haveit=0;
  for (j=0;j<=3;j++)
  {
   if (nowinfo->x[i]==nowinfo->x[j]-1&&nowinfo->y[i]==nowinfo->y[j]) {haveit=1;break;}
  }
   if (haveit==0) /*判断翻转后该格子是否显示,若不显示,将该格子设为背景色*/
  {
   grid[x+nowinfo->x[i]][y+nowinfo->y[i]]=0;
   if (y+nowinfo->y[i]>=4)/*判断该格子是否到了可以显示的区域*/
   floodfill(80+(nowinfo->x[i]+x)*20+1,-20+(nowinfo->y[i]+y)*20+1,color);
  }
 }

 setfillstyle(SOLID_FILL,nowinfo->color);/*设置填冲色为方块的颜色*/
 for (i=0;i<=3;i++)
 {
  nowx=x+nowinfo->x[i]-1;
  nowy=y+nowinfo->y[i];
  if (grid[nowx][nowy]!=2)/*如果该格子还没有被显示*/
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
 int i;/*计数器*/
 int j;
 int haveit;/*当前格子是否已经显示*/
 int nowx;/*记录当前每个格子的位置*/
 int nowy;
 for (i=0;i<=3;i++)
 {
  if (grid[x+1+nowinfo->x[i]][y+nowinfo->y[i]]==1)
  {
   return;/*判断是否可以向右移动*/
  }
 }

 setfillstyle(SOLID_FILL,backcolor);/*设置背景色以消除不需要的格子*/
 for (i=0;i<=3;i++)
 {
  haveit=0;
  for (j=0;j<=3;j++)
  {
   if (nowinfo->x[i]==nowinfo->x[j]+1&&nowinfo->y[i]==nowinfo->y[j]) {haveit=1;break;}
  }
   if (haveit==0)/*判断翻转后该格子是否显示,若不显示,将该格子设为背景色*/
  {
   grid[x+nowinfo->x[i]][y+nowinfo->y[i]]=0;
   if (y+nowinfo->y[i]>=4)/*判断该格子是否到了可以显示的区域*/
   floodfill(80+(nowinfo->x[i]+x)*20+1,-20+(nowinfo->y[i]+y)*20+1,color);
  }
 }


 setfillstyle(SOLID_FILL,nowinfo->color);/*设置填冲色为方块的颜色*/
 for (i=0;i<=3;i++)
 {
  nowx=x+nowinfo->x[i]+1;
  nowy=y+nowinfo->y[i];
  if (grid[nowx][nowy]!=2)
  {
   if (nowy>=4)/*判断该格子是否到了可以显示的区域*/
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

 int i;/*计数器*/
 int j;
 int haveit;/*当前格子是否已经显示*/
 int nowx;/*记录当前每个格子的位置*/
 int nowy;
 int key;
 for (i=0;i<=3;i++)
 {
  if (grid[x+nowinfo->x[i]][y+nowinfo->y[i]+1]==1)/*判断方块是否能够下落*/
  {
   for (j=0;j<=3;j++)
   {
    grid[x+nowinfo->x[j]][y+nowinfo->y[j]]=1;
	if (y+nowinfo->y[j]<=3)
	{/*判断游戏是否已经玩完*/
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
   scandel();/*扫描,删除*/
   delay(2500);
   while(bioskey(1)) bioskey(0);/*清除键盘缓冲区*/
  /* clrkey();*/
   nowinfo=nextinfo;/*得到新的方块*/
   nextinfo=get_diamond();/*得到下一个方块*/
   showsubwin(nextinfo);
   x=nowinfo->start_x;/*重新设置方块位置*/
   y=nowinfo->start_y;
   return;
  }
 }

 setfillstyle(SOLID_FILL,backcolor);/*设置背景色以消除不需要的格子*/

 for (i=0;i<=3;i++)
 {
  haveit=0;
  for (j=0;j<=3;j++)
  {
   if (nowinfo->x[i]==nowinfo->x[j]&&nowinfo->y[i]==nowinfo->y[j]+1) {haveit=1;break;}
  }
   if (haveit==0)/*判断翻转后该格子是否显示,若不显示,将该格子设为背景色*/
  {
   grid[x+nowinfo->x[i]][y+nowinfo->y[i]]=0;
   if (y+nowinfo->y[i]>=4)/*判断该格子是否到了可以显示的区域*/
    floodfill(80+(nowinfo->x[i]+x)*20+1,-20+(nowinfo->y[i]+y)*20+1,color);
  }
 }
 setfillstyle(SOLID_FILL,nowinfo->color);/*设置填冲色为方块的颜色*/
 for (i=0;i<=3;i++)
 {
  nowx=x+nowinfo->x[i];
  nowy=y+nowinfo->y[i]+1;
  if (grid[nowx][nowy]!=2)/*如果该格子还没有被显示*/
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
/*void print()测试用函数
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
void interrupt newtimer()/*新的定时中断调用*/
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
void delinfo (struct diamond *info)/*释放开辟的空间*/
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
void addtobuffer(int c)/*向键盘缓冲区中增加一个DOWN*/
{
 unsigned i;
 i=*(unsigned far *)0x0040001CL;
 *(unsigned far *)(0x00400000L | i)=c;
 i+=2;
 if (i>=*(unsigned far *)0x00400082L) i=*(unsigned far *)0x00400080L;
 *(unsigned far *)0x0040001CL=i;
}
/******************************************************************************************/
/*void clrkey()调用dos中断清空键盘缓冲区,未使用此方法.
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
 int i;/*计数器*/
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
 for(i=0;i<=21;i++)/*重新初始化*/
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
 nowinfo=get_diamond ();/*得到一个当前方块*/
 x=nowinfo->start_x;/*初始化方块位置*/
 y=nowinfo->start_y;
 nextinfo=get_diamond ();/*得到下一个方块*/
 showsubwin(nextinfo);
 install(newtimer);
}
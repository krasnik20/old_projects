#include <conio.h>
#include <math.h>
#include <iostream>
#include <stdio.h>
#include <dos.h>
#include <windows.h>
#include <graphics.h>
#include <time.h>
#define UP VK_UP //�������� ������. ���� �������� �� ���, �� ��������� ���������� �����
#define DOWN VK_DOWN
#define RIGHT VK_RIGHT
#define LEFT VK_LEFT
#define SHAKE VK_RETURN
#define EXIT VK_ESCAPE
//"�����"-��� ������ ����� �� ���� ������-���� �������, ���� ����� �� ������. �� ������ � ������
//����� �������� ���, ��� � ���� ����� ������� ��������. ���� ����� ����, ������ ���� ������ ����� ������ - ��������������� ����.
//���� ����� �����, �� 15, ��� ����� � ����. ������, ��������� �� ���, �� ��� ��� ��������.
//� �� � ����� ��� ��������: 
//1) ���� ������� �������
//2) �������, ��� �� ������� ������
//3) ���� ����������, �� ������ �� ���� � ������ �������� ����������
//4) ���� ������� �� ��������� ������ ������. ���� �����-�� ����� ����� ���������� �� ���, �� ���� ���������� � ���� ����� "���"
//5) ����� ����
//6) � �� �����
//���� ���-�� �� �������, ��� �������� - ��������� � �����������, ������� delay ����� ������ �������� ��� ����������� 
using namespace std;
class chip //�����
{
	private:
		double x,y,w;//���������� � ������
		char str[2];//������, � ������� �������� ����� ��� ������ �� �����
	public:
		chip(double,double,double,int);//�����������
		void show();//��������
		void hide();//��������
		void move(double,double);//��������
};
class field //����
{
	private:
		chip*chips[15];//������ ���������� �� �����(�������, ��� ������ ������ �����, ��������� ����� ������������ ������������� ����� � ����� ������ ������������ ����)
		int pos[4][4];//������ ��������� ���������� �����,�.�.� ������ ������ ����� ����� �����, ������� ��������� � ���� ������ �� ����
		double x,y,w,f;//���������� � ������ �������� ���� � ������� (�� �� ���� ������� �������, �� ���� ��� �����), ������� �����
		int zeroX,zeroY;//������� ��������� ������ � ����� (����� ������ ��� �� �� �����������
	public:
		field(double,double,double);//�����������
		~field();//����������. �� ������������� ���������� �������� ��� ���������� main, ��� �������� �� �����. �����, ��������� �� ��� ����������� �������� ������
		void show();//��������. hide �� ����������
		void shake();//���������� �����
		void act(int);//���������� �������� �� �������
};
chip::chip(double xx,double yy,double ww,int n)
{
	x=xx;
	y=yy;
	w=ww;
	sprintf(str,"%2d",n+1);//������������ � ������� n+1, ��������� ���������� � �������, ������ �� ������ �������� �����������, � ����
}//�� ���� �������, ����� � ������ ��������� 1, � ��� ������, ������� ���
void chip::show()
{
	bar(x,y,x+w,y+w);//������� ������ �������������
	rectangle(x,y,x+w-1,y+w-1);//����� �������������� (-1 ������ ��� �� ������ �������� �� 1 ������, ��� bar, �� ������)
	outtextxy(x+w/3,y+w/3,str);//�������� ����� �� ��������
}
void chip::hide()
{
	setfillstyle(SOLID_FILL,YELLOW);
	bar(x,y,x+w,y+w);//������ ����������� ������ ���������������
	setfillstyle(SOLID_FILL,WHITE);
}
void chip::move(double dx,double dy)
{//��������
	hide();
	x+=dx;
	y+=dy;
	show();
}
field::field(double xx,double yy,double ww) 
{
	x=xx+ww/10;//x - ������ ��������������� �������� ����, ������� ������������ �����, ���������� � ������ ww
	y=yy+ww/10;//�� �� �����
	w=ww*4/5;//����� ����� ����������� � ������ ww. ����� - ww*1/10, ��� ����� = 2/10 = 1/5 => ������ �������� ���� w = ww*4/5
	f=ww/10;//������� �����
	for(int i=0;i<4;i++)
		for (int j=0; j<4&&i*j<9;j++)//������ ������� ����� �� ������� �� ������� 15, �������� ���
		{
			chips[i*4+j]=new chip(x+j*w/4,y+i*w/4,w/4,i*4+j);//������� ����� �����, ���������� �� ��� ���������
			pos[i][j]=i*4+j;//����������, ��� �����-�� ����� �� ������-�� ������
		}//����� ���������� �� ���� ����, � � pos ��������� ����� ����� ������� ������� �� 4 
	pos[3][3]=0;//� ��������� ������ 0 
	zeroX=3;//� �������������� ��������� ���� 
	zeroY=3;
}
field::~field(){ for (int i=0; i<15;i++) delete chips[i]; }//����������� ������ 
void field::show()
{
	setfillstyle(SOLID_FILL,YELLOW);
	bar(x-f,y-f,x+w+f,y+w+f);//������������� (������, � ������)
	rectangle(x-f,y-f,x+w+f-1,y+w+f-1);//����� 
	rectangle(x-1,y-1,x+w,y+w);//����� �������� ���� (-1 ����� ���������� �� ������� �� �����, � � ������ ��������� ��� -1, ��������� ��� ������ ������� (������ �����)
	setfillstyle(SOLID_FILL,WHITE);//��� ����� ����
	for(int i=0;i<15;i++)//�������� ��� �����
		chips[i]->show();
}
void field::shake()
{
	int key,i;	
	for(i=0;i<10;i++)//10 �������������
	{//������������� ���������� ��������������� ����� ����������� ������ �����, ��� ���� ������ �������� ��������� ������ � �������, �� ����� ���������� 
	//���������� ����������, � ��� ��� ����? ���. � ��� �� ����� �����, ��� �� ������ ��������� ��� �������� �����, � ����� �������, �����.
		key=rand()%4;//��������� ������� ��������� ������� �������
		if(key==0)key=UP;
		else if(key==1)key=DOWN;
		else if(key==2)key=RIGHT;
		else if(key==3)key=LEFT;//����� � �������� ���������� if-�� �������� key==UP �� key==0, �� ��� ���������, ���� � ���� �������
		if (key==DOWN&&zeroY!=0) //����� ������, ��� ��� ����������, ������� ����� � field::act, ��� ��������� ������ ���� ��, ��� ���
		{
			chips[pos[zeroY-1][zeroX]]->move(0,w/4);
			pos[zeroY][zeroX]=pos[zeroY-1][zeroX];
			pos[zeroY-1][zeroX]=0;
			zeroY--;
		}
		else if (key==UP&&zeroY!=3)
		{
			chips[pos[zeroY+1][zeroX]]->move(0,-w/4);
			pos[zeroY][zeroX]=pos[zeroY+1][zeroX];
			pos[zeroY+1][zeroX]=0;
			zeroY++;
		}
		else if (key==RIGHT&&zeroX!=0)
		{
			chips[pos[zeroY][zeroX-1]]->move(w/4,0);
			pos[zeroY][zeroX]=pos[zeroY][zeroX-1];
			pos[zeroY][zeroX-1]=0;
			zeroX--;
		}
		else if (key==LEFT&&zeroX!=3)
		{
			chips[pos[zeroY][zeroX+1]]->move(-w/4,0);
			pos[zeroY][zeroX]=pos[zeroY][zeroX+1];
			pos[zeroY][zeroX+1]=0;
			zeroX++;
		}
	}
}
void field::act(int key)//���������� ��� ������, ������� ���� ������
{
	if (key==SHAKE)//���� ����������, �� ������ � �������
	{
		shake();
		return;
	}//�����
	double frames=10,dx=0,dy=0;//frames-���������� ������ � ��������, �� ���� �� ������� move ����� ��������� �� ����� �������,dx,dy-����� �� ���� ����� move
	int i;//�����, � ������� ���������� �����������. ��� �����, ����� ������� ���������� � ����� �����, ������ ����� � ����������� ��������, � ����� ��� �������
	//����� � ������ ��� �� ��������� �� ������ for-�
	if (key==DOWN&&zeroY!=0) //���� ������ �������� ���� ������ � ��� ���� ������ ������ �� ������� (�� ���� ����� ��������� ��� ��������)
	{
		dy=w/4/frames; //������ ����� (�.�. ����/4) �������� �� ���������� ������ � ������ ���� (��� y ������������� ����, �� ���� �������� � �� ������������� �����������)
		i=pos[zeroY-1][zeroX];//����������, ����� ����� ����� ������ �� ����
		pos[zeroY][zeroX]=pos[zeroY-1][zeroX];//���������� �� � ������� ����
		pos[zeroY-1][zeroX]=0;//�� �� ����� ������ ����
		zeroY--;//� ������ ����������� ������� ����
	}
	else if (key==UP&&zeroY!=3)//������ ����������� ������. ��� ����� � ����� ������ ������ �� �����
	{
		dy=-w/4/frames;
		i=pos[zeroY+1][zeroX];
		pos[zeroY][zeroX]=pos[zeroY+1][zeroX];
		pos[zeroY+1][zeroX]=0;
		zeroY++;
	}
	else if (key==RIGHT&&zeroX!=0)
	{
		dx=w/4/frames;
		i=pos[zeroY][zeroX-1];
		pos[zeroY][zeroX]=pos[zeroY][zeroX-1];
		pos[zeroY][zeroX-1]=0;
		zeroX--;
	}
	else if (key==LEFT&&zeroX!=3)
	{
		dx=-w/4/frames;
		i=pos[zeroY][zeroX+1];
		pos[zeroY][zeroX]=pos[zeroY][zeroX+1];
		pos[zeroY][zeroX+1]=0;
		zeroX++;
	} else return;//���� ������ �� �����, �� ���� ������ ������� ������, �������
	for(double j=0;j<frames;j++) //� ������ ������� ��������� ����� �� ����-���� � ��������� ������� ������� ���, ������� ������ �������
	{
		chips[i]->move(dx,dy);
		delay(20);//��������� ���������� ��� ���������
	}
}
void tips(double x,double y, double w)
{ 
	settextstyle(COMPLEX_FONT,0,1);
	//4 ������ �� �����������
	rectangle(x+w,y,x+2*w,y+w);
	rectangle(x,y+w,x+w,y+2*w);
	rectangle(x+w,y+w,x+2*w,y+2*w);
	rectangle(x+2*w,y+w,x+3*w,y+2*w);
	//����� �������
	line(x+w/4,y+w+w/2,x+w/2,y+w+w/4);
	line(x+w/4,y+w+w/2,x+w/2,y+w+w*3/4);
	//������
	line(x+2*w+w*3/4,y+w+w/2,x+2*w+w/2,y+w+w/4);
	line(x+2*w+w*3/4,y+w+w/2,x+2*w+w/2,y+w+w*3/4);
	//�������
	line(x+w+w/4,y+w/2,x+w+w/2,y+w/4);
	line(x+w+w/2,y+w/4,x+w+w*3/4,y+w/2);
	//������
	line(x+w+w/4,y+w+w/2,x+w+w/2,y+w+w*3/4);
	line(x+w+w/2,y+w+w*3/4,x+w+w*3/4,y+w+w/2);
	//�������
	outtextxy(x+3.5*w,y+w/2,"- Play");
	
	//enter
	rectangle(x,y+3*w,x+3*w,y+4*w);
	outtextxy(x+w/10,y+3*w+w/10,"Enter");
	outtextxy(x+3.5*w,y+3*w,"- Shake chips");
	
	//escape
	rectangle(x+1.2*w,y+5*w,x+3*w,y+6*w);
	outtextxy(x+1.2*w+w/10,y+5*w+w/10,"Esc");
	outtextxy(x+3.5*w,y+5*w,"- Exit");
}
int main()
{
	srand(time(NULL));
	field F(310,90,300);//������� ����
	
	//������
	initwindow(640,480);//���������� ������ ������, ��� ���� � �����, ����� ����� ������ �� ���������
	setfillstyle(SOLID_FILL,WHITE);
	bar(0,0,640,480);
	//� �� ���� ����� ������� � �����
	
	//� ��� ��� ����������������� ��� �����
	/*int gdriver = DETECT, gmode;
 	initgraph(&gdriver,&gmode,"");*/
	setbkcolor(WHITE);
	setcolor(BLACK);//���� ������ � ������
	tips(20,120,40);
	settextstyle(DEFAULT_FONT,0,2);
	F.show();
	while(true) 
	{
		getch();//���� ������ ���, �� ����� ������ ����������� ����, ��� ������ ���������, � ��� ����� "�����������" ������ ����� ������ �������
		if (GetKeyState(UP)<0) F.act(UP);//������������������������(�����). ���� ������ 0 - ������ ������. 
			//��, �������� �����, �� � � ���� �� �� �����������, ��� �� windows.h, �������. �� � ���� ������ - ��������� �������
		if (GetKeyState(DOWN)<0) F.act(DOWN); //����������
		if (GetKeyState(RIGHT)<0) F.act(RIGHT);
		if (GetKeyState(LEFT)<0) F.act(LEFT);
		if (GetKeyState(SHAKE)<0) F.act(SHAKE);
		if (GetKeyState(EXIT)<0) break;
	}
	
	return 0;
}

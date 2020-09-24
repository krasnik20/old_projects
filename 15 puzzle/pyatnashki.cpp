#include <conio.h>
#include <math.h>
#include <iostream>
#include <stdio.h>
#include <dos.h>
#include <windows.h>
#include <graphics.h>
#include <time.h>
#define UP VK_UP //названия клавиш. если поменять их тут, то автоматом поменяются везде
#define DOWN VK_DOWN
#define RIGHT VK_RIGHT
#define LEFT VK_LEFT
#define SHAKE VK_RETURN
#define EXIT VK_ESCAPE
//"абрис"-это черная линия по краю какого-либо объекта, если вдруг не знаешь. не путать с рамкой
//прога работает так, как я тебе тогда словами описывал. есть класс поле, только один объект этого класса - непосредственно поле.
//есть класс фишка, их 15, они лежат в поле. точнее, указатели на них, но это уже тонкости.
//и мы в итоге так работаем: 
//1) ждем нажатие клавиши
//2) смотрим, что за клавишу нажали
//3) если подходящую, то отдаем ее полю и просим проявить активность
//4) поле смотрит на положение пустой клетки. если какой-то фишке можно сдвинуться на нее, то поле обращается к этой фишке "иди"
//5) фишка идет
//6) и по новой
//если что-то не понятно, как рисуется - поиграйся с параметрами, поставь delay после каждой операции для наглядности 
using namespace std;
class chip //фишка
{
	private:
		double x,y,w;//координаты и размер
		char str[2];//строка, в которой хранится число для вывода на экран
	public:
		chip(double,double,double,int);//конструктор
		void show();//показать
		void hide();//спрятать
		void move(double,double);//сдвинуть
};
class field //поле
{
	private:
		chip*chips[15];//массив указателей на фишки(удобнее, чем просто массив фишек, поскольку можно пользоваться конструктором фишек в цикле внутри конструктора поля)
		int pos[4][4];//массив положений конкретных фишек,т.е.в каждой ячейке лежит номер фишки, которая находится в этой клетке на поле
		double x,y,w,f;//координаты и размер игрового поля с фишками (НО НЕ ВСЕЙ ИГРУШКИ ЦЕЛИКОМ, ТО ЕСТЬ БЕЗ РАМКИ), толщина рамки
		int zeroX,zeroY;//текущее положение клетки с нулем (чтобы каждый раз ее не разыскивать
	public:
		field(double,double,double);//конструктор
		~field();//деструктор. он автоматически вызывается системой при завершении main, его вызывать не нежно. нужен, поскольку мы там динамически выделяем память
		void show();//показать. hide не пригодится
		void shake();//перемешать фишки
		void act(int);//произвести действие по клавише
};
chip::chip(double xx,double yy,double ww,int n)
{
	x=xx;
	y=yy;
	w=ww;
	sprintf(str,"%2d",n+1);//записывается в строчку n+1, поскольку нумеруются в массиве, откуда мы дальше вызываем конструктор, с нуля
}//но если хочется, можно в вызове добавлять 1, а тут убрать, разницы нет
void chip::show()
{
	bar(x,y,x+w,y+w);//залитый цветом прямоугольник
	rectangle(x,y,x+w-1,y+w-1);//абрис прямоугольника (-1 потому что он рисует наоборот на 1 больше, чем bar, хз почему)
	outtextxy(x+w/3,y+w/3,str);//написать число на клеточке
}
void chip::hide()
{
	setfillstyle(SOLID_FILL,YELLOW);
	bar(x,y,x+w,y+w);//просто закрашиваем желтым прямоугольником
	setfillstyle(SOLID_FILL,WHITE);
}
void chip::move(double dx,double dy)
{//очевидно
	hide();
	x+=dx;
	y+=dy;
	show();
}
field::field(double xx,double yy,double ww) 
{
	x=xx+ww/10;//x - начало непосредственно игрового поля, поэтому компенсируем рамку, заложенную в размер ww
	y=yy+ww/10;//то же самое
	w=ww*4/5;//чтобы рамки вписывались в размер ww. рамка - ww*1/10, две рамки = 2/10 = 1/5 => ширина игрового поля w = ww*4/5
	f=ww/10;//толщина рамки
	for(int i=0;i<4;i++)
		for (int j=0; j<4&&i*j<9;j++)//второе условие чтобы не залезть по индексу 15, которого нет
		{
			chips[i*4+j]=new chip(x+j*w/4,y+i*w/4,w/4,i*4+j);//создаем новую фишку, запоминаем на нее указатель
			pos[i][j]=i*4+j;//записываем, что такая-то фишка по такому-то адресу
		}//можно переделать на один цикл, а в pos адресацию вести через деление индекса на 4 
	pos[3][3]=0;//в последней ячейке 0 
	zeroX=3;//и соответственно положение нуля 
	zeroY=3;
}
field::~field(){ for (int i=0; i<15;i++) delete chips[i]; }//освобождаем память 
void field::show()
{
	setfillstyle(SOLID_FILL,YELLOW);
	bar(x-f,y-f,x+w+f,y+w+f);//прямоугольник (заметь, с рамкой)
	rectangle(x-f,y-f,x+w+f-1,y+w+f-1);//абрис 
	rectangle(x-1,y-1,x+w,y+w);//абрис игрового поля (-1 чтобы получилось не заехать на фишки, а у вторых координат нет -1, поскольку оно рисует лишнего (описал ранее)
	setfillstyle(SOLID_FILL,WHITE);//для фишек цвет
	for(int i=0;i<15;i++)//показать все фишки
		chips[i]->show();
}
void field::shake()
{
	int key,i;	
	for(i=0;i<10;i++)//10 перемешиваний
	{//перемешивание происходит непосредственно энным количеством сдвига фишек, ибо если просто рандомно заполнить массив с фишками, то может получиться 
	//нерешаемая комбинация, а оно нам надо? нет. а тут мы точно знаем, что мы просто испортили уже решенную штуку, и можно вернуть, играя.
		key=rand()%4;//случайным образом эмулируем нажатие клавиши
		if(key==0)key=UP;
		else if(key==1)key=DOWN;
		else if(key==2)key=RIGHT;
		else if(key==3)key=LEFT;//можно в условиях дальнейших if-ов поменять key==UP на key==0, но так нагляднее, хотя и чуть сложнее
		if (key==DOWN&&zeroY!=0) //чтобы понять, что тут происходит, загляни лучше в field::act, тут урезанная версия того же, что там
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
void field::act(int key)//передается код кнопки, которая была нажата
{
	if (key==SHAKE)//если перемешать, то мешаем и выходим
	{
		shake();
		return;
	}//иначе
	double frames=10,dx=0,dy=0;//frames-количество кадров в анимации, то есть за сколько move фишка доберется до новой позиции,dx,dy-сдвиг за один вызов move
	int i;//фишка, с которой происходит манипуляция. это нужно, чтобы клавиши обработать в одном месте, выбрав фишку и направление движения, а после уже двигать
	//чтобы в каждом ифе не вставлять по одному for-у
	if (key==DOWN&&zeroY!=0) //если кнопка движения вниз нажата и при этом пустая клетка не наверху (то есть можно совершить это движение)
	{
		dy=w/4/frames; //ширина фишки (т.е. поле/4) поделить на количество кадров и движем вниз (ось y ориентирована вниз, то есть движемся в ее положительном направлении)
		i=pos[zeroY-1][zeroX];//запоминаем, какая фишка лежит сверху от нуля
		pos[zeroY][zeroX]=pos[zeroY-1][zeroX];//перемещаем ее в таблице вниз
		pos[zeroY-1][zeroX]=0;//на ее место кладем ноль
		zeroY--;//и меняем сохраненную позицию нуля
	}
	else if (key==UP&&zeroY!=3)//другие аналогичные случаи. тут вверх и чтобы пустая клетка не снизу
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
	} else return;//если ничего из этого, то есть ничего сделать нельзя, выходим
	for(double j=0;j<frames;j++) //а теперь двигаем выбранную фишку по чуть-чуть в выбранную сторону столько раз, сколько кадров указано
	{
		chips[i]->move(dx,dy);
		delay(20);//небольшая задержечка для плавности
	}
}
void tips(double x,double y, double w)
{ 
	settextstyle(COMPLEX_FONT,0,1);
	//4 кнопки со стрелочками
	rectangle(x+w,y,x+2*w,y+w);
	rectangle(x,y+w,x+w,y+2*w);
	rectangle(x+w,y+w,x+2*w,y+2*w);
	rectangle(x+2*w,y+w,x+3*w,y+2*w);
	//левая стрелка
	line(x+w/4,y+w+w/2,x+w/2,y+w+w/4);
	line(x+w/4,y+w+w/2,x+w/2,y+w+w*3/4);
	//правая
	line(x+2*w+w*3/4,y+w+w/2,x+2*w+w/2,y+w+w/4);
	line(x+2*w+w*3/4,y+w+w/2,x+2*w+w/2,y+w+w*3/4);
	//верхняя
	line(x+w+w/4,y+w/2,x+w+w/2,y+w/4);
	line(x+w+w/2,y+w/4,x+w+w*3/4,y+w/2);
	//нижняя
	line(x+w+w/4,y+w+w/2,x+w+w/2,y+w+w*3/4);
	line(x+w+w/2,y+w+w*3/4,x+w+w*3/4,y+w+w/2);
	//подпись
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
	field F(310,90,300);//создаем поле
	
	//отсюда
	initwindow(640,480);//специально выбран размер, как есть в турбо, чтобы потом ничего не подгонять
	setfillstyle(SOLID_FILL,WHITE);
	bar(0,0,640,480);
	//и до сюда нужно удалить в турбо
	
	//а вот это раскомментировать для турбо
	/*int gdriver = DETECT, gmode;
 	initgraph(&gdriver,&gmode,"");*/
	setbkcolor(WHITE);
	setcolor(BLACK);//цвет абриса и текста
	tips(20,120,40);
	settextstyle(DEFAULT_FONT,0,2);
	F.show();
	while(true) 
	{
		getch();//если убрать его, то будем гонять бесконечный цикл, это грузит процессор, а так прога "просыпается" только когда нажали клавишу
		if (GetKeyState(UP)<0) F.act(UP);//получитьСостояниеКлавиши(вверх). если меньше 0 - значит нажата. 
			//да, странная штука, но я у себя ту же использовал, она из windows.h, кажется. ну и если нажата - запускаем движуху
		if (GetKeyState(DOWN)<0) F.act(DOWN); //аналогично
		if (GetKeyState(RIGHT)<0) F.act(RIGHT);
		if (GetKeyState(LEFT)<0) F.act(LEFT);
		if (GetKeyState(SHAKE)<0) F.act(SHAKE);
		if (GetKeyState(EXIT)<0) break;
	}
	
	return 0;
}

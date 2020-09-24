#include <stdio.h>
#include <windows.h>
#include <irrKlang.h>
#include <GL/freeglut.h>
using namespace irrklang;
//класс "струна"
class str 
{
 private: void show();
          void showLayer();
          double amp=0,dock,AMP=0.3;
          ISound*sound=NULL;
          int id,coef=1,fret=0,*prev;
          unsigned char*keys;
 public:  str(int,double,FILE*);
          ~str();
          void setFret(int);
          void play(ISoundEngine*);
          void stop();
};
//класс "гитара"
class gtr 
{
 private: ISoundEngine*engine;//звуковой движок
          void show();
 public:  static const int STRINGS=4, FRETS=11;//количество струн и количество ладов, включая нулевой
          gtr();
          ~gtr();
          void play();
 private: str*strings[STRINGS];
} G;
class m
{
 public: static void line(double,double,double,double);//рисовка линий
         static void bar(double,double,double,double); //рисовка прямоугольников
         static void ellipse(double,double,double,double);//рисовка эллипса
         static void renderScene();//функция для главного цикла glut
};
void m::line(double x1,double y1,double x2,double y2) 
{
 glBegin(GL_LINES);
  glVertex2d(-1+x1,1-y1);
  glVertex2d(-1+x2,1-y2);
 glEnd();
}
void m::bar(double l, double t, double r, double b)
{
 glBegin(GL_QUADS);
  glVertex2d(-1+l,1-t);
  glVertex2d(-1+l,1-b);
  glVertex2d(-1+r,1-b);
  glVertex2d(-1+r,1-t);
 glEnd();
}
void m::ellipse(double x, double y, double xr, double yr)
{
 glBegin(GL_POLYGON);
  for(double i=0;i<2*3.14;i+=0.1)
   glVertex2d(-1+x+xr*cos(i),1-y-yr*sin(i));
 glEnd();
}
str::str(int i,double d,FILE*f)
{
 id=i;
 dock=d;
 prev=new int[gtr::FRETS];
 keys=new unsigned char[gtr::FRETS];
 fread(keys,sizeof(char),gtr::FRETS,f);
}
str::~str()
{ 
 if(sound)sound->drop(); 
 delete prev;
 delete keys;
}
void str::play(ISoundEngine*engine) 
{   //анализ нажатий
 int playing=0;
 for(int i=0;i<gtr::FRETS;i++) 
 {
  int tmp=GetKeyState(keys[i])<0;
  if(tmp&&!prev[i])
   fret=i, playing=1;
  prev[i]=tmp;
 }
 if(playing) //если нажата новая клавиша
 {
  amp=AMP;//устанавливается большая амплитуда
  //если струна звучала до этого - выключить звук
  if(sound)sound->stop();
  //запустить новый звук
  char tmp[20];
  sprintf(tmp,"sounds/%d%X.wav",id,fret);
  sound=engine->play2D(tmp,0,0,1,ESM_NO_STREAMING,1);
 }
 show();//отобразить
}
void str::stop()
{
 amp=0; //убрать амплитуду колебания
 if(sound)sound->stop();//остановить звук
 show();//отобразить
}
void str::showLayer()
{
 double H=2, W=2;
 //кусочек струны до порожка
 m::line(W/6,H/4+H*(0.5+id)/gtr::STRINGS/2,0,H/4+H*dock/gtr::STRINGS/2);
 //на порожке
 m::line(W/6,H/4+H*(id+0.5)/gtr::STRINGS/2,W/5,H/4+H*(id+0.5)/gtr::STRINGS/2);
 //неподвижная часть струны
 m::line(W/5,
         H/4+H*(id+0.5)/gtr::STRINGS/2,
         W/5+W*4*fret/5/gtr::FRETS,
         H/4+H*(id+0.5)/gtr::STRINGS/2);
 //колеблющаяся часть струны
 m::line(W/5+W*4*fret/5/gtr::FRETS,
         H/4+H*(id+0.5)/gtr::STRINGS/2,
         2*W,
         H/4+H*(id+0.5+amp*coef)/gtr::STRINGS/2);
}
void str::show()
{
 double H=2, W=2;
 //высота экрана в пикселях
 double h=glutGet(GLUT_WINDOW_HEIGHT);
 if(amp>0.0001)amp-=0.00020;
 coef*=-1;
 //тень
 glLineWidth((id+1)*h*0.01);
 glColor4d(0,0,0,0.2);
 showLayer();
 glLineWidth((id+1)*h*0.005);
 glColor4d(0,0,0,0.3);
 showLayer();
 //кружочек нажатия
 glColor4d(1.0,0.94,0,0.3);
 m::ellipse(W-(W*4/5)*(gtr::FRETS-fret+0.5)/gtr::FRETS,
            H/4+H*(0.5+id)/gtr::STRINGS/2,
            W/40*amp/AMP,
			H/20*amp/AMP);
 //струна
 glLineWidth((id+1)*h*0.003);
 glColor3d(0.5,0.5,0.5);
 showLayer();
 //рифление струны
 glColor3d(0.9,0.9,0.9);
 glLineStipple(1,0x5555);
 showLayer();
 glLineStipple(1,0xFFFF);
}
gtr::gtr() 
{
 engine=createIrrKlangDevice();
 FILE*f=fopen("key.bin","rb");
 strings[0]=new str(0,0.5,f);
 strings[1]=new str(1,1.2,f);
 strings[2]=new str(2,2.8,f);
 strings[3]=new str(3,3.5,f);
 fclose(f);
}
gtr::~gtr()
{ 
  for(int i=0;i<STRINGS;i++) delete strings[i]; 
  engine->drop();
}
void gtr::play()
{
 show();
 if(GetKeyState(VK_RETURN)<0)
  for(int i=0;i<STRINGS;i++)strings[i]->stop();
 else 
  for(int i=0;i<STRINGS;i++)strings[i]->play(engine);
 Sleep(4);//иначе тормозит
}
void gtr::show()
{
 double H=2, W=2;
 //ширина экрана в пикселях
 double w=glutGet(GLUT_WINDOW_WIDTH);
 //голова грифа
 glColor3d(0.10,0.10,0.10);//темно-серый
 m::bar(0,0,W/6,H*9/10);
 glColor3d(0.25,0.78,1);//голубой
 m::ellipse(W/6,0,H/2.2,H/4);
 m::ellipse(W/6,H,H/2.2,H/4);
 //порожек и обрамление грифа
 glColor3d(1,0.98,0.75);//бежевый
 m::bar(W/6,H/4,W,H*3/4);
 //ямки на порожке
 for(int i=0;i<STRINGS;i++)
 {
  glColor4d(0,0,0,0.3);
  m::bar(W/6,
         H/4+H*(i+0.5)/STRINGS/2-H*0.01/4-(i+1)*H*0.003/2,
         W/5,
         H/4+H*(i+0.5)/STRINGS/2+H*0.01/4+(i+1)*H*0.003/2);
  glColor4d(0,0,0,0.1);
  m::bar(W/6,
         H/4+H*(i+0.5)/STRINGS/2-H*0.02/4-(i+1)*H*0.003/2,
         W/5,
         H/4+H*(i+0.5)/STRINGS/2+H*0.02/4+(i+1)*H*0.003/2);
 }
 //накладка грифа
 glColor3d(0.26,0.13,0.05);//коричневый
 m::bar(W/5,H*1.05/4,W,H*2.95/4);
 //тень от порожка
 glColor4d(0,0,0,0.5);
 m::bar(W/5,H/4,W/4.95,H*3/4);
 glColor4d(0,0,0,0.3);
 m::bar(W/4.95,H/4,W/4.90,H*3/4);
 glColor4d(0,0,0,0.1);
 m::bar(W/4.90,H/4,W/4.85,H*3/4);
 //лады
 glLineWidth(w/300); 
 glColor3d(0.7,0.7,0.7);//серый
 for(int i=1;i<=FRETS;i++)
  m::line(W/5+W*4*i/5/FRETS-W/650,
          H*1.0/4,
          W/5+W*4*i/5/FRETS-W/650,
          H*3/4);
 glColor3d(0.6,0.6,0.6);//серый потемнее
 for(int i=1;i<=FRETS;i++)
  m::line(W/5+W*4*i/5/FRETS+W/650,
          H*1.0/4,
          W/5+W*4*i/5/FRETS+W/650,
          H*3/4);
 //игра света
 glColor4d(1,1,1,0.9);//посветлее сверху
 m::bar(W/6,H/4,W,H*1.01/4);
 glColor4d(1,1,1,0.5);
 m::bar(W/6,H*1.01/4,W,H*1.05/4);
 glColor4d(0,0,0,0.3);//потемнее снизу
 m::bar(W/6,H*3/4,W,H*2.98/4);
 glColor4d(0,0,0,0.1);
 m::bar(W/6,H*2.98/4,W,H*2.95/4);
 //инкрустация
 glColor3d(0.8,0.8,0.8);
 for(double i=2.5;i<=8.5;i+=2)
  m::ellipse(W/5+W*i*4/5/FRETS,H/2,W/70,H/35);
}
void m::renderScene()
{
 glClearColor(0.25,0.78,1,1);
 glClear(GL_COLOR_BUFFER_BIT);
 G.play();
 glutSwapBuffers();
 glutPostRedisplay();
}
int main(int argc,char**argv)
{
 glutInit(&argc,argv);
 glutInitWindowPosition(0,0);
 glutInitWindowSize(1000,500);
 glutInitDisplayMode(GLUT_RGBA|GLUT_SINGLE);
 glutCreateWindow("GUI-tar"); 
 glEnable(GL_LINE_STIPPLE);
 glEnable(GL_BLEND);
 glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
 glutDisplayFunc(m::renderScene);
 glutMainLoop();
}

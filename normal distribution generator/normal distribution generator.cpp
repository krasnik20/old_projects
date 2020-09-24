
//ТУТ НЕ ВСЕ КОММЕНТАРИИ
#include "iostream" 
#include "math.h"
#include <time.h>
#include <GL/glut.h>
#include <GL/freeglut.h>
using namespace std;

long N, M, MAX;
double * array;
const int columns = 10; //количество столбиков в гистограмме, можно сразу тут менять
long for_histo [columns] = {0}; 
void graph_to_file(int x1, int y1, int x2, int y2, char name [256]);
//Подпрограмма, показывающая текст на экране (для расположения координат и названий осей графиков) 
void output_text(double x, double y, char str[256]) {
  glColor3f(0.0, 0.0, 0.0); //черный цвет
  glRasterPos2f(x, y);//поставить позицию растрового вывода на нужную позицию
  for (int i = 0; str[i] != '\0'; i++)
    glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, str[i]);  // нарисовать каждую букву
}
double uniform_random() {// Возвращает псевдослучайное равномернораспределенное число из диапазона [-1,1]
  return (double)rand() / RAND_MAX * (rand() % 2? 1: -1); //случайное число делается по модулю не больше 1 и умножается на случайный знак
}
double normal() {// Возвращает значение согласно формуле
  double sum = 0,curr;
  for (int i = 0; i < M; i++) {
    sum += uniform_random ();
  }
  return sum;
}
void render_scene() {//рисовка графика
  int Max = N > 200? N:200;
  char str[256];
  double size = 0.1;//пропорция: тут x и y могут меняться от -1 до 1 (координаты не в пикселях, вот так), так получается 1/20 экрана по ширине и длине
  //определяем масштаб
  int incN = 1, incM = 1, i, j;
  while (Max/ incN > 10) incN *= 10;//узнаем порядок чисел N и M
  while (M / incM > 10) incM *= 10;
  int segmentsX = Max/incN + (Max%incN != 0); //узнаем количество единиц (округленных в большую сторону) в том разряде, который выяснен на пред. шаге, то есть количество больших отрезков
  //"большие" - которые подписываются, маленькие - нет
  int segmentsY = M/incM + (M%incM != 0); //это по модулю, а на деле в 2 раза больше: в худшем случае у нас случайная величина будет -M или M, поэтому с расчетом на это
  double incX = 1.8/segmentsX;//сколько один отрезок с таким масштабом. 1,8 потому что у нас поля на экране в один size есть со всех сторон, то есть 2-0,1*2=1,8
  double incY = 1.8/segmentsY/2;//2 потому что я о чем раньше говорил
  double zeroX = -9*size, zeroY = 0; //позиция точки графика (0;0)
  double rightX = -zeroX, rightY = zeroY; //это как бы крайняя точка оси Х (я так обозначил, потому что она справа
  double leftX = zeroX, leftY = 9*size; //это у оси Y
  glClearColor(1,1,1,1);//белый фон
  glClear(GL_COLOR_BUFFER_BIT);//туда же
  glLineWidth(1);  //ширина линии
  //график, а координаты после графика, чтобы они нарисовались поверх и их было видно
  glColor3d(0.5,0.5,1);//светло-синий цвет
  glBegin (GL_LINES); //начинаем рисовать линии
  //glPointSize(2);//если с точками, то эти 2 строки расомментировать, а предыдущую убрать
  //glBegin(GL_POINTS); 
    double incXsmall = incX/incN;//считаем, на сколько изменяется позиция на экране для каждой следующей точки
    double incYsmall = incY/incM;//а тут, сколько...ну, скажем, расстояния приходится на изменение значения на 1 
    for (i = 0; i < N-1; i++) {//ну и сами линии рисуем
      glVertex2f (zeroX+i*incXsmall, zeroY+array[i]*incYsmall);// между вот этой точкой и той, что в следующей строке
      glVertex2f (zeroX+(i+1)*incXsmall, zeroY+array[i+1]*incYsmall);//если с точками, это закомментировать
    }
  glEnd();//конец рисования линий. это типа операторных скобок {}, только для рисования
  //теперь рисуем координаты 
  glColor3d(0,0,0);//черный цвет
  glLineWidth(2); //и потолще  
  glBegin (GL_LINES); //рисуются линии
    //ось X
    glVertex2f (zeroX,zeroY); // от этой точки
    glVertex2f (rightX, rightY); //к этой
    //стрелочка
    glVertex2f (rightX, rightY); // от этой
    glVertex2f (rightX-size/8, rightY-size/8); //к этой
    //вторая ее часть
    glVertex2f (rightX, rightY); //и т.д.
    glVertex2f (rightX-size/8, rightY+size/8);
    //черточки
    for(i = 0; i < segmentsX; i++) {
      glVertex2f (zeroX + incX*i,zeroY+size/4);
      glVertex2f (zeroX + incX*i,zeroY-size/4);
      for (j = 0; j < 5; j++) {//внутри отрезка черточки поменьше (их мы не будем подписывать)
        glVertex2f (zeroX + incX*(i+j/5.0),zeroY+size/8);
        glVertex2f (zeroX + incX*(i+j/5.0),zeroY-size/8);
      }
    }
    //ось Y 
    glVertex2f (leftX,-leftY);
    glVertex2f (leftX, leftY);
    
    glVertex2f (leftX, leftY);
    glVertex2f (leftX-size/16, leftY-size/4);
    
    glVertex2f (leftX, leftY);
    glVertex2f (leftX+size/16, leftY-size/4);
    
    for(i = -segmentsY; i < segmentsY; i++) {
      glVertex2f (leftX + size/8,zeroY+incY*i);
      glVertex2f (leftX - size/8,zeroY+incY*i);
      for (j = 0; j < 5; j++) {
        glVertex2f (leftX + size/16,zeroY+incY*(i+j/5.0));
        glVertex2f (leftX - size/16,zeroY+incY*(i+j/5.0));
      }
    }  
  glEnd();
  //циферки 
  for(i = 0, j = 0; j<=segmentsX; i+= incN, j++) { //втыкаем текст
    sprintf(str, "%d", i);//записываем число в строку
    output_text(zeroX + incX*j-0.01, zeroY-size/3-0.02, str);//и выводим. 0.01 это чтобы не заезжало на черточки
  }
  output_text(zeroX + incX*(j-1)+0.01, zeroY+size/3, "k");//подпись для оси
  for(i = -M, j = -segmentsY; j <= M; i+= incM, j++) {//это у игреков
    sprintf(str, "%5d", i);
    output_text(leftX - size, zeroY+incY*j, str);
  }
  cout << j;
  output_text(leftX, leftY, "X(k)");
  graph_to_file(0,0,glutGet(GLUT_SCREEN_WIDTH),glutGet(GLUT_SCREEN_HEIGHT)/2, "graph.bmp");//скриншот
  glutSwapBuffers();
}
void render_histo() {//гистограмма
  char str[256];
  double size = 0.1;
  int incN = 1, j;
  double i;
  //определяем масштаб аналогично графику
  while (MAX / incN > 10) incN *= 10;
  int segmentsY = MAX/incN + (MAX%incN != 0);
  double incX = 1.8/(columns+2);//+2 это по одному пустому столбику по бокам, чтобы прямоугольники на стрелки не наезжали
  double incY = 1.8/segmentsY;
  double zeroX = -9*size, zeroY = -9*size;//примерно то же самое
  double rightX = -zeroX, rightY = zeroY;
  double leftX = 0, leftY = 9*size;
  glClearColor(255.0, 255.0, 255.0, 1.0);//белый фон
  glClear(GL_COLOR_BUFFER_BIT);
  glLineWidth(1);  
  //график
  glColor3d(0.5,0.5,1);//синий цвет
  for(j = 1; j < columns+2; j++) {
    glBegin (GL_QUADS); //квадрат между этими точками:
      glVertex2f (zeroX + incX*j,rightY);
      glVertex2f (zeroX + incX*(j+1),rightY);
      glVertex2f (zeroX + incX*(j+1),rightY+for_histo[j]*1.8/MAX);
      glVertex2f (zeroX + incX*j,rightY+for_histo[j]*1.8/MAX);
    glEnd();
  }

  //координаты после графика, чтобы они нарисовались поверх и их было видно
  glColor3d(0,0,0);//черный цвет
  glLineWidth(2); //и потолще  
  glBegin (GL_LINES); //рисуются линии
    //ось X
    glVertex2f (zeroX,zeroY); // от этой точки
    glVertex2f (rightX, rightY); //к этой
    //стрелочка
    glVertex2f (rightX, rightY); // от этой
    glVertex2f (rightX-size/8, rightY-size/8); //к этой
    //вторая ее часть
    glVertex2f (rightX, rightY); //и т.д.
    glVertex2f (rightX-size/8, rightY+size/8);
    //черточки
    for(i = 1; i < columns+2; i++) {
      glVertex2f (zeroX + incX*i,zeroY+size/8);
      glVertex2f (zeroX + incX*i,zeroY-size/8);
    }
    //ось Y 
    glVertex2f (leftX,-leftY);
    glVertex2f (leftX, leftY);
    
    glVertex2f (leftX, leftY);
    glVertex2f (leftX-size/16, leftY-size/4);
    
    glVertex2f (leftX, leftY);
    glVertex2f (leftX+size/16, leftY-size/4);
    
    for(i = 0; i < segmentsY; i++) {
      glVertex2f (leftX + size/8,zeroY+incY*i);
      glVertex2f (leftX - size/8,zeroY+incY*i);
    for (j = 0; j < 5; j++) {
        glVertex2f (leftX + size/16,zeroY+incY*(i+j/5.0));
        glVertex2f (leftX - size/16,zeroY+incY*(i+j/5.0));
    }
    }  
  glEnd();
  for(i = -M, j = 1; j <= columns+1; i+= M*2.0/columns, j++) {
    sprintf(str, "%.1f", i);
    output_text(zeroX + incX*j-0.01, zeroY-size/3-0.02, str);
  }
  output_text(zeroX + incX*j+0.01, zeroY+size/3, "X");
  for(i = incN, j = 1; j <= segmentsY; i+= incN, j++) {
    sprintf(str, "%3.0f", i);
    output_text(leftX - size/2, zeroY+incY*j, str);
  }
  output_text(leftX, zeroY+incY*(j-1), "Q");
  graph_to_file(0,0,glutGet(GLUT_SCREEN_WIDTH),glutGet(GLUT_SCREEN_HEIGHT)/2-100, "histo.bmp");//не знаю, почему именно такие параметры)))) наугад подобрал, иначе черные поля были
  glutSwapBuffers();
}
void graph_to_file(int x1, int y1, int x2, int y2, char name [256]){
//тут создается структурка файла бмпшного, заполняется,записывается в файл,удаляется, но без подробностей, взял у Коли целиком
  const int imSize = (x2-x1) * (y2-y1)*3;
  unsigned char* image = new unsigned char[imSize];
  glReadPixels(x1, y1, x2, y2, GL_RGB, GL_UNSIGNED_BYTE, image);
  BITMAPINFO bmi;
  bmi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
  bmi.bmiHeader.biHeight = y2-y1;
  bmi.bmiHeader.biWidth = x2-x1;
  bmi.bmiHeader.biPlanes = 1;
  bmi.bmiHeader.biBitCount = 24;
  bmi.bmiHeader.biCompression = BI_RGB;
  bmi.bmiHeader.biSizeImage = imSize;
  FILE* F = fopen(name, "w");
  int nBitsOffset = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);
  LONG lImageSize = imSize;
  LONG lFileSize = nBitsOffset + lImageSize;
  BITMAPFILEHEADER bmfh;
  bmfh.bfType = 'B' + ('M' << 8);
  bmfh.bfOffBits = nBitsOffset;
  bmfh.bfSize = lFileSize;
  bmfh.bfReserved1 = bmfh.bfReserved2 = 0;
  fwrite(&bmfh, 1, sizeof(BITMAPFILEHEADER), F);
  fwrite(&bmi, 1, sizeof(BITMAPINFOHEADER), F);
  fwrite(image, 1, imSize, F);
  delete[]image;
  fclose(F);
}

int main(int argc, char** argv){
  clock_t start, stop;  
  setlocale(LC_ALL, "RUS");
  srand(time(NULL));
  cout << "Введите N (нужное количество случайных чисел): ";
  cin >> N;
  cout << "Введите M (количество суммируемых чисел для каждой случайной величины): ";
  cin >> M;
  
  array = new double [N];//выделяем память под n элементов
  double interval = 2.0*M/columns; //интервал на один столбик в гистограмме
  for (int i = 0; i < N; i++) {
    array[i] = normal();
    for_histo[(int)floor(columns/2+1+array[i]/interval)]++;//к тому столбику, в который попадает полученное значение, плюсуем
  }
  for (int i = 0; i < columns; i++) 
    if (for_histo[i] > MAX) MAX = for_histo[i];
  if (MAX<200) MAX = 200;
  glutInit(&argc, argv);
  glutInitWindowPosition(0,0); //parametry okna
  glutInitWindowSize(glutGet(GLUT_SCREEN_WIDTH),glutGet(GLUT_SCREEN_HEIGHT)/2);
  glutInitDisplayMode(GLUT_RGBA|GLUT_SINGLE);
  glutCreateWindow("graph"); //sozdat okno
  glutDisplayFunc(render_scene); //risovka na ekrane
  glutInitWindowPosition(0,glutGet(GLUT_SCREEN_HEIGHT)/2+25); //parametry okna
  glutInitWindowSize(glutGet(GLUT_SCREEN_WIDTH),glutGet(GLUT_SCREEN_HEIGHT)/2-100);
  glutInitDisplayMode(GLUT_RGBA|GLUT_SINGLE);
  glutCreateWindow("histogram"); //sozdat okno
  glutDisplayFunc(render_histo); //risovka na ekrane
  glutMainLoop();
}



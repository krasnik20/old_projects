#include <stdio.h>
#include <conio.h>
#include <windows.h>
#include <GL/freeglut.h>//выбрал глют, поскольку в графикс не получается нормальный образ экрана прочитать и уж тем более записать потом в файл
double f(double x, double y){ return x-y; }//правая часть уравнения
void text(double x, double y, char str[256]){//функция вывода текста
  glRasterPos2f(x, y);//поставить позицию растрового вывода на нужную позицию
  for (int i=0; str[i]!='\0';i++)// нарисовать каждую букву по отдельности
    glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, str[i]);  
}
void output(){//вывод в файл
  int imSize = glutGet(GLUT_WINDOW_WIDTH)*glutGet(GLUT_WINDOW_HEIGHT)*3;//размер картинки=ширина окна*высота окна*3 составляющих цвета
  unsigned char* image = new unsigned char[imSize];//выделение памяти
  glReadPixels(0,0, glutGet(GLUT_WINDOW_WIDTH), glutGet(GLUT_WINDOW_HEIGHT), GL_RGB, GL_UNSIGNED_BYTE, image);//чтение образа с экрана
  BITMAPINFO bmi;//структура для файла bmp, далее записывается техническая информация
  bmi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
  bmi.bmiHeader.biHeight = glutGet(GLUT_WINDOW_HEIGHT);
  bmi.bmiHeader.biWidth = glutGet(GLUT_WINDOW_WIDTH);
  bmi.bmiHeader.biPlanes = 1;
  bmi.bmiHeader.biBitCount = 24;
  bmi.bmiHeader.biCompression = BI_RGB;
  bmi.bmiHeader.biSizeImage = imSize;
  FILE* F = fopen("output.bmp", "w");//открывается файл для записи
  int nBitsOffset = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);//считается сдвиг от начала файла до непосредственно изображения
  //еще одна структура и еще один набор технической информации
  BITMAPFILEHEADER bmfh;
  bmfh.bfType = 'B' + ('M' << 8);
  bmfh.bfOffBits = nBitsOffset;
  bmfh.bfSize = nBitsOffset + imSize;
  bmfh.bfReserved1 = bmfh.bfReserved2 = 0;
  fwrite(&bmfh, 1, sizeof(BITMAPFILEHEADER), F);//записываются полученные структуры
  fwrite(&bmi, 1, sizeof(BITMAPINFOHEADER), F);
  fwrite(image, 1, imSize, F);//и само изображение
  delete[]image;//очищается память
  fclose(F);//закрывается файл
}
void sample(){}//функция-пустышка, нужна далее как функция цикла программы:
//глут устроен таким образом, что он должен крутить свой особенный цикл по кругу все время работы, и в этом цикле вызывать какую-то функцию
//поскольку анимации тут нет, то тут достаточно нарисовать все один раз, поэтому в цикле будет крутиться пустышка
void line(double x1,double y1,double x2,double y2){//запихнул это в функцию, потому что вместо лаконичного line постоянно писать вот эту фигню тяжко и нечитаемо
	glVertex2f(x1,y1);//линия = соединить вот эти две вершины
	glVertex2f(x2,y2);
}
int main (int argc, char** argv){
	glutInit(&argc, argv);//тут инициализация глюта
	glutInitWindowPosition(0,0);
	glutInitWindowSize(500,500);
	glutInitDisplayMode(GLUT_RGBA|GLUT_SINGLE);
	glutCreateWindow("Differential equation"); 
	glClearColor(1,1,1,1);//белый фон (то есть RGBA, одно число с плавающей точкой от 0 до 1 для каждого цвета)
	glClear(GL_COLOR_BUFFER_BIT);//и ззаливка этим фоном
	glColor3d(0,0,0);//черный цвет(тут без A)
	//пару слов об экранных координатах: они относительные (т.е. не в пикселях, а в долях окна) и меняются от -1 до 1.
	//например, точка (0,0) - центр, (1,1) - правый верхний угол
	glBegin (GL_LINES); //переходим в режим рисования линий, т.е. дальше программа будет каждые две вершины, то есть glVertex2f, объединять линией
		line(-0.8,0.9,-0.8,-0.9);//ось y
		line(-0.8,-0.9,0.9,-0.9);//ось x
		line(-0.8,0.9,-0.77,0.87);//стрелочка оси y
		line(-0.8,0.9,-0.83,0.87);
		line(0.9,-0.9,0.87,-0.87);//стрелочка оси х
		line(0.9,-0.9,0.87,-0.93);
		double a=0,b=0.5,h=0.05;//исследуемый отрезок, шаг. можно без зазрения совести поменять на любые другие, но только мб текст начнет наезжать друг на друга
		int n=(b-a)/h;//количество отрезков разбиения
		double x[n+1],y[n+1];//точки
		x[0]=0;//начальное условие
		y[0]=0;
		for (int i=1;i<=n;i++){
			x[i]=x[i-1]+h;//увеличиваем икс
			y[i]=y[i-1]+(x[i]-x[i-1])*f(x[i-1],y[i-1]);//считаем y по эйлеру
			line(-0.8+x[i-1]/(b-a)*1.7,//рисуем линию от прошлой точки до новой точки
			     -0.9+y[i-1]*15,//поскольку икс изменяется в определенных рамках, мы его делим на весь исследуемый отрезок 
				 -0.8+x[i]/(b-a)*1.7,//(ищем часть, которую он отхватывает) и умножаем на длину исследуемого отрезка в координатах экрана, а это,судя по 65 строке, 0.8+0.9=1.7
				 -0.9+y[i]*15);//а игрек просто умножаем на какой-то коэффициент
			line(-0.8+x[i-1]/(b-a)*1.7,//ставим черточку на оси x
			     -0.92,
				 -0.8+x[i-1]/(b-a)*1.7,
				 -0.88);
			line(-0.82,//и на оси y
			     -0.9+y[i-1]*15,
				 -0.78,
				 -0.9+y[i-1]*15);
		}
		line(-0.82,//доставляем недостающую черточку на оси y, на оси x ее роль выполнит стрелочка
		     -0.9+y[n]*15,
			 -0.78,
			 -0.9+y[n]*15);
    glEnd();//завершаем режим рисования линий, чтобы дальше рисовать текст
    text(-0.85,0.92,"y");//подписи координат
    text(0.92,-0.9,"x");
	char tmp[256];//для вывода 
    for (int i=0;i<n;i++){
    	sprintf(tmp,"%5.2f",x[i]);//переводим число в строку
    	text(-0.85+x[i]/(b-a)*1.7,-0.98,tmp);//и выводим
    	sprintf(tmp,"%5.4f",y[i]);
    	text(-0.98,y[i]*15-0.92,tmp);
	}
	sprintf(tmp,"%5.4f",y[n]);//недостающее число на оси y (на x оно под стрелочкой, поэтому не выводим
	text(-0.98,y[n]*15-0.92,tmp);
    glutSwapBuffers();//просим глют показать, что мы нарисовали
    output();//выводим это в файл
	glutDisplayFunc(sample); //говорим, что основная функция цикла глюта - пустышка
    glutMainLoop();//и запускаем цикл
}

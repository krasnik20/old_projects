
//��� �� ��� �����������
#include "iostream" 
#include "math.h"
#include <time.h>
#include <GL/glut.h>
#include <GL/freeglut.h>
using namespace std;

long N, M, MAX;
double * array;
const int columns = 10; //���������� ��������� � �����������, ����� ����� ��� ������
long for_histo [columns] = {0}; 
void graph_to_file(int x1, int y1, int x2, int y2, char name [256]);
//������������, ������������ ����� �� ������ (��� ������������ ��������� � �������� ���� ��������) 
void output_text(double x, double y, char str[256]) {
  glColor3f(0.0, 0.0, 0.0); //������ ����
  glRasterPos2f(x, y);//��������� ������� ���������� ������ �� ������ �������
  for (int i = 0; str[i] != '\0'; i++)
    glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, str[i]);  // ���������� ������ �����
}
double uniform_random() {// ���������� ��������������� ������������������������ ����� �� ��������� [-1,1]
  return (double)rand() / RAND_MAX * (rand() % 2? 1: -1); //��������� ����� �������� �� ������ �� ������ 1 � ���������� �� ��������� ����
}
double normal() {// ���������� �������� �������� �������
  double sum = 0,curr;
  for (int i = 0; i < M; i++) {
    sum += uniform_random ();
  }
  return sum;
}
void render_scene() {//������� �������
  int Max = N > 200? N:200;
  char str[256];
  double size = 0.1;//���������: ��� x � y ����� �������� �� -1 �� 1 (���������� �� � ��������, ��� ���), ��� ���������� 1/20 ������ �� ������ � �����
  //���������� �������
  int incN = 1, incM = 1, i, j;
  while (Max/ incN > 10) incN *= 10;//������ ������� ����� N � M
  while (M / incM > 10) incM *= 10;
  int segmentsX = Max/incN + (Max%incN != 0); //������ ���������� ������ (����������� � ������� �������) � ��� �������, ������� ������� �� ����. ����, �� ���� ���������� ������� ��������
  //"�������" - ������� �������������, ��������� - ���
  int segmentsY = M/incM + (M%incM != 0); //��� �� ������, � �� ���� � 2 ���� ������: � ������ ������ � ��� ��������� �������� ����� -M ��� M, ������� � �������� �� ���
  double incX = 1.8/segmentsX;//������� ���� ������� � ����� ���������. 1,8 ������ ��� � ��� ���� �� ������ � ���� size ���� �� ���� ������, �� ���� 2-0,1*2=1,8
  double incY = 1.8/segmentsY/2;//2 ������ ��� � � ��� ������ �������
  double zeroX = -9*size, zeroY = 0; //������� ����� ������� (0;0)
  double rightX = -zeroX, rightY = zeroY; //��� ��� �� ������� ����� ��� � (� ��� ���������, ������ ��� ��� ������
  double leftX = zeroX, leftY = 9*size; //��� � ��� Y
  glClearColor(1,1,1,1);//����� ���
  glClear(GL_COLOR_BUFFER_BIT);//���� ��
  glLineWidth(1);  //������ �����
  //������, � ���������� ����� �������, ����� ��� ������������ ������ � �� ���� �����
  glColor3d(0.5,0.5,1);//������-����� ����
  glBegin (GL_LINES); //�������� �������� �����
  //glPointSize(2);//���� � �������, �� ��� 2 ������ ����������������, � ���������� ������
  //glBegin(GL_POINTS); 
    double incXsmall = incX/incN;//�������, �� ������� ���������� ������� �� ������ ��� ������ ��������� �����
    double incYsmall = incY/incM;//� ���, �������...��, ������, ���������� ���������� �� ��������� �������� �� 1 
    for (i = 0; i < N-1; i++) {//�� � ���� ����� ������
      glVertex2f (zeroX+i*incXsmall, zeroY+array[i]*incYsmall);// ����� ��� ���� ������ � ���, ��� � ��������� ������
      glVertex2f (zeroX+(i+1)*incXsmall, zeroY+array[i+1]*incYsmall);//���� � �������, ��� ����������������
    }
  glEnd();//����� ��������� �����. ��� ���� ����������� ������ {}, ������ ��� ���������
  //������ ������ ���������� 
  glColor3d(0,0,0);//������ ����
  glLineWidth(2); //� �������  
  glBegin (GL_LINES); //�������� �����
    //��� X
    glVertex2f (zeroX,zeroY); // �� ���� �����
    glVertex2f (rightX, rightY); //� ����
    //���������
    glVertex2f (rightX, rightY); // �� ����
    glVertex2f (rightX-size/8, rightY-size/8); //� ����
    //������ �� �����
    glVertex2f (rightX, rightY); //� �.�.
    glVertex2f (rightX-size/8, rightY+size/8);
    //��������
    for(i = 0; i < segmentsX; i++) {
      glVertex2f (zeroX + incX*i,zeroY+size/4);
      glVertex2f (zeroX + incX*i,zeroY-size/4);
      for (j = 0; j < 5; j++) {//������ ������� �������� �������� (�� �� �� ����� �����������)
        glVertex2f (zeroX + incX*(i+j/5.0),zeroY+size/8);
        glVertex2f (zeroX + incX*(i+j/5.0),zeroY-size/8);
      }
    }
    //��� Y 
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
  //������� 
  for(i = 0, j = 0; j<=segmentsX; i+= incN, j++) { //������� �����
    sprintf(str, "%d", i);//���������� ����� � ������
    output_text(zeroX + incX*j-0.01, zeroY-size/3-0.02, str);//� �������. 0.01 ��� ����� �� �������� �� ��������
  }
  output_text(zeroX + incX*(j-1)+0.01, zeroY+size/3, "k");//������� ��� ���
  for(i = -M, j = -segmentsY; j <= M; i+= incM, j++) {//��� � �������
    sprintf(str, "%5d", i);
    output_text(leftX - size, zeroY+incY*j, str);
  }
  cout << j;
  output_text(leftX, leftY, "X(k)");
  graph_to_file(0,0,glutGet(GLUT_SCREEN_WIDTH),glutGet(GLUT_SCREEN_HEIGHT)/2, "graph.bmp");//��������
  glutSwapBuffers();
}
void render_histo() {//�����������
  char str[256];
  double size = 0.1;
  int incN = 1, j;
  double i;
  //���������� ������� ���������� �������
  while (MAX / incN > 10) incN *= 10;
  int segmentsY = MAX/incN + (MAX%incN != 0);
  double incX = 1.8/(columns+2);//+2 ��� �� ������ ������� �������� �� �����, ����� �������������� �� ������� �� ��������
  double incY = 1.8/segmentsY;
  double zeroX = -9*size, zeroY = -9*size;//�������� �� �� �����
  double rightX = -zeroX, rightY = zeroY;
  double leftX = 0, leftY = 9*size;
  glClearColor(255.0, 255.0, 255.0, 1.0);//����� ���
  glClear(GL_COLOR_BUFFER_BIT);
  glLineWidth(1);  
  //������
  glColor3d(0.5,0.5,1);//����� ����
  for(j = 1; j < columns+2; j++) {
    glBegin (GL_QUADS); //������� ����� ����� �������:
      glVertex2f (zeroX + incX*j,rightY);
      glVertex2f (zeroX + incX*(j+1),rightY);
      glVertex2f (zeroX + incX*(j+1),rightY+for_histo[j]*1.8/MAX);
      glVertex2f (zeroX + incX*j,rightY+for_histo[j]*1.8/MAX);
    glEnd();
  }

  //���������� ����� �������, ����� ��� ������������ ������ � �� ���� �����
  glColor3d(0,0,0);//������ ����
  glLineWidth(2); //� �������  
  glBegin (GL_LINES); //�������� �����
    //��� X
    glVertex2f (zeroX,zeroY); // �� ���� �����
    glVertex2f (rightX, rightY); //� ����
    //���������
    glVertex2f (rightX, rightY); // �� ����
    glVertex2f (rightX-size/8, rightY-size/8); //� ����
    //������ �� �����
    glVertex2f (rightX, rightY); //� �.�.
    glVertex2f (rightX-size/8, rightY+size/8);
    //��������
    for(i = 1; i < columns+2; i++) {
      glVertex2f (zeroX + incX*i,zeroY+size/8);
      glVertex2f (zeroX + incX*i,zeroY-size/8);
    }
    //��� Y 
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
  graph_to_file(0,0,glutGet(GLUT_SCREEN_WIDTH),glutGet(GLUT_SCREEN_HEIGHT)/2-100, "histo.bmp");//�� ����, ������ ������ ����� ���������)))) ������ ��������, ����� ������ ���� ����
  glutSwapBuffers();
}
void graph_to_file(int x1, int y1, int x2, int y2, char name [256]){
//��� ��������� ���������� ����� ��������, �����������,������������ � ����,���������, �� ��� ������������, ���� � ���� �������
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
  cout << "������� N (������ ���������� ��������� �����): ";
  cin >> N;
  cout << "������� M (���������� ����������� ����� ��� ������ ��������� ��������): ";
  cin >> M;
  
  array = new double [N];//�������� ������ ��� n ���������
  double interval = 2.0*M/columns; //�������� �� ���� ������� � �����������
  for (int i = 0; i < N; i++) {
    array[i] = normal();
    for_histo[(int)floor(columns/2+1+array[i]/interval)]++;//� ���� ��������, � ������� �������� ���������� ��������, �������
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



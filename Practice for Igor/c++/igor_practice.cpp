#include <stdio.h>
#include <conio.h>
#include <windows.h>
#include <GL/freeglut.h>//������ ����, ��������� � ������� �� ���������� ���������� ����� ������ ��������� � �� ��� ����� �������� ����� � ����
double f(double x, double y){ return x-y; }//������ ����� ���������
void text(double x, double y, char str[256]){//������� ������ ������
  glRasterPos2f(x, y);//��������� ������� ���������� ������ �� ������ �������
  for (int i=0; str[i]!='\0';i++)// ���������� ������ ����� �� �����������
    glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, str[i]);  
}
void output(){//����� � ����
  int imSize = glutGet(GLUT_WINDOW_WIDTH)*glutGet(GLUT_WINDOW_HEIGHT)*3;//������ ��������=������ ����*������ ����*3 ������������ �����
  unsigned char* image = new unsigned char[imSize];//��������� ������
  glReadPixels(0,0, glutGet(GLUT_WINDOW_WIDTH), glutGet(GLUT_WINDOW_HEIGHT), GL_RGB, GL_UNSIGNED_BYTE, image);//������ ������ � ������
  BITMAPINFO bmi;//��������� ��� ����� bmp, ����� ������������ ����������� ����������
  bmi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
  bmi.bmiHeader.biHeight = glutGet(GLUT_WINDOW_HEIGHT);
  bmi.bmiHeader.biWidth = glutGet(GLUT_WINDOW_WIDTH);
  bmi.bmiHeader.biPlanes = 1;
  bmi.bmiHeader.biBitCount = 24;
  bmi.bmiHeader.biCompression = BI_RGB;
  bmi.bmiHeader.biSizeImage = imSize;
  FILE* F = fopen("output.bmp", "w");//����������� ���� ��� ������
  int nBitsOffset = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);//��������� ����� �� ������ ����� �� ��������������� �����������
  //��� ���� ��������� � ��� ���� ����� ����������� ����������
  BITMAPFILEHEADER bmfh;
  bmfh.bfType = 'B' + ('M' << 8);
  bmfh.bfOffBits = nBitsOffset;
  bmfh.bfSize = nBitsOffset + imSize;
  bmfh.bfReserved1 = bmfh.bfReserved2 = 0;
  fwrite(&bmfh, 1, sizeof(BITMAPFILEHEADER), F);//������������ ���������� ���������
  fwrite(&bmi, 1, sizeof(BITMAPINFOHEADER), F);
  fwrite(image, 1, imSize, F);//� ���� �����������
  delete[]image;//��������� ������
  fclose(F);//����������� ����
}
void sample(){}//�������-��������, ����� ����� ��� ������� ����� ���������:
//���� ������� ����� �������, ��� �� ������ ������� ���� ��������� ���� �� ����� ��� ����� ������, � � ���� ����� �������� �����-�� �������
//��������� �������� ��� ���, �� ��� ���������� ���������� ��� ���� ���, ������� � ����� ����� ��������� ��������
void line(double x1,double y1,double x2,double y2){//�������� ��� � �������, ������ ��� ������ ����������� line ��������� ������ ��� ��� ����� ����� � ���������
	glVertex2f(x1,y1);//����� = ��������� ��� ��� ��� �������
	glVertex2f(x2,y2);
}
int main (int argc, char** argv){
	glutInit(&argc, argv);//��� ������������� �����
	glutInitWindowPosition(0,0);
	glutInitWindowSize(500,500);
	glutInitDisplayMode(GLUT_RGBA|GLUT_SINGLE);
	glutCreateWindow("Differential equation"); 
	glClearColor(1,1,1,1);//����� ��� (�� ���� RGBA, ���� ����� � ��������� ������ �� 0 �� 1 ��� ������� �����)
	glClear(GL_COLOR_BUFFER_BIT);//� �������� ���� �����
	glColor3d(0,0,0);//������ ����(��� ��� A)
	//���� ���� �� �������� �����������: ��� ������������� (�.�. �� � ��������, � � ����� ����) � �������� �� -1 �� 1.
	//��������, ����� (0,0) - �����, (1,1) - ������ ������� ����
	glBegin (GL_LINES); //��������� � ����� ��������� �����, �.�. ������ ��������� ����� ������ ��� �������, �� ���� glVertex2f, ���������� ������
		line(-0.8,0.9,-0.8,-0.9);//��� y
		line(-0.8,-0.9,0.9,-0.9);//��� x
		line(-0.8,0.9,-0.77,0.87);//��������� ��� y
		line(-0.8,0.9,-0.83,0.87);
		line(0.9,-0.9,0.87,-0.87);//��������� ��� �
		line(0.9,-0.9,0.87,-0.93);
		double a=0,b=0.5,h=0.05;//����������� �������, ���. ����� ��� �������� ������� �������� �� ����� ������, �� ������ �� ����� ������ �������� ���� �� �����
		int n=(b-a)/h;//���������� �������� ���������
		double x[n+1],y[n+1];//�����
		x[0]=0;//��������� �������
		y[0]=0;
		for (int i=1;i<=n;i++){
			x[i]=x[i-1]+h;//����������� ���
			y[i]=y[i-1]+(x[i]-x[i-1])*f(x[i-1],y[i-1]);//������� y �� ������
			line(-0.8+x[i-1]/(b-a)*1.7,//������ ����� �� ������� ����� �� ����� �����
			     -0.9+y[i-1]*15,//��������� ��� ���������� � ������������ ������, �� ��� ����� �� ���� ����������� ������� 
				 -0.8+x[i]/(b-a)*1.7,//(���� �����, ������� �� �����������) � �������� �� ����� ������������ ������� � ����������� ������, � ���,���� �� 65 ������, 0.8+0.9=1.7
				 -0.9+y[i]*15);//� ����� ������ �������� �� �����-�� �����������
			line(-0.8+x[i-1]/(b-a)*1.7,//������ �������� �� ��� x
			     -0.92,
				 -0.8+x[i-1]/(b-a)*1.7,
				 -0.88);
			line(-0.82,//� �� ��� y
			     -0.9+y[i-1]*15,
				 -0.78,
				 -0.9+y[i-1]*15);
		}
		line(-0.82,//���������� ����������� �������� �� ��� y, �� ��� x �� ���� �������� ���������
		     -0.9+y[n]*15,
			 -0.78,
			 -0.9+y[n]*15);
    glEnd();//��������� ����� ��������� �����, ����� ������ �������� �����
    text(-0.85,0.92,"y");//������� ���������
    text(0.92,-0.9,"x");
	char tmp[256];//��� ������ 
    for (int i=0;i<n;i++){
    	sprintf(tmp,"%5.2f",x[i]);//��������� ����� � ������
    	text(-0.85+x[i]/(b-a)*1.7,-0.98,tmp);//� �������
    	sprintf(tmp,"%5.4f",y[i]);
    	text(-0.98,y[i]*15-0.92,tmp);
	}
	sprintf(tmp,"%5.4f",y[n]);//����������� ����� �� ��� y (�� x ��� ��� ����������, ������� �� �������
	text(-0.98,y[n]*15-0.92,tmp);
    glutSwapBuffers();//������ ���� ��������, ��� �� ����������
    output();//������� ��� � ����
	glutDisplayFunc(sample); //�������, ��� �������� ������� ����� ����� - ��������
    glutMainLoop();//� ��������� ����
}

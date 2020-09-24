#include <cstdio>
#include <ctime>
#include <cmath>
#include <GL/freeglut.h>
#include <conio.h>
#include <algorithm>
#define PI 3.14159265
const int DOTS=1000,DIMS=3,ITERS=15;
double x[DOTS][DIMS],v[DOTS][DIMS],a=0,b=PI/2,seg=b-a,best=1000, alpha=0.1,
       p[DOTS][DIMS]={0},g[DIMS]={0},
       pbest[DOTS],results[2]={0},top=0.33,info=0.38,bottom=-0.33; 
int tries;
FILE*f;
double func(double x[DIMS]) {
 int i,j,final=101;
 double sum, total=0;
 for(j=3;j<=final;j+=2) {
  sum=0;
  for(i=0;i<DIMS;i++) sum+=cos(j*x[i]);
  total+=sum*sum/j/j;
 }
 total*=16/(PI*PI);
 total=sqrt(total);
 sum=0;
 for(i=0;i<DIMS;i++) sum+=cos(x[i]);
 sum*=4/PI;
 if (sum<0)sum*=-1;
 return total/sum;
}
void bars() {
 glColor3d(1,1,1);
 glBegin(GL_QUADS);
  glVertex2d(-1,1); glVertex2d(1,1); glVertex2d(1,info); glVertex2d(-1,info);
  glVertex2d(-1,-1); glVertex2d(1,-1); glVertex2d(1,bottom); glVertex2d(-1,bottom);
  glColor3d(0,0,0);
  glVertex2d(1,info); glVertex2d(-1,info); glVertex2d(-1,bottom); glVertex2d(1,bottom);
 glEnd();
}
double random() { return (rand()%20000)/20000.0; }
void renderScene() {
 double tmp,sum;
 int i,j,k;
 for(i=0; i<DOTS; i++) pbest[i]=1000;
 best=1000;
 for(i=0; i<DOTS; i++){
  for(j=0; j<DIMS; j++){
   x[i][j]=random()*seg+a;
   v[i][j]=random()*seg*(rand()%2 ? 1 : -1);
  }
  tmp=func(x[i]);
  if (tmp<best)
   for(j=0; j<DIMS; j++) p[i][j]=g[j]=x[i][j],best=pbest[i]=tmp;
  else if (tmp<pbest[i])
   for(j=0; j<DIMS; j++) p[i][j]=x[i][j],pbest[i]=tmp;
 }
 for(k=0;best>= 0.11017 && k<ITERS;k++){
  glColor3d(0,0,0);
  glBegin(GL_LINES);
   glVertex2d(-1,top-(top-bottom)*k/ITERS);
   glVertex2d(1,top-(top-bottom)*k/ITERS);
  glEnd();
  glBegin(GL_POINTS);
  for(i=0; i<DOTS; i++) {
   for(j=0; j<DIMS; j++) {
    v[i][j]=alpha*v[i][j]+1.4*random()*(p[i][j]-x[i][j])
           +1.4*random()*(g[j]-x[i][j]);
    x[i][j]+=v[i][j];
    if (x[i][j]<a) x[i][j]*=-1;
    if (x[i][j]>PI/2) x[i][j]/=2;
   }
   glColor3d(1,0,0);
   glVertex2d(-1+x[i][0]*4/PI,top-(top-bottom)*k/ITERS);
   glColor3d(0,1,0);
   glVertex2d(-1+x[i][1]*4/PI,top-(top-bottom)*k/ITERS);
   glColor3d(0,0,1);
   glVertex2d(-1+x[i][2]*4/PI,top-(top-bottom)*k/ITERS);
   tmp=func(x[i]);
   if (tmp<best)
    for(j=0; j<DIMS; j++) p[i][j]=g[j]=x[i][j],best=pbest[i]=tmp;
   else if (tmp<pbest[i])
    for(j=0; j<DIMS; j++) p[i][j]=x[i][j],pbest[i]=tmp;
  }
  glEnd();
  glutSwapBuffers();
  glutPostRedisplay();
 }
 fprintf(f,
         "| %.8f | %.8f | %.8f || u * %.15e || %.2f ||",
         g[0],g[1],g[2],best,alpha);
 k>=ITERS ?  fprintf(f," >%d|\n", ITERS) : fprintf(f," %3d |\n",k);
 std::sort(g,g+DIMS);
 bars();
 glColor3d(1,0,0);
 glBegin(GL_POINTS);
  for(tmp=a;tmp<b;tmp+=0.001)
   glVertex2d(-1+2.0*tmp/seg,info+(1.0-info)*sin(tmp));
  for(tmp=a;tmp<25;tmp+=0.001) 
   glVertex2d(-1+2.0*tmp/25,
              bottom+(bottom+1.0)*sin(tmp)/3-(bottom+1.0)/2);
 glEnd();
 glColor3d(0,0,1);
 glBegin(GL_LINES);
  glVertex2d(-1,info);
  glVertex2d(-1+g[0]*2.0/seg,info);
  glVertex2d(-1+g[0]*2.0/seg,info);
  glVertex2d(-1+g[0]*2.0/seg,info+(1-info)/3);
  glVertex2d(-1+g[0]*2.0/seg,info+(1-info)/3);
  glVertex2d(-1+g[1]*2.0/seg,info+(1-info)/3);
  glVertex2d(-1+g[1]*2.0/seg,info+(1-info)/3);
  glVertex2d(-1+g[1]*2.0/seg,info+(1-info)*2/3);
  glVertex2d(-1+g[1]*2.0/seg,info+(1-info)*2/3);
  glVertex2d(-1+g[2]*2.0/seg,info+(1-info)*2/3);
  glVertex2d(-1+g[2]*2.0/seg,info+(1-info)*2/3);
  glVertex2d(-1+g[2]*2.0/seg,info+(1-info));
  glVertex2d(-1+g[2]*2.0/seg,info+(1-info));
  glVertex2d(1,info+(1-info));
 glEnd();
 for(sum=0,i=0;i<DIMS;i++) sum+=cos(g[i]);
 sum*=4/PI;
 glColor3d(0,0,1);
 glBegin(GL_POINTS);
  for(tmp=a;tmp<b;tmp+=0.001)
   glVertex2d(-1+2.0*tmp/seg,info+(1-info)*sin(tmp)*sum/DIMS);
  for(tmp=a;tmp<25;tmp+=0.001)
   glVertex2d(-1+2.0*tmp/25,
              bottom+(bottom+1)*sin(tmp)*sum/DIMS/3-(bottom+1)/2);
 glEnd();
 if(alpha==0.1) results[0]+=k; else results[1]+=k;
 if(tries<19)tries++; else if(alpha==0.1) alpha = 0.2, tries = 0;
 else {
  fprintf(f,"|____________|____________|____________||____________________________||______||_____|\n\n");
  fprintf(f,"a = 0.1, mean iterations quantity = %3.1f\n",
          results[0]/20.0);
  fprintf(f,"    0.2,                            %3.1f\n", results[1]/20.0);
  system("pause");
  fclose(f);
  exit(0);
 }
}
int main (int argc,char**argv) {
 f=stdout;
 glutInit(&argc,argv);
 glutInitWindowPosition(0,0);
 glutInitWindowSize(500,500);
 glutInitDisplayMode(GLUT_RGBA|GLUT_SINGLE);
 glutCreateWindow("Particles"); 
 glutDisplayFunc(renderScene);
 glEnable(GL_BLEND);
 glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
 srand(time(0));
 for(int i=0;i<DOTS;i++) pbest[i]=20;
  fprintf(f,"_____________________________________________________________________________________\n");
  fprintf(f,"|                                      ||                            ||      ||     |\n");
  fprintf(f,"|                alphas                ||            koef            ||   a  ||  k  |\n");
  fprintf(f,"|______________________________________||____________________________||______||_____|\n");
 tries=0;
 glPolygonMode(GL_FRONT,GL_FILL);
 bars();
 glutMainLoop();
}


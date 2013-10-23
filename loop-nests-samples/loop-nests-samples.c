#include <omp.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/time.h>
#include <time.h>
#include <math.h>

#define DATASIZE 20

int indexArray[DATASIZE];
double A1D1[DATASIZE];
double A2D1[DATASIZE];
double A3D1[DATASIZE];
double A4D1[DATASIZE];
double A1D2[DATASIZE][DATASIZE];
double A2D2[DATASIZE][DATASIZE];
double A3D2[DATASIZE][DATASIZE];
double A1D3[DATASIZE][DATASIZE][DATASIZE];
double A2D3[DATASIZE][DATASIZE][DATASIZE];
double scalarS = 0.0;
double alpha = 1.0;
double beta  = 1.0;

// support functions
unsigned int checksum(double *t, int size){
  unsigned int cs =0;
  int i;
  if (!t) return 0;
  for (i=0; i<size; i++){
    cs = cs + (unsigned int) t[i];
  }
  return cs;
}

void initArrays(){
  int i,j,k;
  for (i=0;i< DATASIZE; i++){
    A1D1[i] =  i;
    A2D1[i] = 2*i;
    A3D1[i] = 3*i;
    A4D1[i] = 4*i;
    indexArray[i] = (i+1)%DATASIZE;
    for (j=0;j< DATASIZE; j++){
      A1D2[i][j] = i*j;
      A2D2[i][j] = 2*i*j;
      A3D2[i][j] = 3*i*j;
      for (k=0;k< DATASIZE; k++){
	A1D3[i][j][k] = i*j*k;
	A2D3[i][j][k] = 2*i*j*k;
      }
    }
  }
}

// sample loop nest
void loopNest0(){
  int i;
  for (i=0;i< DATASIZE/2-1; i++){
    A1D1[2*i] = A2D1[i]+1;
    A3D1[i]   = A1D1[2*i+1];
  }
}

//
void loopNest1(){
  int i,j;
  for (i=0;i< DATASIZE; i++){
    for (j=0;j< DATASIZE; j++){
      A1D2[i][j] = 2*A2D2[i][j];
    }
  }
}

//
void loopNest2(){
  int i,j;
  for (i=0;i< DATASIZE-1; i++){
    for (j=0;j< DATASIZE-1; j++){
      A1D2[i][j] = 2*(A1D2[i+1][j] + A1D2[i+1][j+1]);
    }
  }
}

//
void loopNest3(){
  int i,j;
  double T;
  for (i=0;i< DATASIZE-1; i++){
    T = A1D1[i];
    A1D1[i] = A2D1[i];
    A2D1[i] = T;
    A3D1[i+1] = A3D1[i] + A4D1[i];
  }
}

//
void loopNest4(){
  int i,j;
  double T;
  for (i=0;i< DATASIZE-1; i++){
    A2D1[indexArray[i]] = 2*A2D1[indexArray[i]+1];
  }
}

// 
void loopNest5(){
  int i,j;
  for (i=0;i< DATASIZE/5-1; i++){
    for (j=0;j< DATASIZE; j++){
      A1D2[3*i+2][j] = 2*A1D2[5*i+1][j];
    }
  }
}

//   
void loopNest6(){
  int i,j;
  for (j=1;j< DATASIZE-1; j++){
    for (i=1;i< DATASIZE-1; i++){
      A1D2[j][i] = 2*(A1D2[j-1][i+1] + A1D2[j+1][i]);
    }
  }
}

//
void loopNest7(){
  int i,j;

  for (i=1;i< DATASIZE-1; i++){
    for (j=1;j< DATASIZE-1; j++){
      A1D2[i][j] = A1D2[i-1][j+1]+1;
    }
  }
  for (i=1;i< DATASIZE-1; i++){
    for (j=DATASIZE-2; j>0; j--){
      A1D2[i][j] = A1D2[i-1][j+1]+1;
    }
  }
}

//
void loopNest8(){
  int i,j;
  double T;
  for (i=0;i< DATASIZE; i++){
    scalarS =  scalarS + A1D1[i]*A2D1[i];
  }
}

void loopNest9(){
  int iy, ix;
 
  for (iy = 0; iy < DATASIZE; iy++) {
    A1D2[iy][0] = 255.0;
    A1D2[iy][DATASIZE-1] = 255.0;
  }
  
  for (ix = 1; ix < DATASIZE-1; ix++) {
    A1D2[0][ix] = 255.0;
    A1D2[DATASIZE-1][ix] = 255.0;
  }
  
  for (iy = 1; iy < DATASIZE-1; iy++) {
    for (ix = 1; ix < DATASIZE-1; ix++) {
      double sum_x=0, sum_y=0, sum=0;
      
      sum_x +=  A2D2[iy - 1][ix - 1] * -1;
      sum_x +=  A2D2[iy - 1][ix    ] * -2;
      sum_x +=  A2D2[iy - 1][ix + 1] * -1;
      
      sum_x +=  A2D2[iy + 1][ix - 1] * 1;
      sum_x +=  A2D2[iy + 1][ix    ] * 2;
      sum_x +=  A2D2[iy + 1][ix + 1] * 1;
      
      sum_x = fmin(255.0, fmax( 0.0, sum_x ));
      
      sum_y +=  A2D2[iy - 1][ix - 1] * 1;
      sum_y +=  A2D2[iy - 1][ix + 1] * -1;
      
      sum_y+=   A2D2[iy][ix - 1] * 2;
      sum_y+=   A2D2[iy][ix + 1] * -2;
      
      sum_y+=   A2D2[iy + 1][ix - 1] * 1;
      sum_y+=   A2D2[iy + 1][ix + 1] * -1;
      
      sum_y = fmin(255, fmax( 0, sum_y ));
      sum = fabs(sum_x) + fabs(sum_y); 
      A1D2[iy][ix] = 255.0 - sum;  
    }
  }
}


void loopNest10(){
  int i,j,k;
  for (k = 0; k < DATASIZE; k++){
    for (j = k + 1; j < DATASIZE; j++)
      A1D2[k][j] = A1D2[k][j] / A1D2[k][k];
    for(i = k + 1; i < DATASIZE; i++)
      for (j = k + 1; j < DATASIZE; j++)
	A1D2[i][j] = A1D2[i][j] + A1D2[i][k] * A1D2[k][j];
  }
}

void loopNest11(){
  int i,j,j1,j2;
  double float_n = 1.0;
  double eps = 0.1f;

  for (j = 0; j < DATASIZE; j++){
    A1D1[j] = 0.0;
    for (i = 0; i < DATASIZE; i++)
      A1D1[j] += A1D2[i][j];
    A1D1[j] /= float_n;
  }

  for (j = 0; j < DATASIZE; j++){
    A2D1[j] = 0.0;
    for (i = 0; i < DATASIZE; i++)
      A2D1[j] += (A1D2[i][j] - A1D1[j]) * (A1D2[i][j] - A1D1[j]);
    A2D1[j] /= float_n;
    A2D1[j] = sqrt(A3D1[j]);
    A2D1[j] = A2D1[j] <= eps ? 1.0 : A2D1[j];
  }

  for (i = 0; i < DATASIZE; i++)
    for (j = 0; j < DATASIZE; j++){
      A1D2[i][j] -= A1D1[j];
      A1D2[i][j] /= sqrt(float_n) * A2D1[j];
    }

  for (j1 = 0; j1 < DATASIZE-1; j1++){
    A2D2[j1][j1] = 1.0;
    for (j2 = j1+1; j2 < DATASIZE; j2++){
      A2D2[j1][j2] = 0.0;
      for (i = 0; i < DATASIZE; i++)
	A2D2[j1][j2] += (A1D2[i][j1] * A1D2[i][j2]);
      A2D2[j2][j1] = A2D2[j1][j2];
    }
  }
  A2D2[DATASIZE-1][DATASIZE-1] = 1.0;
}

//
void loopNest12(){
  int t,i1,i2;
  for (t = 0; t < 10; t++){
    for (i1 = 0; i1 < DATASIZE; i1++)
      for (i2 = 1; i2 < DATASIZE; i2++){
	A2D2[i1][i2] = A2D2[i1][i2] - A2D2[i1][i2-1] * A1D2[i1][i2] / A3D2[i1][i2-1];
	A3D2[i1][i2] = A3D2[i1][i2] - A1D2[i1][i2] * A1D2[i1][i2] / A3D2[i1][i2-1];
      }
    for (i1 = 0; i1 < DATASIZE; i1++)
      A2D2[i1][DATASIZE-1] = A2D2[i1][DATASIZE-1] / A3D2[i1][DATASIZE-1];

    for (i1 = 0; i1 < DATASIZE; i1++)
      for (i2 = 0; i2 < DATASIZE-2; i2++)
	A2D2[i1][DATASIZE-i2-2] = (A2D2[i1][DATASIZE-2-i2] 
				   - A2D2[i1][DATASIZE-2-i2-1] * A1D2[i1][DATASIZE-i2-3]) 
	  / A3D2[i1][DATASIZE-3-i2];

    for (i1 = 1; i1 < DATASIZE; i1++)
      for (i2 = 0; i2 < DATASIZE; i2++) {
	A2D2[i1][i2] = A2D2[i1][i2] - A2D2[i1-1][i2] * A1D2[i1][i2] / A3D2[i1-1][i2];
	A3D2[i1][i2] = A3D2[i1][i2] - A1D2[i1][i2] * A1D2[i1][i2] / A3D2[i1-1][i2];
      }

    for (i2 = 0; i2 < DATASIZE; i2++)
      A2D2[DATASIZE-1][i2] = A2D2[DATASIZE-1][i2] / A3D2[DATASIZE-1][i2];

    for (i1 = 0; i1 < DATASIZE-2; i1++)
      for (i2 = 0; i2 < DATASIZE; i2++)
	A2D2[DATASIZE-2-i1][i2] = (A2D2[DATASIZE-2-i1][i2] 
				   - A2D2[DATASIZE-i1-3][i2] * A1D2[DATASIZE-3-i1][i2]) 
	  / A3D2[DATASIZE-2-i1][i2];
  }
}

//
void loopNest13(){
  int t, i, j;
  for (t = 0; t < 10; t++){
    for (i = 1; i < DATASIZE - 1; i++)
      A2D1[i] = 0.33333 * (A1D1[i-1] + A1D1[i] + A1D1[i + 1]);
    for (j = 1; j < DATASIZE - 1; j++)
      A1D1[j] = A2D1[j];
  }
}

//
void loopNest14(){
  int t, i, j;
  for (t = 0; t <= 10 - 1; t++)
    for (i = 1; i<= DATASIZE - 2; i++)
      for (j = 1; j <= DATASIZE - 2; j++)
        A1D2[i][j] = (A1D2[i-1][j-1] + A1D2[i-1][j] + A1D2[i-1][j+1]
                   + A1D2[i][j-1] + A1D2[i][j] + A1D2[i][j+1]
                   + A1D2[i+1][j-1] + A1D2[i+1][j] + A1D2[i+1][j+1])/9.0;
}

//
void loopNest15(){
  int t, i, j,k;
  for (i = 0; i < DATASIZE; ++i){
    double x;
    x = A1D2[i][i];
    for (j = 0; j <= i - 1; ++j)
      x = x - A1D2[i][j] * A1D2[i][j];
    A1D1[i] = 1.0 / sqrt(x);
    for (j = i + 1; j < DATASIZE; ++j){
      x = A1D2[i][j];
      for (k = 0; k <= i - 1; ++k)
	x = x - A1D2[j][k] * A1D2[i][k];
      A1D2[j][i] = x * A1D1[i];
    }
  }
}

//
void loopNest16(){
  int i,j,k;
  for (i = 0; i < DATASIZE; i++){
    for (j = 0; j < DATASIZE; j++){
      A1D2[i][j] *= beta;
      for (k = 0; k < DATASIZE; ++k)
	A1D2[i][j] += alpha * A2D2[i][k] * A3D2[k][j];
    }
  }
}


int main(int argc, char *argv[]){

  initArrays();
  printf("Initial A1D1 checksum %u\n", checksum(A1D1,DATASIZE));
  printf("Initial A2D1 checksum %u\n", checksum(A2D1,DATASIZE));
  printf("Initial A3D1 checksum %u\n", checksum(A3D1,DATASIZE));
  printf("Initial A4D1 checksum %u\n", checksum(A4D1,DATASIZE));
  printf("Initial A1D2 checksum %u\n", checksum((double *) A1D2,DATASIZE*DATASIZE));
  printf("Initial A2D2 checksum %u\n", checksum((double *) A2D2,DATASIZE*DATASIZE));
  printf("Initial A3D2 checksum %u\n", checksum((double *) A3D2,DATASIZE*DATASIZE));
  printf("Initial A1D3 checksum %u\n", checksum((double *) A1D3,DATASIZE*DATASIZE*DATASIZE));
  printf("Initial A2D3 checksum %u\n", checksum((double *) A2D3,DATASIZE*DATASIZE*DATASIZE));

  //loopNest0();
  //loopNest1();
  //loopNest2();
  //loopNest3();
  //loopNest4();
  //loopNest5();
  //loopNest6();
  //loopNest7();
  //loopNest8();
  //loopNest9();
  //loopNest10();
  //loopNest11();
  //loopNest12();
  //loopNest13();
  //loopNest14();
  //loopNest15();
  loopNest16();

  printf("Resulting A1D1 checksum %u\n", checksum(A1D1,DATASIZE));
  printf("Resulting A2D1 checksum %u\n", checksum(A2D1,DATASIZE));
  printf("Resulting A3D1 checksum %u\n", checksum(A3D1,DATASIZE));
  printf("Resulting A4D1 checksum %u\n", checksum(A4D1,DATASIZE));
  printf("Resulting A1D2 checksum %u\n", checksum((double *) A1D2,DATASIZE*DATASIZE));
  printf("Resulting A2D2 checksum %u\n", checksum((double *) A2D2,DATASIZE*DATASIZE));
  printf("Resulting A3D2 checksum %u\n", checksum((double *) A3D2,DATASIZE*DATASIZE));
  printf("Resulting A1D3 checksum %u\n", checksum((double *) A1D3,DATASIZE*DATASIZE*DATASIZE));
  printf("Resulting A2D3 checksum %u\n", checksum((double *) A2D3,DATASIZE*DATASIZE*DATASIZE));
  return 0;
}

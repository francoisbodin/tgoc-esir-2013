#include <stdlib.h>
#include <stdio.h>
#include <sys/time.h>
#include <time.h>

#define DATASIZE 20

int indexArray[DATASIZE];
double A1D1[DATASIZE];
double A2D1[DATASIZE];
double A3D1[DATASIZE];
double A4D1[DATASIZE];
double A1D2[DATASIZE][DATASIZE];
double A2D2[DATASIZE][DATASIZE];
double A1D3[DATASIZE][DATASIZE][DATASIZE];
double A2D3[DATASIZE][DATASIZE][DATASIZE];

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

int main(int argc, char *argv[]){

  initArrays();
  printf("Initial A1D1 checksum %u\n", checksum(A1D1,DATASIZE));
  printf("Initial A2D1 checksum %u\n", checksum(A2D1,DATASIZE));
  printf("Initial A3D1 checksum %u\n", checksum(A3D1,DATASIZE));
  printf("Initial A4D1 checksum %u\n", checksum(A4D1,DATASIZE));
  printf("Initial A1D2 checksum %u\n", checksum((double *) A1D2,DATASIZE*DATASIZE));
  printf("Initial A2D2 checksum %u\n", checksum((double *) A2D2,DATASIZE*DATASIZE));
  printf("Initial A1D3 checksum %u\n", checksum((double *) A1D3,DATASIZE*DATASIZE*DATASIZE));
  printf("Initial A2D3 checksum %u\n", checksum((double *) A2D3,DATASIZE*DATASIZE*DATASIZE));

  //loopNest0();
  //loopNest1();
  //loopNest2();
  //loopNest3();
  //loopNest4();
  //loopNest5();
  //loopNest6();
  loopNest7();

  printf("Resulting A1D1 checksum %u\n", checksum(A1D1,DATASIZE));
  printf("Resulting A2D1 checksum %u\n", checksum(A2D1,DATASIZE));
  printf("Resulting A3D1 checksum %u\n", checksum(A3D1,DATASIZE));
  printf("Resulting A4D1 checksum %u\n", checksum(A4D1,DATASIZE));
  printf("Resulting A1D2 checksum %u\n", checksum((double *) A1D2,DATASIZE*DATASIZE));
  printf("Resulting A2D2 checksum %u\n", checksum((double *) A2D2,DATASIZE*DATASIZE));
  printf("Resulting A1D3 checksum %u\n", checksum((double *) A1D3,DATASIZE*DATASIZE*DATASIZE));
  printf("Resulting A2D3 checksum %u\n", checksum((double *) A2D3,DATASIZE*DATASIZE*DATASIZE));
  return 0;
}

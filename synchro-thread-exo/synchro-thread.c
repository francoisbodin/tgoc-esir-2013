#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <sys/time.h>
#include <time.h>
#include "barrier.h"

#define SERIAL 0
#define N 500
#define NBMAT 5

double counter=0;
volatile unsigned int checksum = 0;

double *matArray1[NBMAT];
double *matArray2[NBMAT];

double serialFunc(){
  counter++;
  return counter;
}

void *multiplyMatCstP1(void *arg){
  int i,j;
  double *M1;
  double *M2;
  int indexMat = *((int *) arg);
  M1 = matArray1[indexMat];
  for (i=0;i<N;i++){
    for (j=0;j<N;j++){
      M1[i*N+j] =  M1[i*N+j]*10.0;
    }
  }
  return NULL;
}

void *multiplyMatCstP2(void *arg){
  int i,j;
  double *M1;
  double *M2;
  int indexMat = *((int *) arg);
  M1 = matArray1[(indexMat+1)%NBMAT];
  M2 = matArray2[indexMat];
  for (i=0;i<N;i++){
    for (j=0;j<N;j++){
      M2[i*N+j] =  M1[i*N+j];
      checksum += (unsigned int) M2[i*N+j];
    }
  }
  return NULL;
}


void *multiplyMatCst(void *arg){
  int i,j;
  double *M1;
  double *M2;
  int indexMat = *((int *) arg);
  M1 = matArray1[indexMat];
  for (i=0;i<N;i++){
    for (j=0;j<N;j++){
      M1[i*N+j] =  M1[i*N+j]*10.0;
    }
  }
  M1 = matArray1[(indexMat+1)%NBMAT];
  M2 = matArray2[indexMat];
  for (i=0;i<N;i++){
    for (j=0;j<N;j++){
      M2[i*N+j] =  M1[i*N+j];
      checksum += (unsigned int) M2[i*N+j];
    }
  }
  return NULL;
}

//return the wallclock time in seconds
double wallclock(){
  struct timeval tv;
  double t;

  gettimeofday(&tv, NULL);

  t = (double)tv.tv_sec;
  t += ((double)tv.tv_usec)/1000000.0;
  return t;
}

int main(int argc, char *argv[]){
  double tstart, tend;
  pthread_t mytask_thread[NBMAT];
  int tm[NBMAT];
  int i,j,k;

  printf("Initializing the matrices\n");
  if (!SERIAL)
    printf("PARALLEL EXECUTION\n");
  else
    printf("SERIAL EXECUTION\n");
  tstart = wallclock();
  for (k=0;k<NBMAT;k++){
    matArray1[k] = (double *) malloc (N*N*sizeof(double));
    matArray2[k] = (double *) malloc (N*N*sizeof(double));
    for (i=0;i<N;i++){
      for(j=0;j<N;j++){
	matArray1[k][i*N+j] = serialFunc();
	matArray2[k][i*N+j] = 0.0;
      }
    }
  }
  tend = wallclock();
  printf("Execution time init part %f sec.\n", tend - tstart);
  for (k=0;k<NBMAT;k++){
    printf("Initial Value --> %f\n",matArray1[k][(N*N+N)/2]);
  }
  tstart = wallclock();
  if (!SERIAL) { // create a set of threads
    for (k=0;k<NBMAT;k++){
      tm[k] = k;
      if(pthread_create(&(mytask_thread[k]), NULL, multiplyMatCst, (void *) &(tm[k]))) {
	fprintf(stderr, "Error creating thread\n");
	return 1;
      }
    }
  } else { //
    for (k=0;k<NBMAT;k++){
      multiplyMatCstP1((void *) &k);
    }
    for (k=0;k<NBMAT;k++){
      multiplyMatCstP2((void *) &k);
    }
  }
  if (!SERIAL){ // if SERIAL no need to join
    // wait for the threads to finish 
    for (k=0;k<NBMAT;k++){
      if(pthread_join(mytask_thread[k], NULL)) {
	fprintf(stderr, "Error joining thread\n");
	return 2;
      }
    }
  }
  tend = wallclock();
  printf("Execution time compute part %f sec.\n", tend - tstart);

  for (k=0;k<NBMAT;k++){
    printf("Results  -->  %f\n",matArray2[k][(N*N+N)/2]);
  }

  printf("Checksum is %u\n",checksum);
  for (k=0;k<NBMAT;k++){
    free(matArray1[k]);
    free(matArray2[k]);
  }
  return 0;
}

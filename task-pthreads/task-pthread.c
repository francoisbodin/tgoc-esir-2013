#include <stdlib.h>
#include <pthread.h>
#include <stdio.h>
#include <sys/time.h>
#include <time.h>

#define N 1000
#define NBMAT 10
double counter=0;

double serialFunc(){
  counter++;
  return counter;
}

void *multiplyMatCst(void *arg){
  int i,j;
  double *M;
  M = (double *) arg;
  for (i=0;i<N;i++){
    for (j=0;j<N;j++){
      M[i*N+j] =  M[i*N+j]*10.0;
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
  double *matArray[NBMAT];
  int i,j,k;

  printf("Initializing the matrices\n");
  tstart = wallclock();
  for (k=0;k<NBMAT;k++){
    matArray[k] = (double *) malloc (N*N*sizeof(double));
    for (i=0;i<N;i++){
      for(j=0;j<N;j++){
	matArray[k][i*N+j] = serialFunc();
      }
    }
  }
  tend = wallclock();
  printf("Execution time serial part %f sec.\n", tend - tstart);
  for (k=0;k<NBMAT;k++){
    printf("Before creating the threads --> %f\n",matArray[k][(N*N+N)/2]);
  }
  tstart = wallclock();
  // create a set of threads
  for (k=0;k<NBMAT;k++){
    if(pthread_create(&(mytask_thread[k]), NULL, multiplyMatCst, matArray[k])) {
      fprintf(stderr, "Error creating thread\n");
      return 1;
    }
  }
  // wait for the threads to finish 
  for (k=0;k<NBMAT;k++){
    if(pthread_join(mytask_thread[k], NULL)) {
      fprintf(stderr, "Error joining thread\n");
      return 2;
    }
  }
  tend = wallclock();
  printf("Execution time parallel part %f sec.\n", tend - tstart);

  for (k=0;k<NBMAT;k++){
    printf("After joining the threads -->  %f\n",matArray[k][(N*N+N)/2]);
  }

  for (k=0;k<NBMAT;k++){
    free(matArray[k]);
  }
  return 0;
}

#include <omp.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <time.h>

#define N 5000
double counter=0;
double Y[N], X[N];
double M[N][N];

double serialFunc(){
  counter++;
  return counter;
}

void *mytask(){
  int i,j;
  for (i=0;i<N;i++){
    for (j=0;j<N;j++){
      M[i][j] = X[i]*Y[j];
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

int main (int argc, char *argv[]) {
  int nthreads, tid;
  double tstart, tend;
  int i;
  for (i=0;i<N;i++){
    Y[i] = serialFunc();
    X[i] = serialFunc();
  }
 // create a parallel section ~ fork
  tstart = wallclock();
#pragma omp parallel private(nthreads, tid)
  {
    // get the thread number running
    tid = omp_get_thread_num();
    printf("I am thread id = %d\n", tid);

    // the master thread 
    if (tid == 0) {
      nthreads = omp_get_num_threads();
      printf("Number of threads = %d\n", nthreads);
    } else {
      mytask();
    }
    // end of the parallel section ~ join operation   
  }
  tend = wallclock();
  printf("--> result %f\n",M[N/2][N/2]);
  return 0;
}


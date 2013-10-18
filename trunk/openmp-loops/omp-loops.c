#include <omp.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <time.h>

#define N 10000
double counter=0;
double Y[N], X[N];
double M[N][N];

double serialFunc(){
  counter++;
  return counter;
}

void mytask(){
  int i,j;
  // create a parallel section ~ fork
#pragma omp parallel for private(j)
  for (i=0;i<N;i++){
    for (j=0;j<N;j++){
      M[i][j] = X[i]*Y[j];
    }
  }
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
  int nthreads;
  double tstart, tend;
  int i;
  for (i=0;i<N;i++){
    Y[i] = serialFunc();
    X[i] = serialFunc();
  }
  omp_set_num_threads(2);
#pragma omp parallel
  {
#pragma omp master
    {
      nthreads = omp_get_num_threads();
    }
  }
  printf("Number of threads = %d\n", nthreads);
  tstart = wallclock();
  mytask();
  tend = wallclock();
  printf("--> result %f\n",M[N/2][N/2]);
  printf("Execution time %f sec.\n", tend - tstart);
  return 0;
}


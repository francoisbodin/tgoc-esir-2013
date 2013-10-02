#include <stdio.h>
#include <unistd.h>
#include <sys/time.h>
#include <time.h>

#define N 1000
double counter=0;
double Y[N], X[N];
double M[N][N];

double serialFunc(){
  counter++;
  return counter;
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
  int i,j;
  printf("Starting\n");
  tstart = wallclock();
  for (i=0;i<N;i++){
    Y[i] = serialFunc();
    X[i] = serialFunc();
  }
  tend = wallclock();
  printf("Execution time serial part %f sec.\n", tend - tstart);

  tstart = wallclock();
  for (i=0;i<N;i++){
    for (j=0;j<N;j++){
      M[i][j] = X[i]*Y[j];
    }
  }
  tend = wallclock();
  printf("Execution time parallel part %f sec.\n", tend - tstart);
  printf("result %f\n",M[N/2][N/2]);
  return 0;
}

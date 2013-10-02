#include <pthread.h>
#include <stdio.h>
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

int main(int argc, char *argv[]){
  double tstart, tend;
  pthread_t mytask_thread;

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
  // create a second thread which executes mytask 
  if(pthread_create(&mytask_thread, NULL, mytask, NULL)) {
    fprintf(stderr, "Error creating thread\n");
    return 1;
  }
  tend = wallclock();
  printf("Before join result %f\n",M[N/2][N/2]);
  printf("Execution time parallel part %f sec.\n", tend - tstart);
  // wait for the second thread to finish 
  if(pthread_join(mytask_thread, NULL)) {

    fprintf(stderr, "Error joining thread\n");
    return 2;

  }
  printf("After joining the threads --> result %f\n",M[N/2][N/2]);
  return 0;
}

#include <stdio.h>
#include <unistd.h>
#include <sys/time.h>
#include <time.h>

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

  printf("Starting\n");
  tstart = wallclock();
  sleep(2);
  tend = wallclock();
  printf("Second - Sleeping during %f\n", tend - tstart);

  return 0;
}

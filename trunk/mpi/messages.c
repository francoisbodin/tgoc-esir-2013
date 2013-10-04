#include <stdlib.h>
#include <stdio.h>
#include <sys/time.h>
#include <time.h>
#include <mpi.h>

#define  MASTER   0

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
  int   numtasks, taskid, len;
  char hostname[MPI_MAX_PROCESSOR_NAME];
  //just starting the processes
  MPI_Init(&argc, &argv);
  MPI_Comm_size(MPI_COMM_WORLD, &numtasks);
  MPI_Comm_rank(MPI_COMM_WORLD,&taskid);
  MPI_Get_processor_name(hostname, &len);
  printf ("Hello from task %d on %s!\n", taskid, hostname);
  if (taskid == MASTER)
    printf("MASTER: Number of MPI tasks is: %d\n",numtasks);
  MPI_Finalize();
  return 0;
}

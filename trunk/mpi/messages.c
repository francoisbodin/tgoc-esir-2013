#include <stdlib.h>
#include <stdio.h>
#include <sys/time.h>
#include <time.h>
#include <mpi.h>

#define  MASTER   0
#define  WORKER1   1

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
  MPI_Status status;
  int tab[1] ={0};
  int label1=1;
  //just starting the processes
  MPI_Init(&argc, &argv);
  MPI_Comm_size(MPI_COMM_WORLD, &numtasks);
  MPI_Comm_rank(MPI_COMM_WORLD,&taskid);
  MPI_Get_processor_name(hostname, &len);
  printf ("Task %d on %s!\n", taskid, hostname);
  if (taskid == MASTER)
    printf("MASTER: Number of MPI tasks is: %d\n",numtasks);
  // start echanging some message in an asynchronous way
  switch(taskid){
  case MASTER:
    MPI_Recv(tab, 1, MPI_INT, WORKER1, label1, MPI_COMM_WORLD, &status);
    printf("master receiving value %d\n",tab[0]);
    break;
  case WORKER1:
    tab[0] = 999;
    printf("worker sending value %d\n",tab[0]);
    MPI_Send(tab, 1, MPI_INT, MASTER, label1, MPI_COMM_WORLD);
    break;
  default:
    // unused MPI process
    printf("unused worker %d\n",taskid);
    break;
  }
  MPI_Finalize();
  return 0;
}

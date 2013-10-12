#include <stdlib.h>
#include <stdio.h>
#include <sys/time.h>
#include <time.h>
#include <mpi.h>
#define  MASTER   0

#define REPEAT 10
#define DATASIZE 100

double MyBigDataArrayIn[DATASIZE][DATASIZE];
double MyBigDataArrayOut[DATASIZE][DATASIZE];

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
  int i,j;
  unsigned int checksum = 0;
  double tstart, tend;
  int k;
  int   numtasks, taskid, len;
  char hostname[MPI_MAX_PROCESSOR_NAME];
  // init MPI
  MPI_Init(&argc, &argv);
  MPI_Comm_size(MPI_COMM_WORLD, &numtasks);
  MPI_Comm_rank(MPI_COMM_WORLD,&taskid);
  MPI_Get_processor_name(hostname, &len);
  printf ("Task %d on %s!\n", taskid, hostname);
  if (taskid == MASTER)
    printf("MASTER: Number of MPI tasks is: %d\n",numtasks);

  // initialize the data
  for (i=0;i< DATASIZE; i++){
    for (j=0;j< DATASIZE; j++){
      MyBigDataArrayIn[i][j] = i*j;
      MyBigDataArrayOut[i][j] = 0;
    }
  }
  //Do the transpose here
  tstart = wallclock();
  for (k=0;k< REPEAT; k++){
    for (i=0;i< DATASIZE; i++){
      for (j=0;j< DATASIZE; j++){
	MyBigDataArrayOut[i][j] = MyBigDataArrayIn[j][i];
      }
    }
  }
  tend = wallclock();
  printf("Executing time is %f sec.\n",tend-tstart);
  // compute a check sum on result
  for (i=0;i< DATASIZE; i++){
    for (j=0;j< DATASIZE; j++){
      checksum = checksum + (unsigned int) MyBigDataArrayOut[i][j];
    }
  }
  // display the check sum same value in serial or parallel execution
  if (taskid == MASTER)
    printf("checksum is %u\n",checksum);
  MPI_Finalize();
  return 0;
}

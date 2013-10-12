#include <stdlib.h>
#include <stdio.h>
#include <sys/time.h>
#include <time.h>
#include <mpi.h>
#define  MASTER   0
#define  WORKER1   1

#define REPEAT 100
#define DATASIZE 1000

// let distribute the data on the first array dimension
// data size can be divided by 2
// + 2 is needed to take care of border access
double MyBigDataArrayIn[DATASIZE/2+2][DATASIZE];
double MyBigDataArrayOut[DATASIZE/2+2][DATASIZE];

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
  // this array is fully replicated between the processes
  double coef[5] = {1.0,2.0,3.0,4.0,5.0};
  unsigned int checksum = 0;
  double tstart, tend;
  int k;
  int   numtasks, taskid, len;
  char hostname[MPI_MAX_PROCESSOR_NAME];
  int label1=1;
  MPI_Status status;

  // init MPI
  MPI_Init(&argc, &argv);
  MPI_Comm_size(MPI_COMM_WORLD, &numtasks);
  MPI_Comm_rank(MPI_COMM_WORLD,&taskid);
  MPI_Get_processor_name(hostname, &len);
  printf ("Task %d on %s!\n", taskid, hostname);
  if (taskid == MASTER)
    printf("MASTER: Number of MPI tasks is: %d\n",numtasks);

  // init the data
  for (i=0;i< DATASIZE/2+2; i++){
    for (j=0;j< DATASIZE; j++){
      if (taskid == MASTER)
	MyBigDataArrayIn[i][j] = i*j;
      else 
	MyBigDataArrayIn[i][j] = (DATASIZE/2-1+i)*j;
      MyBigDataArrayOut[i][j] = 0;
    }
  }
  // compute some stuff - does not matter here
  MPI_Barrier(MPI_COMM_WORLD);
  tstart = wallclock();
  for (k=0;k< REPEAT; k++){
    if (taskid == MASTER) {
      // master Lower array
      for (i=1;i< DATASIZE/2; i++){
	for (j=1;j< DATASIZE-1; j++){
	  MyBigDataArrayOut[i][j] = MyBigDataArrayOut[i][j] +  coef[0] * MyBigDataArrayIn[i][j] 
	    + coef[1] * MyBigDataArrayIn[i-1][j] 
	    + coef[2] * MyBigDataArrayIn[i+1][j]
	    + coef[3] * MyBigDataArrayIn[i][j-1]
	    + coef[4] * MyBigDataArrayIn[i][j+1];
	}
      }
    } else {
      // worker upper array
      for (i=DATASIZE/2;i< DATASIZE-1; i++){
        for (j=1;j< DATASIZE-1; j++){
	           MyBigDataArrayOut[i-(DATASIZE/2-1)][j] = MyBigDataArrayOut[i-(DATASIZE/2-1)][j] 
	    + coef[0] * MyBigDataArrayIn[i-(DATASIZE/2-1)][j]
            + coef[1] * MyBigDataArrayIn[i-(DATASIZE/2-1)-1][j]
            + coef[2] * MyBigDataArrayIn[i-(DATASIZE/2-1)+1][j]
            + coef[3] * MyBigDataArrayIn[i-(DATASIZE/2-1)][j-1]
            + coef[4] * MyBigDataArrayIn[i-(DATASIZE/2-1)][j+1];
	  }
      }
    }
  }
  MPI_Barrier(MPI_COMM_WORLD);
  tend = wallclock();
  
  if (taskid == MASTER)
    printf("Executing time is %f sec.\n",tend-tstart);
  // compute a check sum on result
  if (taskid == MASTER) {
    // master 
    for (i=0;i< DATASIZE/2; i++){
      for (j=0;j< DATASIZE; j++){
	checksum = checksum + (unsigned int) MyBigDataArrayOut[i][j];
      }
    }
  } else {
    // worker
    for (i= DATASIZE/2;i< DATASIZE; i++){
      for (j=0;j< DATASIZE; j++){
        checksum = checksum + (unsigned int) MyBigDataArrayOut[i-(DATASIZE/2-1)][j];
      }
    }
  }
  // display the check sum same value in serial or parallel execution
  if (taskid == MASTER){
    unsigned int wchecksum = 0;
    //    printf("process %d local checksum is %u\n",taskid, checksum);
    MPI_Recv(&wchecksum,1, MPI_INT, WORKER1, label1, MPI_COMM_WORLD, &status);
    //printf("process %d remote checksum is %u\n",taskid, wchecksum);
    checksum = checksum + wchecksum;
    printf("checksum is %u\n", checksum);
  } else {
    //printf("process %d localchecksum is %u\n",taskid, checksum);
    MPI_Send(&checksum, 1, MPI_INT, MASTER, label1, MPI_COMM_WORLD);
  }
  MPI_Finalize();
  return 0;
}

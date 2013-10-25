#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <OpenCL/opencl.h>
#include <sys/time.h>
#include <time.h>



#define DATASIZE 200000
#define REPEAT 100

float A1D1[DATASIZE];

// compute a checksum on the data
unsigned int checksum(float *t, int size){
  unsigned int cs =0;
  int i;
  if (!t) return 0;
  for (i=0; i<size; i++){
    cs = cs + (unsigned int) t[i];
  }
  return cs;
}

// initialize the data structures
void initArrays(){
  int i,j,k;
  for (i=0;i< DATASIZE; i++){
    A1D1[i] = rand();
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

// 
int main(int argc, char** argv){
  double tstart, tend;
  int err;                            // for error code
  cl_device_id device_id;             // compute device id 
  cl_context context;                 // compute context
  cl_command_queue commands;          // compute command queue
  cl_program program;                 // compute program
  int gpu = 1;                        // select the GPU device (0 is CPU)    
  cl_mem buffer;                      // device memory used for the GPU data
  int i,j;
  double alpha;
    
    initArrays();
    printf("Initial A1D1 checksum %u\n", checksum(A1D1,DATASIZE));
    err = clGetDeviceIDs(NULL, gpu ? CL_DEVICE_TYPE_GPU : CL_DEVICE_TYPE_CPU, 1, &device_id, NULL);
    if (err == CL_SUCCESS)
      context = clCreateContext(0, 1, &device_id, NULL, NULL, &err);
    if (!context) err = !CL_SUCCESS;
    if (err == CL_SUCCESS)
      commands = clCreateCommandQueue(context, device_id, 0, &err);
    if (!commands) err =!CL_SUCCESS;

    if (err == CL_SUCCESS)
      buffer = clCreateBuffer(context,  CL_MEM_READ_WRITE,  sizeof(float) * DATASIZE, NULL, NULL);
    if (!buffer) err =!CL_SUCCESS;

    tstart = wallclock();
    for (j=0; j< REPEAT ; j++){
      // write on the GPU
      if (err == CL_SUCCESS)
	err = clEnqueueWriteBuffer(commands, buffer, CL_TRUE, 0, 
				   sizeof(float) * DATASIZE, A1D1, 0, NULL, NULL); 
      
      // to check copy really happened
      //for (i=0;i< DATASIZE; i++){
      //  A1D1[i] = 0;
      //}
      
      // read back the value
      if (err == CL_SUCCESS)
	err = clEnqueueReadBuffer( commands, buffer, CL_TRUE, 0, 
				   sizeof(float) * DATASIZE, A1D1, 0, NULL, NULL );  
    
    }
    clFinish(commands);
    tend = wallclock();
    alpha = (tend - tstart) / ((double)2*DATASIZE);
    printf("Executing during res %f sec.\n",tend - tstart);
    printf("BW is equal to %f MBs\n",((double) sizeof(float))*(1.0/alpha)/1e6);
    // display checksum
    printf("Resulting A1D1 checksum %u\n", checksum(A1D1,DATASIZE));
    if (err != CL_SUCCESS){
        printf("an error happened with OpenCL!\n");
        exit(1);
      }
    // release stuff
    clReleaseMemObject(buffer);
    clReleaseCommandQueue(commands);
    clReleaseContext(context);
 
    return 0;
}


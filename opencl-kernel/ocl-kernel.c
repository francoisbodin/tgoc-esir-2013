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


// must be a multiple of the workgroup size
#define DATASIZE 2000000
#define REPEAT 10

float A1D1[DATASIZE];
float A2D1[DATASIZE]; // control array

// put the code of the kernel in a string
const char *KernelSource = "\n" \
"__kernel void twice (             \n" \
"   __global float* array,         \n" \
"   const unsigned int count)      \n" \
"{                                 \n" \
"   int i = get_global_id(0);      \n" \
"   if(i < count)                  \n" \
"       array[i] = 2*array[i];   \n" \
"}                                 \n" \
  "\n";



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
    A2D1[i] = A1D1[i];
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
  int err;  
  double tstart, tend;
  cl_device_id device_id;             // compute device id 
  cl_context context;                 // compute context
  cl_command_queue commands;          // compute command queue
  cl_program program;                 // compute program
  cl_kernel kernel;                   // compute kernel
  int gpu = 1;                        // select the GPU device (0 is CPU)    
  cl_mem buffer;                      // device memory used for the GPU data
  int i,j;
  double alpha;
  unsigned int count = DATASIZE;
    
  // error control have been omitted for conciseness, OpenCL is quite verbose!
  initArrays();
  printf("Initial A1D1 checksum %u\n", checksum(A1D1,DATASIZE));
  printf("Initial A2D1 checksum %u\n", checksum(A2D1,DATASIZE));
  clGetDeviceIDs(NULL, gpu ? CL_DEVICE_TYPE_GPU : CL_DEVICE_TYPE_CPU, 1, &device_id, NULL);
  context = clCreateContext(0, 1, &device_id, NULL, NULL, &err);
  commands = clCreateCommandQueue(context, device_id, 0, &err);
  buffer = clCreateBuffer(context,  CL_MEM_READ_WRITE,  sizeof(float) * DATASIZE, NULL, NULL);
  if (!buffer){
      printf("Error: Failed to allocate device memory!\n");
      exit(1);
    } 
  // control computation (beware of rounding errors)
  for (j=0; j< REPEAT ; j++){
    for (i=0;i< DATASIZE; i++){
      A2D1[i] = 2*A2D1[i];
    }
  }
  // create the code for the GPU - error control have been omitted for conciseness
  program = clCreateProgramWithSource(context, 1, (const char **) & KernelSource, NULL, &err);
  err = clBuildProgram(program, 0, NULL, NULL, NULL, NULL);
  kernel = clCreateKernel(program, "twice", &err);
  if (!kernel || err != CL_SUCCESS){
    printf("Error: Failed to create compute kernel!\n");
    exit(1);
  }
  // start the computation on the GPU
  tstart = wallclock();
  for (j=0; j< REPEAT ; j++){
    size_t global;                      // global domain size for our calculation
    size_t local;                       // local domain size for our calculation
    // write on the GPU
    err = clEnqueueWriteBuffer(commands, buffer, CL_TRUE, 0, 
			       sizeof(float) * DATASIZE, A1D1, 0, NULL, NULL); 
    if (err != CL_SUCCESS){
        printf("Error: Failed to write to source array!\n");
        exit(1);
      } 
    // set kernel parameters
    err = clSetKernelArg(kernel, 0, sizeof(cl_mem), &buffer);
    if (err != CL_SUCCESS){
        printf("Error: Failed to set kernel argument 0! %d\n", err);
        exit(1);
    }
    err = clSetKernelArg(kernel, 1, sizeof(unsigned int), &count);
    if (err != CL_SUCCESS){
        printf("Error: Failed to set kernel argument 1! %d\n", err);
        exit(1);
    }
    err = clGetKernelWorkGroupInfo(kernel, device_id, CL_KERNEL_WORK_GROUP_SIZE, sizeof(local), &local, NULL);
    if (err != CL_SUCCESS){
      printf("Error: Failed to retrieve kernel work group info! %d\n", err);
      exit(1);
    } else {
      printf(" local work group size %d\n",(int) local);
    }
    global = count;
    //    err = clEnqueueNDRangeKernel(commands, kernel, 1, NULL, &global, &local, 0, NULL, NULL);
    err = clEnqueueNDRangeKernel(commands, kernel, 1, NULL, &global, NULL, 0, NULL, NULL);
    if (err){
      printf("Error: Failed to execute kernel!\n");
      return EXIT_FAILURE;
    }
    clFinish(commands);

    // read back the value
    err = clEnqueueReadBuffer( commands, buffer, CL_TRUE, 0, 
			       sizeof(float) * DATASIZE, A1D1, 0, NULL, NULL );  
    
  }
  tend = wallclock();
  printf("Executing during res %f sec.\n",tend - tstart);
  // make a diff
  for (i=0;i< DATASIZE; i++){
    if  (A2D1[i] != A1D1[i]){
      printf ("item %d A2D1 %f A1D1 %f\n",i,A2D1[i],A1D1[i]);
      break;
    }
  }  
  // display checksum
  printf("Resulting A1D1 checksum %u\n", checksum(A1D1,DATASIZE));
  printf("Control   A2D1 checksum %u\n", checksum(A2D1,DATASIZE));
  // release stuff
  clReleaseMemObject(buffer);
  clReleaseCommandQueue(commands);
  clReleaseContext(context);
  return 0;
}


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
#define NBCOEFF   4
#define REPEAT   10

float A1D1[DATASIZE];
float A2D1[DATASIZE];
float coeffArray[NBCOEFF];
float A3D1[DATASIZE]; // control array

// put the code of the kernel in a string
const char *KernelSource = "\n" \
"__kernel void filtre (             \n" \
"   __global float* array1,         \n" \
"   __global float* array2,         \n" \
"   __global float* coeffarray,    \n" \
"   const unsigned int count)      \n" \
"{                                 \n" \
"  __local float coeff[4];         \n" \
"   int i = get_global_id(0);      \n" \
"   int li = get_local_id(0);      \n" \
"  if (li == 0){                   \n" \
"    coeff[0] = coeffarray[0];     \n" \
"    coeff[1] = coeffarray[1];     \n" \
"    coeff[2] = coeffarray[2];     \n" \
"    coeff[3] = coeffarray[3];     \n" \
"  }                               \n" \
"  barrier(CLK_LOCAL_MEM_FENCE);   \n" \
"   if(i < count-4)                \n" \
"       array2[i] =  coeff[0]*array1[i+0]+\n" \
"                    coeff[1]*array1[i+1]+\n" \
"                    coeff[2]*array1[i+2]+\n" \
"                    coeff[3]*array1[i+3];\n" \
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
  coeffArray[0] = 0.5;
  coeffArray[1] = 0.75;
  coeffArray[2] = 1.0;
  coeffArray[3] = 1.25;
  for (i=0;i< DATASIZE; i++){
    A1D1[i] = rand();
    A2D1[i] = 0.0;
    A3D1[i] = 0.0;
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
  cl_mem buffer1;                     // device memory used for the GPU data
  cl_mem buffer2;                     // device memory used for the GPU data
  cl_mem coeffbuffer;                 // coeff buffer
  int i,j;
  double alpha;
  unsigned int count = DATASIZE;
    
  // error control have been omitted for conciseness, OpenCL is quite verbose!
  initArrays();
  printf("Initial A1D1 checksum %u\n", checksum(A1D1,DATASIZE));
  printf("Initial A2D1 checksum %u\n", checksum(A2D1,DATASIZE));
  printf("Initial A3D1 checksum %u\n", checksum(A3D1,DATASIZE));
  clGetDeviceIDs(NULL, gpu ? CL_DEVICE_TYPE_GPU : CL_DEVICE_TYPE_CPU, 1, &device_id, NULL);
  context = clCreateContext(0, 1, &device_id, NULL, NULL, &err);
  commands = clCreateCommandQueue(context, device_id, 0, &err);
  buffer1 = clCreateBuffer(context,  CL_MEM_READ_WRITE,  sizeof(float) * DATASIZE, NULL, NULL);
  buffer2 = clCreateBuffer(context,  CL_MEM_READ_WRITE,  sizeof(float) * DATASIZE, NULL, NULL);
  coeffbuffer = clCreateBuffer(context,  CL_MEM_READ_ONLY,  sizeof(float) * NBCOEFF, NULL, NULL);

  // control computation (beware of rounding errors)
  for (i=0;i< DATASIZE-NBCOEFF; i++){
      A3D1[i] = coeffArray[0]*A1D1[i]+coeffArray[1]*A1D1[i+1]+coeffArray[2]*A1D1[i+2]+coeffArray[3]*A1D1[i+3];
  }
  
  // create the code for the GPU - error control have been omitted for conciseness
  program = clCreateProgramWithSource(context, 1, (const char **) & KernelSource, NULL, &err);
  err = clBuildProgram(program, 0, NULL, NULL, NULL, NULL);
  if (err != CL_SUCCESS)
    {
      size_t len;
      char buffer[2000]; 
      printf("Error: Failed to build program executable!\n");
      clGetProgramBuildInfo(program, device_id, CL_PROGRAM_BUILD_LOG, sizeof(buffer), buffer, &len);
      printf("%s\n", buffer);
      exit(1);
    }

  kernel = clCreateKernel(program, "filtre", &err);
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
    err = clEnqueueWriteBuffer(commands, buffer1, CL_TRUE, 0, 
			       sizeof(float) * DATASIZE, A1D1, 0, NULL, NULL); 
    err = clEnqueueWriteBuffer(commands, coeffbuffer, CL_TRUE, 0, 
			       sizeof(float) * NBCOEFF, coeffArray, 0, NULL, NULL); 

    // set kernel parameters
    err = clSetKernelArg(kernel, 0, sizeof(cl_mem), &buffer1);
    err = clSetKernelArg(kernel, 1, sizeof(cl_mem), &buffer2);
    err = clSetKernelArg(kernel, 2, sizeof(cl_mem), &coeffbuffer);
    err = clSetKernelArg(kernel, 3, sizeof(unsigned int), &count);
    global = count;
    err = clEnqueueNDRangeKernel(commands, kernel, 1, NULL, &global, NULL, 0, NULL, NULL);
    clFinish(commands);
    // read back the value
    err = clEnqueueReadBuffer( commands, buffer2, CL_TRUE, 0, 
			       sizeof(float) * DATASIZE, A2D1, 0, NULL, NULL );  
  }
  tend = wallclock();
  printf("Executing during res %f sec.\n",tend - tstart);
  // make a diff
  for (i=0;i< DATASIZE; i++){
    if  (A2D1[i] != A3D1[i]){
      printf ("item %d A3D1 %f A2D1 %f\n",i,A3D1[i],A2D1[i]);
      break;
    }
  }  
  // display checksum
  printf("Resulting A2D1 checksum %u\n", checksum(A2D1,DATASIZE));
  printf("Control   A3D1 checksum %u\n", checksum(A3D1,DATASIZE));
  // release stuff
  clReleaseMemObject(buffer1);
  clReleaseMemObject(buffer2);
  clReleaseMemObject(coeffbuffer);
  clReleaseCommandQueue(commands);
  clReleaseContext(context);
  return 0;
}


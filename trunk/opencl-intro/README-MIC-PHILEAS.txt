
Specific things to do for running on the Xeon Phi (MIC)

source /opt/intel/composer_xe_2013.2.146/bin/compilervars.sh intel64

export  LD_LIBRARY_PATH=/opt/intel/opencl-1.2-3.1.1.11385/lib64:$LD_LIBRARY_PATH

in the ocl-intro.c (and ocl-info.c)

#include <OpenCL/opencl.h> --> #include <CL/opencl.h>

CL_DEVICE_TYPE_GPU ---> CL_DEVICE_TYPE_ACCELERATOR
To compile the command is 

gcc ocl-intro.c  -L/opt/intel/opencl/lib64 -lOpenCL -I/opt/intel/opencl/include



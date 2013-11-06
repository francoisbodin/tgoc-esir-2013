
all: ocl-kernel.exe

ocl-kernel.exe: ocl-kernel.c
	gcc ocl-kernel.c -o ocl-kernel.exe -cl-std=CL1.1 -framework OpenCL

run: ocl-kernel.exe
	./ocl-kernel.exe

clean:
	rm *.exe *~


all: ocl-mem.exe

ocl-mem.exe: ocl-mem.c
	gcc ocl-mem.c -o ocl-mem.exe -cl-std=CL1.1 -framework OpenCL

run: ocl-mem.exe
	./ocl-mem.exe

clean:
	rm *.exe *~


all: ocl-intro.exe ocl-info.exe

ocl-intro.exe: ocl-intro.c
	gcc ocl-intro.c -o ocl-intro.exe -cl-std=CL1.1 -framework OpenCL

ocl-info.exe: ocl-info.c
	gcc -w ocl-info.c -o ocl-info.exe -cl-std=CL1.1 -framework OpenCL

run: ocl-intro.exe
	./ocl-intro.exe

clean:
	rm *.exe *~

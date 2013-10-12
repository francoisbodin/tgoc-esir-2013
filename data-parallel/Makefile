

all: data-parallel-step3.exe data-parallel-step2.exe data-parallel-step1.exe data-parallel-init.exe

data-parallel-step3.exe: data-parallel-step3.c
	mpicc data-parallel-step3.c -o data-parallel-step3.exe

data-parallel-step2.exe: data-parallel-step2.c
	mpicc data-parallel-step2.c -o data-parallel-step2.exe

data-parallel-step1.exe: data-parallel-step1.c
	mpicc data-parallel-step1.c -o data-parallel-step1.exe

data-parallel-init.exe: data-parallel-init.c
	gcc data-parallel-init.c -o data-parallel-init.exe

run: all
	./data-parallel-init.exe
#	mpirun -np 2 data-parallel-step1.exe	
#	mpirun -np 2 data-parallel-step2.exe	
	mpirun -np 2 data-parallel-step3.exe	
clean:
	rm *.exe *~

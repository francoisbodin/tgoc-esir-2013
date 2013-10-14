

all: transpose.exe 

transpose.exe: transpose.c
	mpicc transpose.c -o transpose.exe

run: all
	mpirun -np 2 transpose.exe	
clean:
	rm *.exe *~



all: transpose.exe 

transpose.exe: transpose.c
	mpicc transpose.c -o transpose.exe

run: all
	mpirun -np 4 transpose.exe	
clean:
	rm *.exe *~

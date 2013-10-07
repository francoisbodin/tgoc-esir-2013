

messages.exe: messages.c
	mpicc messages.c -o messages.exe

run: messages.exe
	mpirun -np 2 messages.exe	
clean:
	rm messages.exe *~

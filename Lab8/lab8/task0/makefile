all: myshell

myshell: myELF.o
	gcc -g -m32 -Wall -o task0 myELF.o

myELF.o: myELF.c myELF.o
	gcc -m32 -g -Wall -ansi -c -o myELF.o myELF.c 

.PHONY: clean

clean: 
	rm -f *.o task0

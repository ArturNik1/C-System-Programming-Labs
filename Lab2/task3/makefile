all: magic

magic: menu_map.o
	gcc -m32 -g -Wall -o menu_map menu_map.o

menu_map.o: menu_map.c
	gcc -m32 -g -Wall -ansi -c -nostdlib -fno-stack-protector menu_map.c -o menu_map.o

.PHONY: clean
clean:
	rm -f *.o menu_map

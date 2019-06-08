all: bin/main

bin/main: bin/main.o
	mkdir -p bin
	ld -o bin/main bin/main.o

bin/main.o: main.asm
	mkdir -p bin
	nasm -f elf64 -g -F dwarf main.asm -o bin/main.o

clean:
	rm -rf bin

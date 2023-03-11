all: vmm

vmm: loader.o main.o
	gcc -o vmm loader.o main.o

loader.o: loader.c vm.h
	gcc -o loader.o -c loader.c

main.o: main.c vm.h
	gcc -o main.o -c main.c

clean:
	rm -f *.o vmm
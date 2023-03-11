all: loader

loader: loader.c
	gcc -o loader loader.c
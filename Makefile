CC := gcc
EXE = vmm
C_FILES = $(wildcard *.c)
OBJ_FILES = $(C_FILES: %.c=%.o)

all:$(OBJ_FILES)
	$(CC) -o $(EXE) $(OBJ_FILES)  

%.o:%.c
	$(CC) -c $^ -o $@

.PHONY:clean
clean:
	rm -rf *.o $(EXE)
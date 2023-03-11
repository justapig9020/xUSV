CC = gcc
LD = ld
LDFILE = script.ld
OBJCOPY = objcopy

SFILE = kernel.s

all: kernel.img

# Step 1: gcc call as let kernel.S assemble to kernel.o
kernel.o: $(SFILE)
	$(CC) -c $(SFILE) -o kernel.o

# Step 2: ld call linker script script.ld let kernel.o link to executable file kernel.elf
kernel.elf: kernel.o
	$(LD) kernel.o -o kernel.elf -T $(LDFILE)

# Step 3: objcopy remove unused sections of kernel.elf (.pdr, .comment, .note),
#		 Strop all symbol messege, and output binary file kernel.img
kernel.img: kernel.elf
	@$(OBJCOPY) -R .pdr -R .comment -R .note -S -O binary kernel.elf kernel.img

clean:
	@rm -rf kernel.o kernel.elf kernel.img

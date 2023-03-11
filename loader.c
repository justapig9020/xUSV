#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdint.h>

#include "loader.h"

#define X86_BOOT_ADDRESS 0x17C00

static void *load_vm(char *img_name, uintptr_t load_addr) {
    int fd;
    struct stat sb;
    void *alloc;
    void *program = NULL;

    fd = open(img_name, O_RDONLY);
    if (fd == -1) {
        perror("open");
        goto err;
    }

    if (fstat(fd, &sb) == -1) {
        perror("fstat");
        goto err;
    }

    size_t page_size = getpagesize();
    void *text_base = (void*)(load_addr & ~(page_size - 1));

    alloc = mmap(
        (void*)(load_addr & ~(page_size - 1)),
        sb.st_size,
        PROT_READ | PROT_WRITE | PROT_EXEC,
        MAP_PRIVATE | MAP_FIXED | MAP_ANONYMOUS,
        -1,
        0
    );

    if (alloc == MAP_FAILED) {
        perror("mmap");
        goto err;
    }

    if (read(fd, (void*)load_addr, sb.st_size) == -1) {
        perror("read");
        goto unmap;
    }

    if (close(fd) == -1) {
        perror("close");
        goto unmap;
    }

   return alloc;
unmap:
    if (munmap(alloc, sb.st_size) == -1) {
        perror("munmap");
        exit(EXIT_FAILURE);
    }

err:
    return NULL;
}

bool init_vm(struct VM *vm, char *img_name) {
    void *text = load_vm(img_name, X86_BOOT_ADDRESS);
    if (!text)
        return false;
    vm->text = text;
    vm->entry = (void*)X86_BOOT_ADDRESS;
    return true;
}
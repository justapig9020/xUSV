#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

#define BOOT_ADDRESS 0x17c00

int main() {
    int fd;
    struct stat sb;
    void *text;

    // 開啟 kernel.img
    fd = open("kernel.img", O_RDONLY);
    if (fd == -1) {
        perror("open");
        exit(EXIT_FAILURE);
    }

    // 取得 kernel.img 的檔案屬性
    if (fstat(fd, &sb) == -1) {
        perror("fstat");
        exit(EXIT_FAILURE);
    }

    size_t page_size = getpagesize();
    void *text_address = (void*)(BOOT_ADDRESS & ~(page_size - 1));
    printf("text: %p\n", text_address);
    text = mmap((void*)(BOOT_ADDRESS & ~(page_size - 1)), sb.st_size, PROT_READ | PROT_WRITE | PROT_EXEC, MAP_PRIVATE | MAP_FIXED | MAP_ANONYMOUS, -1, 0);
    printf("mmap: %p\n", text);
    if (text == MAP_FAILED) {
        perror("mmap");
        exit(EXIT_FAILURE);
    }

    if (read(fd, (void*)BOOT_ADDRESS, sb.st_size) == -1) {
        perror("read");
        exit(EXIT_FAILURE);
    }

    // 關閉檔案
    if (close(fd) == -1) {
        perror("close");
        exit(EXIT_FAILURE);
    }

    // 執行映射區段的內容
    printf("Enter\n");
    int (*entry)() = (int (*)())BOOT_ADDRESS;
    int result = entry();
    printf("Done\n");

    // 解除映射
    if (munmap(text, sb.st_size) == -1) {
        perror("munmap");
        exit(EXIT_FAILURE);
    }

    return result;
}
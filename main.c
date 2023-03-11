#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <stdlib.h>
#include <ucontext.h>
#include "loader.h"

#define KERNEL "kernel.img"

void sig_handler(int signo);

int main(void) {
    printf("main is waiting for a signal\n");
    if (signal(SIGSEGV, sig_handler) == SIG_ERR) {
        perror("signal error");
        exit(EXIT_FAILURE);
    }
    printf("signal!\n");

    // Init vm
    struct VM vm;
    if (!init_vm(&vm, KERNEL))
        return EXIT_FAILURE;

    // start VM
    vm.entry();
    for(;;)
    return EXIT_SUCCESS;
}

void sig_handler(int signo) {
    printf("catch the signal SIGSEGV %d\n", signo);
}

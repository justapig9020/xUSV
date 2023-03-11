#define _GNU_SOURCE
#include <stdio.h>
#include <unistd.h>
#include <stdint.h>
#include <signal.h>
#include <stdlib.h>
#include <ucontext.h>
#include <stdbool.h>
#include "loader.h"

#define KERNEL "kernel.img"

void handler(int signum, siginfo_t *info, void *context) {
    ucontext_t *ctx = (ucontext_t *) context;
    uintptr_t rip = ctx->uc_mcontext.gregs[REG_RIP];
    printf("CPU context at RIP = %p\n", (void*) rip);
    printf("Inst: ");
    for (int i=0; i<10; i++) {
        printf("%x", ((int*)rip)[i]);
    }
    puts("");
    ctx->uc_mcontext.gregs[REG_RIP] = rip + 2;
    printf("Returning to RIP = %llx\n", ctx->uc_mcontext.gregs[REG_RIP]);
}

int register_handlers() {
    struct sigaction sa;
    sa.sa_sigaction = handler;
    sa.sa_flags = SA_SIGINFO;
    return sigaction(SIGSEGV, &sa, NULL);
}

int main(void) {
    printf("Register hadler");
    if (register_handlers() == -1) {
        perror("signal error");
        exit(EXIT_FAILURE);
    }
    printf("[Done]\n");

    // Init vm
    printf("Init VM");
    struct VM vm;
    if (!init_vm(&vm, KERNEL))
        return EXIT_FAILURE;
    printf("[Done]\n");

    // start VM
    printf("Start VM");
    vm.entry();
    for(;;)
    return EXIT_SUCCESS;
}


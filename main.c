#define _GNU_SOURCE
#include <signal.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <ucontext.h>
#include <unistd.h>

#include "inst_parser.h"
#include "loader.h"

void handler(int signum, siginfo_t *info, void *context) {
    ucontext_t *ctx = (ucontext_t *)context;
    uintptr_t rip = ctx->uc_mcontext.gregs[REG_RIP];

    struct Inst inst;
    if (!parse_inst((void *)rip, &inst)) {
        puts("Instruction parse failed");
        exit(EXIT_FAILURE);
    }

    inst.emulator(ctx);

    ctx->uc_mcontext.gregs[REG_RIP] = rip + inst.len;
}

int register_handlers() {
    struct sigaction sa;
    sa.sa_sigaction = handler;
    sa.sa_flags = SA_SIGINFO;
    return sigaction(SIGSEGV, &sa, NULL);
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: ./vmm <image>\n");
        return EXIT_FAILURE;
    }
    char *image = argv[1];
    printf("Register hadler");
    if (register_handlers() == -1) {
        perror("signal error");
        exit(EXIT_FAILURE);
    }
    puts("[Done]");

    // Init vm
    printf("Init VM");
    struct VM vm;
    if (!init_vm(&vm, image))
        return EXIT_FAILURE;
    puts("[Done]");

    // start VM
    puts("Start VM");
    vm.entry();
    return EXIT_SUCCESS;
}

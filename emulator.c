#define _GNU_SOURCE
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <ucontext.h>
#include <unistd.h>

static bool int_10_write_string(ucontext_t *ctx) {
    /*
     * Work around: Since we are not able to load kernel into correct address,
     * we have to add the offset(0x10000) manually
     * TODO: Remove the work around
     */
    const uintptr_t work_around_offset = 0x10000;
    uintptr_t bp =
        (ctx->uc_mcontext.gregs[REG_RBP] & 0xFFFF) + work_around_offset;
    size_t cx = ctx->uc_mcontext.gregs[REG_RCX] & 0xFFFF;

    write(STDOUT_FILENO, (void *)bp, cx);
    return true;
}

static bool int_10_emulation(ucontext_t *ctx) {
    uint64_t rax = ctx->uc_mcontext.gregs[REG_RAX];
    uint8_t ah = (rax >> 8) & 0xff;
    bool success = false;

    switch (ah) {
    case 0x13:
        success = int_10_write_string(ctx);
        break;
    }
    return success;
}

bool int_imm8_emulation(ucontext_t *ctx) {
    void *rip = (void *)ctx->uc_mcontext.gregs[REG_RIP];
    uint8_t int_num = ((char *)rip)[1];
    bool success = false;

    switch (int_num) {
    case 0x10:
        success = int_10_emulation(ctx);
        break;
    }
    return success;
}
#pragma once

#include <stdbool.h>
#include <stdint.h>
#include <ucontext.h>

typedef bool (*emulator_t)(ucontext_t *ctx);

struct Inst {
    char *name;
    size_t len;
    emulator_t emulator;
};

bool parse_inst(void *rip, struct Inst *inst);

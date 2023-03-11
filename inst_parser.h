#pragma once

#include <ucontext.h>
#include <stdbool.h>
#include <stdint.h>

typedef bool(*emulator_t)(ucontext_t* ctx);

struct Inst {
  char *name;
  size_t  len;
  emulator_t emulator;
};

bool parse_inst(void *rip, struct Inst *inst);

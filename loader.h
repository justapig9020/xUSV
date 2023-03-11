#pragma once

#include "stdbool.h"
#include "vm.h"

// Initialize struct VM with given kernal image
bool init_vm(struct VM *vm, char *img_name);

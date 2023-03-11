#pragma once

typedef void (*entry_t)();

struct VM {
    void *text;
    entry_t entry;
};
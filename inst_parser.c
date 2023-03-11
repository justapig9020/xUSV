#include "inst_parser.h"

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

#include "emulator.h"

typedef void *(*parser_t)(uint8_t opcode, struct Inst *inst);

static void *first_byte_parser(uint8_t opcode, struct Inst *inst) {
    parser_t next = NULL;
    switch (opcode) {
    case 0xCD: // int
        inst->name = "int";
        inst->len = 2;
        inst->emulator = int_imm8_emulation;
        break;
    case 0xF4: // hlt
        inst->name = "hlt";
        inst->len = 1;
        inst->emulator = halt_emulation;
        break;
    default:
        next = (void *)-1;
    }
    return next;
}

bool parse_inst(void *rip, struct Inst *inst) {
    char *opcode = (char *)rip;
    parser_t parse = first_byte_parser;
    while (parse) {
        parse = (parser_t)parse(*opcode, inst);
        opcode += 1;
        if ((intptr_t)parse == -1)
            return false;
    }
    return true;
}

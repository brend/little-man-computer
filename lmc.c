#include "lmc.h"
#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>

INSTRUCTION opcode(int i, int *operand) {
    *operand = 0;

    switch (i) {
    case 901:
        return OP_INP;
    case 902:
        return OP_OUT;
    case 0:
        return OP_HLT;
    default:
        *operand = i % 100;

        return i / 100;
    }
}

void write_outbox(int i) {
    printf("%i\n", i);
}

int read_inbox() {
    int i = 0;

    printf("Enter a number: ");
    scanf("%d", &i);
    printf("\n");

    if (i < 0 || i > 99) {
        printf("*** OUT OF RANGE\n");
        exit(ERR_USER_INPUT_OUT_OF_RANGE);
    }

    return i;
}

void run_program(LMC lmc) {
    while (1) {
        int i = lmc.mailbox[lmc.pc];
        int operand = 0;
        INSTRUCTION op = opcode(i, &operand);

        ++lmc.pc;

        switch (op) {
        case OP_ADD:
            lmc.accumulator += lmc.mailbox[operand];

            if (lmc.accumulator >= 100) {
                lmc.accumulator = 0;
                lmc.zeroFlag = 1;
            }
            break;
        case OP_SUB:
            lmc.accumulator -= lmc.mailbox[operand];

            if (lmc.accumulator < 0) {
                lmc.accumulator = 0;
                lmc.zeroFlag = 1;
            }
            break;
        case OP_STA:
            lmc.mailbox[operand] = lmc.accumulator;
            break;
        case OP_LDA:
            lmc.accumulator = lmc.mailbox[operand];
            break;
        case OP_BRA:
            lmc.pc = operand;
            break;
        case OP_BRZ:
            if (lmc.accumulator && !lmc.zeroFlag) {
                lmc.pc = operand;
            }
            break;
        case OP_BRP:
            if (lmc.zeroFlag) {
                lmc.pc = operand;
            }
            break;
        case OP_INP:
            lmc.accumulator = read_inbox();
            break;
        case OP_OUT:
            write_outbox(lmc.accumulator);
            break;
        case OP_HLT:
            printf("*** HLT\n");
            return;
        case OP_DAT:
            break;
        default:
            fprintf(stderr, "*** UNKNOWN INSTRUCTION: %i\n", i);
            return;
        }
    }
}

LMC enter_program(int count, ...) {
    LMC lmc = {0};
    va_list valist;
    
    va_start(valist, count);

    for (int i = 0; i < count; ++i) {
        lmc.mailbox[i] = va_arg(valist, int);
    }
	
    va_end(valist);

    return lmc;
}
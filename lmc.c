#include "lmc.h"
#include <stdio.h>
#include <stdarg.h>

INSTRUCTION opcode(int i, int *operand) {
    *operand = 0;

    switch (i) {
    case 901:
        return INP;
    case 902:
        return OUT;
    case 0:
        return HLT;
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

    return i;
}

void run_program(LMC lmc) {
    while (1) {
        int i = lmc.mailbox[lmc.pc];
        int operand = 0;
        INSTRUCTION op = opcode(i, &operand);

        ++lmc.pc;

        switch (op) {
        case ADD:
            lmc.accumulator += lmc.mailbox[operand];
            break;
        case SUB:
            lmc.accumulator -= lmc.mailbox[operand];
            break;
        case STA:
            lmc.mailbox[operand] = lmc.accumulator;
            break;
        case LDA:
            lmc.accumulator = lmc.mailbox[operand];
            break;
        case BRA:
            lmc.pc = operand;
            break;
        case BRZ:
            if (lmc.accumulator && !lmc.zeroFlag) {
                lmc.pc = operand;
            }
            break;
        case BRP:
            if (lmc.zeroFlag) {
                lmc.pc = operand;
            }
            break;
        case INP:
            lmc.accumulator = read_inbox();
            break;
        case OUT:
            write_outbox(lmc.accumulator);
            break;
        case HLT:
            printf("*** HLT\n");
            return;
        case DAT:
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
#ifndef __LMC_H__
#define __LMC_H__

typedef struct {
    int mailbox[100];
    int pc;
    int overflow;
    int accumulator;
    int zeroFlag;
} LMC;

typedef enum {
    ADD = 1,
    SUB = 2,
    STA = 3,
    LDA = 5,
    BRA = 6,
    BRZ = 7,
    BRP = 8,
    INP = 901,
    OUT = 902,
    HLT = 0,
    DAT = 1024
} INSTRUCTION;

typedef enum {
    ERR_USER_INPUT_OUT_OF_RANGE = -2,
    ERR_CONSTANT_OUT_OF_RANGE = -3
} ERROR;

LMC enter_program(int mailbox0, ...);

void run_program(LMC lmc);

#endif
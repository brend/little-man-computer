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
    OP_ADD = 1,
    OP_SUB = 2,
    OP_STA = 3,
    OP_LDA = 5,
    OP_BRA = 6,
    OP_BRZ = 7,
    OP_BRP = 8,
    OP_INP = 901,
    OP_OUT = 902,
    OP_HLT = 0,
    OP_DAT = 1024
} INSTRUCTION;

typedef enum {
    ERR_USER_INPUT_OUT_OF_RANGE     = 2,
    ERR_CONSTANT_OUT_OF_RANGE       = 3,
    ERR_OPEN_FILE                   = 4,
    ERR_PROGRAM_TOO_LONG            = 5,
    ERR_TOO_MANY_TOKENS             = 6,
    ERR_UNKNOWN_INSTRUCTION         = 7,
} ERROR;

LMC enter_program(int mailbox0, ...);

void run_program(LMC lmc);

#endif
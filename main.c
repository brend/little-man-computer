#include <stdio.h>

#include "lmc.h"
#include "parser.h"

void print_program(LMC lmc) {
    for (int i = 0; i < 100; ++i) {
        printf("[%d] = %3d\n", i, lmc.mailbox[i]);
    }
}

int main(int argc, char **argv) {
    if (argc < 2) {
        printf("usage: lmc <filename.lmc>\n");
        return 0;
    }

    LMC lmc = parse(argv[1]);

    //print_program(lmc);

    run_program(lmc);
    
    return 0;
}
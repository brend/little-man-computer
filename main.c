#include <stdio.h>

#include "lmc.h"
#include "parser.h"

int main(int argc, char **argv) {
    if (argc < 2) {
        printf("usage: lmc <filename.lmc>\n");
        return 0;
    }

    LMC lmc = parse(argv[1]);

    run_program(lmc);
    
    return 0;
}
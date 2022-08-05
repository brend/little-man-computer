#include <stdio.h>

#include "lmc.h"

int main() {
    LMC lmc = enter_program(
        8,
        901,
        308,
        901,
        309,
        508,
        209,
        902,
        000
    );

    run_program(lmc);
    
    return 0;
}
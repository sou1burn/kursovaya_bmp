#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include "kurs.h"


int main(int argc, char** argv) {
    if (argc != 4) {
        fprintf(stderr, "Usage of %s caused error\n", argv[0]);
        return 1;
    }

    resizeBMP(argv[1], argv[2]);
    downsizeBMP(argv[1], argv[3]);

    return 0;
}
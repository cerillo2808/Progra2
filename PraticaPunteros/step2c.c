#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

const size_t BUFFER_SIZE = 100;

void main() {
    uintptr_t x = 0;
    size_t total = 0;
    // Ajuste el número abajo para que el programa pida 1GB de memoria adicional
    // a la memoria física de la máquina.
    for (int i = 0; i < 1000; i++) {
        printf("%zu %d |", total, i);
        x ^= (uintptr_t)malloc(BUFFER_SIZE);
    }
    printf("\n%zu", x);
}
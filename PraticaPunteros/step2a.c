#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void main() {
    char* x = strdup("practica punteros");
    int acc = 0;
    for(int i = 0; i < 50000000; i++) {
        printf("%d: %p -> %c", i, &x[i], x[i]);
        acc ^= x[i];
    }
    printf("%d\n", acc);
}
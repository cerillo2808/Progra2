#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

struct user {
    char name[10];
    bool is_admin;
};

void main() {
    struct user s1;
    s1.is_admin = false;

    printf("Escriba su nombre y presione enter:");
    scanf("%9s", s1.name);

    if (s1.is_admin) {
        printf("Hola %s, tienes acceso al sistema!\n", s1.name);
    } else {
        printf("Hola %s, no eres administrador\n", s1.name);
    }
}
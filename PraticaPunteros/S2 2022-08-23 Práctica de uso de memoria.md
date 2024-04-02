Universidad de Costa Rica
Escuela de Ciencias de la Computación e Informática
Programación 2 - CI0113
Prof. Esteban Rodríguez Betancourt

# Práctica 1
Nombre: Liqing Yosery Zheng Lu
Carnet: C38680

En esta práctica se verá el manejo de memoria en C y C++, además de algunos
errores comunes. También se utilizarán herramientas como Valgrind para detectar
algunos errores en el uso de la memoria y `strace` para ver las llamadas al sistema. Para esta práctica es necesario el uso de Linux y GCC.

Al final suban a mediación este documento con sus respuestas (pongan R/ cómo prefijo de sus respuestas). adjunten también los archivos '*.c' creados.


# Step 1: malloc y free
En C la memoria se pide generalmente con las funciones `malloc` y `free`. Sin
embargo, no son las únicas funciones disponibles para ello. `calloc` se puede
usar para pedir memoria inicializada en cero, `strdup` puede duplicar strings
(secuencias de caracteres terminadas con un caracter nulo), etc.

Para iniciar vamos a hacer un programa simple que pide memoria:

```c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct student {
    char name[10];
    int age;
    int score;
};

void print_student(struct student s) {
    printf("name: %s, age: %d, score: %d\n", s.name, s.age, s.score);
}

void main() {
    struct student s1;
    s1.age = 20;
    s1.score = 100;
    strcpy(s1.name, "Maria");

    print_student(s1);
}
```

Compile el programa usando `gcc step1.c -o step1` y ejecútelo. ¿Qué imprime? R/ La información de Maria.

## Respondan

1. ¿En dónde se inicializa la variable `s1`? ¿Stack o heap? R/ En el stack.

2. ¿Cómo creen que se pasa la variable `s1` a la función `print_student`? ¿Por
valor o por referencia/puntero? R/ Por valor.

3. ¿De qué tamaño creen que es la variable `s1` en bytes? R/ 1 byte, 8 bits.

En la función `main` añadan la siguiente línea, para averiguar el tamaño de
la variable `s1`:

```c
    printf("Tamaño de s1: %zu\n", sizeof(s1));
```

Compilen el archivo y córranlo. ¿Acertaron el tamaño del `struct student`?
¿Es más o menos de lo que estimaron? R/ No, el tamaño fue 20.

Es muy posible que estimaran mal el tamaño del struct. ¿Qué tal si averiguamos
el tamaño de otros tipos de datos en C? Añadan las siguientes líneas, compilen
el programa, córranlo y reporten los resultados.

```c
    printf("Tamaño de int: %zu\n", sizeof(int));
    printf("Tamaño de char[10]: %zu\n", sizeof(char[10]));
```

Reporten: ¿Es `sizeof(struct student) == sizeof(int) + sizeof(int) + sizeof(char[10])`?
¿Cuanta es la diferencia? R/ No. s1 es de 20, int es de 4, y char fue de 10. 10+4+4=18, que es diferente a 20.

Añadan otro array de tipo `char` y tamaño 1 al `struct student`. Compilen y
corra el programa. ¿El tamaño del struct cambió o siguió igual? Sigan incrementando
el tamaño de dicho array hasta que tenga tamaño 5. ¿Qué cambios observaron? R/ Después de agregar el array, el tamaño del struct pasó de 20 a 24. Siguió siendo del mismo tamaño, incluso cuando el array era de 4. Sin embargo, en el array de tamaño 5, el struct empezó a ser de tamaño 28.

Dependiendo de su compilador, sistema operativo y opciones de compilación es
posible que obtengan resultados diferentes. Esto es normal, ¿pero porqué?

Aunque la memoria es byte-direccionable, el procesador en realidad no pide los
datos a la memoria byte por byte, sino que pide más (imaginen ir de compras al
supermercado por una cosa y volver a la casa, luego ir por otra y volver. No: uno
va una vez y compra todo lo que puede). Históricamente, la memoria en realidad
se pedía por words (_word_ es el tamaño de los datos nativos de la arquitectura):
ejm 16 bits o 32 bits. Por lo tanto, para el procesador es más eficiente pedir
memoria que esté alineada con un _word_. Por este motivo, algunos compiladores
agregan _padding_ entre los elementos de un struct: para que estén alineados. Incluso
a veces pueden reordenar los miembros del struct.

Si desean aprender más de este tema pueden ver los siguientes artículos:
- https://en.wikipedia.org/wiki/Data_structure_alignment
- https://en.wikipedia.org/wiki/Word_(computer_architecture)

Eliminen ese array extra que agregaron y guarden el archivo `step1.c`. Se va a usar luego.

## Step 1a: punteros
Pasar parámetros por valor tiene sus ventajas: no se modifica el original. Sin
embargo, para objetos muy grandes esto es problemático, ya que se deben hacer
copias.

En general, uno quiere pasar los parámetros por puntero/referencia cuando:

- Desea poder modificar la variable original
- El objeto es muy grande y desea evitar hacer copias, ya que esto es costoso (lento)
- Desea reducir el uso de memoria teniendo una única instancia

Hacer copias no necesariamente es algo costoso: los punteros en un
sistema de 64 bits miden 8 bytes. Los procesadores actuales pueden copiar hasta
64 bytes tan rápido como pueden copiar un puntero.

Las ventajas de pasar los parámetros por valor pueden ser las siguientes:

- No se modifica el valor original
- Posiblemente se facilita la gestión de la memoria

En este caso deseamos añadir punteros... porque es una práctica de punteros.

Copien el archivo `step1.c`a `step1a.c` (los vamos a usar luego).

Para pedir memoria dinámica vamos a usar `malloc`. Para esto debemos modificar
la declaración de `s1`, para que se lea así:

```c
    struct student* s1 = malloc(sizeof(struct student));
```

Intenten compilar el programa. ¿Qué error les da? R/expected ‘struct student’ but argument is of type ‘struct student *’

El motivo del error es que están intentando acceder a un miembro de un puntero.
Los punteros son direcciones de memoria, no tienen miembros. Para resolver el
problema debemos desreferenciar el puntero. Es decir, en lugar de hacer
`(puntero).xyz` queremos hacer `(struct).xyz`. Para desreferenciar un puntero
usamos el operador `*`. Para no confundir con la multiplicación es una buena
práctica usar paréntesis. Modifiquen el código para que se lea así:

```c
    struct student* s1 = malloc(sizeof(struct student));
    (*s1).age = 20;
    (*s1).score = 100;
    strcpy((*s1).name, "Maria");
    printf("Tamaño de s1: %zu\n", sizeof(s1));
    print_student(*s1);
```

Si `s1` ahora es un puntero entonces podemos obtener "leer" el struct al que apunta
usando el operador de desreferencia `*`. Una vez que ya estamos en el struct podemos
acceder a sus miembros, usando el punto "`.`".

Compilen de nuevo el programa y córranlo. Reporten:
- ¿Qué tamaño tiene ahora s1? ¿Qué tamaño tenía en `step1`? R/ Ahora tiene 8, antes era de 20.
- ¿s1 se está pasando a print_student por valor o por puntero? R/ Por puntero.

Averiguen el tamaño de `*s1`. Añadan `printf("Tamaño de *s1: %zu\n", sizeof(*s1));`,
compilen y ejecuten el programa. ¿Qué tamaño tiene `*s1`? R/ 20

Modifiquen `print_student` para que modifique la edad después de imprimir los datos.
Llamen a `print_student` otra vez. ¿Se imprimió lo mismo o algo diferente? ¿Porqué? R/ Imprimió lo mismo, porque lo que cambió fue la copia, no la variable original.

## Step 1b: paso de parámetros por puntero
Ya vimos que podemos pasar parámetros por valor. Ahora queremos pasar los parámetros
por puntero. Para esto vamos a copiar el archivo `step1a.c` a `step1b.c`.

En primer lugar, es muy molesto estar escribiendo `(*s1).xyz` cada vez que querramos
acceder a un miembro de `s1`. Por ejemplo, en otros lenguajes (como Java) uno
simplemente usa un punto. En C y C++ tenemos una opción más corta y es usar el operador
"arrow" (`->`). Modifiquen `step1b.c` para que se use el operador `->` en lugar
del de desreferencia. Compilen y corran el programa para validar que sigue funcionando.

Ahora, modifiquen la función `print_student` para que reciba un `struct student*`
en lugar de un `struct student`.

(El párrafo anterior se lee "... un puntero a struct student en lugar de un struct student")

Para ello modifiquen la firma de la función, de forma que reciba un puntero a un struct
student. Compilen el programa y anoten los mensajes de error. Corrijan los errores que indica el
compilador y logren que el programa funcione correctamente. R/ expression must have struct or union type but it has type "struct student *", argument of type "struct student" is incompatible with parameter of type "struct student *"

¿Recuerdan que se modificó `print_student` para que cambiara la edad? ¿La segunda vez que se llama
a `print_student` muestra los mismos datos que la primera vez? ¿Porqué? R/ No se mostraron los datos de la primera vez, se modificaron porque el print_student tuvo acceso a la dirección de la memoria original.

A veces uno desea modificar los valores pasados como parámetros. A veces uno no desea poder hacerlo,
pero requiere las ventajas de rendimiento de pasar los datos por puntero. ¿Cómo se puede resolver?
En C y C++ es posible declarar algo como constante. Cambien la firma de `print_student` para que sea
`void print_student(const struct student* s)`. Compilen y anoten los errores. Luego eliminen la
instrucción problemática y vuelvan a ejecutar el programa. R/ assignment of member ‘age’ in read-only object

El uso de const es muy recomendado para APIs donde se desea garantizar que no se va a modificar
la variable. También es posible devolver tipos const, para que el usuario no pueda modificarlos.
Por supuesto, esto es algo para evitar errores simples: alguien podría simplemente hacer un
casting del tipo (convertirlo) y con eso se quita la protección provista por el const. Por ejemplo,
añadan a `print_student` la línea `((struct student*)s)->age = 5;` y corran el programa de nuevo.
¿El compilador reporta algún error? ¿Qué imprime el programa? R/ No imprimió ningún error. El programa imprime la edad cambiada.

## Step 1c: Desreferencia
La función `malloc` nos devuelve punteros. ¿Pero cómo obtenemos el puntero a algo? Con el
operador de "address of" `&` (no confundir con el and lógico o and de operaciones de bits).

Copien `step1b.c`a `step1c.c`. En este otro archivo vamos a crear varios estudiantes. Pero en lugar
de pedir la memoria con malloc vamos a usar la memoria de la pila:

```c
struct student s2 = {"Ana", 25, 100};
```

Ahora, modifiquen el programa para que también imprima los datos de Ana. En primer lugar, intenten
llamar a la función `print_student(s2);`. Compilen e indiquen qué error mostró el compilador.
Luego resuelvanlo para que el programa funcione apropiadamente.

Noten que el operador "address of" puede obtener la dirección de lo que sea. Por ejemplo,
creen esta función:

```c
void call_wrapper(void (f)(const struct student*), const struct student* s) {
    printf("antes\n");
    f(s);
    printf("despues\n");
}
```

Y la pueden llamar usando `call_wrapper(&print_student, s1);`. Alternativamente,
podrían usar una firma de función `void call_wrapper(void (*f)(const struct student*), const struct student* s)`
y llamarla usando `call_wrapper(print_student, s1);`.


# Step 2: memoria y fugas
En este paso vamos ver qué pasa si hacemos operaciones inapropiadas con la memoria. Se usará
la herramienta Valgrind para detectar fugas, strace para ver las llamadas al sistema y vamos
a conocer el segmentation fault, un error típico al manejar mal la memoria.

Para comenzar vamos a usar Valgrind para detectar fugas de memoria. Ejecuten el programa `step1b`
bajo valgrind, usando:

```
    valgrind ./step1b
```

Reporten el resultado que les da Valgrind. Noten las recomendaciones que da el programa. Ejecuten
Valgrind usando los flags recomendados. R/ Dice que tengo 20 bytes en 1 block que aún es reachable.

Valgrind es una herramienta que es capaz de detectar fugas de memoria. Por ejemplo, en step1b
"se nos olvidó" liberar la memoria. Noten que Valgrind reporta que los bytes perdidos son "still reachable".
Eso quiere decir que existe en la memoria del programa un puntero que apunta a la memoria que perdimos.
Ejemplos típicos de esto suceden cuando se nos olvida liberar la memoria de una estructura de datos compleja.

Aún no vamos a solucionar la fuga de memoria. Primero copien `step1b.c` a `step2.c`. Luego agreguen los `free`
al final del programa en `step1b.c` y `step1c.c`. Compilen los programas y vuelvanlos a correr bajo Valgrind,
para confirmar que ya no tienen fugas de memoria.

En `step2.c` en lugar de agregar la función `free` al final del programa vamos a hacer cosas no válidas
y ver qué sucede. Al final del programa agreguen `s1 = NULL;`. Compilen el programa y córranlo sin Valgrind.
Luego córranlo con Valgrind. ¿Cambió el error reportado por Valgrind? ¿Porqué no? R/ No, el error no fue cambiado porque no se liberó la memoria, sólo se le asignó nulo.

Es posible que Valgrind indique que nuestra fuga de memoria sea "still reachable". ¿Valgrind no sirve? No, no
es por eso. La respuesta es que el compilador optimiza nuestro programa. Como nadie lee `s1`
el compilador puede decidir eliminar esa instrucción (tengan cuidado cuando están haciendo benchmarks.
A veces parece que algo es más rápido, pero el compilador simplemente borró el código).
Para obligarlo a generar código necesitamos usar el valor de `s1` después de asignarlo. Agreguen al final
del programa `printf("%p\n", s1);` y vuelvanlo a ejecutar sobre Valgrind. Ahora sí, la memoria está
definitivamente perdida.

## Segmentation fault
En `step2.c`, muevan la línea `s1 = NULL` para que esté entre los dos `print_student`. Corran el programa
tanto sin Valgrind como con Valgrind y reporten los resultados. R/ Sin Valgrind, sólo me imprime el s1 y termina después de ponerme 'Segmentation fault (core dumped)'. Con Valgrind, me dice que tengo los bytes definitivamente perdidos y que tengo 1 error de un contexto.

Acceder a posiciones de memoria inválida produce lo que se conoce como _segmentation fault_. Esto no sucede
solamente cuando accedemos a un puntero nulo, sino cuando accedemos a cualquier posición de memoria que no
le pertenece al programa.

Por ejemplo, creen el archivo `step2a.c` y escriban lo siguiente:

```c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void main() {
    char* x = strdup("hola");
    int acc = 0;
    for(int i = 0; i < 32; i++) {
        printf("%d: %p -> %c\n", i, &x[i], x[i]);
        acc ^= x[i];
    }
    printf("%d\n", acc);
}
```

Compilen y ejecuten ese programa varias veces. Noten por ejemplo, que el resultado del
final del programa (hacer XOR de cada caracter leído) es diferente. Esto es porque la memoria
no necesariamente es limpiada antes de la ejecución de un programa. En C y C++ por esto
es importantísimo siempre inicializar la memoria.

Incrementen el 32 por un número alto. ¿En qué dirección les da el segmentation fault? (quiten el
salto de línea para evitar que tengan muchas líneas en la terminal) R/ 134466: 0x564109Segmentation fault (core dumped)

Vean que en este caso el segmentation fault ocurrió solamente con leer de la memoria, también puede
ocurrir con escribir o intentar ejecutar de un segmento no ejecutable.

## Use after free
¿Qué sucede si intentamos usar la memoria después de liberarla? Copien `step2.c` a `step2b.c` y averiguémoslo.

Modifiquen `step2b.c` y pongan `free(s1)` entre los dos `print_student` y quiten el `s1 = NULL;`. Corran el programa
sin valgrind y con valgrind. Comenten qué observan al ejecutar el programa varias veces.

Intenten asignar la edad y la nota de María después de hacer free. ¿El programa corre bien
después de eso? ¿Porqué? ¿Qué reporta Valgrind? R/ el programa corre, pero no bien. No le hace caso a la edad que le asigné. Esto es porque le caí encima de la memoria, entonces empezó a imprimir cosas que ya están en memoria, y no lo que yo quiero que imprima. Valgrind me reporta que hay invalid reads.

También existe otra herramienta, integrada en los compiladores GCC y Clang que se llama Address Sanitizer o
ASAN. Para usarla compile el programa usando los flags `-fsanitize=address -static-libasan -g`. Corra el
programa y comente los resultados obtenidos. 

OJO: Address Sanitizer hace al programa un poco más lento. Típicamente no se usan en producción.
Y Valgrind es muchísimo más lento y ese nunca se usa en producción.

## Fuga de memoria

ATENCIÓN: Este ejercicio podría hacer que su computadora se congele y requiera ser reiniciada a la fuerza. GUARDEN TODO SU TRABAJO ANTES DE EMPEZARLO.

Escriban este programa en `step2c.c`:
```c
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

const size_t BUFFER_SIZE = 1024*1024*4;

void main() {
    uintptr_t x = 0;
    size_t total = 0;
    // Ajuste el número abajo para que el programa pida 1GB de memoria adicional
    // a la memoria física de la máquina.
    for (int i = 0; i < 1000000000000; i++) {
        printf("%zu %d |", total, i);
        x ^= (uintptr_t)malloc(BUFFER_SIZE);
    }
    printf("\n%zu", x);
}
```

Abran el monitor del sistema, de forma que puedan ver el gráfico de
consumo de memoria. Cuando corran el programa observen cómo crece el
gráfico de consumo de memoria y luego el de memoria paginada.

Corran el programa. ¿Qué sucede?

Revisen la salida del log del sistema usando el comando `less /var/log/syslog` en
la terminal (presionen g para ir al final). ¿Quién terminó el programa?

Cambie el valor del buffer, por ejemplo a `1024*1024*512`. Vuelva a correr el programa
¿la memoria sigue subiendo como antes? ¿Porqué?

Finalmente, cambie el tamaño del buffer por algo pequeño, como 100. Reduzca el límite
del for loop a 1000. Compile el programa y ejecútelo sobre strace (`strace step2c`). Con ese
comando va a observar las llamadas a funciones del sistema, en particular las llamadas
a `brk` y `mmap`. ¿Observa llamadas a estas llamadas a función cada vez que llama a `malloc`?

# Step 3: Buffer Overflow
Un error típico al programar en C (o usar bibliotecas escritas en C desde C++)
es el buffer overflow. Este consiste en intentar escribir en un buffer más
allá del límite de este.

Copien el archivo `step1.c` a `step3.c`.

En este curso nadie se llama María (ya revisé en Mediación Virtual). Coloquen
su nombre completo (con apellidos) en la línea que ejecuta la función `strcpy`
y compilen el programa. Reporten qué sucede.

El compilador es lo suficientemente inteligente para saber que estamos haciendo
algo mal. Típicamente uno debería ir a arreglar este error, pero no en esta
práctica.

Ejecuten el programa y reporten qué sucede. ¿De qué forma falló?

Dependiendo del compilador, es posible que haya fallado de formas diferentes
(o que no fallara). GCC introduce cierta protección contra sobreescritura de
la pila, conocida como Stack Smashing Protection.

Ahora, vuelvan a compilar el programa, pero esta vez deshabiliten Stack Smashing
Protection, usando el flag: `-fno-stack-protector`. Corran el programa y reporten
qué errores observan. ¿Qué edad y qué nota tienen?

Lo que sucedió es que al hacer la copia de su nombre esta sobreescribió otros
valores, como la edad y la nota. Pero noten que quizá el programa no falló:
¡los datos están muy malos y corre perfectamente bien! (si les da un error
intenten acortar el nombre).

Este es un programa pequeño y pensado para que falle. Pero imagínense un error
de estos en un programa grande: un sistema operativo o un navegador pueden tener
millones de líneas de código. Protecciones como Stack Smashing Protector ayudan
a crear código más seguro, ya que este tipo de errores pueden ser usados para tomar
control de la máquina por parte de un usuario malicioso.

Corran el programa usando Valgrind (`valgrind ./step3`). ¿Qué error reporta?

Modifiquen el programa para que el nombre mida menos de 20 caracteres (por ejemplo,
12 caracteres). Compilen y ejecuten el programa con Valgrind. ¿Muestra el mismo
error al correrlo con Valgrind? ¿Su compilador mostró algún warning esta vez?
¿Qué edad tienen?

Como pueden observar, es increiblemente fácil producir un buffer overflow. Y
este último error no puede ser detectado ni por el compilador ni por herramientas
especializadas como Valgrind. Para evitar esto, es importantísimo seguir buenas prácticas
de desarrollo: usar herramientas de análisis estático, herramientas como Valgrind,
pruebas, code review.

## Step 3a: pwned!
Creen el archivo `step3a.c` con el siguiente código:

```c
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
    scanf("%s", s1.name);

    if (s1.is_admin) {
        printf("Hola %s, tienes acceso al sistema!\n", s1.name);
    } else {
        printf("Hola %s, no eres administrador\n", s1.name);
    }
}
```

Compílenlo usando `gcc step3a.c -o step3a`. Ejecútenlo e intenten obtener
credenciales de administrador. Procuren que el programa no falle.
¿Con qué nombres no obtuvieron acceso? ¿Con qué nombres sí? ¿Con qué nombres
hicieron que el programa fallara? ¿Qué opinan de este último ejercicio?

Un programa debe desconfiar de TODAS las entradas externas. Es importante sanitizar los datos que vienen de terceros, no confiar en que tengan el formato esperado y los errores deben ser manejados de forma apropiada (ejm: mostrar un error en lugar de caerse). ¿Qué estrategias pueden tomar para arreglar el programa anterior?

Modifiquen el programa para que use `"%9s"` e intenten volver a obtener acceso de administrador. ¿Lo lograron? ¿porqué sí/no?

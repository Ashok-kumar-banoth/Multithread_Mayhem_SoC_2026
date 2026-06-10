// 01_basic_template.c
// Example: the minimal xv6 user program.
// Every xv6 user program must:
//   1. Include kernel/types.h and user/user.h
//   2. Call exit(0) at the end
//   3. Be added to UPROGS in the Makefile

#include "kernel/types.h"
#include "user/user.h"

int main(int argc, char *argv[]) {
    printf("Hello from xv6!\n");
    printf("I received %d arguments:\n", argc);

    for (int i = 0; i < argc; i++) {
        printf("  argv[%d]: %s\n", i, argv[i]);
    }

    exit(0);
}

// 02_fork_demo.c
// Example: demonstrating fork(), getpid(), wait(), and exit().
//
// The parent creates a child, both print their PIDs,
// the parent waits for the child to finish.

#include "kernel/types.h"
#include "user/user.h"

int main() {
    int pid = fork();

    if (pid < 0) {
        printf("fork failed\n");
        exit(1);
    }

    if (pid == 0) {
        // Child
        printf("Child:  my PID is %d, parent PID is %d\n", getpid(), getppid());
        exit(0);
    } else {
        // Parent
        printf("Parent: my PID is %d, child PID is %d\n", getpid(), pid);
        wait(0);
        printf("Parent: child has exited. Goodbye.\n");
        exit(0);
    }
}

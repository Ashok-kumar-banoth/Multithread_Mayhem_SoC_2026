// 04_multi_syscall_demo.c
// Example: calling several xv6 system calls to see
// what they return and how they behave.

#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int main() {
    int pid, ret;

    printf("=== Multi-Syscall Demo ===\n");
    printf("My PID: %d\n", getpid());
    printf("Uptime: %d ticks\n", uptime());

    pid = fork();
    if (pid == 0) {
        printf("  [Child] PID: %d\n", getpid());
        printf("  [Child] Sleeping for 5 ticks...\n");
        sleep(5);
        exit(0);
    }

    printf("Parent: child PID is %d\n", pid);
    ret = wait(0);
    printf("Parent: wait() returned %d\n", ret);

    printf("Uptime now: %d ticks\n", uptime());
    printf("Done.\n");
    exit(0);
}

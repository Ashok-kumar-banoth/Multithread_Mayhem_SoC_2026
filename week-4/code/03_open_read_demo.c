// 03_open_read_demo.c
// Example: opening a file, reading its contents,
// and printing them to the console.
//
// Usage: open_read_demo <filename>
// If no filename is given, reads README by default.

#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int main(int argc, char *argv[]) {
    char buf[512];
    char *filename;
    int fd, n;

    if (argc < 2) {
        filename = "README";
    } else {
        filename = argv[1];
    }

    fd = open(filename, 0); // 0 = O_RDONLY
    if (fd < 0) {
        printf("open: %s failed\n", filename);
        exit(1);
    }

    printf("Contents of '%s':\n", filename);
    while ((n = read(fd, buf, sizeof(buf))) > 0) {
        write(1, buf, n);
    }

    printf("\n--- end of file ---\n");
    close(fd);
    exit(0);
}

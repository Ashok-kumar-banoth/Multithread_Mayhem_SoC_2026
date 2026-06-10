# Week 4 Theory: xv6 Setup & Codebase Exploration

---

## 1. What is an Operating System?

An operating system is a layer of software that sits between hardware and user programs. It has two main jobs:

- **Resource management**: the CPU, memory, disk, and devices are shared among running programs. The OS decides who gets what, when, and for how long.
- **Abstraction**: the OS hides the messy details of hardware behind clean interfaces. A file is easier to work with than a spinning disk sector. A process is easier to work with than a CPU core and register file.

### Three core abstractions

| Abstraction | What it hides | What the OS provides |
|---|---|---|
| **Process** | CPU, registers, interrupts | The illusion that each program owns the CPU |
| **Address space** | RAM, page tables | Each program gets its own private memory |
| **File** | Disk sectors, block caches | Named, persistent byte sequences |

Every OS you've ever used — Linux, Windows, macOS — provides these three. xv6 provides them too, in about 10,000 lines of C.

---

## 2. User Space vs Kernel Space

### Privilege levels

Processors provide different privilege levels. On RISC-V:

| Mode | Name | What you can do |
|---|---|---|
| **U-mode** | User mode | Run your code, access your own memory |
| **S-mode** | Supervisor mode | Configure page tables, handle interrupts, control devices |
| **M-mode** | Machine mode | Change the CPU's physical configuration (rarely used) |

Your programs run in U-mode. The xv6 kernel runs in S-mode. The hardware blocks any attempt by U-mode code to:
- Execute privileged instructions (`csrw`, `sret`)
- Access kernel memory
- Disable interrupts
- Directly talk to devices

**Why the separation?** Without it, a bug in any program could crash the entire machine. A malicious program could read another program's files, steal passwords, or wipe the disk. The hardware-enforced boundary limits the damage.

### Crossing the boundary: system calls

When a user program needs a privileged operation — reading a file, creating a process — it cannot do it directly. It asks the kernel via a **system call**.

The mechanism on RISC-V:

```
User program                 Kernel
──────────────────────────────────────────
  write(fd, buf, n)
       │
       ▼
  user wrapper (usys.S)      syscall() dispatch
       │                          │
       │  ecall instruction        │
       ├────────────────────────►  │
       │                          ▼
       │                     sys_write()
       │                          │
       │  return from trap        │
       ◄──────────────────────────┘
       │
       ▼
  back in user code
```

The `ecall` instruction:
1. Raises privilege from U-mode to S-mode
2. Saves the program counter
3. Sets `scause` to the "environment call from U-mode" value
4. Jumps to the trap handler address in `stvec`

The kernel handles the request, then returns via `sret`, which restores U-mode and resumes the user program exactly where it left off.

You don't need to understand every line of this path yet — just the idea that **a system call is a controlled crossing from user space into kernel space**, and the hardware makes sure you can only cross at designated entry points.

---

## 3. Introduction to xv6

xv6 is a reimplementation of Sixth Edition Unix (V6) for modern RISC-V processors, developed at MIT. It is:

- **Small**: ~10,000 lines total — readable in a long weekend
- **Complete**: processes, virtual memory, file system, pipes, device drivers
- **Modifiable**: designed so students can add features
- **Runnable**: boots on QEMU or real RISC-V hardware

### The source tree

```
xv6-riscv/
├── kernel/          # OS kernel code
│   ├── proc.h       # Process table and PCB (struct proc)
│   ├── proc.c       # Process management: allocproc, fork, scheduler, wait, exit
│   ├── syscall.h    # System call numbers
│   ├── syscall.c    # Dispatch table and argument extractors
│   ├── sysproc.c    # Process-related system call implementations
│   ├── trap.c       # Trap handling: usertrap, kerneltrap
│   ├── vm.c         # Virtual memory: page tables, mappings
│   ├── kalloc.c     # Physical memory allocator
│   ├── file.c       # File abstraction layer
│   ├── fs.c         # File system implementation
│   ├── fs.h         # File system on-disk structures (inodes, superblock)
│   ├── bio.c        # Buffer cache for block I/O
│   ├── pipe.c       # Pipe implementation
│   ├── console.c    # UART console driver
│   ├── printf.c     # Kernel printf
│   ├── spinlock.h   # Spinlock implementation
│   ├── string.c     # Memory/string utilities
│   ├── entry.S      # Kernel entry point assembly
│   ├── trampoline.S # Trap handler assembly (uservec, userret)
│   ├── kernelvec.S  # Kernel trap handler assembly
│   └── main.c       # Kernel initialization sequence
│
├── user/            # User-space programs
│   ├── user.h       # System call declarations for user programs
│   ├── usys.pl      # Script that generates usys.S (syscall wrappers)
│   ├── usys.S       # Generated assembly stubs (one per syscall)
│   ├── init.c       # First user process — starts the shell
│   ├── sh.c         # The xv6 shell
│   ├── ls.c         # List directory contents
│   ├── cat.c        # Concatenate and print files
│   ├── echo.c       # Echo arguments
│   ├── forktest.c   # Fork stress test
│   ├── stressfs.c   # File system stress test
│   └── ...
│
├── Makefile         # Build system — find UPROGS here
├── mkfs/            # Tool to create the file system image
│   └── mkfs.c
└── kernel.ld        # Linker script that lays out the kernel in memory
```

### Key data structures

**`struct proc`** — the Process Control Block (`kernel/proc.h`):

```c
struct proc {
    struct spinlock lock;
    enum procstate state;   // UNUSED, SLEEPING, RUNNABLE, RUNNING, ZOMBIE
    void *chan;             // sleep channel (if SLEEPING)
    int pid;
    struct proc *parent;
    pagetable_t pagetable;  // user page table
    uint64 kstack;          // kernel stack address
    uint64 sz;              // process memory size
    struct trapframe *trapframe;
    struct context context; // for context switching
    struct file *ofile[NOFILE];
    struct inode *cwd;      // current working directory
    char name[16];
};
```

**Process states** (`kernel/proc.h`):

```c
enum procstate { UNUSED, SLEEPING, RUNNABLE, RUNNING, ZOMBIE };
```

**`struct inode`** — file system inode (`kernel/file.h`, `kernel/fs.h`): represents a file or directory.

**`struct superblock`** (`kernel/fs.h`): describes the file system layout on disk (total blocks, inode count, etc.).

---

## 4. The Build Process

When you run `make`, the following happens:

1. **Cross-compilation**: `gcc-riscv64-unknown-elf` compiles every `.c` file in `kernel/` to RISC-V object files, then links them into `kernel/kernel` (the OS binary).

2. **User programs**: every program listed in `UPROGS` in the Makefile is compiled into a RISC-V executable.

3. **File system image**: the `mkfs` tool packages all user programs into `fs.img` along with a `README` and any other static files. This is the "disk" that xv6 will boot with.

4. **QEMU launch**: QEMU loads two images:
   - `xv6.img` — the kernel binary
   - `fs.img` — the root file system

**Important**: user programs are baked into `fs.img` at build time. You cannot add a program at runtime — it must be in `UPROGS` before you run `make`.

---

## 5. Writing Your First xv6 Program

Every xv6 user program follows this template:

```c
#include "kernel/types.h"    // basic type definitions
#include "user/user.h"       // system call declarations

int main(int argc, char *argv[]) {
    // your code here
    exit(0);                 // every program must call exit!
}
```

Key differences from Linux user-space programming:
- No `#include <stdio.h>` — use kernel-provided headers
- `printf` comes from xv6's own library (declared in `user/user.h`)
- `exit(0)` is mandatory — falling off the end of `main` is not defined
- Standard library is minimal: no `malloc`, no `fopen`, just system calls

### Adding your program to the build

1. Save your file as `user/myprog.c`
2. In the **Makefile**, find `UPROGS` and add: `$U/_myprog\`
3. Rebuild: `make clean && make && make qemu`
4. Run at the `$` prompt: `myprog`

---

## 6. References

- [xv6-riscv book](https://pdos.csail.mit.edu/6.828/2025/xv6/book-riscv-rev4.pdf) — Chapters 1 (OS overview), 2 (xv6 architecture)
- [xv6-riscv source](https://github.com/mit-pdos/xv6-riscv) — the repo you cloned
- [OSTEP](https://pages.cs.wisc.edu/~remzi/OSTEP/) — Chapters 2, 4, 5 for OS concepts
- RISC-V Privileged Architecture — `ecall`, `sret`, `stvec` documentation

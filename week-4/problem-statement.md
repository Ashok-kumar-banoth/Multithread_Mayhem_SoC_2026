# Week 4: Problem Set — xv6 Setup & Codebase Exploration

This week is about setting up xv6 and understanding its internals by reading the source and writing simple user programs. **No kernel modifications required.** All programs run entirely in user space.

---

## Setup

1. Clone xv6-riscv: `git clone https://github.com/mit-pdos/xv6-riscv.git`
2. Install dependencies (RISC-V toolchain + QEMU) — see `theory.pdf` for instructions
3. Build and boot: `make && make qemu`
4. Verify the shell works: `ls`, `cat README`, `echo hello`

For each problem, you'll add a `.c` file to `user/`, add it to `UPROGS` in the Makefile, rebuild, and test inside QEMU.

---

## Problem 1 — Hello xv6! (Warm-up)

Write a user program `hello_xv6.c` that:

1. Prints a greeting that includes xv6's name and your name/alias.
2. Prints the number of arguments passed to it.
3. Prints each argument on its own line.

**Interesting twist:** Print how many system calls you can find in `kernel/syscall.h` by embedding that count directly as a `#define` in your program. You'll need to read the file to count them.

**Expected output:**
```
$ hello_xv6
Hello, xv6! This is <your_name>.
Number of arguments: 0
$ hello_xv6 hi there!
Hello, xv6! This is <your_name>.
Number of arguments: 2
arg 1: hi
arg 2: there!
xv6 knows 23 system calls.
```

**Hints:**
- xv6 programs only use headers from inside the xv6 tree: `kernel/types.h`, `user/user.h`.
- Use `argc` and `argv[]` in `main()`.
- Count the syscalls in `kernel/syscall.h` manually and `#define` the number.

---

## Problem 2 — Codebase Detective

This problem requires **no code** — it's a source code exploration exercise. Open the xv6 source files in a text editor (or browse the MIT GitHub repo) and answer the following:

**A. Process states** (`kernel/proc.h`)
- What are the five possible states of an xv6 process?
- What state is a process in right after `fork()` returns in the child?
- What state is a process in between `exit()` and `wait()`?

**B. The boot process** (`kernel/main.c`)
- List the initialization steps executed in `main()`, in order.
- Which subsystem is initialized first? Which one last?
- At the end of `main()`, the kernel calls `scheduler()`. What does this function do? (Read `kernel/proc.c`.)

**C. The system call table** (`kernel/syscall.c`)
- How many system calls does xv6 currently support?
- Find the `syscalls[]` array. What pattern do you notice about the indices and the system call numbers in `syscall.h`?
- What function is called if a user program uses an invalid system call number?

**D. The shell** (`user/sh.c`)
- Find the `runcmd()` function. How does the shell run a command? (Look for calls to `fork()`, `exec()`, and `wait()`.)
- What happens when you type `ls | cat` conceptually? Can you trace the pipe setup in the code?

**Deliverable:** Submit your answers as a plain text or markdown file.

---

## Problem 3 — Fork Chain

Write a user program `fork_chain.c` that creates a chain of N processes:

```
process 0 (pid: A) → child (pid: B) → child (pid: C) → ... → child (pid: Nth)
```

1. Take N as a command-line argument (default 5).
2. Each process prints its PID, its parent's PID, and its depth in the chain.
3. Only the last process (the leaf) should print "I am the leaf! No more children."
4. Each process waits for its child to finish before exiting.

**Interesting twist:** After the chain completes, print the **total number of processes that were created** and the **depth you reached**. Verify they match.

**Expected output:**
```
$ fork_chain 3
Depth 0: PID 4, Parent PID 2
Depth 1: PID 5, Parent PID 4
Depth 2: PID 6, Parent PID 5
I am the leaf! No more children.
Total processes in chain: 3
```

**Hints:**
- Use `fork()` in a loop or recursively.
- Use `getpid()` and the return value of `fork()` to distinguish parent from child.
- Every process must call `exit(0)`.
- Use `wait(0)` to wait for the direct child.

## Problem 4 — Syscall Spy

Write a user program `syscall_spy.c` that calls as many different xv6 system calls as it can and prints what each one returns.

At minimum, call:

| System call | What to expect |
|---|---|
| `getpid()` | Your PID |
| `uptime()` | Ticks since boot |
| `fork()` | Child PID (parent) or 0 (child) |
| `write(1, buf, n)` | Bytes written (usually n) |
| `sleep(n)` | 0 on success |
| `getpid()` again | Same PID |
| `dup(0)` | New file descriptor number |

**Interesting twist:** Use a child process that calls `getpid()`, then have the parent call `wait()` and compare the PID the parent got from `fork()` with the PID the child printed — they should match. Also detect the case where `fork()` returns 0 (you're in the child) and print "I am the child!" in that branch.

**Expected output:**
```
$ syscall_spy
Syscall Spy Report for PID 3:
  getpid()       → 3
  uptime()       → 2741
  fork()         → 4  (child PID)
  --- in child (PID 4) ---
  getpid()       → 4
  --- back in parent (PID 3) ---
  sleep(10)      → 0
  write(1, ...)  → 13
  dup(0)         → 3
  wait()         → 4  (reaped child PID 4)
```

**Hints:**
- Call `fork()` early. Inside the child (`pid == 0`), print your PID with a special marker, then `exit(0)`.
- Use `wait(0)` in the parent to reap the child before continuing.
- `write(1, "Hello, world!\n", 14)` prints to stdout and returns 14.
- `dup(0)` duplicates stdin onto the next available fd.

---

# Week 4 — xv6 Setup & Codebase Exploration

> Goal of the week: set up the xv6 operating system, understand the boundary between user space and kernel space, explore the xv6 source tree from top to bottom, and write your first xv6 user programs.

For the first three weeks you ran multithreaded programs on your host OS. This week you step inside a real (small) OS — you will compile it, boot it, navigate its source tree, and understand how user programs interact with the kernel. No kernel modifications yet. That comes next week. This week is about **seeing the machine from the OS's point of view**.

---

## What you'll know by the end of the week

- The difference between **user mode** and **kernel mode**, and how RISC-V enforces the boundary
- How to **clone, build, boot, and shut down** xv6 on QEMU
- The layout of the **xv6 source tree** — where each subsystem lives
- What the **key data structures** in xv6 look like: `struct proc`, the system call table, the file system layout
- How to **write, add, build, and run** a user-space program inside xv6
- How `fork()` and `exec()` work at the conceptual level to create and load programs

---

## What's in this folder

| File / folder | What it is |
|---|---|
| `theory.pdf` | All the concepts for the week. Read this first. |
| `problem-statement.pdf` | The problems you'll work on. |
| `code/` | Example xv6 user programs referenced from the theory and problems. |
| `sample-outputs/` | Reference output from each problem running inside xv6. |

---

## Suggested plan for the week

| Day | What to do |
|-----|------------|
| 1 | Read sections 1–3 of `theory.pdf`. Install the RISC-V toolchain and QEMU. |
| 2 | Clone xv6-riscv, build it, boot it. Play with `ls`, `cat`, `echo`, `forktest` inside xv6. |
| 3 | Read sections 4–5 of `theory.pdf`. Write and run your first xv6 user program. |
| 4 | Work through problems 1–3. Read actual xv6 source code. |
| 5 | Work through problems 4. Finish your report. |

---

## How to set up xv6 on your machine

### Option A: Native installation (Linux — Ubuntu 24.04+ recommended)

- This is also given in the xv-6 book in the Resources

```bash
sudo apt update
sudo apt install -y git build-essential qemu-system-riscv64 gcc-riscv64-unknown-elf

git clone https://github.com/mit-pdos/xv6-riscv.git
cd xv6-riscv
make
make qemu
```

### Option B: Docker (works everywhere)

```bash
git clone https://github.com/mit-pdos/xv6-riscv.git
docker pull arghyadipchak/xv6-riscv-devbox:latest
cd xv6-riscv
docker run --rm -it -u $(id -u):$(id -g) -v $(pwd):/xv6-riscv arghyadipchak/xv6-riscv-devbox:latest
# Inside the container:
make
make qemu
```

**Exit QEMU:** `Ctrl+A` then `X`.

---

## How to add your own program to xv6

1. Write a `.c` file (e.g., `myprog.c`) and place it in `user/`.
2. Open the **`Makefile`** and find the `UPROGS` variable. Add `$U/_myprog\` to the list.
3. Rebuild and boot:

```bash
make clean && make && make qemu
```
- OR just ```make qemu``` also works (Why ?)

4. At the xv6 `$` prompt, run:

```
$ myprog
```

Any time you change a user program, you need to rebuild. The quickest way is `make <program_name>` (e.g., `make myprog`) followed by `make qemu` — but if you also changed kernel files, do `make clean && make && make qemu`.

---

## Commands to try inside xv6

```
$ ls              # list files
$ cat README      # read a file
$ echo hello      # print a string
$ forktest        # run a fork stress test
$ kill <pid>      # send a signal
$ stressfs        # stress the file system
```

---

## A word of warning before you begin

**xv6 is not Linux.** You can't `apt install` or `gcc` inside it. Programs must be compiled on your host machine *before* booting. The shell is minimal — no tab completion, no history, no `man` pages. If your program crashes, you'll see a message like `pid 3: panic: ...` and the whole OS may freeze. That's normal. Close QEMU (`Ctrl+A, X`), fix your code, rebuild, and try again.

**Two things will save you time:**
1. **`make clean && make`** — always do a clean build after modifying files. Stale objects cause mystifying bugs.
2. **Read the source.** xv6 is only ~10,000 lines. The best way to understand it is to read it. Start with `kernel/proc.h` and `kernel/proc.c`.

---

## Submission instructions

Submissions for this week will be collected via a **Google Form**. We will share the form link separately.

### Files to keep ready

| Problem | File(s) to keep ready |
|---|---|
| Problem 1 — Hello xv6! | `user/hello_xv6.c` |
| Problem 2 — Codebase Detective | Your answers (plain text or markdown) |
| Problem 3 — Fork Chain | `user/fork_chain.c` |
| Problem 4 — Syscall Spy | `user/syscall_spy.c` |

Also keep ready a short **report** — half a page is plenty — covering:

- Which problems you attempted
- The most interesting thing you found in the xv6 source code
- One thing that surprised you about how xv6 works
- What happened when you ran your fork chain — did the output match your expectation?
- Anything you got stuck on

### What's expected

- A working xv6 build that boots in QEMU
- User programs that compile and produce the expected output
- Evidence that you read actual xv6 source code (mention specific files and line numbers in your report)

### What's NOT expected

- Kernel modifications. That starts in Week 5.
- A perfect understanding of every xv6 subsystem — just the big picture this week.
- All five problems. Problems 1, 2, and 3 are the minimum.

### File hygiene

Submit only your user programs and your exploration notes — not the entire xv6 tree.

---

## Stuck?

- `make` fails? → Check `gcc-riscv64-unknown-elf` and `qemu-system-riscv64` are installed. Verify with `--version`.
- QEMU boots but you don't see the `$` prompt? → Wait a few seconds, then press Enter.
- Your program isn't found at the shell? → Did you add it to `UPROGS` in the Makefile?
- Program crashes inside xv6? → Did you use `exit(0)` at the end? xv6 requires explicit exit.
- Getting `panic`? → Read the panic message — it tells you the file and line where it happened. Then think about what your last change was.
- Still stuck? → Open an issue or message your mentor.

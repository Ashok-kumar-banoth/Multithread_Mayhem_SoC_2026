# Makefile for Week 4 code examples — Multithread Mayhem
#
# NOTE: These programs are meant to run INSIDE xv6, not on your host OS.
# To use them, copy the .c files into xv6-riscv/user/ and add them to
# the UPROGS list in the xv6 Makefile, then:
#
#   make clean && make && make qemu
#
# This Makefile is provided for reference only — the xv6 Makefile is
# what actually builds these programs into RISC-V binaries.

CC      = gcc
CFLAGS  = -Wall -Wextra -O0 -g
TARGETS = basic_template fork_demo open_read_demo multi_syscall_demo

.PHONY: all clean

all: $(TARGETS)

basic_template: 01_basic_template.c
	$(CC) $(CFLAGS) $< -o $@

fork_demo: 02_fork_demo.c
	$(CC) $(CFLAGS) $< -o $@

open_read_demo: 03_open_read_demo.c
	$(CC) $(CFLAGS) $< -o $@

multi_syscall_demo: 04_multi_syscall_demo.c
	$(CC) $(CFLAGS) $< -o $@

clean:
	rm -f $(TARGETS)

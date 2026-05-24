# Makefile for Week 2 code examples — Multithread Mayhem
#
# Usage:
#   make             # build everything
#   make clean       # remove built binaries
#   make run-fixed   # build and run the three programs that terminate
#                    # (deadlock_demo is NOT run automatically — it hangs)
#
# Build only one:
#   make counter_fixed
#   make atomic_counter
#   make producer_consumer
#   make deadlock_demo

CC      = gcc
CFLAGS  = -Wall -Wextra -pthread -O0 -g -std=c11
TARGETS = counter_fixed atomic_counter producer_consumer deadlock_demo

.PHONY: all clean run-fixed

all: $(TARGETS)

counter_fixed: 01_counter_fixed.c
	$(CC) $(CFLAGS) $< -o $@

atomic_counter: 02_atomic_counter.c
	$(CC) $(CFLAGS) $< -o $@

producer_consumer: 03_producer_consumer.c
	$(CC) $(CFLAGS) $< -o $@

deadlock_demo: 04_deadlock_demo.c
	$(CC) $(CFLAGS) $< -o $@

# Note: deadlock_demo deliberately hangs forever. Run it by hand
# (./deadlock_demo) and Ctrl+C when you've seen the symptom.
run-fixed: counter_fixed atomic_counter producer_consumer
	@echo "=== 01 counter_fixed ==="
	./counter_fixed
	@echo
	@echo "=== 02 atomic_counter ==="
	./atomic_counter
	@echo
	@echo "=== 03 producer_consumer ==="
	./producer_consumer

clean:
	rm -f $(TARGETS)

/*
 * 02_atomic_counter.c
 *
 * Same problem as 01_counter_fixed.c, different solution: C11 atomics.
 *
 * Mutexes are general-purpose: you can protect any block of code with
 * them. But for simple operations like "increment a counter," they
 * are overkill. The CPU has hardware-level support for atomic
 * read-modify-write instructions (compare-and-swap, fetch-and-add,
 * etc.). C11 exposes these via the <stdatomic.h> header.
 *
 * An atomic_long behaves like a regular long for most purposes, but
 * any operation on it is guaranteed to be indivisible. No lock
 * required.
 *
 * Compile:
 *   gcc -Wall -pthread -std=c11 02_atomic_counter.c -o atomic_counter
 *
 * Run and compare timing vs 01_counter_fixed:
 *   time ./counter_fixed
 *   time ./atomic_counter
 *
 * The atomic version is typically 2-5x faster than the mutex version
 * for this simple workload. But atomics only work for very simple
 * operations — anything more complex than a single read/write/RMW
 * still needs a mutex.
 */

#include <stdio.h>
#include <pthread.h>
#include <stdatomic.h>

#define ITERS 10000000

/* atomic_long instead of long. No separate lock needed. */
atomic_long counter = 0;

void *increment(void *arg) {
    (void)arg;
    for (long i = 0; i < ITERS; i++) {
        /*
         * atomic_fetch_add returns the OLD value, atomically adds the
         * second argument, and stores the result. Hardware-supported.
         * We could also write counter++ on an atomic variable and the
         * compiler would emit the same instruction — but using the
         * atomic_* functions makes the intent explicit.
         */
        atomic_fetch_add(&counter, 1);
    }
    return NULL;
}

int main(void) {
    pthread_t t1, t2;
    pthread_create(&t1, NULL, increment, NULL);
    pthread_create(&t2, NULL, increment, NULL);
    pthread_join(t1, NULL);
    pthread_join(t2, NULL);

    long expected = 2L * ITERS;
    long final = atomic_load(&counter);
    printf("Final counter:  %ld\n", final);
    printf("Expected:       %ld\n", expected);
    if (final == expected)
        printf("Result: CORRECT — and faster than a mutex.\n");
    else
        printf("Result: WRONG by %ld.\n", expected - final);

    return 0;
}

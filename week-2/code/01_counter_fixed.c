/*
 * 01_counter_fixed.c
 *
 * The race_counter from Week 1, now actually correct.
 *
 * Two threads each increment a SHARED counter ITERS times. The
 * critical section — counter++ — is protected by a pthread mutex,
 * so only one thread can execute it at a time. The bad interleaving
 * that produced the wrong answer last week is now structurally
 * impossible.
 *
 * Compile:
 *   gcc -Wall -pthread 01_counter_fixed.c -o counter_fixed
 *
 * Run many times. Unlike last week, the result is always exactly
 * 2 * ITERS. Try it:
 *   for i in {1..20}; do ./counter_fixed; done
 *
 * Notice also that this is SLOWER than the racy version. Locks have
 * a cost — the threads spend time waiting for each other instead of
 * doing real work. We'll talk about that trade-off in the theory.
 */

#include <stdio.h>
#include <pthread.h>

#define ITERS 10000000

/* The shared counter, and the lock that protects it. */
long counter = 0;
pthread_mutex_t counter_lock = PTHREAD_MUTEX_INITIALIZER;

void *increment(void *arg) {
    (void)arg;
    for (long i = 0; i < ITERS; i++) {
        pthread_mutex_lock(&counter_lock);
        counter++;                    /* critical section */
        pthread_mutex_unlock(&counter_lock);
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
    printf("Final counter:  %ld\n", counter);
    printf("Expected:       %ld\n", expected);
    if (counter == expected)
        printf("Result: CORRECT — every time.\n");
    else
        printf("Result: WRONG by %ld — something is very broken.\n",
               expected - counter);

    pthread_mutex_destroy(&counter_lock);
    return 0;
}

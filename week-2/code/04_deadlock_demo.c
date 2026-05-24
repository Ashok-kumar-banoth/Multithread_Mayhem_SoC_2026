/*
 * 04_deadlock_demo.c
 *
 * A program that deadlocks on purpose, so you can see the symptom
 * and learn to recognize it.
 *
 * Setup: two threads, two locks.
 *   - Thread A locks resource_x, then tries to lock resource_y.
 *   - Thread B locks resource_y, then tries to lock resource_x.
 *
 * If the timing is right (and we make sure it is with the sleep),
 * A holds X and waits for Y, while B holds Y and waits for X.
 * Neither will ever release. The program just hangs.
 *
 * This is one of the four Coffman conditions ("circular wait") in
 * action.
 *
 * Compile:
 *   gcc -Wall -pthread 04_deadlock_demo.c -o deadlock_demo
 *
 * Run — and BE READY TO PRESS CTRL+C, the program never finishes:
 *   ./deadlock_demo
 *
 * Look at the output. Both threads print that they took their first
 * lock, then print that they're waiting on the second — and that's
 * where it stops, forever.
 *
 * THE FIX (don't apply it here, but understand it):
 * Always acquire locks in the same global order. If both threads
 * locked X then Y, there would be no cycle and no deadlock. This is
 * called "lock ordering" and is the most common deadlock-prevention
 * technique in real codebases.
 */

#include <stdio.h>
#include <unistd.h>
#include <pthread.h>

pthread_mutex_t resource_x = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t resource_y = PTHREAD_MUTEX_INITIALIZER;

void *thread_a(void *arg) {
    (void)arg;
    printf("[A] trying to lock X\n");
    pthread_mutex_lock(&resource_x);
    printf("[A] got X, now sleeping briefly...\n");

    /* The sleep gives Thread B time to grab Y, guaranteeing the
       deadlock. Without it, one thread might grab both before the
       other wakes up — race conditions can hide deadlocks too. */
    sleep(1);

    printf("[A] now trying to lock Y\n");
    pthread_mutex_lock(&resource_y);    /* hangs forever */
    printf("[A] got both locks — should never print this\n");
    pthread_mutex_unlock(&resource_y);
    pthread_mutex_unlock(&resource_x);
    return NULL;
}

void *thread_b(void *arg) {
    (void)arg;
    printf("[B] trying to lock Y\n");
    pthread_mutex_lock(&resource_y);
    printf("[B] got Y, now sleeping briefly...\n");
    sleep(1);
    printf("[B] now trying to lock X\n");
    pthread_mutex_lock(&resource_x);    /* hangs forever */
    printf("[B] got both locks — should never print this\n");
    pthread_mutex_unlock(&resource_x);
    pthread_mutex_unlock(&resource_y);
    return NULL;
}

int main(void) {
    pthread_t a, b;
    printf("Starting deadlock demo. Press Ctrl+C when you've seen enough.\n\n");
    pthread_create(&a, NULL, thread_a, NULL);
    pthread_create(&b, NULL, thread_b, NULL);
    pthread_join(a, NULL);     /* main will hang here too — forever */
    pthread_join(b, NULL);
    printf("This line will never run.\n");
    return 0;
}

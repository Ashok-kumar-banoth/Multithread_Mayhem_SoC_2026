/*
 * 03_producer_consumer.c
 *
 * The classic producer-consumer pattern, built with a mutex and two
 * condition variables.
 *
 * One producer thread generates ITEMS_TO_PRODUCE items and puts them
 * into a shared bounded buffer of size BUFFER_SIZE. One consumer
 * thread takes items out and "processes" them (we just print).
 *
 * The interesting parts:
 *   - The buffer is shared, so all accesses are inside a critical
 *     section guarded by buffer_lock.
 *   - If the producer arrives when the buffer is full, it WAITS on
 *     not_full instead of spinning.
 *   - If the consumer arrives when the buffer is empty, it WAITS on
 *     not_empty.
 *   - Each side SIGNALS the other when it has changed the state.
 *   - The waits are inside while-loops because of spurious wakeups
 *     and because by the time we re-acquire the lock the condition
 *     might no longer hold.
 *
 * Compile:
 *   gcc -Wall -pthread 03_producer_consumer.c -o producer_consumer
 *
 * Run:
 *   ./producer_consumer
 *
 * You'll see produce/consume lines interleaving naturally. The
 * producer pauses when the buffer fills (size 5); the consumer
 * pauses when it empties. Nothing is ever lost or duplicated.
 */

#define _DEFAULT_SOURCE      /* expose usleep under strict -std=c11 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

#define BUFFER_SIZE        5
#define ITEMS_TO_PRODUCE  20

/* Shared bounded buffer (a simple ring) and its bookkeeping. */
int  buffer[BUFFER_SIZE];
int  count = 0;     /* how many items currently in the buffer */
int  in    = 0;     /* next slot to write */
int  out   = 0;     /* next slot to read  */

/* One lock protects all four shared variables. */
pthread_mutex_t buffer_lock = PTHREAD_MUTEX_INITIALIZER;

/* Two condition variables for the two waiting conditions. */
pthread_cond_t  not_full  = PTHREAD_COND_INITIALIZER;   /* room to put */
pthread_cond_t  not_empty = PTHREAD_COND_INITIALIZER;   /* item to take */

void *producer(void *arg) {
    (void)arg;
    for (int i = 0; i < ITEMS_TO_PRODUCE; i++) {
        pthread_mutex_lock(&buffer_lock);

        /* Wait until there's room. WHILE, not IF — see theory. */
        while (count == BUFFER_SIZE) {
            printf("[producer] buffer full, waiting...\n");
            pthread_cond_wait(&not_full, &buffer_lock);
        }

        /* Critical section: enqueue. */
        buffer[in] = i;
        in = (in + 1) % BUFFER_SIZE;
        count++;
        printf("[producer] produced %d  (buffer now has %d)\n", i, count);

        /* Wake up the consumer in case it was waiting. */
        pthread_cond_signal(&not_empty);
        pthread_mutex_unlock(&buffer_lock);

        usleep(50000);     /* simulate work — 50 ms */
    }
    return NULL;
}

void *consumer(void *arg) {
    (void)arg;
    for (int i = 0; i < ITEMS_TO_PRODUCE; i++) {
        pthread_mutex_lock(&buffer_lock);

        while (count == 0) {
            printf("[consumer] buffer empty, waiting...\n");
            pthread_cond_wait(&not_empty, &buffer_lock);
        }

        int item = buffer[out];
        out = (out + 1) % BUFFER_SIZE;
        count--;
        printf("[consumer] consumed %d  (buffer now has %d)\n", item, count);

        pthread_cond_signal(&not_full);
        pthread_mutex_unlock(&buffer_lock);

        usleep(120000);    /* consumer is slower than producer — 120 ms */
    }
    return NULL;
}

int main(void) {
    pthread_t p, c;
    pthread_create(&p, NULL, producer, NULL);
    pthread_create(&c, NULL, consumer, NULL);
    pthread_join(p, NULL);
    pthread_join(c, NULL);

    printf("\nAll %d items produced and consumed. No items lost.\n",
           ITEMS_TO_PRODUCE);

    pthread_mutex_destroy(&buffer_lock);
    pthread_cond_destroy(&not_full);
    pthread_cond_destroy(&not_empty);
    return 0;
}

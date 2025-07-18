#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

#define PRODUCERS 3
#define CONSUMERS 3
#define QUEUE_SIZE 10
#define CLOCK_SIZE 3

typedef struct {
    int vector[CLOCK_SIZE];
} VectorClock;

VectorClock queue[QUEUE_SIZE];
int count = 0;

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond_not_full = PTHREAD_COND_INITIALIZER;
pthread_cond_t cond_not_empty = PTHREAD_COND_INITIALIZER;

int producer_id[PRODUCERS] = {0, 1, 2};
int consumer_id[CONSUMERS] = {0, 1, 2};

// Fila circular
int front = 0, rear = 0;

void enqueue(VectorClock clk) {
    queue[rear] = clk;
    rear = (rear + 1) % QUEUE_SIZE;
    count++;
}

VectorClock dequeue() {
    VectorClock clk = queue[front];
    front = (front + 1) % QUEUE_SIZE;
    count--;
    return clk;
}

void print_clock(VectorClock clk, int cid) {
    printf("\033[0;31m[Consumidor %d] Clock: [", cid);
    for (int i = 0; i < CLOCK_SIZE; i++) {
        printf("%d", clk.vector[i]);
        if (i < CLOCK_SIZE - 1) printf(", ");
    }
    printf("]\033[0m\n");
}

void* producer(void* arg) {
    int id = *(int*)arg;
    VectorClock local_clock = {{0, 0, 0}};

    while (1) {
        sleep(1);

        pthread_mutex_lock(&mutex);
        while (count == QUEUE_SIZE)
            pthread_cond_wait(&cond_not_full, &mutex);

        local_clock.vector[id]++;
        enqueue(local_clock);
        printf("\033[0;32m[Produtor %d] Inseriu: [%d, %d, %d]\033[0m\n", id,
            local_clock.vector[0], local_clock.vector[1], local_clock.vector[2]);

        pthread_cond_signal(&cond_not_empty);
        pthread_mutex_unlock(&mutex);
    }
    return NULL;
}

void* consumer(void* arg) {
    int id = *(int*)arg;

    while (1) {
        sleep(3);

        pthread_mutex_lock(&mutex);
        while (count == 0)
            pthread_cond_wait(&cond_not_empty, &mutex);

        VectorClock clk = dequeue();
        print_clock(clk, id);

        pthread_cond_signal(&cond_not_full);
        pthread_mutex_unlock(&mutex);
    }
    return NULL;
}

int main() {
    pthread_t prod[PRODUCERS], cons[CONSUMERS];

    for (int i = 0; i < PRODUCERS; i++)
        pthread_create(&prod[i], NULL, producer, &producer_id[i]);

    for (int i = 0; i < CONSUMERS; i++)
        pthread_create(&cons[i], NULL, consumer, &consumer_id[i]);

    for (int i = 0; i < PRODUCERS; i++)
        pthread_join(prod[i], NULL);
    for (int i = 0; i < CONSUMERS; i++)
        pthread_join(cons[i], NULL);

    return 0;
}

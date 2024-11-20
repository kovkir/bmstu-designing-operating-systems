/*
    Вариант: 0
    Мьютекс: алгоритм Петерсона с барьерной инструкцией
*/

#include <pthread.h>
#include <unistd.h>

#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>


#define NUMBER_OF_ITERATIONS 1000000
#define NUMBER_OF_THREADS 2
#define SUCCESS 0
#define PTHREAD_CREATE_ERROR -1

#define MFENCE() { __asm__("mfence" ::: "memory"); }


int turn;
int flag[NUMBER_OF_THREADS];
int amount = 0;


void lock(int thread_number) {
    flag[thread_number] = 1; // выражает свое желание войти в критическую секцию
    turn = 1 - thread_number; // дает возможность другому потоку войти в критическую секцию

    MFENCE(); // вид барьерной инструкции, которая приказывает компилятору и центральному процессору устанавливать строгую последовательность между обращениями к памяти до и после барьера

    while (flag[1 - thread_number] == 1 && turn == 1 - thread_number);
}


void unlock(int thread_number) {
    flag[thread_number] = 0;
}


void* function_thread(void* arg) {
    int thread_number = *(int*) arg;

    for (int i = 0; i < NUMBER_OF_ITERATIONS; i++) {
        lock(thread_number);
        // начало критической секции
        amount += thread_number + 1;
        // конец критической секции
        unlock(thread_number);
    }

    pthread_exit(0);
}


int main(void) {
    pthread_t threads[NUMBER_OF_THREADS];
    int threads_indexes[NUMBER_OF_THREADS] = {0, 1};

    for (int i = 0; i < NUMBER_OF_THREADS; i++) {
        if (pthread_create(&threads[i], NULL, function_thread, (void *) &threads_indexes[i])) {
            return PTHREAD_CREATE_ERROR;
        }
    }
    for (int i = 0; i < NUMBER_OF_THREADS; i++) {
        pthread_join(threads[i], NULL);
    }

    printf("amount = %d\n", amount);

    return SUCCESS;
}

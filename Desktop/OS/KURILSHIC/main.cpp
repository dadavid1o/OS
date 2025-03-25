#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <time.h>

int available_item = -1;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond = PTHREAD_COND_INITIALIZER;
pthread_mutex_t cout_mutex = PTHREAD_MUTEX_INITIALIZER;

void print(const char* msg) {
    pthread_mutex_lock(&cout_mutex);
    printf("%s\n", msg);
    pthread_mutex_unlock(&cout_mutex);
}

void* smoker_odin(void* arg) {
    while(1) {
        pthread_mutex_lock(&mutex);
        while(available_item != 0)
            pthread_cond_wait(&cond, &mutex);

        available_item = -1;
        print("Курильщик odin: получил табак и курит");
        pthread_cond_signal(&cond);
        pthread_mutex_unlock(&mutex);
        sleep(1);
    }
    return NULL;
}

void* smoker_dva(void* arg) {
    while(1) {
        pthread_mutex_lock(&mutex);
        while(available_item != 1)
            pthread_cond_wait(&cond, &mutex);

        available_item = -1;
        print("Курильщик dva: получил бумагу и курит");
        pthread_cond_signal(&cond);
        pthread_mutex_unlock(&mutex);
        sleep(1);
    }
    return NULL;
}

void* smoker_tri(void* arg) {
    while(1) {
        pthread_mutex_lock(&mutex);
        while(available_item != 2)
            pthread_cond_wait(&cond, &mutex);

        available_item = -1;
        print("Курильщик tri: получил спички и курит");
        pthread_cond_signal(&cond);
        pthread_mutex_unlock(&mutex);
        sleep(1);
    }
    return NULL;
}

void* bartender(void* arg) {
    srand(time(NULL));
    while(1) {
        int item = rand() % 3;

        pthread_mutex_lock(&mutex);
        while(available_item != -1)
            pthread_cond_wait(&cond, &mutex);

        available_item = item;
        char msg[50];
        snprintf(msg, sizeof(msg), "Бармен положил: %d", item);
        print(msg);

        pthread_cond_broadcast(&cond);
        pthread_mutex_unlock(&mutex);
        usleep(500000);
    }
    return NULL;
}

int main() {
    pthread_t tids[4];

    pthread_create(&tids[0], NULL, smoker_odin, NULL);
    pthread_create(&tids[1], NULL, smoker_dva, NULL);
    pthread_create(&tids[2], NULL, smoker_tri, NULL);
    pthread_create(&tids[3], NULL, bartender, NULL);

    for(int i = 0; i < 4; i++) {
        pthread_join(tids[i], NULL);
    }

    pthread_mutex_destroy(&mutex);
    pthread_cond_destroy(&cond);
    pthread_mutex_destroy(&cout_mutex);
    return 0;
}
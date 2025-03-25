#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <time.h>

int table_item = -1;          // -1 = пусто, 0 = табак, 1 = бумага, 2 = спички
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond = PTHREAD_COND_INITIALIZER;
pthread_mutex_t print_mutex = PTHREAD_MUTEX_INITIALIZER;

// вывод
void print(const char* msg, int id) {
    pthread_mutex_lock(&print_mutex);
    const char* names[] = {"si <нужен табак>", "pi <нужна бумага>", "ri  <нужны спички>"};
    printf("[%s] %s\n", names[id], msg);
    pthread_mutex_unlock(&print_mutex);
}

void* smoker(void* arg) {
    int id = *(int*)arg;
    const int needed_item = id;

    while(1) {
        pthread_mutex_lock(&mutex);

        while(table_item != needed_item || table_item == -1) {
            pthread_cond_wait(&cond, &mutex);
        }

        table_item = -1;
        print("начал курить", id);
        pthread_mutex_unlock(&mutex);


        sleep(1 + rand() % 2);

        print("закончил курить", id);
        pthread_cond_signal(&cond);
    }
    return NULL;
}


void* bartender(void* arg) {
    const char* items[] = {"табак", "бумага", "спички"};

    while(1) {
        pthread_mutex_lock(&mutex);

        while(table_item != -1) {
            pthread_cond_wait(&cond, &mutex);
        }

        // рандомно от 0-2
        table_item = rand() % 3;
        printf("\nБармен положил: %s\n", items[table_item]);

        pthread_cond_broadcast(&cond);
        pthread_mutex_unlock(&mutex);

        usleep(500000);
    }
    return NULL;
}

int main() {
    srand(time(NULL));
    pthread_t smokers[3], bartender_thread;
    int ids[] = {0, 1, 2};

    pthread_create(&bartender_thread, NULL, bartender, NULL);
    for(int i = 0; i < 3; i++) {
        pthread_create(&smokers[i], NULL, smoker, &ids[i]);
    }

    pthread_join(bartender_thread, NULL);
    for(int i = 0; i < 3; i++) {
        pthread_join(smokers[i], NULL);
    }

    pthread_mutex_destroy(&mutex);
    pthread_cond_destroy(&cond);
    pthread_mutex_destroy(&print_mutex);

    return 0;
}
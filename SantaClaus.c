#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>

int reindeer_num = 9;
int elves_num = 10;
sem_t santa;
sem_t reindeer;
sem_t elf;
sem_t reindeer_full;
sem_t elf_full;
sem_t santa_mutex;
sem_t reindeer_mutex;
sem_t elf_mutex;

int reindeer_count = 0;
int elf_count = 0;

void *SantaClaus() {
    while (1) {
        sleep(1);
        if (reindeer_count == 9) {
            printf("Santa Claus is delivering gifts\n");
            sleep(1);

            sem_post(&reindeer);
        }
        if (elf_count == 3) {
            printf("Santa Claus is helping elves\n");
            sleep(1);

            sem_post(&elf);
        }
    }
}
void *Reindeer(void *arg) {
    int id = *(int *)arg;
    while (1) {
        sleep(rand() % 5 + 1);
        printf("Reindeer %d is back\n", id);

        sem_wait(&reindeer_mutex);

        // 如果驯鹿数量达到9，则不能再有驯鹿返回
        sem_wait(&reindeer_full);
        sem_post(&reindeer_full);

        printf("Reindeer %d is waiting for Santa Claus\n", id);

        reindeer_count++;
        if (reindeer_count == 9) {
            sem_wait(&reindeer_full);
        }

        sem_post(&reindeer_mutex);

        sem_wait(&reindeer);
        reindeer_count--;
        if (reindeer_count == 0) {
            sem_post(&reindeer_full);
        } else
            sem_post(&reindeer);
    }
}

void *Elf(void *arg) {
    int id = *(int *)arg;
    while (1) {
        sleep(rand() % 5 + 1);
        printf("Elf %d needs help\n", id);

        sem_wait(&elf_mutex);

        // 如果精灵数量达到3，则不能再有精灵请求帮助
        sem_wait(&elf_full);
        sem_post(&elf_full);

        printf("Elf %d is waiting for help\n", id);

        elf_count++;
        if (elf_count == 3) {
            sem_wait(&elf_full);
        }

        sem_post(&elf_mutex);

        sem_wait(&elf);
        elf_count--;
        if (elf_count == 0) {
            sem_post(&elf_full);
        } else
            sem_post(&elf);
    }
}
int main() {
    // 初始化信号量
    sem_init(&santa, 0, 0);
    sem_init(&reindeer, 0, 0);
    sem_init(&elf, 0, 0);
    sem_init(&santa_mutex, 0, 1);
    sem_init(&reindeer_mutex, 0, 1);
    sem_init(&elf_mutex, 0, 1);
    sem_init(&reindeer_full, 0, 1);
    sem_init(&elf_full, 0, 1);

    // 进程id
    pthread_t santa_id;
    pthread_t reindeer_id[20];
    pthread_t elf_id[20];

    // 创建圣诞老人线程
    pthread_create(&santa_id, NULL, SantaClaus, NULL);

    // 创建驯鹿线程
    for (int i = 0; i < reindeer_num; i++) {
        reindeer_id[i] = i;
        int *arg = malloc(sizeof(*arg));
        *arg = i;
        pthread_create(&reindeer_id[i], NULL, Reindeer, arg);
    }

    // 创建精灵线程
    for (int i = 0; i < elves_num; i++) {
        elf_id[i] = i;
        int *arg = malloc(sizeof(*arg));
        *arg = i;
        pthread_create(&elf_id[i], NULL, Elf, arg);
    }

    // 等待线程结束
    pthread_join(santa_id, NULL);
    for (int i = 0; i < reindeer_num; i++) {
        pthread_join(reindeer_id[i], NULL);
    }
    for (int i = 0; i < elves_num; i++) {
        pthread_join(elf_id[i], NULL);
    }

    // 销毁信号量
    sem_destroy(&santa);
    sem_destroy(&reindeer);
    sem_destroy(&elf);
    sem_destroy(&santa_mutex);
    sem_destroy(&reindeer_mutex);
    sem_destroy(&elf_mutex);
    sem_destroy(&reindeer_full);
    sem_destroy(&elf_full);
}
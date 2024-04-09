// * purpose:
// * To simulate a burger shop with chefs, cashiers, and customers.
// * The chefs make burgers and put them on the rack.
// * The cashiers take orders from customers and give them burgers.
// * The customers come and get burgers.
// * The program will end when all customers have got burgers.
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>

// 设置参数
int chefs_num = 3;
int max_burgers = 4;
int cashier_num = 2;
int customers_num = 8;

// 计数用于程序的结束
int shared_cnt = 0;

// 设置信号量
sem_t chef_order;
sem_t rack_is_full;
sem_t rack_is_empty;
sem_t customer_ready;
sem_t cashier_ready;
sem_t exit_mutex;

// 厨师线程
void *chef(void *arg) {
    int id = *(int *)arg;
    while (1) {
        sem_wait(&chef_order);
        sem_wait(&rack_is_full);
        sem_post(&chef_order);
        printf("Chef %d is making a burger\n", id);
        sleep(1);
        sem_post(&rack_is_empty);
    }
    return NULL;
}

// 收银员线程
void *cashier(void *arg) {
    int id = *(int *)arg;
    while (1) {
        sem_wait(&customer_ready);
        printf("Cashier %d is taking an order\n", id);
        sleep(1);
        sem_wait(&rack_is_empty);
        printf("Cashier %d is giving a burger\n", id);
        sleep(1);
        sem_post(&rack_is_full);
        sem_post(&cashier_ready);
    }
    return NULL;
}

// 顾客线程
void *customer(void *arg) {
    int id = *(int *)arg;

    printf("Customer %d comes\n", id);
    sem_post(&customer_ready);
    sleep(1);
    sem_wait(&cashier_ready);
    printf("Customer %d gets a burger\n", id);
    sleep(1);
    sem_wait(&exit_mutex);
    shared_cnt++;
    if (shared_cnt == customers_num) {
        exit(0);
    }
    sem_post(&exit_mutex);
    return NULL;
}
int main(int argc, char *argv[]) {
    if (argc == 5) {
        chefs_num = atoi(argv[1]);
        max_burgers = atoi(argv[2]);
        cashier_num = atoi(argv[3]);
        customers_num = atoi(argv[4]);
    }
    if (argc == 4) {
        chefs_num = atoi(argv[1]);
        max_burgers = atoi(argv[2]);
        cashier_num = atoi(argv[3]);
    }
    if (argc == 3) {
        chefs_num = atoi(argv[1]);
        max_burgers = atoi(argv[2]);
    }
    if (argc == 2) {
        chefs_num = atoi(argv[1]);
    }

    if (chefs_num < 1 || chefs_num > 20 || max_burgers < 1 || max_burgers > 20 || cashier_num < 1 || cashier_num > 20 || customers_num < 1 || customers_num > 20) {
        printf("Invalid input\n");
        return 0;
    }
    // 进程id
    pthread_t chefs[20];
    pthread_t cashiers[20];
    pthread_t customers[20];

    // 初始化信号量
    sem_init(&rack_is_full, 0, max_burgers);
    sem_init(&rack_is_empty, 0, 0);
    sem_init(&customer_ready, 0, 0);
    sem_init(&cashier_ready, 0, 0);
    sem_init(&chef_order, 0, 1);
    sem_init(&exit_mutex, 0, 1);

    // 创建厨师线程
    for (int i = 0; i < chefs_num; i++) {
        chefs[i] = i;
        int *arg = malloc(sizeof(*arg));
        *arg = i;
        pthread_create(&chefs[i], NULL, chef, arg);
    }

    // 创建收银员线程
    for (int i = 0; i < cashier_num; i++) {
        cashiers[i] = i;
        int *arg = malloc(sizeof(*arg));
        *arg = i;
        pthread_create(&cashiers[i], NULL, cashier, arg);
    }

    // 创建顾客线程
    for (int i = 0; i < customers_num; i++) {
        customers[i] = i;
        int *arg = malloc(sizeof(*arg));
        *arg = i;
        pthread_create(&customers[i], NULL, customer, arg);
    }

    // 等待线程结束
    for (int i = 0; i < chefs_num; i++) {
        pthread_join(chefs[i], NULL);
    }
    for (int i = 0; i < cashier_num; i++) {
        pthread_join(cashiers[i], NULL);
    }
    for (int i = 0; i < customers_num; i++) {
        pthread_join(customers[i], NULL);
    }

    // 销毁信号量
    sem_destroy(&rack_is_full);
    sem_destroy(&rack_is_empty);
    sem_destroy(&customer_ready);
    sem_destroy(&cashier_ready);
    sem_destroy(&chef_order);
    sem_destroy(&exit_mutex);
    return 0;
}
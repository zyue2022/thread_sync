#include <pthread.h>
#include <stdio.h>
#include <unistd.h>

/*
    简易读写锁运用
*/

// 共享数据
int num = 0;

// 读写锁保证数据安全
pthread_rwlock_t rwlock;

void* writeNum(void* arg) {
    while (1) {
        pthread_rwlock_wrlock(&rwlock);
        printf("写线程：%ld, num = %d\n", pthread_self(), ++num);
        pthread_rwlock_unlock(&rwlock);
        usleep(1000);
    }
}

void* readNum(void* arg) {
    while (1) {
        pthread_rwlock_rdlock(&rwlock);
        printf("读线程：%ld, num = %d\n", pthread_self(), num);
        pthread_rwlock_unlock(&rwlock);
        usleep(1000);
    }
}

int main() {
    // 读写锁初始化
    pthread_rwlock_init(&rwlock, NULL);

    // 创建3个写线程
    pthread_t wtids[3];
    for (int i = 0; i < 3; ++i) {
        pthread_create(&wtids[i], NULL, writeNum, NULL);
    }

    // 创建5个读线程
    pthread_t rtids[5];
    for (int i = 0; i < 5; ++i) {
        pthread_create(&rtids[i], NULL, readNum, NULL);
    }

    // 读线程分离
    for (int i = 0; i < 3; ++i) {
        pthread_detach(wtids[i]);
    }

    // 写线程分离
    for (int i = 0; i < 5; ++i) {
        pthread_detach(rtids[i]);
    }

    pthread_exit(NULL);

    pthread_rwlock_destroy(&rwlock);

    return 0;
}
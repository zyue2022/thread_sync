#include <pthread.h>
#include <stdio.h>
#include <unistd.h>

/*
    简易互斥锁运用之多线程买票
*/


// 票数
int tickets = 100;

// 创建互斥量
pthread_mutex_t mutex;

void* sell(void* arg) {
    // 买票
    while (1) {
        // 加锁
        pthread_mutex_lock(&mutex);
        if (tickets > 0) {
            usleep(6000);
            printf("这里是线程%ld, 正在卖第%d张票...\n", pthread_self(), tickets);
            --tickets;
        } else {
            // 解锁
            pthread_mutex_unlock(&mutex);
            break;
        }
        pthread_mutex_unlock(&mutex);
    }
    return NULL;
}

int main() {
    // 初始化互斥量
    pthread_mutex_init(&mutex, NULL);

    // 1.创建3个子线程
    pthread_t tid1, tid2, tid3;
    pthread_create(&tid1, NULL, sell, NULL);
    pthread_create(&tid2, NULL, sell, NULL);
    pthread_create(&tid3, NULL, sell, NULL);

    // 2.回收子线程资源，是阻塞的
    pthread_join(tid1, NULL);
    pthread_join(tid2, NULL);
    pthread_join(tid3, NULL);

    // 3.退出主线程
    pthread_exit(NULL);

    // 释放互斥量
    pthread_mutex_destroy(&mutex);

    return 0;
}

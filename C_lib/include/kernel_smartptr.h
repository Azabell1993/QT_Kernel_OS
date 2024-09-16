#pragma once
#ifndef KERNEL_SMARTPTR_H
#define KERNEL_SMARTPTR_H

// 기존 코드
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <unistd.h>
#include <errno.h>
#include <semaphore.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <stdbool.h>
#include <stdarg.h>
#include <dlfcn.h>

#include "kernel_engine.h"
#include "../src/ename.c.inc"

#define NUM_THREADS 3
#define MAX_STRING_SIZE 100

typedef struct SmartPtr SmartPtr;
#define CREATE_SMART_PTR(type, ...) create_smart_ptr(sizeof(type), __VA_ARGS__)

static void retain(SmartPtr *sp);
static void release(SmartPtr *sp);
static void* thread_function(void* arg);

// static pthread_mutex_t print_mutex = PTHREAD_MUTEX_INITIALIZER;

/**
 * @struct SmartPtr
 * @brief 스마트 포인터 구조체
 *
 * 이 구조체는 포인터와 참조 카운트, 뮤텍스를 관리합니다.
 */
typedef struct SmartPtr {
    void *ptr;                ///< 실제 메모리를 가리킴
    int *ref_count;           ///< 참조 카운트
    pthread_mutex_t *mutex;   ///< 뮤텍스 보호
} SmartPtr;

/**
 * @brief 네트워크 정보를 저장하는 구조체
 */
typedef struct NetworkInfo {
    char ip[INET_ADDRSTRLEN];  ///< IPv4 주소
    sa_family_t family;        ///< 주소 패밀리 (AF_INET 등)
} NetworkInfo;

/**
 * @brief 로컬 네트워크 정보를 가져오는 함수
 *
 * @return NetworkInfo 로컬 네트워크 정보가 저장된 구조체
 */
NetworkInfo get_local_network_info() {
    struct addrinfo hints, *res;
    NetworkInfo net_info;

    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE;

    char hostname[256];
    gethostname(hostname, sizeof(hostname));
    if (getaddrinfo(hostname, NULL, &hints, &res) != 0) {
        perror("getaddrinfo 실패");
        exit(EXIT_FAILURE);
    }

    struct sockaddr_in *ipv4 = (struct sockaddr_in *)res->ai_addr;
    inet_ntop(AF_INET, &(ipv4->sin_addr), net_info.ip, INET_ADDRSTRLEN);
    net_info.family = res->ai_family;

    freeaddrinfo(res);
    return net_info;
}

/**
 * @brief 스마트 포인터를 생성하는 함수 (가변 인자 사용)
 *
 * @param size 할당할 메모리 크기
 * @param ... 가변 인자 리스트 (초기값)
 * @return SmartPtr 스마트 포인터 구조체
 */
SmartPtr create_smart_ptr(size_t size, ...) {
    (void)size;
    SmartPtr sp;
    sp.ptr = malloc(size);
    sp.ref_count = (int *)malloc(sizeof(int));
    *(sp.ref_count) = 1;
    sp.mutex = (pthread_mutex_t *)malloc(sizeof(pthread_mutex_t));
    pthread_mutex_init(sp.mutex, NULL);

    va_list args;
    va_start(args, size);

    if (size == sizeof(int)) {
        int value = va_arg(args, int);
        *(int *)sp.ptr = value;
    } else if (size == sizeof(char) * MAX_STRING_SIZE) {
        const char *str = va_arg(args, const char *);
        strncpy((char *)sp.ptr, str, MAX_STRING_SIZE);
    }

    va_end(args);
    return sp;
}

/**
 * @brief 스마트 포인터의 참조 카운트를 증가시키는 함수
 *
 * @param sp 증가시킬 스마트 포인터
 */
static void retain(SmartPtr *sp) {
    pthread_mutex_lock(sp->mutex);
    (*(sp->ref_count))++;
    pthread_mutex_unlock(sp->mutex);
}

/**
 * @brief 스마트 포인터의 참조 카운트를 감소시키고 필요시 메모리를 해제하는 함수
 *
 * @param sp 해제할 스마트 포인터
 */
static void release(SmartPtr *sp) {
    int should_free = 0;

    pthread_mutex_lock(sp->mutex);
    (*(sp->ref_count))--;
    safe_kernel_printf("Smart pointer released (ref_count: %d)\n", *(sp->ref_count));

    if (*(sp->ref_count) == 0) {
        should_free = 1;
        safe_kernel_printf("Reference count is 0, freeing memory...\n");
    }

    pthread_mutex_unlock(sp->mutex);

    if (should_free) {
        free(sp->ptr);
        sp->ptr = NULL;
        free(sp->ref_count);
        sp->ref_count = NULL;

        pthread_mutex_destroy(sp->mutex);
        free(sp->mutex);
        sp->mutex = NULL;

        safe_kernel_printf("Memory has been freed\n");
    }
}

/**
 * @brief 스레드에서 수행할 함수
 *
 * @param arg 스레드 인수 (스레드 번호)
 * @return NULL
 */
void* thread_function(void* arg) {
    int thread_num = *((int*)arg);

    NetworkInfo net_info = get_local_network_info();

    safe_kernel_printf("Thread %d: 시작 - 로컬 IP 주소: %s\n", thread_num, net_info.ip);

    sleep(1);

    safe_kernel_printf("Thread %d: 종료 - 주소 패밀리: %d\n", thread_num, net_info.family);
    return NULL;
}


#endif  // KERNEL_SMARTPTR_H
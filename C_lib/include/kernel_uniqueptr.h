#pragma once
#ifndef KERNEL_UNIQUEPTR_H
#define KERNEL_UNIQUEPTR_H

#include <dlfcn.h>
#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/wait.h>
#include <semaphore.h>
#include <stdbool.h>
#include <netdb.h>
#include <arpa/inet.h>
#include "kernel_print.h"
#include "kernel_engine.h"

#include "../src/ename.c.inc"  // 파일의 상대 경로로 수정


#define BUF_SIZE 100
#define NUM_THREADS 3
#define MAX_STRING_SIZE 100

#define RETAIN_SHARED_PTR(ptr) retain_shared_ptr(ptr);
#define RELEASE_SHARED_PTR(ptr) release_shared_ptr(ptr);

/**
 * @brief 기본 소멸자 함수 (free 사용)
 *
 * @param ptr 해제할 포인터
 */
void default_deleter(void *ptr) {
    free(ptr);
}

/**
 * @struct SharedPtr
 * @brief 공유 스마트 포인터
 */
typedef struct {
    void *ptr;               ///< 실제 메모리
    int *ref_count;          ///< 참조 카운트
    pthread_mutex_t *mutex;  ///< 뮤텍스
    void (*deleter)(void*);  ///< 소멸자 함수
} SharedPtr;

/**
 * @struct UniquePtr
 * @brief 고유 스마트 포인터
 */
typedef struct {
    void *ptr;               ///< 실제 메모리
    void (*deleter)(void*);  ///< 소멸자 함수
} UniquePtr;

/**
 * @brief shared_ptr 생성 함수
 *
 * @param size 할당할 메모리 크기
 * @param deleter 소멸자 함수
 * @return 생성된 SharedPtr 구조체
 */
SharedPtr create_shared_ptr(size_t size, void (*deleter)(void*)) {
    SharedPtr sp;
    sp.ptr = malloc(size);
    sp.ref_count = (int*)malloc(sizeof(int));
    *(sp.ref_count) = 1;
    sp.mutex = (pthread_mutex_t*)malloc(sizeof(pthread_mutex_t));
    sp.deleter = deleter ? deleter : default_deleter;
    pthread_mutex_init(sp.mutex, NULL);

    return sp;
}

/**
 * @brief unique_ptr 생성 함수
 *
 * @param size 할당할 메모리 크기
 * @param deleter 소멸자 함수
 * @return 생성된 UniquePtr 구조체
 */
UniquePtr create_unique_ptr(size_t size, void (*deleter)(void*)) {
    UniquePtr up;
    up.ptr = malloc(size);
    up.deleter = deleter ? deleter : default_deleter;
    return up;
}

/**
 * @brief shared_ptr 참조 카운트 증가
 *
 * @param sp 참조할 SharedPtr
 */
void retain_shared_ptr(SharedPtr *sp) {
    pthread_mutex_lock(sp->mutex);
    (*(sp->ref_count))++;
    pthread_mutex_unlock(sp->mutex);
}

/**
 * @brief shared_ptr 참조 카운트 감소 및 메모리 해제
 *
 * @param sp 해제할 SharedPtr
 */
void release_shared_ptr(SharedPtr *sp) {
    if (sp->ptr == NULL) {
        safe_kernel_printf("SharedPtr is already released\n");
        return;
    }

    sp->deleter(sp->ptr);
    sp->ptr = NULL;

    free(sp->ref_count);
    sp->ref_count = NULL;
    pthread_mutex_destroy(sp->mutex);
    free(sp->mutex);
    sp->mutex = NULL;
}

/**
 * @brief unique_ptr 메모리 해제
 *
 * @param up 해제할 UniquePtr
 */
void release_unique_ptr(UniquePtr *up) {
    if (up->ptr) {
        up->deleter(up->ptr);
        up->ptr = NULL;
    }
}

/**
 * @brief unique_ptr 소유권 이전
 *
 * @param up 소유권을 이전할 UniquePtr
 * @return 새로 생성된 UniquePtr
 */
UniquePtr transfer_unique_ptr(UniquePtr *up) {
    UniquePtr new_up = *up;
    up->ptr = NULL;
    return new_up;
}

/**
 * @brief 스레드 함수 (shared_ptr 사용)
 *
 * @param arg SharedPtr 인수
 * @return NULL
 */
void* thread_function_shared(void* arg) {
    SharedPtr *sp = (SharedPtr*)arg;
    retain_shared_ptr(sp);
    printf("스레드에서 shared_ptr 사용 중 - ref_count: %d\n", *(sp->ref_count));

    sleep(1);

    release_shared_ptr(sp);
    return NULL;
}

/**
 * @brief 스레드 함수 (unique_ptr 사용)
 *
 * @param arg UniquePtr 인수
 * @return NULL
 */
void* thread_function_unique(void* arg) {
    (void)arg; // Ignore the unused variable
    printf("스레드에서 unique_ptr 사용 중\n");

    sleep(1);
    return NULL;
}

#endif // KERNEL_UNIQUEPTR_H
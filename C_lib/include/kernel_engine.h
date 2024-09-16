/*
 * Advanced Kernel Engine Implementation
 *
 * Maintainer: Azabell1993 Github master
 * Created   : 2024-09-04
 *
 * Purpose   : Implements advanced kernel operations, including
 *             thread/process management, error handling, synchronization
 *             mechanisms, and data structures.
 */

#pragma once
#ifndef ENGINE_H
#define ENGINE_H

#include <pthread.h>
#include <semaphore.h>
#include <stdarg.h>
#include <stdbool.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief 전역 뮤텍스 변수
 * 
 * 출력 시 스레드 동기화를 위해 사용됩니다.
 */
static pthread_mutex_t print_mutex = PTHREAD_MUTEX_INITIALIZER;

/**
 * @brief 스레드 안전한 출력 함수 선언
 * 
 * @param format 포맷 문자열
 * @param ... 가변 인자 리스트
 */
void safe_kernel_printf(const char *format, ...);

/**
 * @struct Node
 * @brief 연결 리스트의 노드를 정의하는 구조체
 * 
 * 각 노드는 데이터를 저장하고, 다음 노드를 가리킵니다.
 */
typedef struct Node {
    void* data;           /**< 데이터를 가리키는 포인터 */
    struct Node* next;    /**< 다음 노드를 가리키는 포인터 */
} Node;

/**
 * @struct LinkedList
 * @brief 연결 리스트를 정의하는 구조체
 * 
 * 연결 리스트의 헤드, 테일, 크기를 포함합니다.
 */
typedef struct LinkedList {
    Node* head;           /**< 연결 리스트의 첫 번째 노드를 가리키는 포인터 */
    Node* tail;           /**< 연결 리스트의 마지막 노드를 가리키는 포인터 */
    int size;             /**< 연결 리스트의 크기 */
} LinkedList;

/**
 * @brief 연결 리스트 생성 함수 선언
 * 
 * @return 생성된 연결 리스트의 포인터
 */
LinkedList* create_linkedlist();

/**
 * @brief 연결 리스트에 요소 추가 함수 선언
 * 
 * @param list 연결 리스트의 포인터
 * @param data 추가할 데이터 포인터
 */
void push(LinkedList* list, void* data);

/**
 * @brief 연결 리스트에서 요소 제거 함수 선언
 * 
 * @param list 연결 리스트의 포인터
 * @return 제거된 데이터 포인터
 */
void* pop(LinkedList* list);

/**
 * @brief 연결 리스트가 비었는지 확인하는 함수 선언
 * 
 * @param list 연결 리스트의 포인터
 * @return true 리스트가 비었을 경우
 * @return false 리스트에 요소가 있을 경우
 */
bool is_empty(LinkedList* list);

/**
 * @brief 연결 리스트 삭제 함수 선언
 * 
 * @param list 연결 리스트의 포인터
 */
void destroy_linkedlist(LinkedList* list);

/**
 * @brief 스레드 생성 함수 선언
 * 
 * @param num_threads 생성할 스레드 수
 * @param ... 스레드 함수 포인터
 */
void create_threads(int num_threads, ...);

/**
 * @brief 단일 프로세스 생성 함수 선언
 * 
 * @param func 실행할 함수 포인터
 */
void create_single_process(void (*func)());

/**
 * @brief 다중 프로세스 생성 함수 선언
 * 
 * @param num_processes 생성할 프로세스 수
 * @param ... 프로세스 함수 포인터
 */
void create_multi_processes(int num_processes, ...);

/**
 * @brief 세마포어 초기화 함수 선언
 * 
 * @param value 초기화할 값
 * @return 세마포어 포인터
 */
sem_t* init_semaphore(int value);

/**
 * @brief 뮤텍스 초기화 함수 선언
 * 
 * @return 뮤텍스 포인터
 */
pthread_mutex_t* init_mutex();

/**
 * @brief 커널 오류 메시지 출력 함수 선언
 * 
 * @param format 포맷 문자열
 * @param ... 가변 인자 리스트
 */
void kernel_errMsg(const char *format, ...);

/**
 * @brief 커널 오류 종료 함수 선언
 * 
 * @param format 포맷 문자열
 * @param ... 가변 인자 리스트
 */
void kernel_errExit(const char *format, ...);

/**
 * @brief 커널 오류 종료 함수 선언 (오류 번호 사용)
 * 
 * @param errnum 오류 번호
 * @param format 포맷 문자열
 * @param ... 가변 인자 리스트
 */
void kernel_errExitEN(int errnum, const char *format, ...);

/**
 * @brief 커널 치명적 오류 함수 선언
 * 
 * @param format 포맷 문자열
 * @param ... 가변 인자 리스트
 */
void kernel_fatal(const char *format, ...);

/**
 * @brief 커널 사용법 오류 함수 선언
 * 
 * @param format 포맷 문자열
 * @param ... 가변 인자 리스트
 */
void kernel_usageErr(const char *format, ...) __attribute__ ((__noreturn__));

/**
 * @brief 커맨드 라인 오류 처리 함수 선언
 * 
 * @param format 포맷 문자열
 * @param ... 가변 인자 리스트
 */
void kernel_cmdLineErr(const char *format, ...) __attribute__ ((__noreturn__));

/**
 * @brief 세마포어를 사용하는 스레드 작업 함수 선언
 * 
 * @param arg 세마포어 포인터
 * @return NULL
 */
void* semaphore_thread(void* arg);

/**
 * @brief 뮤텍스를 사용하는 스레드 작업 함수 선언
 * 
 * @param arg 뮤텍스 포인터
 * @return NULL
 */
void* mutex_thread(void* arg);

/**
 * @brief 멀티스레드 실행 함수 선언 (쓰레드 수 및 동기화 방법을 입력받음)
 * 
 * @param num_threads 생성할 스레드 수
 * @param use_semaphore 세마포어 사용 여부
 * @param ... 동기화 방법 (세마포어 또는 뮤텍스)
 */
void run_multithreading(int num_threads, int use_semaphore, ...);

#ifdef __cplusplus
}
#endif

#endif // ENGINE_H

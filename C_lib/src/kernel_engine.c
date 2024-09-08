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

#include "kernel_engine.h"
#include "kernel_print.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <sys/wait.h>

#include "kernel_engine.h"

void safe_kernel_printf(const char *format, ...) {
    va_list args;
    va_start(args, format);

    pthread_mutex_lock(&print_mutex);  // 출력 뮤텍스 잠금
    vprintf(format, args);  // 커널 printf 함수 호출
    fflush(stdout);
    pthread_mutex_unlock(&print_mutex);  // 출력 뮤텍스 해제

    va_end(args);
}

// Smart pointer creation function
SmartPtr create_smart_ptr(void *ptr) {
    SmartPtr sp;
    sp.ptr = ptr;
    sp.ref_count = (int *)malloc(sizeof(int));
    if (sp.ref_count == NULL) {
        perror("Failed to allocate memory for ref_count");
        exit(EXIT_FAILURE);
    }
    *(sp.ref_count) = 1;
    sp.mutex = (pthread_mutex_t *)malloc(sizeof(pthread_mutex_t));
    if (sp.mutex == NULL) {
        perror("Failed to allocate memory for mutex");
        free(sp.ref_count);
        exit(EXIT_FAILURE);
    }
    pthread_mutex_init(sp.mutex, NULL);
    return sp;
}

// Smart pointer retain (increase reference count) function
void retain(SmartPtr *sp) {
    pthread_mutex_lock(sp->mutex);
    (*(sp->ref_count))++;
    pthread_mutex_unlock(sp->mutex);
}

// Smart pointer release (decrease reference count and free memory) function
void release(SmartPtr *sp) {
    int should_free = 0;

    pthread_mutex_lock(sp->mutex);
    (*(sp->ref_count))--;
    if (*(sp->ref_count) == 0) {
        should_free = 1;
    }
    pthread_mutex_unlock(sp->mutex);

    if (should_free) {
        free(sp->ptr);
        free(sp->ref_count);
        pthread_mutex_destroy(sp->mutex);
        free(sp->mutex);
    }
}


// Thread creation function
void create_threads(int num_threads, ...) {
    pthread_t* threads = (pthread_t*)malloc(num_threads * sizeof(pthread_t));
    va_list args;
    va_start(args, num_threads);

    for (int i = 0; i < num_threads; i++) {
        void* (*thread_func)(void*) = va_arg(args, void* (*)(void*));
        int err = pthread_create(&threads[i], NULL, thread_func, NULL);
        if (err != 0) {
            kernel_errExitEN(err, "Failed to create thread %d", i);
        }
    }

    for (int i = 0; i < num_threads; i++) {
        pthread_join(threads[i], NULL);
    }

    va_end(args);
    free(threads);
}

// Single process creation function
void create_single_process(void (*func)()) {
    pid_t pid = fork();
    if (pid < 0) {
        kernel_errExit("Failed to fork process");
    } else if (pid == 0) {
        func();
        exit(EXIT_SUCCESS);
    } else {
        wait(NULL);
    }
}

// Multi-process creation function
void create_multi_processes(int num_processes, ...) {
    va_list args;
    va_start(args, num_processes);

    for (int i = 0; i < num_processes; i++) {
        pid_t pid = fork();
        if (pid < 0) {
            kernel_errExit("Failed to fork process %d", i);
        } else if (pid == 0) {
            void (*process_func)() = va_arg(args, void (*)());
            process_func();
            exit(EXIT_SUCCESS);
        }
    }

    for (int i = 0; i < num_processes; i++) {
        wait(NULL);
    }

    va_end(args);
}

// Network tcp function
void create_network_tcp_process(int num_tcp_proc, ...) {
    va_list args;
    va_start(args, num_tcp_proc);
    
    // tcp를 주고 받는 정도
    // 잘 구현되면 확장

    va_end(args);
}

// Semaphore initialization function
sem_t* init_semaphore(int value) {
    sem_t* sem = (sem_t*)malloc(sizeof(sem_t));
    if (sem == NULL) {
        perror("Failed to allocate memory for semaphore");
        exit(EXIT_FAILURE);
    }

    #ifdef __APPLE__
        // macOS에서는 named semaphore 사용
        char sem_name[20];
        snprintf(sem_name, sizeof(sem_name), "/semaphore_%d", getpid());
        sem_unlink(sem_name);  // 기존 세마포어 삭제 (중복 방지)
        sem = sem_open(sem_name, O_CREAT, 0644, value);
        if (sem == SEM_FAILED) {
            perror("Failed to initialize semaphore (macOS)");
            free(sem);
            return NULL;
        }
    #else
        // Linux에서는 unnamed semaphore 사용
        if (sem_init(sem, 0, value) != 0) {
            perror("Failed to initialize semaphore (Linux)");
            free(sem);
            return NULL;
        }
    #endif

    return sem;
}

// Mutex initialization function
pthread_mutex_t* init_mutex() {
    pthread_mutex_t* mutex = (pthread_mutex_t*)malloc(sizeof(pthread_mutex_t));
    if (pthread_mutex_init(mutex, NULL) != 0) {
        kernel_errExit("Failed to initialize mutex");
    }
    return mutex;
}

// Linked list creation function
LinkedList* create_linkedlist() {
    LinkedList* list = (LinkedList*)malloc(sizeof(LinkedList));
    list->head = list->tail = NULL;
    list->size = 0;
    return list;
}

// Linked list element addition function
void push(LinkedList* list, void* data) {
    Node* new_node = (Node*)malloc(sizeof(Node));
    new_node->data = data;
    new_node->next = NULL;

    if (is_empty(list)) {
        list->head = list->tail = new_node;
    } else {
        list->tail->next = new_node;
        list->tail = new_node;
    }
    list->size++;
}

// Linked list element removal function
void* pop(LinkedList* list) {
    if (is_empty(list)) {
        kernel_errMsg("Attempt to pop from an empty list");
        return NULL;
    }

    Node* temp = list->head;
    void* data = temp->data;
    list->head = list->head->next;

    if (list->head == NULL) {
        list->tail = NULL;
    }

    free(temp);
    list->size--;
    return data;
}

// Linked list empty check function
bool is_empty(LinkedList* list) {
    return list->size == 0;
}

// Linked list destruction function
void destroy_linkedlist(LinkedList* list) {
    while (!is_empty(list)) {
        pop(list);
    }
    free(list);
}

// 고급 오류 처리 함수 구현
#include "ename.c.inc"

static void terminate(bool useExit3)
{
    char *s = getenv("EF_DUMPCORE");

    if (s != NULL && *s != '\0')
        abort();
    else if (useExit3)
        exit(EXIT_FAILURE);
    else
        _exit(EXIT_FAILURE);
}

static void outputError(bool useErr, int err, bool flushStdout, const char *format, va_list ap)
{
    #define BUF_SIZE 500
    char buf[BUF_SIZE], userMsg[BUF_SIZE], errText[BUF_SIZE];

    vsnprintf(userMsg, BUF_SIZE, format, ap);

    if (useErr)
        snprintf(errText, BUF_SIZE, " [%s %s]",
                 (err > 0 && err <= MAX_ENAME) ? ename[err] : "?UNKNOWN?", strerror(err));
    else
        snprintf(errText, BUF_SIZE, ":");

    snprintf(buf, BUF_SIZE, "ERROR%s %s\n", errText, userMsg);

    if (flushStdout)
        fflush(stdout);

    fputs(buf, stderr);
    fflush(stderr);
}

void kernel_errMsg(const char *format, ...)
{
    va_list argList;
    int savedErrno = errno;

    va_start(argList, format);
    outputError(true, errno, true, format, argList);
    va_end(argList);

    errno = savedErrno;
}

void kernel_errExit(const char *format, ...) {
    va_list argList;

    va_start(argList, format);
    outputError(true, errno, true, format, argList);
    va_end(argList);

    terminate(true);  // Ensure the program terminates properly
}

void kernel_errExitEN(int errnum, const char *format, ...) {
    va_list argList;

    va_start(argList, format);
    outputError(true, errnum, true, format, argList);
    va_end(argList);

    terminate(true);  // Ensure the program terminates properly
}

void kernel_fatal(const char *format, ...) {
    va_list argList;

    va_start(argList, format);
    outputError(false, 0, true, format, argList);
    va_end(argList);

    terminate(true);  // Ensure the program terminates properly
}

void kernel_usageErr(const char *format, ...)
{
    va_list argList;

    fflush(stdout);

    fprintf(stderr, "Usage: ");
    va_start(argList, format);
    vfprintf(stderr, format, argList);
    va_end(argList);

    fflush(stderr);
    exit(EXIT_FAILURE);
}

void kernel_cmdLineErr(const char *format, ...)
{
    va_list argList;

    fflush(stdout);

    fprintf(stderr, "Command-line usage error: ");
    va_start(argList, format);
    vfprintf(stderr, format, argList);
    va_end(argList);

    fflush(stderr);
    exit(EXIT_FAILURE);
}

// 스레드 작업 함수
void* thread_function(void* arg) {
    int thread_num = *((int*)arg);
    safe_kernel_printf("Thread %d: 시작\n", thread_num);

    sleep(1);  // 작업을 모방하기 위한 대기 시간
    
    safe_kernel_printf("Thread %d: 종료\n", thread_num);
    
    return NULL;
}

// 세마포어를 사용하는 스레드 작업 함수
void* semaphore_thread(void* arg) {
    sem_t* semaphore = (sem_t*)arg;

    safe_kernel_printf("세마포어 대기\n");
    sem_wait(semaphore);
    safe_kernel_printf("세마포어 획득\n");

    sleep(1);  // 작업을 모방하기 위한 대기 시간

    sem_post(semaphore);  // 세마포어 해제
    if(sem_post(semaphore) == -1) {
        kernel_errExit("Failed to release semaphore");
    } else {
        safe_kernel_printf("세마포어 해제\n");
    }

    return NULL;
}

// 뮤텍스를 사용하는 스레드 작업 함수
void* mutex_thread(void* arg) {
    pthread_mutex_t* mutex = (pthread_mutex_t*)arg;

    safe_kernel_printf("뮤텍스 대기\n");
    pthread_mutex_lock(mutex);  // 뮤텍스 잠금
    safe_kernel_printf("뮤텍스 획득\n");
    
    sleep(1);  // 작업을 모방하기 위한 대기 시간
    
    safe_kernel_printf("뮤텍스 해제\n");
    if(pthread_mutex_unlock(mutex) != 0) {
        kernel_errExit("Failed to release mutex");
    } else {
        safe_kernel_printf("뮤텍스 해제\n");
    }
    
    return NULL;
}

// 멀티스레드 실행 함수 (쓰레드 수 및 동기화 방법을 입력받음)
void run_multithreading(int num_threads, int use_semaphore, ...) {
    if (num_threads < 1 || num_threads > 100) {
        safe_kernel_printf("쓰레드 수는 1 이상 100 이하의 값이어야 합니다.\n");
        return;
    }

    safe_kernel_printf("멀티스레드 실행 시작 (쓰레드 수: %d, 동기화 방법: %s)\n",
                       num_threads, use_semaphore ? "세마포어" : "뮤텍스");
    // kernel_printf("멀티스레드 실행 시작 (쓰레드 수: %d, 동기화 방법: %s)\n",
    //               num_threads, use_semaphore ? "세마포어" : "뮤텍스");

    pthread_t* threads = (pthread_t*)malloc(num_threads * sizeof(pthread_t));
    int* thread_ids = (int*)malloc(num_threads * sizeof(int));
    va_list args;

    sem_t* semaphore = NULL;
    pthread_mutex_t* mutex = NULL;

    if (use_semaphore) {
        semaphore = init_semaphore(1);  // 세마포어 초기화
        safe_kernel_printf("세마포어 초기화 완료\n");
        kernel_printf("세마포어 초기화 완료\n");
    } else {
        mutex = init_mutex();  // 뮤텍스 초기화
        safe_kernel_printf("뮤텍스 초기화 완료\n");
        kernel_printf("뮤텍스 초기화 완료\n");
    }

    va_start(args, use_semaphore);  // 수정된 부분
    for (int i = 0; i < num_threads; i++) {
        thread_ids[i] = i + 1;

        if (use_semaphore) {
            int err = pthread_create(&threads[i], NULL, semaphore_thread, semaphore);
            if (err != 0) {
                kernel_errExitEN(err, "Thread %d 생성 실패", i);
            }
        } else {
            int err = pthread_create(&threads[i], NULL, mutex_thread, mutex);
            if (err != 0) {
                kernel_errExitEN(err, "Thread %d 생성 실패", i);
            }
        }
    }
    va_end(args);

    for (int i = 0; i < num_threads; i++) {
        pthread_join(threads[i], NULL);
    }

    // 세마포어 또는 뮤텍스 해제
    if (use_semaphore) {
        #ifdef __APPLE__
            sem_close(semaphore);
            sem_unlink("/semaphore");
            safe_kernel_printf("세마포어 해제 완료 (macOS)\n");
        #else
            sem_destroy(semaphore);
            free(semaphore);
            safe_kernel_printf("세마포어 해제 완료 (Linux)\n");
        #endif
    } else {
        pthread_mutex_destroy(mutex);
        free(mutex);
        safe_kernel_printf("뮤텍스 해제 완료\n");
    }

    free(threads);
    free(thread_ids);

    safe_kernel_printf("멀티스레드 실행 종료\n");
}

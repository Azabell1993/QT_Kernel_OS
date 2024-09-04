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
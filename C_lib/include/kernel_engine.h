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

// Global mutex
static pthread_mutex_t print_mutex = PTHREAD_MUTEX_INITIALIZER;

// Smart pointer struct definition
typedef struct SmartPtr {
    void *ptr;
    int *ref_count;
    pthread_mutex_t *mutex;
} SmartPtr;

// Smart pointer function declarations
SmartPtr create_smart_ptr(void *ptr);
void retain(SmartPtr *sp);
void release(SmartPtr *sp);

// Safe print function declaration
void safe_kernel_printf(const char *format, ...);

// Linked list struct definitions
typedef struct Node {
    void* data;
    struct Node* next;
} Node;

typedef struct LinkedList {
    Node* head;
    Node* tail;
    int size;
} LinkedList;

// Linked list function declarations
LinkedList* create_linkedlist();
void push(LinkedList* list, void* data);
void* pop(LinkedList* list);
bool is_empty(LinkedList* list);
void destroy_linkedlist(LinkedList* list);

// Thread and process management function declarations
void create_threads(int num_threads, ...);
void create_single_process(void (*func)());
void create_multi_processes(int num_processes, ...);

// Semaphore and mutex initialization function declarations
sem_t* init_semaphore(int value);
pthread_mutex_t* init_mutex();

// Error handling function declarations
void kernel_errMsg(const char *format, ...);
void kernel_errExit(const char *format, ...);
void kernel_errExitEN(int errnum, const char *format, ...);
void kernel_fatal(const char *format, ...);
void kernel_usageErr(const char *format, ...) __attribute__ ((__noreturn__));
void kernel_cmdLineErr(const char *format, ...) __attribute__ ((__noreturn__));

// 스레드 작업 함수
void* thread_function(void* arg);
void* semaphore_thread(void* arg);
void* mutex_thread(void* arg);
void run_multithreading(int num_threads, int use_semaphore, ...);

#ifdef __cplusplus
}
#endif

#endif // ENGINE_H

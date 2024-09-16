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

// 고급 오류 처리 함수 구현
#include "ename.c.inc"

#define BUF_SIZE 500

/**
 * @brief 스레드 안전한 출력 함수
 * 
 * @param format 포맷 문자열
 * @param ... 가변 인자 리스트
 */
void safe_kernel_printf(const char *format, ...) {
    va_list args;
    va_start(args, format);

    pthread_mutex_lock(&print_mutex);  // 출력 뮤텍스 잠금
    vprintf(format, args);  // 커널 printf 함수 호출
    fflush(stdout);
    pthread_mutex_unlock(&print_mutex);  // 출력 뮤텍스 해제

    va_end(args);
}

/**
 * @brief 스레드 생성 함수
 * 
 * @param num_threads 생성할 스레드 수
 * @param ... 스레드 함수 포인터
 */
void create_threads(int num_threads, ...) {
    pthread_t* threads = (pthread_t*)malloc(num_threads * sizeof(pthread_t));
    va_list args;
    va_start(args, num_threads);

    for (int i = 0; i < num_threads; i++) {
        void* (*thread_func)(void*) = va_arg(args, void* (*)(void*));
        int err = pthread_create(&threads[i], NULL, thread_func, NULL);
        if (err != 0) {
            kernel_errExitEN(err, "스레드 %d 생성 실패", i);
        }
    }

    for (int i = 0; i < num_threads; i++) {
        pthread_join(threads[i], NULL);
    }

    va_end(args);
    free(threads);
}

/**
 * @brief 단일 프로세스 생성 함수
 * 
 * @param func 실행할 함수 포인터
 */
void create_single_process(void (*func)()) {
    pid_t pid = fork();
    if (pid < 0) {
        kernel_errExit("프로세스 생성 실패");
    } else if (pid == 0) {
        func();
        exit(EXIT_SUCCESS);
    } else {
        wait(NULL);
    }
}

/**
 * @brief 다중 프로세스 생성 함수
 * 
 * @param num_processes 생성할 프로세스 수
 * @param ... 프로세스 함수 포인터
 */
void create_multi_processes(int num_processes, ...) {
    va_list args;
    va_start(args, num_processes);

    for (int i = 0; i < num_processes; i++) {
        pid_t pid = fork();
        if (pid < 0) {
            kernel_errExit("프로세스 %d 생성 실패", i);
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

/**
 * @brief 세마포어 초기화 함수
 * 
 * @param value 초기화할 값
 * @return 세마포어 포인터
 */
sem_t* init_semaphore(int value) {
    sem_t* sem = (sem_t*)malloc(sizeof(sem_t));
    if (sem == NULL) {
        perror("세마포어 메모리 할당 실패");
        exit(EXIT_FAILURE);
    }

#ifdef __APPLE__
    // macOS에서 named semaphore 사용
    char sem_name[20];
    snprintf(sem_name, sizeof(sem_name), "/semaphore_%d", getpid());
    sem_unlink(sem_name);  // 기존 세마포어 삭제 (중복 방지)
    sem = sem_open(sem_name, O_CREAT, 0644, value);
    if (sem == SEM_FAILED) {
        perror("세마포어 초기화 실패 (macOS)");
        free(sem);
        return NULL;
    }
#else
    // Linux에서는 unnamed semaphore 사용
    if (sem_init(sem, 0, value) != 0) {
        perror("세마포어 초기화 실패 (Linux)");
        free(sem);
        return NULL;
    }
#endif

    return sem;
}

/**
 * @brief 뮤텍스 초기화 함수
 * 
 * @return 뮤텍스 포인터
 */
pthread_mutex_t* init_mutex() {
    pthread_mutex_t* mutex = (pthread_mutex_t*)malloc(sizeof(pthread_mutex_t));
    if (pthread_mutex_init(mutex, NULL) != 0) {
        kernel_errExit("뮤텍스 초기화 실패");
    }
    return mutex;
}

/**
 * @brief 연결 리스트 생성 함수
 * 
 * @return LinkedList 포인터
 */
LinkedList* create_linkedlist() {
    LinkedList* list = (LinkedList*)malloc(sizeof(LinkedList));
    list->head = list->tail = NULL;
    list->size = 0;
    return list;
}

/**
 * @brief 연결 리스트에 요소 추가 함수
 * 
 * @param list LinkedList 포인터
 * @param data 추가할 데이터
 */
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

/**
 * @brief 연결 리스트에서 요소 제거 함수
 * 
 * @param list LinkedList 포인터
 * @return 제거된 데이터
 */
void* pop(LinkedList* list) {
    if (is_empty(list)) {
        kernel_errMsg("빈 리스트에서 pop 시도");
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

/**
 * @brief 연결 리스트가 비었는지 확인하는 함수
 * 
 * @param list LinkedList 포인터
 * @return true 리스트가 비었을 경우
 * @return false 리스트에 요소가 있을 경우
 */
bool is_empty(LinkedList* list) {
    return list->size == 0;
}

/**
 * @brief 연결 리스트 삭제 함수
 * 
 * @param list LinkedList 포인터
 */
void destroy_linkedlist(LinkedList* list) {
    while (!is_empty(list)) {
        pop(list);
    }
    free(list);
}

/**
 * @brief 프로세스 종료 처리 함수
 * 
 * @param useExit3 exit 함수 사용 여부
 */
static void terminate(bool useExit3) {
    char *s = getenv("EF_DUMPCORE");

    if (s != NULL && *s != '\0')
        abort();
    else if (useExit3)
        exit(EXIT_FAILURE);
    else
        _exit(EXIT_FAILURE);
}

/**
 * @brief 오류 메시지 출력 함수
 * 
 * @param useErr 오류 번호 사용 여부
 * @param err 오류 번호
 * @param flushStdout stdout 플러시 여부
 * @param format 포맷 문자열
 * @param ap 가변 인자 리스트
 */
static void outputError(bool useErr, int err, bool flushStdout, const char *format, va_list ap) {
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

/**
 * @brief 커널 오류 메시지 출력 함수
 * 
 * @param format 포맷 문자열
 * @param ... 가변 인자 리스트
 */
void kernel_errMsg(const char *format, ...) {
    va_list argList;
    int savedErrno = errno;

    va_start(argList, format);
    outputError(true, errno, true, format, argList);
    va_end(argList);

    errno = savedErrno;
}

/**
 * @brief 커널 오류 종료 함수
 * 
 * @param format 포맷 문자열
 * @param ... 가변 인자 리스트
 */
void kernel_errExit(const char *format, ...) {
    va_list argList;

    va_start(argList, format);
    outputError(true, errno, true, format, argList);
    va_end(argList);

    terminate(true);  // 프로그램이 올바르게 종료되도록 보장
}

/**
 * @brief 커널 오류 종료 함수 (오류 번호 사용)
 * 
 * @param errnum 오류 번호
 * @param format 포맷 문자열
 * @param ... 가변 인자 리스트
 */
void kernel_errExitEN(int errnum, const char *format, ...) {
    va_list argList;

    va_start(argList, format);
    outputError(true, errnum, true, format, argList);
    va_end(argList);

    terminate(true);  // 프로그램이 올바르게 종료되도록 보장
}

/**
 * @brief 커널 치명적 오류 함수
 * 
 * @param format 포맷 문자열
 * @param ... 가변 인자 리스트
 */
void kernel_fatal(const char *format, ...) {
    va_list argList;

    va_start(argList, format);
    outputError(false, 0, true, format, argList);
    va_end(argList);

    terminate(true);  // 프로그램이 올바르게 종료되도록 보장
}

/**
 * @brief 커널 사용법 오류 함수
 * 
 * @param format 포맷 문자열
 * @param ... 가변 인자 리스트
 */
void kernel_usageErr(const char *format, ...) {
    va_list argList;

    fflush(stdout);

    fprintf(stderr, "Usage: ");
    va_start(argList, format);
    vfprintf(stderr, format, argList);
    va_end(argList);

    fflush(stderr);
    exit(EXIT_FAILURE);
}

/**
 * @brief 커맨드 라인 오류 처리 함수
 * 
 * @param format 포맷 문자열
 * @param ... 가변 인자 리스트
 */
void kernel_cmdLineErr(const char *format, ...) {
    va_list argList;

    fflush(stdout);

    fprintf(stderr, "Command-line usage error: ");
    va_start(argList, format);
    vfprintf(stderr, format, argList);
    va_end(argList);

    fflush(stderr);
    exit(EXIT_FAILURE);
}

/**
 * @brief 세마포어를 사용하는 스레드 작업 함수
 * 
 * @param arg 세마포어 포인터
 * @return NULL
 */
void* semaphore_thread(void* arg) {
    sem_t* semaphore = (sem_t*)arg;

    safe_kernel_printf("세마포어 대기\n");
    sem_wait(semaphore);
    safe_kernel_printf("세마포어 획득\n");

    sleep(1);  // 작업을 모방하기 위한 대기 시간

    sem_post(semaphore);  // 세마포어 해제
    if(sem_post(semaphore) == -1) {
        kernel_errExit("세마포어 해제 실패");
    } else {
        safe_kernel_printf("세마포어 해제\n");
    }

    return NULL;
}

/**
 * @brief 뮤텍스를 사용하는 스레드 작업 함수
 * 
 * @param arg 뮤텍스 포인터
 * @return NULL
 */
void* mutex_thread(void* arg) {
    pthread_mutex_t* mutex = (pthread_mutex_t*)arg;

    safe_kernel_printf("뮤텍스 대기\n");
    pthread_mutex_lock(mutex);  // 뮤텍스 잠금
    safe_kernel_printf("뮤텍스 획득\n");
    
    sleep(1);  // 작업을 모방하기 위한 대기 시간
    
    safe_kernel_printf("뮤텍스 해제\n");
    if(pthread_mutex_unlock(mutex) != 0) {
        kernel_errExit("뮤텍스 해제 실패");
    } else {
        safe_kernel_printf("뮤텍스 해제\n");
    }
    
    return NULL;
}

/**
 * @brief 멀티스레드 실행 함수 (쓰레드 수 및 동기화 방법을 입력받음)
 * 
 * @param num_threads 생성할 스레드 수
 * @param use_semaphore 세마포어 사용 여부
 * @param ... 동기화 방법 (세마포어 또는 뮤텍스)
 */
void run_multithreading(int num_threads, int use_semaphore, ...) {
    if (num_threads < 1 || num_threads > 100) {
        safe_kernel_printf("쓰레드 수는 1 이상 100 이하의 값이어야 합니다.\n");
        return;
    }

    safe_kernel_printf("멀티스레드 실행 시작 (쓰레드 수: %d, 동기화 방법: %s)\n",
                       num_threads, use_semaphore ? "세마포어" : "뮤텍스");

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

    va_start(args, use_semaphore);
    for (int i = 0; i < num_threads; i++) {
        thread_ids[i] = i + 1;

        if (use_semaphore) {
            int err = pthread_create(&threads[i], NULL, semaphore_thread, semaphore);
            if (err != 0) {
                kernel_errExitEN(err, "스레드 %d 생성 실패", i);
            }
        } else {
            int err = pthread_create(&threads[i], NULL, mutex_thread, mutex);
            if (err != 0) {
                kernel_errExitEN(err, "스레드 %d 생성 실패", i);
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

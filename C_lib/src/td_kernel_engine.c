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
#include <pthread.h>
#include <semaphore.h>
#include <sys/socket.h>

#define NUM_THREADS 3
#define NUM_PROCESSES 2

// Forward declarations of test functions
static void test_smart_pointer();
static void test_file_reading();
static void test_multithreading();
static void test_multiprocessing();
static void test_synchronization();
static void run_all_tests();

int main(int argc __attribute__((unused)), char** argv __attribute__((unused))) {
    int ret, sock_fd[2];
    int status;
    char buf[] = "Hello from Parent!", child_buf[] = "Hello from Child!", line[BUFSIZ];
    pid_t pid;

    // 소켓 페어 생성
    ret = socketpair(AF_LOCAL, SOCK_STREAM, 0, sock_fd);
    if(ret == -1) {
        perror("socketpair");
        exit(1);
    }

    // 소켓 파일 디스크립터 출력
    kernel_printf("Socket pair created:\n");
    kernel_printf("Socket 1: %d\n", sock_fd[0]);
    kernel_printf("Socket 2: %d\n", sock_fd[1]);

    setvbuf(stdout, NULL, _IONBF, 0);  // 출력 버퍼링 비활성화

    safe_kernel_printf("\n메인 함수 시작\n");

    // 자식 프로세스 생성
    if((pid = fork()) < 0) {
        perror("fork()");
    } else if(pid == 0) { // Child process
        close(sock_fd[0]); // 자식 프로세스에서는 sock_fd[0]을 닫음
        read(sock_fd[1], line, BUFSIZ); // 부모 프로세스에서 전송된 데이터를 읽음
        kernel_printf("Child received: %s\n", line);

        write(sock_fd[1], child_buf, sizeof(child_buf)); // 부모에게 데이터를 다시 전송
        close(sock_fd[1]); // 사용 후 sock_fd[1]도 닫음

        // 모든 테스트 실행
        safe_kernel_printf("모든 테스트 실행 시작\n");
        run_all_tests();
        safe_kernel_printf("모든 테스트 실행 종료\n");

        exit(0);
    } else if(pid > 0) { // Parent process
        close(sock_fd[1]); // 부모 프로세스에서는 sock_fd[1]을 닫음
        write(sock_fd[0], buf, sizeof(buf)); // 자식 프로세스에 데이터를 전송

        wait(&status);
        read(sock_fd[0], line, BUFSIZ); // 자식 프로세스에서 전송된 데이터를 읽음
        kernel_printf("Parent received: %s\n", line);
        close(sock_fd[0]); // 사용 후 sock_fd[0]도 닫음
    }

    safe_kernel_printf("메인 함수 종료\n");
    return 0;
}

// 스마트 포인터 테스트 함수
static void test_smart_pointer() {
    safe_kernel_printf("스마트 포인터 테스트 시작\n");

    // 스마트 포인터 생성
    SmartPtr sp = create_smart_ptr(malloc(100));
    safe_kernel_printf("스마트 포인터 생성, 참조 카운트: 1\n");

    // 참조 카운트 증가
    retain(&sp);
    safe_kernel_printf("스마트 포인터 참조 증가, 참조 카운트: 2\n");

    // 참조 카운트 감소
    release(&sp);
    safe_kernel_printf("스마트 포인터 참조 감소, 참조 카운트: 1\n");

    // 마지막 참조 해제
    release(&sp);
    safe_kernel_printf("스마트 포인터 참조 해제, 메모리 해제됨\n");

    safe_kernel_printf("스마트 포인터 테스트 종료\n");
}

// 파일 내용을 읽어 출력하는 함수 예제
static void test_file_reading() {
    FILE *file = fopen("non_existent_file.txt", "r");
    if (!file) {
        safe_kernel_printf("파일 열기 실패: non_existent_file.txt\n");
        return;
    }

    safe_kernel_printf("파일 열기 성공: non_existent_file.txt\n");

    char buffer[256];
    while (fgets(buffer, sizeof(buffer), file) != NULL) {
        safe_kernel_printf("파일 내용: %s", buffer);
    }

    fclose(file);
}

// 스레드 함수 예제
static void* thread_function(void* arg) {
    int thread_num = *((int*)arg);
    safe_kernel_printf("Thread %d: 시작\n", thread_num);
    sleep(1); // 작업을 모방하기 위한 대기 시간
    safe_kernel_printf("Thread %d: 종료\n", thread_num);
    return NULL;
}

// 멀티스레드 테스트 함수
static void test_multithreading() {
    safe_kernel_printf("멀티스레드 테스트 시작\n");

    int thread_ids[NUM_THREADS] = {1, 2, 3};
    pthread_t threads[NUM_THREADS];

    for (int i = 0; i < NUM_THREADS; i++) {
        pthread_create(&threads[i], NULL, thread_function, &thread_ids[i]);
    }

    for (int i = 0; i < NUM_THREADS; i++) {
        pthread_join(threads[i], NULL);
    }

    safe_kernel_printf("멀티스레드 테스트 종료\n");
}

// 프로세스 함수 예제
static void process_function() {
    pid_t pid = getpid();
    safe_kernel_printf("Process %d: 시작\n", pid);
    sleep(2); // 작업을 모방하기 위한 대기 시간
    safe_kernel_printf("Process %d: 종료\n", pid);
}

// 멀티프로세스 테스트 함수
static void test_multiprocessing() {
    safe_kernel_printf("멀티프로세스 테스트 시작\n");

    create_multi_processes(NUM_PROCESSES, process_function, process_function);

    safe_kernel_printf("멀티프로세스 테스트 종료\n");
}

// 세마포어 스레드 함수 예제
static void* semaphore_thread(void* arg) {
    sem_t* semaphore = (sem_t*)arg;
    safe_kernel_printf("세마포어 스레드: 대기 중...\n");
    sem_wait(semaphore);
    safe_kernel_printf("세마포어 스레드: 세마포어 얻음!\n");
    sleep(2); // 작업을 모방하기 위한 대기 시간
    safe_kernel_printf("세마포어 스레드: 작업 완료\n");
    sem_post(semaphore);
    return NULL;
}

// 뮤텍스 스레드 함수 예제
static void* mutex_thread(void* arg) {
    pthread_mutex_t* mutex = (pthread_mutex_t*)arg;
    safe_kernel_printf("뮤텍스 스레드: 대기 중...\n");
    pthread_mutex_lock(mutex);
    safe_kernel_printf("뮤텍스 스레드: 뮤텍스 잠금!\n");
    sleep(2); // 작업을 모방하기 위한 대기 시간
    safe_kernel_printf("뮤텍스 스레드: 작업 완료\n");
    pthread_mutex_unlock(mutex);
    return NULL;
}

// 동기화 테스트 함수
static void test_synchronization() {
    safe_kernel_printf("동기화 테스트 시작\n");

    sem_t* semaphore = init_semaphore();
    pthread_mutex_t* mutex = init_mutex();

    pthread_t sem_threads[2];
    pthread_t mutex_threads[2];

    // 세마포어 테스트
    for (int i = 0; i < 2; i++) {
        pthread_create(&sem_threads[i], NULL, semaphore_thread, semaphore);
    }

    for (int i = 0; i < 2; i++) {
        pthread_join(sem_threads[i], NULL);
    }

    // 뮤텍스 테스트
    for (int i = 0; i < 2; i++) {
        pthread_create(&mutex_threads[i], NULL, mutex_thread, mutex);
    }

    for (int i = 0; i < 2; i++) {
        pthread_join(mutex_threads[i], NULL);
    }

#ifdef __APPLE__
    // macOS의 경우, sem_destroy를 호출하지 않음
#else
    sem_destroy(semaphore); // 다른 플랫폼에서는 호출
#endif

    pthread_mutex_destroy(mutex);

    safe_kernel_printf("동기화 테스트 종료\n");
}

// 모든 테스트를 실행하는 함수
static void run_all_tests() {
    test_smart_pointer();
    test_file_reading();
    test_multithreading();
    test_multiprocessing();
    test_synchronization();
}

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
#include <fcntl.h>

#define NUM_THREADS 3
#define NUM_PROCESSES 2

// Forward declarations of test functions
static void test_smart_pointer();
static void test_file_reading();
static void test_multithreading();
static void test_multiprocessing();
static void test_synchronization();
static void run_all_tests();
static pid_t kernel_fork();
static void kernel_create_thread(pthread_t *thread, void *(*start_routine)(void *), void *arg);
static void kernel_join_thread(pthread_t thread);
static void kernel_socket_communication(int sock_fd, const char *message, char *response, size_t response_size);
static void kernel_wait_for_process(pid_t pid);


void run_multithreading(int num_threads, int use_semaphore, ...);
void* semaphore_thread(void* arg);
void* mutex_thread(void* arg);
void* thread_function(void* arg);


// Function prototype for process function
static void process_function();

// Main function
int main(int argc __attribute__((unused)), char** argv __attribute__((unused))) {
    kernel_printf("커널 메인 함수 시작\n");

    // 단계 1: 변수 초기화
    int ret, sock_fd[2];
    char buf[] = "부모 프로세스에서 보냄!", line[BUFSIZ];
    pid_t pid;
    int thread_ids[NUM_THREADS] = {1, 2, 3};
    pthread_t threads[NUM_THREADS];
    sem_t* kernel_semaphore;
    pthread_mutex_t* kernel_mutex;

    // 단계 2: 환경 설정
    kernel_printf("커널 환경 설정 중\n");
    kernel_semaphore = init_semaphore(1);
    kernel_mutex = init_mutex();

    // 단계 3: IPC를 위한 소켓 페어 생성
    kernel_printf("IPC를 위한 소켓 페어 생성 중\n");
    ret = socketpair(AF_LOCAL, SOCK_STREAM, 0, sock_fd);
    if (ret == -1) {
        kernel_printf("소켓 페어 생성 실패\n");
    }
    kernel_printf("소켓 페어 생성됨: FD1=%d, FD2=%d\n", sock_fd[0], sock_fd[1]);

    // 단계 4: 자식 프로세스 생성
    kernel_printf("자식 프로세스 생성 중\n");
    pid = kernel_fork();

    if (pid == 0) { // 자식 프로세스
        kernel_printf("자식 프로세스 시작됨\n");
        close(sock_fd[0]); // 사용하지 않는 소켓 닫기

        // 단계 5: 자식 프로세스 작업 수행
        kernel_printf("자식 프로세스 작업 수행 중\n");
        kernel_socket_communication(sock_fd[1], buf, line, sizeof(line));

        safe_kernel_printf("자식 프로세스가 테스트를 시작합니다...\n");

        // 자식 프로세스에서 테스트 실행
        test_file_reading(); // 자식 프로세스에서 파일 읽기 테스트 실행
        // run_all_tests();

        // 자식 프로세스에서의 특정 작업
        kernel_printf("자식 프로세스에서 특정 작업 실행 중\n");
        kernel_create_thread(&threads[0], thread_function, &thread_ids[0]);
        kernel_join_thread(threads[0]);

        kernel_printf("자식 프로세스 작업 완료\n");
        close(sock_fd[1]); // 소켓 닫기
        kernel_printf("자식 프로세스 종료\n");
        exit(0);
    } else { // 부모 프로세스
        printf("부모 프로세스 시작됨\n");
        close(sock_fd[1]); // 사용하지 않는 소켓 닫기

        // 단계 6: 부모 프로세스에서 메시지 전송 및 수신
        printf("부모 프로세스가 자식과 통신 중\n");
        kernel_socket_communication(sock_fd[0], buf, line, sizeof(line));
        printf("부모 프로세스에서 받은 메시지: %s\n", line);

        kernel_wait_for_process(pid); // 자식 프로세스 대기

        close(sock_fd[0]); // 소켓 닫기

        // 부모 프로세스에서 테스트 실행
        test_file_reading(); // 부모 프로세스에서 파일 읽기 테스트 실행

        // 단계 7: 부모 프로세스에서 멀티스레딩 테스트 수행
        kernel_printf("부모 프로세스에서 멀티스레딩 테스트 수행 중\n");
        for (int i = 0; i < NUM_THREADS; i++) {
            kernel_create_thread(&threads[i], thread_function, &thread_ids[i]);
        }
        for (int i = 0; i < NUM_THREADS; i++) {
            kernel_join_thread(threads[i]);
        }

        // 단계 8: 스마트 포인터 연산 테스트
        kernel_printf("부모 프로세스에서 스마트 포인터 연산 테스트 중\n");
        SmartPtr sp = create_smart_ptr(malloc(256));  // 유효한 포인터 전달
        retain(&sp);
        release(&sp);
        release(&sp);

        // 단계 9: 세마포어 동기화 테스트
        kernel_printf("부모 프로세스에서 세마포어 동기화 테스트 중\n");
        pthread_t sem_threads[2];
        for (int i = 0; i < 2; i++) {
            kernel_create_thread(&sem_threads[i], semaphore_thread, kernel_semaphore);
        }
        for (int i = 0; i < 2; i++) {
            kernel_join_thread(sem_threads[i]);
        }

        // 단계 10: 뮤텍스 동기화 테스트
        kernel_printf("부모 프로세스에서 뮤텍스 동기화 테스트 중\n");
        pthread_t mutex_threads[2];
        for (int i = 0; i < 2; i++) {
            kernel_create_thread(&mutex_threads[i], mutex_thread, kernel_mutex);
        }
        for (int i = 0; i < 2; i++) {
            kernel_join_thread(mutex_threads[i]);
        }

        // 단계 11: 고급 오류 처리 테스트
        kernel_printf("부모 프로세스에서 고급 오류 처리 테스트 중\n");
        char *invalid_ptr = NULL;
        if (invalid_ptr == NULL) {
            kernel_errMsg("널 포인터 감지, 복구 시도 중\n");
        }

        FILE *invalid_file = fopen("/invalid/path", "r");
        if (!invalid_file) {
            kernel_errMsg("유효하지 않은 파일 열기 실패, 계속 진행\n");
        }

        // 단계 12: 프로세스 관리 시뮬레이션
        kernel_printf("부모 프로세스에서 프로세스 관리 시뮬레이션 중\n");
        pid_t worker_pid = kernel_fork();
        if (worker_pid == 0) {
            kernel_printf("작업자 프로세스 시작됨\n");
            process_function();
            kernel_printf("작업자 프로세스 종료\n");
            exit(0);
        } else {
            kernel_wait_for_process(worker_pid);
        }

        // 단계 13: 메모리 관리 시뮬레이션
        kernel_printf("부모 프로세스에서 메모리 관리 시뮬레이션 중\n");
        void *memory_block = malloc(1024);
        if (!memory_block) {
            kernel_printf("메모리 할당 실패\n");
        }
        kernel_printf("메모리 할당됨, 주소: %p\n", memory_block);
        free(memory_block);
        kernel_printf("메모리 해제됨\n");

        // 단계 14: 장치 I/O 시뮬레이션
        kernel_printf("부모 프로세스에서 장치 I/O 시뮬레이션 중\n");
        int fd = open("/dev/null", O_WRONLY);
        if (fd == -1) {
            kernel_printf("'/dev/null' 열기 실패\n");
        }
        write(fd, "커널 테스트 데이터", 16);
        close(fd);
        kernel_printf("장치 I/O 완료\n");

        // 단계 15: 파일 시스템 작업 시뮬레이션
        kernel_printf("부모 프로세스에서 파일 시스템 작업 시뮬레이션 중\n");
        char *test_file_path = "/tmp/kernel_test_file";
        FILE *test_file = fopen(test_file_path, "w");
        if (!test_file) {
            kernel_printf("테스트 파일 생성 실패\n");
        }
        fprintf(test_file, "커널 테스트 데이터\n");
        fclose(test_file);
        kernel_printf("테스트 파일 생성됨: %s\n", test_file_path);

        test_file = fopen(test_file_path, "r");
        if (test_file) {
            char read_buffer[128];
            while (fgets(read_buffer, sizeof(read_buffer), test_file)) {
                kernel_printf("테스트 파일에서 읽음: %s", read_buffer);
            }
            fclose(test_file);
        }

        if (remove(test_file_path) == 0) {
            kernel_printf("테스트 파일 삭제됨: %s\n", test_file_path);
        } else {
            kernel_errMsg("테스트 파일 삭제 실패\n");
        }

        // 단계 16: 네트워크 시뮬레이션 (스텁)
        kernel_printf("부모 프로세스에서 네트워크 작업 시뮬레이션 중\n");
        // 보통 여기서는 소켓과 네트워크 인터페이스를 포함합니다.
        // tcp...
        kernel_printf("네트워크 작업 시뮬레이션 완료\n");

        // 단계 17: 시스템 호출 시뮬레이션
        kernel_printf("부모 프로세스에서 시스템 호출 시뮬레이션 중\n");
        int syscall_result = getpid(); // 시스템 호출을 getpid()로 대체
        kernel_printf("시스템 호출 SYS_getpid 반환값: %d\n", syscall_result);

        syscall_result = getuid(); // 시스템 호출을 getuid()로 대체
        kernel_printf("시스템 호출 SYS_getuid 반환값: %d\n", syscall_result);

        // 단계 18: 하드웨어 인터럽트 시뮬레이션 (스텁)
        kernel_printf("부모 프로세스에서 하드웨어 인터럽트 시뮬레이션 중\n");
        // 보통 여기서는 인터럽트 벡터와 핸들러를 포함합니다.
        kernel_printf("하드웨어 인터럽트 시뮬레이션 완료\n");

        // 단계 19: 커널 패닉 시뮬레이션
        kernel_printf("부모 프로세스에서 커널 패닉 시뮬레이션 중\n");
        if (1 == 0) { // 이 조건은 절대 참이 아닙니다.
            kernel_printf("이 패닉은 절대 발생하지 말아야 합니다\n");
        } else {
            kernel_printf("커널 패닉 시뮬레이션 회피됨\n");
        }

        // 단계 20: 최종 정리 작업
        kernel_printf("부모 프로세스에서 최종 정리 작업 수행 중\n");

#ifdef __APPLE__
        // macOS 특정 정리 작업
#else
        sem_destroy(kernel_semaphore); // 다른 플랫폼
#endif
        pthread_mutex_destroy(kernel_mutex);

        kernel_printf("최종 정리 작업 완료\n");
    }

    // 단계 21: 커널 종료
    kernel_printf("커널 메인 함수 종료\n");
    return 0;
}


// Function definitions

static pid_t kernel_fork() {
    pid_t pid = fork();
    if (pid < 0) {
        kernel_printf("Failed to fork process");
    }
    return pid;
}

static void kernel_create_thread(pthread_t *thread, void *(*start_routine)(void *), void *arg) {
    int err = pthread_create(thread, NULL, start_routine, arg);
    if (err != 0) {
        kernel_printf("Failed to create thread");
    }
}

static void kernel_join_thread(pthread_t thread) {
    int err = pthread_join(thread, NULL);
    if (err != 0) {
        kernel_printf("Failed to join thread");
    }
}

static void kernel_socket_communication(int sock_fd, const char *message, char *response, size_t response_size) {
    if (write(sock_fd, message, strlen(message)) == -1) {
        kernel_printf("Failed to send message through socket");
    }

    ssize_t bytes_read = read(sock_fd, response, response_size - 1);
    if (bytes_read == -1) {
        kernel_printf("Failed to receive message from socket");
    }
    response[bytes_read] = '\0';
}

static void kernel_wait_for_process(pid_t pid) {
    int status;
    if (waitpid(pid, &status, 0) < 0) {
        kernel_printf("Failed to wait for process");
    }
}

// Remaining functions (like semaphore_thread, mutex_thread, etc.) should be defined as before


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

// 동기화 테스트 함수
static void test_synchronization() {
    safe_kernel_printf("동기화 테스트 시작\n");

    sem_t* semaphore = init_semaphore(1);
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

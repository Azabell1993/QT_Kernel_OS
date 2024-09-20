#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdarg.h>
#include <sys/stat.h>
#include "kernel_smartptr.h"
#include "kernel_uniqueptr.h"
#include <fcntl.h>
#include <pthread.h>

#define DEFAULT_TCP_PORT 5100
#define MAX_CLIENTS 10
#define BUFFER_SIZE 1024


/**
 * @brief 자동 데몬화 모드 함수
 * @param void
 * @return void
 */
void auto_daemon_mode();
void manual_server_mode();


/**
 * @brief 네트워크와 클라이언트 간의 채팅 처리 함수
 * 
 * @param num_args 가변 인자 개수 (서버 IP와 포트 전달)
 * @param ... 서버 IP 주소와 포트를 가변 인자로 전달
 */
/**
 * @brief 고유 포인터 구조체
 * 
 * 참조 카운트가 1인 스마트 포인터로, 다른 스마트 포인터와 공유되지 않습니다.
 */
void list_users();

/**
 * @brief 클라이언트를 강제로 퇴장시키는 함수
 * 
 * @param username 퇴장시킬 클라이언트의 사용자명
 */
void kill_user(const char *username);

/**
 * @brief 클라이언트를 강제로 퇴장시키는 함수
 * 
 * @param username 퇴장시킬 클라이언트의 사용자명
 */
void kill_room(int room_id);

/**
 * @brief 클라이언트 정보를 담는 구조체
 * 
 * 각 클라이언트의 소켓 FD, ID, 채팅방 ID, 사용자명을 포함하고 있으며, 뮤텍스를 관리합니다.
 */
typedef struct {
    int client_fd;               /**< 클라이언트의 소켓 파일 디스크립터 */
    int client_id;               /**< 클라이언트 ID */
    int room_id;                 /**< 클라이언트가 참여한 채팅방 ID */
    char username[BUFFER_SIZE];  /**< 클라이언트 사용자명 */
    pthread_mutex_t *client_mutex; /**< 클라이언트 별 뮤텍스 */
} ClientInfo;

/**
 * @brief 클라이언트 정보를 스마트 포인터로 관리하는 배열
 * 
 * @note 최대 MAX_CLIENTS 개의 클라이언트를 관리합니다.
 */
SmartPtr client_infos[MAX_CLIENTS];

/**
 * @brief 클라이언트 정보를 스마트 포인터로 관리하는 배열
 * @param client_infos 클라이언트 정보를 담는 스마트 포인터 배열
 * @return void
 */
UniquePtr create_unique_ptr(size_t size, void (*deleter)(void*));

/**
 * @brief TCP 서버를 생성하고 클라이언트 연결을 처리하는 함수
 * 
 * @param num_tcp_proc 생성할 TCP 프로세스 수
 * @param ... 서버의 IP 주소와 포트를 인자로 받습니다.
 * @return int 성공 시 0, 실패 시 -1 반환
 */
int create_network_tcp_process(int num_tcp_proc, ...);

/**
 * @brief 특정 채팅방에 있는 모든 클라이언트에게 메시지를 브로드캐스트하는 함수
 * 
 * @param sender_fd 메시지를 보낸 클라이언트의 파일 디스크립터
 * @param message 브로드캐스트할 메시지
 * @param room_id 메시지를 보낼 채팅방의 ID
 */
void broadcast_message(int sender_fd, char *message, int room_id);

/**
 * @brief 서버 측에서 발생한 채팅 메시지를 로그로 저장하는 함수
 * 
 * @param message 저장할 메시지
 */
void log_chat_message(const char *message);

/**
 * @brief 클라이언트와의 통신을 처리하는 스레드 함수
 * 
 * @param arg 클라이언트 정보를 담고 있는 스마트 포인터 구조체의 포인터
 * @return void* 스레드 종료 시 반환값 (NULL)
 */
void *client_handler(void *arg);

/**
 * @brief 클라이언트 정보를 스마트 포인터로 관리하는 배열
 * 
 * @param client_infos 클라이언트 정보를 담는 스마트 포인터 배열
 * @return void
 */
void release_client(int sock);

/**
 * @brief 서버 측에서 사용자 입력을 처리하는 스레드 함수
 * 
 * 표준 입력으로부터 메시지를 받아 서버 메시지로 브로드캐스트합니다.
 * 
 * @param arg 미사용
 * @return void* 스레드 종료 시 반환값 (NULL) */
void *server_input_handler(void *arg);

/**
 * @brief 서버 메시지를 브로드캐스트하는 함수
 * 
 * @param message 서버에서 브로드캐스트할 메시지
 * @return void
 */
void send_server_message(char *message);

/**
 * @brief 클라이언트를 강제로 퇴장시키는 함수
 * 
 * @param username 퇴장시킬 클라이언트의 사용자명
 * @return void
 */
void kill_user(const char *username);

/**
 * @brief 클라이언트를 강제로 퇴장시키는 함수
 * @param username 퇴장시킬 클라이언트의 사용자명
 * @return void
 */
void list_users() {
    printf("현재 접속 중인 유저 목록:\n");
    for (int i = 0; i < MAX_CLIENTS; i++) {
        if (client_infos[i].ptr != NULL) {
            ClientInfo *client_info = (ClientInfo *)client_infos[i].ptr;
            printf("User: %s, Room: %d\n", client_info->username, client_info->room_id);
        }
    }

    for (int room_id = 1; room_id <= 5; room_id++) {
        int user_count = 0;
        for (int i = 0; i < MAX_CLIENTS; i++) {
            if (client_infos[i].ptr != NULL) {
                ClientInfo *client_info = (ClientInfo *)client_infos[i].ptr;
                if (client_info->room_id == room_id) {
                    user_count++;
                }
            }
        }
        printf("Room %d: %d명\n", room_id, user_count);
    }
}

/**
 * @brief 클라이언트를 강제로 퇴장시키는 함수
 * @param username 퇴장시킬 클라이언트의 사용자명
 * @return void
 */
void kill_room(int room_id) {
    for (int i = 0; i < MAX_CLIENTS; i++) {
        if (client_infos[i].ptr != NULL) {
            ClientInfo *client_info = (ClientInfo *)client_infos[i].ptr;
            if (client_info->room_id == room_id) {
                send(client_info->client_fd, "The room has been closed. You have been kicked out.\n", strlen("The room has been closed. You have been kicked out.\n"), 0);
                release_client(i);  // Properly release client
            }
        }
    }
    printf("Room %d has been closed, and all users have been kicked.\n", room_id);
}

/**
 * @brief 서버 메시지를 브로드캐스트하는 함수
 * @param message 서버에서 브로드캐스트할 메시지
 * @param argument message 서버에서 브로드캐스트할 메시지
 * @return void
 */
SmartPtr client_infos[MAX_CLIENTS];

/**
 * @brief 클라이언트 정보를 스마트 포인터로 관리하는 배열
 * @param client_infos 클라이언트 정보를 담는 스마트 포인터 배열
 * @return void
 */
void add_new_client(int sock, int client_id, const char *username) {
    ClientInfo *client_info = (ClientInfo *)malloc(sizeof(ClientInfo));
    client_info->client_fd = sock;
    client_info->client_id = client_id;
    strcpy(client_info->username, username); 
    pthread_mutex_t *client_mutex = (pthread_mutex_t *)malloc(sizeof(pthread_mutex_t));
    pthread_mutex_init(client_mutex, NULL);
    client_info->client_mutex = client_mutex;
    
    // client_infos[sock] = create_smart_ptr(client_info, free);
    client_infos[sock] = create_smart_ptr(sizeof(ClientInfo), client_info, free);
}

/**
 * @brief 클라이언트 정보를 스마트 포인터로 관리하는 배열
 * @param client_infos 클라이언트 정보를 담는 스마트 포인터 배열
 * @return void
 */
void release_client(int sock) {
    if (client_infos[sock].ptr != NULL) {
        ClientInfo *client_info = (ClientInfo *)client_infos[sock].ptr;
        close(client_info->client_fd);
        release_shared_ptr((SharedPtr*)&client_infos[sock]);
        printf("클라이언트 %d 연결 종료 및 메모리 해제 완료\n", client_info->client_id);
    }
}


/**
 * @brief 클라이언트 정보를 스마트 포인터로 관리하는 배열
 * @param client_infos 클라이언트 정보를 담는 스마트 포인터 배열
 * @return void
 */
void kill_user(const char *username) {
    for (int i = 0; i < MAX_CLIENTS; i++) {
        if (client_infos[i].ptr != NULL) {
            ClientInfo *client_info = (ClientInfo *)client_infos[i].ptr;
            if (strcmp(client_info->username, username) == 0) {
                send(client_info->client_fd, "You have been kicked from the chat.\n", strlen("You have been kicked from the chat.\n"), 0);
                release_client(i);  // Properly release client
                printf("User %s has been kicked.\n", username);
                break;
            }
        }
    }
}

/**
 * @brief 특정 채팅방에 있는 모든 클라이언트에게 메시지를 브로드캐스트하는 함수
 * @param sender_fd 메시지를 보낸 클라이언트의 파일 디스크립터
 * @param message 브로드캐스트할 메시지
 * @param room_id 메시지를 보낼 채팅방의 ID
 * @return void
 */
void broadcast_message(int sender_fd, char *message, int room_id) {
    char broadcast_message[BUFFER_SIZE + 50];
    ClientInfo *sender_info = (ClientInfo *)client_infos[sender_fd].ptr;

    snprintf(broadcast_message, sizeof(broadcast_message), "[%s]: %s", sender_info->username, message);
    log_chat_message(broadcast_message);

    for (int i = 0; i < MAX_CLIENTS; i++) {
        if (client_infos[i].ptr != NULL) {
            ClientInfo *client_info = (ClientInfo *)client_infos[i].ptr;
            if (client_info->room_id == room_id && client_info->client_fd != sender_fd) {
                write(client_info->client_fd, broadcast_message, strlen(broadcast_message));
            }
        }
    }
}


#include <time.h>

pthread_mutex_t log_mutex = PTHREAD_MUTEX_INITIALIZER;

/**
 * @brief 채팅 메시지를 로그 파일에 저장하는 함수
 * @param message 저장할 메시지
 * @return void
 */
void log_chat_message(const char *message) {
    // 절대 경로로 로그 파일 지정
    char log_path[BUFFER_SIZE];
    time_t now = time(NULL);
    struct tm *t = localtime(&now);

    // 현재 날짜를 기반으로 로그 파일명을 만듦
    // 반드시 touch 명령어로 해당 파일을 미리 생성해두어야 함
    // 권한에 대해서도 고려해야 함
    // sudo touch /var/log/chatlog_20240915.log
    // sudo chmod 777 /var/log/chatlog_20240915.log
    strftime(log_path, sizeof(log_path), "/var/log/chatlog_%Y%m%d.log", t);

    // 뮤텍스 잠금으로 동시 접근 제어
    pthread_mutex_lock(&log_mutex);

    FILE *log_file = fopen(log_path, "a");
    if (log_file == NULL) {
        perror("로그 파일을 열 수 없습니다.");
        pthread_mutex_unlock(&log_mutex);  // 잠금 해제
        return;
    }

    // 로그 파일에 메시지 기록
    fprintf(log_file, "%s\n", message);
    fclose(log_file);

    // 뮤텍스 잠금 해제
    pthread_mutex_unlock(&log_mutex);
}

/**
 * @brief 클라이언트와의 통신을 처리하는 스레드 함수
 * @param arg 클라이언트 정보를 담고 있는 스마트 포인터 구조체의 포인터
 * @return void* 스레드 종료 시 반환값 (NULL)
 */
void *client_handler(void *arg) {
    SmartPtr *sp = (SmartPtr *)arg;
    ClientInfo *client_info = (ClientInfo *)sp->ptr;
    char buffer[BUFFER_SIZE];
    int nbytes;

    // 사용자명 수신
    memset(buffer, 0, sizeof(buffer));
    nbytes = read(client_info->client_fd, buffer, BUFFER_SIZE);
    if (nbytes <= 0) {
        printf("사용자명 수신 실패 또는 클라이언트 연결 종료\n");
        close(client_info->client_fd);
        release(sp);
        return NULL;
    }
    strncpy(client_info->username, buffer, BUFFER_SIZE);
    printf("사용자명: %s\n", client_info->username);

    // 채팅방 선택 수신
    memset(buffer, 0, sizeof(buffer));
    nbytes = read(client_info->client_fd, buffer, BUFFER_SIZE);
    if (nbytes <= 0) {
        printf("채팅방 수신 실패 또는 클라이언트 연결 종료\n");
        close(client_info->client_fd);

        // 클라이언트 종료 시 뮤텍스 제거
        printf("클라이언트 %d 연결 종료에 따른 뮤텍스 파괴", client_info->client_id);
        pthread_mutex_destroy(client_info -> client_mutex);
        free(client_info -> client_mutex);
        printf("뮤텍스 파괴 완료. 클라이언트 아이디 : [ %d ] -> destroyed\n", client_info->client_id);

        if(client_info->room_id != 0) {
            printf("클라이언트 %d가 채팅방 %d에서 퇴장했습니다.\n", client_info->client_id, client_info->room_id);
        } else {
            printf("클라이언트 %d 연결 종료\n", client_info->client_id);
        }

        release(sp);
        return NULL;
    }
    
    client_info->room_id = atoi(buffer);
    printf("클라이언트 %d가 채팅방 %d에 입장했습니다.\n", client_info->client_id, client_info->room_id);

    // 메시지 처리
    while ((nbytes = read(client_info->client_fd, buffer, BUFFER_SIZE)) > 0) {
        buffer[nbytes] = '\0';
        printf("클라이언트 %d (%s) 메시지: %s\n", client_info->client_id, client_info->username, buffer);
        broadcast_message(client_info->client_fd, buffer, client_info->room_id);
    }

    printf("클라이언트 %d 연결 종료\n", client_info->client_id);

    // 뮤텍스 파괴 및 참조 감소 확인
    printf("클라이언트 %d 연결 종료. 뮤텍스 파괴 중...\n", client_info->client_id);
    pthread_mutex_destroy(client_info -> client_mutex);
    free(client_info -> client_mutex);
    printf("뮤텍스 파괴 완료. 클라이언트 아이디 : [ %d ] -> destroyed\n", client_info->client_id);

    close(client_info->client_fd);
    release(sp);  // 스마트 포인터 해제
    return NULL;
}
/**
 * @brief TCP 서버를 생성하고 클라이언트 연결을 처리하는 함수
 * @param num_tcp_proc 생성할 TCP 프로세스 수
 * @param ... 서버의 IP 주소와 포트를 인자로 받습니다.
 * @return int 성공 시 0, 실패 시 -1 반환
 */
int create_network_tcp_process(int num_tcp_proc, ...) {
    va_list args;
    va_start(args, num_tcp_proc);

    for (int i = 0; i < num_tcp_proc; i++) {
        int ssock, client_count = 1;
        socklen_t clen;
        struct sockaddr_in servaddr, cliaddr;
        // char buffer[BUFFER_SIZE];
        // const char *ip_address = va_arg(args, const char*);
        char client_ip[INET_ADDRSTRLEN];

        int port = va_arg(args, int);

        if ((ssock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
            perror("socket()");
            return -1;
        }

        int enable = 1;
        if (setsockopt(ssock, SOL_SOCKET, SO_REUSEADDR, &enable, sizeof(int)) < 0) {
            perror("setsockopt(SO_REUSEADDR) failed");
            return -1;
        }

        memset(&servaddr, 0, sizeof(servaddr));
        servaddr.sin_family = AF_INET;
        servaddr.sin_addr.s_addr = htons(INADDR_ANY);
        servaddr.sin_port = htons(port);

        if (bind(ssock, (struct sockaddr *)&servaddr, sizeof(servaddr)) < 0) {
            perror("bind()");
            return -1;
        }

        if (listen(ssock, 8) < 0) {
            perror("listen()");
            return -1;
        } else {
            printf("서버가 포트 %d에서 듣고 있습니다.\n", port);
        }

        printf("서버가 클라이언트의 연결을 기다립니다...\n");

        pthread_t tid;
        pthread_create(&tid, NULL, server_input_handler, NULL); // 서버 입력 처리 스레드 생성

        while (1) {
            clen = sizeof(cliaddr);
            int csock = accept(ssock, (struct sockaddr *)&cliaddr, &clen);
            if (csock > 0) {
                inet_ntop(AF_INET, &cliaddr.sin_addr, client_ip, INET_ADDRSTRLEN);
                printf("[ 클라이언트 %d가 연결되었습니다. IP: %s ]\n", client_count, client_ip);

                pthread_mutex_t *client_mutex = (pthread_mutex_t *)malloc(sizeof(pthread_mutex_t));
                pthread_mutex_init(client_mutex, NULL);

                ClientInfo *client_info = (ClientInfo *)malloc(sizeof(ClientInfo));
                client_info->client_fd = csock;
                client_info->client_id = client_count++;
                client_info->client_mutex = client_mutex;

                // 클라이언트 정보를 스마트 포인터로 관리
                // client_infos[csock] = create_smart_ptr(client_info, free);
                client_infos[csock] = create_smart_ptr(sizeof(ClientInfo), client_info, free);

                // 클라이언트 스레드 생성
                pthread_create(&tid, NULL, client_handler, (void *)&client_infos[csock]);

                printf("mutex %d called\n", client_info->client_id);
                
                // 추가: 클라이언트 종료 시 뮤텍스 제거
                pthread_detach(tid);  // 스레드 분리
            }
        }

        close(ssock);  // 소켓 닫기
    }

    va_end(args);
    return 0;
}

/**
 * @brief 서버 측에서 사용자 입력을 처리하는 스레드 함수
 * @param arg 미사용
 * @param return void* 스레드 종료 시 반환값 (NULL)
 * @param argument arg 미사용
 * @return void* 스레드 종료 시 반환값 (NULL)
 */
void *server_input_handler(void *arg) {
    (void)arg;  // 미사용 인자 처리
    char buffer[BUFFER_SIZE];

    while (1) {
        // 표준 입력으로부터 메시지 입력받기
        fgets(buffer, BUFFER_SIZE, stdin);
        buffer[strcspn(buffer, "\n")] = '\0';  // 개행 문자 제거

        // 종료 명령어 처리
        if (strcmp(buffer, "exit") == 0 || strcmp(buffer, "...") == 0) {
            printf("채팅을 종료합니다.\n");
            exit(0);
        }

        // list 명령어 처리
        if (strcmp(buffer, "list") == 0) {
            list_users();
        }
        
        // kill 명령어 처리
        if (strncmp(buffer, "kill ", 5) == 0) {
            char *username = buffer + 5;
            kill_user(username);
        }
        
        // kill room 명령어 처리
        if (strncmp(buffer, "kill room ", 10) == 0) {
            int room_id = atoi(buffer + 10);
            kill_room(room_id);
        }

        // grep -r 명령어 처리
        if (strncmp(buffer, "grep -r", 7) == 0) {
            char command[BUFFER_SIZE + 100];
            char log_filename[50];
            time_t now = time(NULL);
            struct tm *t = localtime(&now);

            // 로그 파일명에 날짜 붙이기
            strftime(log_filename, sizeof(log_filename), "/var/log/chatlog_%Y%m%d.log", t);

            // grep 명령어에 로그 파일 경로 포함
            snprintf(command, sizeof(command), "%s %s", buffer, log_filename);
            system(command);  // 로그 파일에서 grep 명령 실행
        }

        if (strlen(buffer) > 0) {
            send_server_message(buffer);  // 서버 메시지 전송
        }
    }
    return NULL;
}
/**
 * @brief 서버 메시지를 브로드캐스트하는 함수
 * @param message 서버에서 브로드캐스트할 메시지
 * @param argument message 서버에서 브로드캐스트할 메시지
 * @return void
 */
void send_server_message(char *message) {
    char server_message[BUFFER_SIZE + 50];
    snprintf(server_message, sizeof(server_message), "[서버]: %s", message);
    log_chat_message(server_message);

    // 채팅방에 있는 클라이언트들에게 메시지 전송
    for (int i = 0; i < MAX_CLIENTS; i++) {
        if (client_infos[i].ptr != NULL) {
            write(i, server_message, strlen(server_message));
        }
    }   
}

/**
 * @brief daemonize 함수 구현
 * @param void
 * @return void
 */
void daemonize() {
    pid_t pid, sid;

    pid = fork();
    if (pid < 0) {
        exit(EXIT_FAILURE);
    }

    if (pid > 0) {
        exit(EXIT_SUCCESS);  // 부모 프로세스 종료
    }

    // umask(0);

    // 로그 파일을 엽니다. 기존 코드에서 파일을 올바르게 여는지 확인하세요.
    int fd = open("/home/pi/Desktop/veda/workspace/save/chat_server.log", O_RDWR | O_CREAT | O_APPEND, 0600);
        if (fd == -1) {
        perror("open log");
        printf("로그 파일 열기 실패\n");
        printf("server.c 내의 daemonize()함수에서 로그 파일을 열지 못했습니다. 새로 시작하는 장소이니 올바른 경로를 지정하세요.\n");
        exit(EXIT_FAILURE);
    } else {
        printf("로그 파일 열기 성공\n");
        printf("올바른 경로로 지정이 되었고, 시작이 되었습니다. 이 작동은 client에서 한번 Exit를 하면 server도 종료가 되므로, 반드시 daemon_start.sh를 사용하여 실행을 하세요.\n");
    }

    // 로그 파일로 stdout과 stderr 리다이렉션
    dup2(fd, STDOUT_FILENO);
    dup2(fd, STDERR_FILENO);
    close(fd);  // 파일 디스크립터 닫기

    // 세션 ID 설정
    sid = setsid();
    if (sid < 0) {
        exit(EXIT_FAILURE);
    }

    if ((chdir("/")) < 0) {
        exit(EXIT_FAILURE);
    }

    // stdin, stdout, stderr 닫기
    close(STDIN_FILENO);
    close(STDOUT_FILENO);
    close(STDERR_FILENO);
}

void logo() {
    printf("\033[0;34m ▌ \033[0;33m▐·▄▄▄ .·▄▄▄▄   ▄▄▄·      ▄▄▄·  ▄▄·  ▄▄▄· ·▄▄▄▄  ▄▄▄ .• \033[0;34m▌ \033[0;37m▄ ·.  ▄· ▄▌\n");
    printf("\033[0;34m▪█·█▌▀▄.▀·██▪ ██ ▐█ ▀█     ▐█ ▀█ ▐█ ▌▪▐█ ▀█ ██▪ ██ ▀▄.▀··\033[0;37m██ ▐███▪▐█▪██▌\n");
    printf("\033[0;34m▐█▐█•▐▀▀▪▄▐█· ▐█▌▄█▀▀█     ▄█▀▀█ ██ ▄▄▄█▀▀█ ▐█· ▐█▌▐▀▀▪▄▐█ \033[0;37m▌▐▌▐█·▐█▌▐█▪\n");
    printf("\033[0;34m ███ ▐█▄▄▌██. ██ ▐█ ▪▐▌    ▐█ ▪▐▌▐███▌▐█ ▪▐▌██. ██ ▐█▄▄▌\033[0;37m██ ██▌▐█▌ ▐█▀·.\n");
    printf("\033[0;34m. \033[0;33m▀   ▀▀▀ ▀▀▀▀▀•  ▀  ▀      ▀  ▀ ·▀▀▀  ▀  ▀ ▀▀▀▀▀•  \033[0;37m▀▀▀ ▀▀  █▪▀▀▀  ▀ •\n");

    // 색상 리셋
    printf("\033[0m");
}

/** 
 * @brief 자동 데몬화 모드 함수
 * @param void
 * @return void
 */
void auto_daemon_mode() {
    printf("자동으로 데몬화하여 서버를 시작합니다.\n");
    daemonize();  // 데몬화 함수 호출
    create_network_tcp_process(1, "127.0.0.1", DEFAULT_TCP_PORT);
}

/**
 * @brief 수동 서버 관리 모드 함수
 * @param void
 * @return void
 */
void manual_server_mode() {
    char yn = '\0';
    printf("\nPress Enter to continue...");
    while (getchar() != '\n');  // Enter 키가 눌릴 때까지 대기
    printf("이 프로그램을 자동으로 데몬 화 하시겠습니까? (y/n): ");
    
    while (1) {
        yn = getchar();  // 한 문자를 입력받음
        if (yn == 'y' || yn == 'n') {
            break;  // y 또는 n을 입력받으면 루프 종료
        }
        printf("잘못된 입력입니다. (y/n): ");
        while (getchar() != '\n');  // 버퍼를 비워서 남은 입력을 처리
    }

    if (yn == 'y') {
        daemonize();  // 데몬화 함수 호출
    } else {
        printf("데몬화를 하지 않습니다.\n");
    }

    create_network_tcp_process(1, "127.0.0.1", DEFAULT_TCP_PORT);
}

// /**
//  * @brief main 함수
//  * @param void
//  * @return int
//  */
// int main()
// {
//     char yn = '\0'; // 문자를 저장할 변수

//     // 스크립트를 통해 바로 시작을 하고 싶으면
//     auto_daemon_mode();

//     // server코드를 통해 직접 관리하고 싶으면
//     /*
//         // 로고 출력
//         logo();
        // printf("이 프로그램을 자동으로 데몬 화 하고 싶으시다면 스크립트를 참조하세요.\n");

        // // 사용자가 동작 방식을 선택할 수 있게 함
        // printf("서버 동작 방식을 선택하세요: \n");
        // printf("1. 자동 데몬화\n");
        // printf("2. 수동 서버 관리\n");
        // printf("선택: ");

        // yn = getchar();
        // while (getchar() != '\n');  // 남은 입력 버퍼 비우기

        // if (yn == '1') {
        //     auto_daemon_mode();  // 자동 데몬화 모드 실행
        // } else if (yn == '2') {
        //     manual_server_mode();  // 수동 서버 관리 모드 실행
        // } else {
        //     printf("잘못된 선택입니다. 프로그램을 종료합니다.\n");
        //     return -1;
        // }
//     */

//     return 0;
// }


#define MAX_CLIENTS 10
#define BUFFER_SIZE 1024

/**
 * @brief 네트워크와 클라이언트 간의 채팅 처리 함수
 * 
 * @param num_args 가변 인자 개수 (서버 IP와 포트 전달)
 * @param ... 서버 IP 주소와 포트를 가변 인자로 전달
 */
void kernel_chat(int num_args, ...) {
    int network_fd;
    struct sockaddr_in server_addr;
    va_list args;

    // 가변 인자 리스트 초기화
    va_start(args, num_args);

    const char *server_ip = va_arg(args, const char *); // 첫 번째 인자는 서버 IP
    int port = va_arg(args, int); // 두 번째 인자는 포트

    va_end(args);  // 가변 인자 리스트 종료

    // 네트워크 설정
    network_fd = socket(AF_INET, SOCK_STREAM, 0);  // 소켓 생성
    if (network_fd < 0) {
        perror("socket() 생성 실패");
        kernel_printf("socket() 생성 실패\n");
        exit(EXIT_FAILURE);
    } else {
        kernel_printf("socket() 생성 성공\n");
        kernel_printf("만약 어플리케이션 작동을 시도하였다면 [lsof -i :5100]으로 확인후 [kill -9 1234]후에 다시 시작하시오.\n");
    }

    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = inet_addr(server_ip);  // 가변 인자로 받은 서버 IP
    server_addr.sin_port = htons(port);  // 가변 인자로 받은 포트

    // 소켓 바인딩
    if (bind(network_fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("bind() 실패: 포트가 사용 중일 수 있습니다.");
        kernel_printf("bind() 실패: 포트가 사용 중일 수 있습니다.\n");
        close(network_fd);
        exit(EXIT_FAILURE);
    }
    
    int enable = 1;
    if (setsockopt(network_fd, SOL_SOCKET, SO_REUSEADDR, &enable, sizeof(int)) < 0) {
        perror("setsockopt(SO_REUSEADDR) 실패");
        kernel_printf("setsockopt(SO_REUSEADDR) 실패\n");
        close(network_fd);
        exit(EXIT_FAILURE);
    }

    kernel_printf("포트 %d에서 대기 중입니다. 포트 상태를 확인합니다...\n", port);
    system("netstat -an | grep 5100");

    // /var/log/chatlog 디렉토리가 존재하는지 확인하고, 없으면 생성
    struct stat st = {0};
    if (stat("/var/log/chatlog", &st) == -1) {
        if (mkdir("/var/log/chatlog", 0700) < 0) {
            perror("/var/log/chatlog 디렉토리 생성 실패");
            exit(EXIT_FAILURE);
        }
        kernel_printf("/var/log/chatlog 디렉토리를 생성했습니다.\n");
    }

    int fd = open("/var/log/chatlog/chat_server.log", O_RDWR | O_CREAT | O_APPEND, 0600);
    if (fd < 0) {
        perror("로그 파일 열기 실패");
        exit(EXIT_FAILURE);
    }
    
    // int fd = open("/var/log/chat_server.log", O_RDWR | O_CREAT | O_APPEND, 0600);
    dup2(fd, STDOUT_FILENO);
    dup2(fd, STDERR_FILENO);

    // 소켓 리스닝
    if (listen(network_fd, MAX_CLIENTS) < 0) {
        perror("listen() 실패");
        close(network_fd);
        exit(EXIT_FAILURE);
    }
    printf("서버가 포트 %d에서 대기 중입니다...\n", port);

    // 소켓 포트 정보 조회
    printf("현재 포트 %d 상태를 확인합니다.\n", port);
    printf("**************************************************\n");
    if (system("lsof -i :5100") != 0) {
        perror("lsof 명령 실행 실패");
        kernel_printf("lsof 명령 실행 실패\n");
        close(network_fd);
        exit(EXIT_FAILURE);
    }
    printf("**************************************************\n");

    // 불필요한 변수들을 주석 처리하거나 삭제
    // while (1) {
    //     socklen_t client_len = sizeof(client_addr);
    //     client_fd = accept(network_fd, (struct sockaddr *)&client_addr, &client_len);
    //     if (client_fd < 0) {
    //         perror("accept() 실패");
    //         continue;
    //     }
    //     printf("클라이언트가 연결되었습니다.\n");
    //     ssize_t nbytes;
    //     while ((nbytes = read(client_fd, buffer, sizeof(buffer))) > 0) {
    //         buffer[nbytes] = '\0';
    //         printf("클라이언트 메시지: %s\n", buffer);
    //         const char *response = "서버에서 수신했습니다.\n";
    //         write(client_fd, response, strlen(response));
    //     }
    //     printf("클라이언트 연결 종료\n");
    //     close(client_fd);
    // }

    close(network_fd);  // 네트워크 소켓 닫기
}
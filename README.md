## QT_Kernel_OS 프로젝트
###### 2024.09.20 Update

<img width="801" alt="스크린샷 2024-09-20 오후 3 19 54" src="https://github.com/user-attachments/assets/aead8c10-24aa-4fdd-b386-938faf8915e2">
<img width="799" alt="스크린샷 2024-09-20 오후 3 21 29" src="https://github.com/user-attachments/assets/0b4fd327-4220-4a3c-9c27-7d49efe113bd">


**[tree]**
```
azabell@azabellui-MacBookPro:~/Desktop/workspace_qt/QT_Kernel_OS$ ls -alc
total 42168
drwxr-xr-x   57 azabell  staff     1824  9 20 14:55 .
drwxr-xr-x    4 azabell  staff      128  9 20 15:00 ..
-rw-r--r--@   1 azabell  staff    10244  9 20 13:55 .DS_Store
drwxr-x---    3 azabell  staff       96  8 29 22:10 .cache
drwxr-x---    4 azabell  staff      128  8 28 18:07 .ccls-cache
drwxr-xr-x   15 azabell  staff      480  9 20 14:37 .git
-rw-r--r--    1 azabell  staff       66  8 19 10:29 .gitattributes
-rw-r--r--    1 azabell  staff       29  8 26 17:18 .gitignore
drwxr-xr-x    3 azabell  staff       96  9  2 14:11 .vscode
-rw-r--r--    1 azabell  staff     2841  9 17 23:02 CMakeLists.txt
-rw-r--r--    1 azabell  staff    13688  9 20 14:55 CMakeLists.txt.user
-rw-r--r--    1 azabell  staff    21770  8 25 22:14 CMakeLists.txt.user.0f0aaec
-rw-r--r--    1 azabell  staff    13688  8 25 22:14 CMakeLists.txt.user.1df1f4f
-rw-r--r--    1 azabell  staff    30707  8 25 22:14 CMakeLists.txt.user.2ba5d43
drwxr-xr-x   16 azabell  staff      512  9 20 14:58 C_lib
-rw-r--r--    1 azabell  staff   129159  9 18 23:21 Doxyfile
-rw-r--r--    1 azabell  staff   129104  9 18 22:57 Doxyfile.bak
-rw-r--r--    1 azabell  staff     1068  8 25 22:14 LICENSE
-rw-r--r--    1 azabell  staff    19288  9  8 14:12 README.md
-rw-r--r--@   1 azabell  staff    99628  9 20 13:55 architecture.png
-rw-r--r--    1 azabell  staff     3814  9 18 22:57 architecture.puml
-rw-r--r--@   1 azabell  staff    77198  9 20 13:55 architecture.svg
-rw-r--r--@   1 azabell  staff     9216  8 30 18:56 asm_.srctrlbm
-rw-r--r--@   1 azabell  staff  9623552  8 30 18:56 asm_.srctrldb
-rw-r--r--@   1 azabell  staff     1141  8 30 18:56 asm_.srctrlprj
-rw-r--r--    1 azabell  staff   964922  8 25 22:14 booting_img.jpg
-rw-r--r--    1 azabell  staff     4135  8 25 22:14 bootloader.cpp
-rw-r--r--    1 azabell  staff      450  8 25 22:14 bootloader.h
drwxr-xr-x@  12 azabell  staff      384  9 18 22:01 build
-rw-r--r--    1 azabell  staff     4858  8 25 22:14 cmd.png
-rw-r--r--    1 azabell  staff    56849  9 17 22:57 cmdwindow.cpp
-rw-r--r--    1 azabell  staff     1379  9 17 21:35 cmdwindow.h
-rw-r--r--    1 azabell  staff      588  9 17 11:45 cmdwindow.ui
-rw-r--r--    1 azabell  staff      588  9 17 11:48 cmdwindow.ui.autosave
drwxr-xr-x  184 azabell  staff     5888  9 20 14:37 docs
-rw-r--r--@   1 azabell  staff    28035  9 17 22:52 exit.png
drwxr-xr-x@ 184 azabell  staff     5888  9 18 23:20 html
drwxr-xr-x   96 azabell  staff     3072  9 18 23:19 latex
-rwxr-xr-x    1 azabell  staff      689  8 26 17:16 mac_run.sh
-rw-r--r--    1 azabell  staff      286  8 25 22:14 main.cpp
-rw-r--r--    1 azabell  staff     8032  9 17 22:53 mainwindow.cpp
-rw-r--r--    1 azabell  staff      771  8 25 22:14 mainwindow.h
-rw-r--r--    1 azabell  staff      692  8 25 22:14 mainwindow.ui
-rwxrwxrwx    1 azabell  staff       27  9 16 23:15 make_architecture_png.sh
-rw-r--r--    1 azabell  staff    15752  8 25 22:14 mypc.png
-rw-r--r--    1 azabell  staff     1933  8 25 22:14 processmanager.cpp
-rw-r--r--    1 azabell  staff      882  8 25 22:14 processmanager.h
-rw-r--r--@   1 azabell  staff     9216  8 30 19:36 rebuild_kernelos.srctrlbm
-rw-r--r--@   1 azabell  staff  9656320  8 30 19:36 rebuild_kernelos.srctrldb
-rw-r--r--@   1 azabell  staff      786  8 30 19:36 rebuild_kernelos.srctrlprj
-rw-r--r--    1 azabell  staff      283  9 17 22:52 resources.qrc
-rw-r--r--    1 azabell  staff    33335  8 25 22:14 site_logo.png
-rw-r--r--    1 azabell  staff     1116  9 17 22:56 smartpointerdialog.cpp
-rw-r--r--    1 azabell  staff      603  9 17 22:57 smartpointerdialog.h
-rw-r--r--    1 azabell  staff      930  9 17 22:57 smartpointerdialog.ui
-rw-r--r--    1 azabell  staff   457579  8 25 22:14 start.png
-rw-r--r--    1 azabell  staff    34602  8 25 22:14 trashcan.png
```

```
azabell@azabellui-MacBookPro:~/Desktop/workspace_qt/QT_Kernel_OS/C_lib$ tree
.
├── Makefile
├── include
│   ├── kernel_asm.h
│   ├── kernel_chat.h
│   ├── kernel_engine.h
│   ├── kernel_lib.h
│   ├── kernel_print.h
│   ├── kernel_smartptr.h
│   └── kernel_uniqueptr.h
├── include_printf
│   ├── az_atoi.arm64.o
│   ├── az_atoi.c
│   ├── az_atoi.x86_64.o
│   ├── az_bzero.arm64.o
│   ├── az_bzero.c
│   ├── az_bzero.x86_64.o
│   ├── az_chrpos.arm64.o
│   ├── az_chrpos.c
│   ├── az_chrpos.x86_64.o
│   ├── az_isalpha.arm64.o
│   ├── az_isalpha.c
│   ├── az_isalpha.x86_64.o
│   ├── az_isdigit.arm64.o
│   ├── az_isdigit.c
│   ├── az_isdigit.x86_64.o
│   ├── az_isprint.arm64.o
│   ├── az_isprint.c
│   ├── az_isprint.x86_64.o
│   ├── az_itoa.arm64.o
│   ├── az_itoa.c
│   ├── az_itoa.x86_64.o
│   ├── az_memalloc.arm64.o
│   ├── az_memalloc.c
│   ├── az_memalloc.x86_64.o
│   ├── az_memset.arm64.o
│   ├── az_memset.c
│   ├── az_memset.x86_64.o
│   ├── az_nbrlen.arm64.o
│   ├── az_nbrlen.c
│   ├── az_nbrlen.x86_64.o
│   ├── az_putchar.arm64.o
│   ├── az_putchar.c
│   ├── az_putchar.x86_64.o
│   ├── az_putchar_fd.arm64.o
│   ├── az_putchar_fd.c
│   ├── az_putchar_fd.x86_64.o
│   ├── az_puthex.arm64.o
│   ├── az_puthex.c
│   ├── az_puthex.x86_64.o
│   ├── az_putnbr.arm64.o
│   ├── az_putnbr.c
│   ├── az_putnbr.x86_64.o
│   ├── az_putoctal.arm64.o
│   ├── az_putoctal.c
│   ├── az_putoctal.x86_64.o
│   ├── az_putstr.arm64.o
│   ├── az_putstr.c
│   ├── az_putstr.x86_64.o
│   ├── az_putunsigned.arm64.o
│   ├── az_putunsigned.c
│   ├── az_putunsigned.x86_64.o
│   ├── az_strchr.arm64.o
│   ├── az_strchr.c
│   ├── az_strchr.x86_64.o
│   ├── az_strcmp.arm64.o
│   ├── az_strcmp.c
│   ├── az_strcmp.x86_64.o
│   ├── az_strlen.arm64.o
│   ├── az_strlen.c
│   ├── az_strlen.x86_64.o
│   ├── az_strsub.arm64.o
│   ├── az_strsub.c
│   ├── az_strsub.x86_64.o
│   └── kernel_pr_he.h
├── kernel_chat.a
├── kernel_engine.a
├── kernel_lib.a
├── kernel_printf.a
├── memo.c
├── non_existent_file.txt
├── src
│   ├── ename.c.inc
│   ├── kernel_asm.arm64.o
│   ├── kernel_asm.c
│   ├── kernel_asm.x86_64.o
│   ├── kernel_chat.arm64.o
│   ├── kernel_chat.c
│   ├── kernel_chat.x86_64.o
│   ├── kernel_engine.arm64.o
│   ├── kernel_engine.c
│   ├── kernel_engine.x86_64.o
│   ├── kernel_lib.arm64.o
│   ├── kernel_lib.c
│   ├── kernel_lib.x86_64.o
│   ├── kernel_printf.arm64.o
│   ├── kernel_printf.c
│   ├── kernel_printf.x86_64.o
│   ├── td_kernel_engine.arm64.o
│   ├── td_kernel_engine.c
│   └── td_kernel_engine.x86_64.o
├── td_kernel_engine.a
├── td_kernel_engine.exec
└── test

4 directories, 100 files
```

**QT_Kernel_OS는 Hanwha Vision VEDA Academy에서 개발된 간단한 커널 OS 프로토타입입니다. 이 프로젝트는 Qt를 사용하여 기본적인 운영 체제 기능을 시뮬레이션하고, 커맨드 라인 명령어 처리와 스마트 포인터 관리 기능을 포함하고 있습니다.**

#### 프로젝트 개요
1. 버전: 1.2 (2024.09.20)
2. 주요 기능: 커맨드 라인 명령어 처리, 스마트 포인터 관리, 멀티스레드 및 멀티프로세스 처리, 네트워크 통신 관리
3. 개발자: 박지우 (Azabell1993)

#### 해당 프로젝트의 docs
https://azabell1993.github.io/QT_Kernel_OS/files.html

#### 주요 기능
1. 커맨드 라인 명령어 처리
> QT_Kernel_OS는 실제 명령어를 처리할 수 있는 커맨드 라인 인터페이스를 제공합니다. 사용자는 다음과 같은 명령어를 사용할 수 있습니다.
```
pwd: 현재 작업 디렉터리 출력
ls: 파일 및 디렉터리 목록 출력
cd <directory>: 디렉터리 변경
cp <source> <destination>: 파일 복사
mv <source> <destination>: 파일 이동
df -h: 디스크 사용량 확인
du <directory>: 디렉터리 크기 확인
printf("message"): 메시지 출력
history: 이전에 입력한 명령어 기록 출력
mkdir <directory>: 디렉터리 생성
rmdir <directory>: 디렉터리 삭제
clear: 화면 지우기
ifconfig: 네트워크 인터페이스 정보 출력
netstat: 네트워크 연결 정보 출력
help_modal: 새로운 모달 창에서 도움말 표시
exit: 셸 종료
shutdown || poweroff: 시스템 종료
```

#### 2. 스마트 포인터 관리
QT_Kernel_OS 프로젝트는 스마트 포인터를 관리하기 위한 smartptr.h 라이브러리를 사용합니다. 이 라이브러리는 자동 메모리 관리를 통해 메모리 누수를 방지하고, 동시 접근 제어를 위해 뮤텍스와 참조 카운트 관리 기능을 포함하고 있습니다.
라이브러리 위치: c_smartpointer

#### 3. 채팅 서버 구현
이 프로젝트는 스마트 포인터 라이브러리를 활용하여 멀티 클라이언트 채팅 서버를 구현하였습니다. 서버는 다중 사용자의 메시지 브로드캐스트, 채팅방 관리, 사용자 관리 기능을 포함하고 있습니다.
채팅 서버 위치: smartpointer_multi_chat

#### 4. 멀티스레드 및 멀티프로세스 처리
프로젝트는 멀티스레드 및 멀티프로세스 환경에서 스마트 포인터를 활용하여 안전한 메모리 관리를 제공합니다. 스레드 간의 동기화, 프로세스 간의 통신, 에러 처리를 위한 다양한 메커니즘이 구현되어 있습니다.

#### 5. 네트워크 통신 관리
프로젝트는 TCP/IP 네트워크 통신을 시뮬레이션하며, 클라이언트와 서버 간의 안정적인 메시지 전송을 위해 스마트 포인터를 사용한 소켓 통신 관리를 포함하고 있습니다.

#### 6. 핵심 로직
### 1. kernel_chat.h 파일
 kernel_chat.h 파일은 채팅 서버와 클라이언트 간의 네트워크 통신을 처리하기 위한 여러 함수를 포함하고 있습니다. 이 파일은 socket, bind, listen 등의 시스템 호출을 통해 TCP 서버를 설정하고 클라이언트 요청을 처리합니다. 주로 스마트 포인터(SmartPtr), 뮤텍스(pthread_mutex_t), 그리고 클라이언트 관리 기능이 포함되어 있습니다.

### 주요 함수 및 설명
`auto_daemon_mode()`  
이 함수는 자동으로 서버를 데몬화하여 백그라운드에서 실행되도록 설정합니다.  
daemonize() 함수를 호출하여 프로세스를 데몬화하고, 서버를 실행하는데 필요한 네트워크 설정을 완료합니다.  
  
`list_users()`  
현재 접속 중인 클라이언트의 사용자명을 출력하는 함수입니다.  
client_infos 배열에 저장된 클라이언트 정보를 순회하며, 각 클라이언트의 사용자명과 참여 중인 채팅방 ID를 출력합니다.  
  
`broadcast_message(int sender_fd, char *message, int room_id)`   
특정 채팅방에 있는 모든 클라이언트에게 메시지를 브로드캐스트하는 함수입니다.  
메시지를 보낸 클라이언트를 제외한 모든 클라이언트에게 메시지를 전달합니다.  
메시지는 브로드캐스트되기 전에 log_chat_message()를 통해 로그로 저장됩니다.  
  
`log_chat_message(const char *message)`  
채팅 메시지를 로그 파일에 저장하는 함수입니다.  
현재 날짜를 기준으로 로그 파일명을 생성하고, 메시지를 해당 파일에 기록합니다.  
로그 파일 접근은 뮤텍스를 사용하여 동기화됩니다.  
  
`client_handler(void *arg)`  
각 클라이언트와의 통신을 처리하는 스레드 함수입니다.  
클라이언트가 연결되면, 사용자명을 받고, 해당 클라이언트의 채팅방을 설정한 후, 클라이언트로부터 메시지를 읽고 브로드캐스트합니다.  
클라이언트 연결이 종료되면 리소스를 정리하고 스레드를 종료합니다.  
  
### 2. kernel_smartptr.h 파일
 kernel_smartptr.h 파일은 스마트 포인터의 구조체와 이를 관리하는 함수들을 정의하고 있습니다. 스마트 포인터는 메모리 관리와 참조 카운트를 통해 메모리 누수를 방지하고, 동기화를 위해 뮤텍스를 사용합니다.

### 주요 함수 및 설명
#### SmartPtr 구조체
이 구조체는 스마트 포인터의 메모리 주소(ptr), 참조 카운트(ref_count), 그리고 뮤텍스(mutex)를 관리합니다.
이 구조체를 통해 동시 접근이 발생할 수 있는 멀티스레드 환경에서도 안전하게 메모리를 관리할 수 있습니다.

`create_smart_ptr(size_t size, ...)`  
스마트 포인터를 생성하는 함수입니다.
메모리 할당과 함께 참조 카운트를 1로 초기화하고, 뮤텍스를 초기화합니다.
가변 인자를 받아 초기값을 설정할 수 있습니다.

`retain(SmartPtr *sp)`  
스마트 포인터의 참조 카운트를 증가시키는 함수입니다.
참조 카운트 변경 시 뮤텍스를 사용하여 동기화합니다.

`release(SmartPtr *sp)`  
스마트 포인터의 참조 카운트를 감소시키고, 필요시 메모리를 해제하는 함수입니다.
참조 카운트가 0이 되면, 메모리를 해제하고 뮤텍스를 파괴합니다.

### 스마트포인터 라이브러리 참조

**[c_smartpointer]**  
[![Readme Card](https://github-readme-stats.vercel.app/api/pin/?username=Azabell1993&repo=c_smartpointer)](https://github.com/Azabell1993/c_smartpointer)

<img width="558" alt="스크린샷 2024-09-13 오후 4 23 41" src="https://github.com/user-attachments/assets/dc5e9bd2-22d7-4e85-956b-b58b39b5bf74">

### 3. kernel_uniqueptr.h 파일
 kernel_uniqueptr.h 파일은 고유 스마트 포인터(UniquePtr)와 공유 스마트 포인터(SharedPtr)를 구현한 파일입니다. 이 파일은 메모리 소유권 이전, 참조 카운트 관리, 메모리 해제 등의 기능을 포함하고 있습니다.

### 주요 함수 및 설명
`create_unique_ptr(size_t size, void (*deleter)(void*))`  
고유 스마트 포인터를 생성하는 함수입니다.
메모리를 할당하고, 지정된 소멸자 함수를 설정합니다.

`release_unique_ptr(UniquePtr *up)`  
고유 스마트 포인터의 메모리를 해제하는 함수입니다.
메모리가 해제되면, 소멸자 함수가 호출됩니다.

`create_shared_ptr(size_t size, void (*deleter)(void*))`  
공유 스마트 포인터를 생성하는 함수입니다.
메모리를 할당하고, 참조 카운트를 1로 초기화하며, 뮤텍스를 설정합니다.

`retain_shared_ptr(SharedPtr *sp)`  
공유 스마트 포인터의 참조 카운트를 증가시키는 함수입니다.
참조 카운트가 증가할 때마다 뮤텍스를 사용하여 동기화합니다.

`release_shared_ptr(SharedPtr *sp)`  
공유 스마트 포인터의 참조 카운트를 감소시키고, 필요 시 메모리를 해제하는 함수입니다.
참조 카운트가 0이 되면, 메모리와 뮤텍스를 해제합니다.

### 4. td_kernel_engine.c 파일
 td_kernel_engine.c 파일은 고급 커널 엔진을 구현한 코드로, 멀티스레딩, 멀티프로세싱, 스마트 포인터, 동기화 메커니즘, 네트워크 통신 등을 포함한 다양한 시스템 시뮬레이션을 제공합니다.

### 주요 함수 및 설명
`main()`  
프로그램의 진입점으로, 다양한 커널 기능을 테스트하고 실행합니다.
멀티스레딩, 멀티프로세싱, 스마트 포인터 테스트, 동기화 메커니즘 테스트 등을 포함합니다.
네트워크 통신 시뮬레이션은 kernel_chat() 함수를 사용하여 채팅 서버와 클라이언트 간의 통신을 처리합니다.

`kernel_chat(int num_args, ...)`  
네트워크 통신을 위한 함수로, TCP 서버를 생성하고 클라이언트 연결을 처리합니다.
소켓을 생성하고, 서버 IP와 포트를 설정하며, 클라이언트 요청을 처리합니다.
이 함수는 kernel_chat.h 파일의 채팅 서버 구현을 참고하여 작성되었습니다.

### 채팅서버 참조

**[smartpointer_multi_chat]**  
[![Readme Card](https://github-readme-stats.vercel.app/api/pin/?username=Azabell1993&repo=smartpointer_multi_chat)](https://github.com/Azabell1993/smartpointer_multi_chat)

#### 채팅 서버 이식 관련
kernel_chat.h 파일에 정의된 함수들을 사용하여 채팅 서버를 구현할 수 있습니다.
채팅 서버는 클라이언트와의 통신을 관리하고, 메시지 브로드캐스트, 클라이언트 관리, 로그 기록 등의 기능을 제공합니다.
이식된 코드에서는 kernel_chat(int num_args, ...) 함수가 네트워크 통신을 처리하며, 서버의 IP 주소와 포트를 인자로 받아 TCP 서버를 설정합니다.

----------- 
### 설치 및 빌드 방법
1. QT_Kernel_OS
저장소를 클론합니다.
> git clone https://github.com/Azabell1993/QT_Kernel_OS.git

2. 프로젝트 디렉토리로 이동합니다.
> cd QT_Kernel_OS/C_lib
> make
make시 마지막에 아래와 같은 내용이 뜬다면 성공적으로 build가 된 것입니다.
  
```
clang -std=c11 -Wall -Wextra -Iinclude -Iinclude_printf -Wformat -Werror -Wno-unused-label -Wno-unused-function -fcommon -c src/kernel_lib.c -o src/kernel_lib.arm64.o
Compiling for arm64: src/kernel_printf.c
clang -std=c11 -Wall -Wextra -Iinclude -Iinclude_printf -Wformat -Werror -Wno-unused-label -Wno-unused-function -fcommon -c src/kernel_printf.c -o src/kernel_printf.arm64.o
Compiling for arm64: src/td_kernel_engine.c
clang -std=c11 -Wall -Wextra -Iinclude -Iinclude_printf -Wformat -Werror -Wno-unused-label -Wno-unused-function -fcommon -c src/td_kernel_engine.c -o src/td_kernel_engine.arm64.o
Creating static library kernel_lib.a
libtool -static -o kernel_lib.a src/kernel_asm.x86_64.o src/kernel_chat.x86_64.o src/kernel_engine.x86_64.o src/kernel_lib.x86_64.o src/kernel_printf.x86_64.o src/td_kernel_engine.x86_64.o src/kernel_asm.arm64.o src/kernel_chat.arm64.o src/kernel_engine.arm64.o src/kernel_lib.arm64.o src/kernel_printf.arm64.o src/td_kernel_engine.arm64.o
/Library/Developer/CommandLineTools/usr/bin/libtool: file: src/kernel_asm.x86_64.o has no symbols
/Library/Developer/CommandLineTools/usr/bin/libtool: file: src/kernel_asm.arm64.o has no symbols
ranlib kernel_lib.a
/Library/Developer/CommandLineTools/usr/bin/ranlib: file: kernel_lib.a(kernel_asm.x86_64.o) has no symbols
/Library/Developer/CommandLineTools/usr/bin/ranlib: file: kernel_lib.a(kernel_asm.arm64.o) has no symbols
Creating kernel_engine.a static library
libtool -static -o kernel_engine.a src/kernel_engine.x86_64.o
ranlib kernel_engine.a
Creating kernel_chat.a static library
libtool -static -o kernel_chat.a src/kernel_chat.x86_64.o
ranlib kernel_chat.a
Creating td_kernel_engine.a static library
libtool -static -o td_kernel_engine.a src/td_kernel_engine.x86_64.o
ranlib td_kernel_engine.a
Building td_kernel_engine executable
clang -std=c11 -Wall -Wextra -Iinclude -Iinclude_printf -Wformat -Werror -Wno-unused-label -Wno-unused-function -fcommon -o td_kernel_engine.exec src/td_kernel_engine.c kernel_lib.a kernel_engine.a kernel_printf.a kernel_chat.a td_kernel_engine.a
```

3. td_kernel_engine.exec를 실행하여 확인을 합니다.
<img width="1792" alt="스크린샷 2024-09-20 오후 2 50 42" src="https://github.com/user-attachments/assets/b9a4e74d-ee1a-44cb-9673-146b535a35b2">

4. QT Creator에 들어가서 작동을 확인합니다.
<img width="1430" alt="스크린샷 2024-09-20 오후 2 52 25" src="https://github.com/user-attachments/assets/008fa330-5582-4454-ab35-63c8ed91e1d9">

-> 만약, 아래와 같은 메세지가 뜬 다면, QT 어플리케이션이 완전히 종료되지 않아 모두 닫고 재시작을 해야합니다. (포트번호 중복 이슈)
<img width="888" alt="스크린샷 2024-09-20 오후 2 52 35" src="https://github.com/user-attachments/assets/607da6d8-fa30-4def-bd40-864b49a0ef63">


5. 크로스 컴파일이 가능합니다.
<img width="1249" alt="스크린샷 2024-09-20 오후 2 51 57" src="https://github.com/user-attachments/assets/2043438b-60ef-4118-be8f-938f32a5f2f7">


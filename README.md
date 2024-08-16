# QT_Kernel_OS
A simple kernel OS prototype built using Qt at Hanwha Vision VEDA Academy.  
Park Ji-woo, a first-term trainee of Hanwha Vision VEDA Academy.  
A company called Hanwha Vision in Korea selected 60 students for training at the academy.  

## Version 1.0 (2024.08.16)

This project is a prototype version of a basic operating system kernel simulator created using Qt. The main features include a bootloader screen simulation, command processing, and a simulated kernel process creation.

### Features

- **Bootloader Simulation**: 
  - The system begins with a bootloader screen that simulates the loading of the kernel.
  - The boot process is displayed as a progress bar, simulating the loading of the kernel and initialization of subsystems.

- **Process Creation**:
  - A simple C function `kernel_create_process` is used to simulate the creation of a kernel process.
  - The success or failure of the process creation is displayed using both `qDebug()` for debugging output and `QMessageBox` for user notification.

### Screenshots

![Bootloader Screen](https://github.com/user-attachments/assets/ce57e7b5-0489-4061-932b-5046c28c175c)  
*Bootloader screen displaying the kernel loading process.*

![Process Creation Success](https://github.com/user-attachments/assets/eb6f511d-8864-4b54-b794-058a9f7af576)  
*MessageBox showing successful process creation.*

![Process Creation Failure](https://github.com/user-attachments/assets/6097e3df-3a2e-40a5-b6e7-44c4fed937e8)  
*MessageBox showing failure in process creation.*

### Video Demo

Watch the full demonstration of the prototype on [YouTube](https://youtu.be/F4Z4J-MmfZI).

### Project Structure

- **os (Main Project Directory)**
  - **bootloader.cpp**: Implements the bootloader simulation and handles process creation.
  - **main.cpp**: Entry point of the application, initializing the bootloader.
  - **C_lib**: Contains the C implementation of the kernel process management.
    - **kernel_lib.c**: Defines the `kernel_create_process` function.
    - **kernel_lib.h**: Header file for the C library.

### Detailed Code Description

#### 1. `bootloader.cpp`

This file implements the bootloader UI and logic using Qt. It includes functions to simulate the kernel loading process, handle key events, and initiate the main OS window.

- **Bootloader Constructor**: Initializes the bootloader window with a logo and a booting message.
- **`simulateBootProcess` Function**: Simulates the kernel loading process with a progress bar.
- **`startOS` Function**: Calls the `kernel_create_process` C function and displays the result using `QMessageBox`.
- **`keyPressEvent` Function**: Detects the Enter key press to proceed from the bootloader to the main OS window.

#### 2. `main.cpp`

This file contains the `main` function, which initializes the Qt application and starts the bootloader.

#### 3. `C_lib/kernel_lib.c`

This file provides a basic implementation of the kernel process creation function. The `kernel_create_process` function simulates the creation of a process by printing a message to the console and returning a success status.

- **`kernel_create_process` Function**: Simulates a process creation, always returning `true` to indicate success.

### How to Build

To build this project, ensure you have Qt installed, then follow these steps:

1. Clone the repository.
2. Navigate to the project directory.
3. Run `cmake` or open the project in Qt Creator.
4. Build the project using the appropriate build tool (e.g., `make`, `ninja`).

### Future Enhancements

- Implement actual process management with more detailed simulation.
- Expand the command set for the simulated OS.
- Improve the bootloader interface with more realistic elements.

---------------

# QT_Kernel_OS
QT를 사용하여 `Hanwha Vision VEDA Academy`에서 개발한 간단한 커널 OS 프로토타입입니다.  
`Hanwha Vision VEDA Academy`(이하 `한화비전 베다 아카데미`) 1기 교육생 박지우.  
  
## 버전 1.0 (2024.08.16)

이 프로젝트는 Qt를 사용하여 만든 기본적인 운영 체제 커널 시뮬레이터의 프로토타입 버전입니다. 주요 기능으로는 부트로더 화면 시뮬레이션, 명령어 처리, 그리고 커널 프로세스 생성을 포함하고 있습니다.

### 주요 기능

- **부트로더 시뮬레이션**:
  - 시스템이 커널을 로드하는 부트로더 화면으로 시작됩니다.
  - 부팅 과정은 진행 상태 표시줄로 표시되며, 커널 로드와 서브시스템 초기화를 시뮬레이션합니다.

- **프로세스 생성**:
  - 간단한 C 함수인 `kernel_create_process`를 사용하여 커널 프로세스 생성을 시뮬레이션합니다.
  - 프로세스 생성의 성공 여부는 `qDebug()`를 통해 디버깅 출력으로, `QMessageBox`를 통해 사용자에게 시각적으로 표시됩니다.

### 스크린샷

![Bootloader Screen](https://github.com/user-attachments/assets/ce57e7b5-0489-4061-932b-5046c28c175c)  
*커널 로드 과정을 표시하는 부트로더 화면.*

![Process Creation Success](https://github.com/user-attachments/assets/eb6f511d-8864-4b54-b794-058a9f7af576)  
*프로세스 생성 성공을 알리는 메시지 박스.*

![Process Creation Failure](https://github.com/user-attachments/assets/6097e3df-3a2e-40a5-b6e7-44c4fed937e8)  
*프로세스 생성 실패를 알리는 메시지 박스.*

### 동영상 데모

프로토타입의 전체 데모를 [YouTube](https://youtu.be/F4Z4J-MmfZI)에서 시청하세요.

### 프로젝트 구조

- **os (메인 프로젝트 디렉터리)**
  - **bootloader.cpp**: 부트로더 시뮬레이션을 구현하고 프로세스 생성을 처리합니다.
  - **main.cpp**: 애플리케이션의 진입점으로, 부트로더를 초기화합니다.
  - **C_lib**: 커널 프로세스 관리의 C 구현을 포함합니다.
    - **kernel_lib.c**: `kernel_create_process` 함수를 정의합니다.
    - **kernel_lib.h**: C 라이브러리의 헤더 파일.

### 코드 설명

#### 1. `bootloader.cpp`

이 파일은 Qt를 사용하여 부트로더 UI와 로직을 구현합니다. 커널 로딩 과정을 시뮬레이트하고, 키 이벤트를 처리하며, 메인 OS 윈도우를 시작하는 함수들을 포함하고 있습니다.

- **부트로더 생성자**: 로고와 부팅 메시지로 부트로더 창을 초기화합니다.
- **`simulateBootProcess` 함수**: 진행 상태 표시줄로 커널 로딩 과정을 시뮬레이션합니다.
- **`startOS` 함수**: `kernel_create_process` C 함수를 호출하고 결과를 `QMessageBox`로 표시합니다.
- **`keyPressEvent` 함수**: 엔터 키를 감지하여 부트로더에서 메인 OS 윈도우로 이동합니다.

#### 2. `main.cpp`

이 파일은 `main` 함수를 포함하며, Qt 애플리케이션을 초기화하고 부트로더를 시작합니다.

#### 3. `C_lib/kernel_lib.c`

이 파일은 커널 프로세스 생성 함수의 기본 구현을 제공합니다. `kernel_create_process` 함수는 프로세스를 생성하는 것을 시뮬레이션하며, 콘솔에 메시지를 출력하고 성공 상태를 반환합니다.

- **`kernel_create_process` 함수**: 프로세스 생성을 시뮬레이션하며 항상 `true`를 반환하여 성공을 가정합니다.

### 빌드 방법

이 프로젝트를 빌드하려면 Qt가 설치되어 있어야 하며, 다음 단계를 따르세요:

1. 저장소를 클론합니다.
2. 프로젝트 디렉토리로 이동합니다.
3. `cmake`를 실행하거나 Qt Creator에서 프로젝트를 엽니다.
4. 적절한 빌드 도구(`make`, `ninja` 등)를 사용하여 프로젝트를 빌드합니다.

### 향후 개선 사항

- 실제 프로세스 관리를 구현하여 더욱 자세한 시뮬레이션을 추가할 예정입니다.
- 시뮬레이션된 OS의 명령어 세트를 확장할 예정입니다.
- 부트로더 인터페이스를 개선하여 더욱 현실적인 요소를 추가할 계획입니다.


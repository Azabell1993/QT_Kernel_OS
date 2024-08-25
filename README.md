# QT_Kernel_OS  

![image](https://github.com/user-attachments/assets/f1d69a48-f0eb-46ca-9c86-0bf5dd1bf5f7)  
  
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
Watch the full demonstration of the prototype on [YouTube](https://www.youtube.com/watch?v=ARBjsnRBlO8).

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

프로토타입의 전체 데모를 [YouTube](https://www.youtube.com/watch?v=ARBjsnRBlO8)에서 시청하세요.

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

------------------------------

**English Version:**

## Version 1.1 (2024.08.24)

In this update, several enhancements have been made to the kernel OS prototype, focusing on improving the functionality and formatting of the custom printf implementation, specifically handling integers, floating-point numbers, and character outputs.

### New Features

- **Enhanced `printf` Functionality:**
  - **Integer Formatting:** Improved handling of integers with various formatting options such as zero-padding, width specification, and handling of arithmetic expressions.
  - **Floating-Point Precision:** Added support for floating-point numbers with precision control. The `printf` function can now handle complex float formatting such as `%f`, `%.2f`, `%+08.3f`, etc.
  - **Character Output:** Enhanced the ability to print characters using `%c` and handle various flags and width specifiers.

### Detailed Code Enhancements

#### 1. `CmdWindow::executeKernelPrintf`

The `executeKernelPrintf` function has been updated to correctly process and format various data types, particularly floating-point numbers and integers. The implementation now supports advanced formatting options, including precision for floats and arithmetic operations within format strings.

- **Floating-Point Formatting:** Properly handles precision (`%.2f`, `%+08.3f`) and width for floating-point numbers.
- **Integer Handling:** Supports formatting with flags like `+`, `-`, `0`, and width specifications. Also, handles arithmetic operations within format strings, making it more versatile for mathematical expressions.
- **Character Output:** Supports character formatting with proper handling of width and alignment.

#### 2. `CmdWindow::parseArguments`

Enhanced the argument parsing to correctly interpret and handle floating-point numbers, integers (including arithmetic expressions), and characters. This function ensures that the correct data types are passed to the `executeKernelPrintf` function for formatting.

#### 3. `CmdWindow::evaluateSimpleExpression`

This function now supports evaluating simple arithmetic expressions provided within format strings, returning the correct integer or floating-point result. This feature allows for more dynamic and calculated outputs directly from the `printf` format strings.

### Testing and Examples
```
  // C Static Library
  #include "kernel_print.h"
  
  // Directly hard-coded C functions (kernel_lib.h)
  extern "C" {
      bool kernel_create_process(const char *process_name);
      void kernel_list_processes();
      bool kernel_kill_process(const char *process_name);
      void register_print_function(void (*print_function)(const char *str));
      void az_printf(const char *format, ...);
      void kernel_putchar(char c);
  }
  
  /** Test **/
  kernel_printf("\n\n******** Test kernel_printf Function ********\n");
  kernel_printf("Hello world!\n");
  kernel_printf("A single character : %c \n", 'T');
  kernel_printf("An integer : %d \n", 37);
  kernel_printf("An integer : %d \n", 299);
  kernel_printf("5-4 = %d\n", 1);
  
  int a = 1;
  int b = 2;
  
  kernel_printf("%d + %d = %d\n", a, b, a + b);
  kernel_printf("%d\t\t\t String.\n", 12345678);
  kernel_printf("-650\n");
  kernel_printf("%+d\n", 430);
  kernel_printf("%+1d\n", 650);
  kernel_printf("%+10d\n", 499);
  kernel_printf("% 3d\n", 1230);
  kernel_printf("%08d\n", 342);
  kernel_printf("%+03d\n", -430);
  kernel_printf("%3d\n", -43);
  kernel_printf("%u\n", 23919293929392);
  kernel_printf("%+-u\n", 12345);
  kernel_printf("%+10u\n", 12345);
  kernel_printf("%-4s\n", "Az");
  kernel_printf("%o\n", 333);
  kernel_printf("%-0#+10o\n", 2048);
  kernel_printf("%X\n", 12345678);
  kernel_printf("%#+x\n", 12345678);
  kernel_printf("\n\nfunction call Test\n");
  kernel_printf("%d + %d = %d", 5, 6, function_Test(5, 6));
  kernel_printf("\n");
  
  /** There are two ways to bring in the static library. The above kernel_printf is from the kernel_printf.h header, and below is from kernel_lib.h. **/
  /** Since only kernel_printf.h is included in this code file, the below declarations should ideally be in the header file as well. **/
  /** However, for educational purposes, we are using the extern macro instead of including the header file. **/
  /** Declaring this removes the warning. **/
  /*** kernel_putchar ***/
  kernel_putchar('H');
  kernel_putchar('E');
  kernel_putchar('L');
  kernel_putchar('L');
  kernel_putchar('O');
  kernel_putchar('\n');
  
  kernel_putchar('V');
  kernel_putchar('E');
  kernel_putchar('D');
  kernel_putchar('A');
  kernel_putchar('\n');
  
  kernel_printf("-\n");
  kernel_printf(".\n");
  kernel_printf("/\n");
  kernel_printf("'()*+,-./\n");
```
These tests have shown that the enhanced `printf` functionality can handle a variety of cases, ensuring that the output is correctly formatted according to the specified precision, width, and flags.

### Screenshots
  
<img width="807" alt="스크린샷 2024-08-24 오후 9 19 18" src="https://github.com/user-attachments/assets/37a2a11f-b18f-4ae0-9ff9-13c59d76610b">  

*QT Desktop Application*
    
<img width="951" alt="스크린샷 2024-08-24 오후 9 21 37" src="https://github.com/user-attachments/assets/0911c744-cbfd-4c3c-8863-faab6df96ce9">  

*C - printf function (prototype)*


### Future Improvements
- Implement actual process management with more detailed simulation.
- Expand the command set for the simulated OS.
- Improve the bootloader interface with more realistic elements.
- Extend `printf` functionality to support even more complex formats, including hexadecimal floats, scientific notation, and more.
- Implement error handling for unsupported or incorrect format specifiers to make the system more robust.

---------------------------------

**Korean Version:**

## 버전 1.1 (2024.08.24)

이번 업데이트에서는 사용자 정의 `printf` 구현의 기능 및 서식 개선에 중점을 두어, 정수, 부동소수점 숫자 및 문자 출력 기능을 향상시켰습니다.

### 새로운 기능

- **향상된 `printf` 기능:**
  - **정수 서식 처리:** 다양한 서식 옵션(제로 패딩, 너비 지정, 산술 표현식 처리)을 개선하여 정수 출력 기능을 향상시켰습니다.
  - **부동소수점 정밀도:** 부동소수점 숫자의 정밀도 제어 기능이 추가되었습니다. 이제 `printf` 함수는 `%f`, `%.2f`, `%+08.3f` 등의 복잡한 소수점 형식을 처리할 수 있습니다.
  - **문자 출력:** `%c`를 사용한 문자 출력 기능을 강화하고, 다양한 플래그 및 너비 지정자를 처리할 수 있습니다.

### 코드 향상 사항

#### 1. `CmdWindow::executeKernelPrintf`

`executeKernelPrintf` 함수는 다양한 데이터 유형, 특히 부동소수점 숫자 및 정수를 올바르게 처리하고 서식화할 수 있도록 업데이트되었습니다. 이번 구현에서는 소수점 정밀도, 정수 서식 처리, 산술 연산 등을 포함한 고급 서식 옵션을 지원합니다.

- **부동소수점 서식:** 소수점 정밀도(`%.2f`, `%+08.3f`) 및 너비를 적절히 처리합니다.
- **정수 처리:** `+`, `-`, `0`와 같은 플래그 및 너비 지정자를 지원하며, 서식 문자열 내 산술 연산을 처리할 수 있습니다.
- **문자 출력:** 너비 및 정렬을 적절히 처리하여 문자 서식을 지원합니다.

#### 2. `CmdWindow::parseArguments`

부동소수점 숫자, 정수(산술 표현식 포함), 문자 등을 올바르게 해석하고 처리할 수 있도록 인수 파싱 기능을 향상시켰습니다. 이 함수는 `executeKernelPrintf` 함수에 올바른 데이터 유형을 전달하여 서식화를 보장합니다.

#### 3. `CmdWindow::evaluateSimpleExpression`

이 함수는 이제 서식 문자열 내 제공된 간단한 산술 표현식을 평가하여 올바른 정수 또는 부동소수점 결과를 반환합니다. 이 기능은 `printf` 서식 문자열에서 더욱 동적이고 계산된 출력을 허용합니다.

### 테스트 및 예제
```
  // C 정적 라이브러리
  #include "kernel_print.h"
  
  // 직접 하드코딩된 C 함수(kernel_lib.h)
  extern "C" {
    bool kernel_create_process(const char *process_name);
    void kernel_list_processes();
    bool kernel_kill_process(const char *process_name);
    void register_print_function(void (*print_function)(const char *str));
    void az_printf(const char *format, ...);
    void kernel_putchar(char c);
  }

  /** test **/
  kernel_printf("\n\n******** Test kernel_printf Function ********\n");
  kernel_printf("Hello world!\n");
  kernel_printf("A single character : %c \n", 'T');
  kernel_printf("An integer : %d \n", 37);
  kernel_printf("An integer : %d \n", 299);
  kernel_printf("5-4 = %d\n", 1);

  int a = 1;
  int b = 2;

  kernel_printf("%d + %d = %d\n", a, b, a + b);
  kernel_printf("%d\t\t\t String.\n", 12345678);
  kernel_printf("-650\n");
  kernel_printf("%+d\n", 430);
  kernel_printf("%+1d\n", 650);
  kernel_printf("%+10d\n", 499);
  kernel_printf("% 3d\n", 1230);
  kernel_printf("%08d\n", 342);
  kernel_printf("%+03d\n", -430);
  kernel_printf("%3d\n", -43);
  kernel_printf("%u\n", 23919293929392);
  kernel_printf("%+-u\n", 12345);
  kernel_printf("%+10u\n", 12345);
  kernel_printf("%-4s\n", "Az");
  kernel_printf("%o\n", 333);
  kernel_printf("%-0#+10o\n", 2048);
  kernel_printf("%X\n", 12345678);
  kernel_printf("%#+x\n", 12345678);
  kernel_printf("\n\nfunction call Test\n");
  kernel_printf("%d + %d = %d", 5, 6, function_Test(5, 6));
  kernel_printf("\n");

  /** 정적 라이브러리를 갖고 오는 방법은 두가지임. 위의 kernel_printf는 kernel_printf.h헤더이며 아래는 kernel_lib.h임. **/
  /** 따라서 이 코드 파일에서 kernel_printf.h만 했기 때문에 아래도 헤더파일로 선언해야하는데 **/
  /** 공부목적을 위해서 헤더 파일이 아닌 extern 메크로를 사용함 **/
  /** 선언을 하니 워닝이 사라짐 **/
  /*** kernel_putchar ***/
  kernel_putchar('H');
  kernel_putchar('E');
  kernel_putchar('L');
  kernel_putchar('L');
  kernel_putchar('O');
  kernel_putchar('\n');

  kernel_putchar('V');
  kernel_putchar('E');
  kernel_putchar('D');
  kernel_putchar('A');
  kernel_putchar('\n');

  kernel_printf("-\n");
  kernel_printf(".\n");
  kernel_printf("/\n");
  kernel_printf("'()*+,-./\n");
```

이러한 테스트는 향상된 `printf` 기능이 다양한 경우를 처리할 수 있음을 보여주며, 지정된 정밀도, 너비 및 플래그에 따라 출력이 올바르게 서식화됩니다.

### 스크린샷


<img width="807" alt="스크린샷 2024-08-24 오후 9 19 18" src="https://github.com/user-attachments/assets/37a2a11f-b18f-4ae0-9ff9-13c59d76610b">    

*QT 데스크톱 화면*


<img width="951" alt="스크린샷 2024-08-24 오후 9 21 37" src="https://github.com/user-attachments/assets/0911c744-cbfd-4c3c-8863-faab6df96ce9">   

*C언어 printf 자작 파서 링킹 모습(초기 프로토타입)*


### 향후 개선 사항
- 실제 프로세스 관리를 구현하여 더욱 자세한 시뮬레이션을 추가할 예정입니다.
- 시뮬레이션된 OS의 명령어 세트를 확장할 예정입니다.
- 부트로더 인터페이스를 개선하여 더욱 현실적인 요소를 추가할 계획입니다.
- `printf` 기능을 확장하여 더 복잡한 서식(16진수 부동소수점, 과학적 표기법 등)을 지원할 예정입니다.
- 지원되지 않거나 잘못된 서식 지정자에 대한 오류 처리를 구현하여 시스템을 더욱 견고하게 만들 계획입니다.



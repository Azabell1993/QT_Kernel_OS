/*
 * CMD Window
 *
 * Maintainer: Park Jiwoo
 *
 * Copyright (C) 2024 Park-Jiwoo
 *
 */

#include <QWidget>
#include <QKeyEvent>
#include <QString>
#include <QTextEdit>
#include <QRegularExpression>
#include <QMetaType>
#include <QDebug>
#include <QTimer>
#include <QStack>
#include <QChar>
#include <QMessageBox>
#include <QProcess>
#include <QInputDialog>
#include <QVBoxLayout>
#include <QLabel>
#include <QDialog>
#include <cstdarg>
#include <QFile>
#include <QDir>
#include <QVariant>
#include <QFileInfo>
#include <QList>

#include "cmdwindow.h"
#include "ui_cmdwindow.h"
#include "smartpointerdialog.h"

// kernel engine
#include "kernel_engine.h"
#include "kernel_smartptr.h"

// 전역에서 접근 가능한 QTextEdit 포인터
QTextEdit* globalProgressLog = nullptr;

// C 정적 라이브러리
#include "kernel_print.h"
// #include "kernel_asm.h"

// 직접 하드코딩된 C 함수
extern "C" {
bool kernel_create_process(const char *process_name);
void kernel_list_processes();
bool kernel_kill_process(const char *process_name);
void register_print_function(void (*print_function)(const char *str));
void az_printf(const char *format, ...);
void kernel_putchar(char c);

sem_t* init_semaphore(int value);
pthread_mutex_t* init_mutex();
void* thread_function(void* arg);
void* semaphore_thread(void* arg);
void* mutex_thread(void* arg);
void run_multithreading(int num_threads, int use_semaphore, ...);
void kernel_chat(int num_args, ...);

//int main_1();
//int main_2();
//int main_3();
//int main_4();
//int main_5();
//int main_6();
//int main_7();
}

#define DEFAULT_TCP_PORT 5100

/*
 * @brief CMD 창 생성자
 * @param parent 부모 위젯, 기본값은 nullptr
 * @details CMD 창을 초기화하고 스타일 및 초기 설정을 적용합니다.
 */
CmdWindow::CmdWindow(QWidget *parent)
    : QDialog(parent), ui(new Ui::cmdWindow)
{
    ui->setupUi(this);
    // runTests();
    kernel_chat(2, "127.0.0.1", DEFAULT_TCP_PORT);

    ui->textEdit->setStyleSheet("background-color: black; color: yellow; font-family: 'Courier'; font-size: 12px;");
    ui->textEdit->installEventFilter(this); // 이벤트 필터 설치

    // C 코드에서 사용할 출력 함수 등록
    register_print_function(qt_print);

    // 초기 명령어 프롬프트
    ui->textEdit->append("QT Kernel OS Version 1.0");
    ui->textEdit->append("\nkernel> ");
}


/**
 * @brief 하얀색 텍스트를 추가하는 함수
 * @param text 추가할 텍스트
 */
void CmdWindow::appendWithWhiteText(const QString &text) {
    QTextCharFormat format;
    format.setForeground(QBrush(QColor("white")));  // 텍스트 색을 하얀색으로 설정
    QTextCursor cursor = ui->textEdit->textCursor();
    cursor.movePosition(QTextCursor::End);
    ui->textEdit->setTextCursor(cursor);
    ui->textEdit->setCurrentCharFormat(format);
    ui->textEdit->insertPlainText(text);
}

/*
 * @brief CMD 창 소멸자
 * @details CMD 창에서 사용한 자원을 해제합니다.
 */
CmdWindow::~CmdWindow() {
    delete ui;
}


/*
 * @brief 키보드 입력 이벤트 처리
 * @param event 키보드 이벤트 객체
 * @details Enter 키를 누를 경우, 명령어 입력을 처리합니다.
 */
void CmdWindow::keyPressEvent(QKeyEvent *event) {
    if (event->key() != Qt::Key_Return && event->key() != Qt::Key_Enter) {
        QWidget::keyPressEvent(event);  // 다른 키에 대한 기본 처리
    }
}

/*
 * @brief 이벤트 필터
 * @param obj 이벤트가 발생한 객체
 * @param event 처리할 이벤트
 * @return bool 이벤트를 처리했는지 여부
 * @details CMD 창에서 특정 키 입력(Enter, 화살표 키, Backspace)에 대한 동작을 커스터마이징합니다.
 */
bool CmdWindow::eventFilter(QObject *obj, QEvent *event) {
    if (obj == ui->textEdit && event->type() == QEvent::KeyPress) {
        QKeyEvent *keyEvent = static_cast<QKeyEvent*>(event);
        QTextCursor cursor = ui->textEdit->textCursor();
        int blockNumber = cursor.blockNumber();
        int positionInBlock = cursor.positionInBlock();

        // Enter 키 처리
        if (keyEvent->key() == Qt::Key_Return || keyEvent->key() == Qt::Key_Enter) {
            on_submitButton_clicked();
            return true; // Enter 키 이벤트를 처리했음을 알림
        }

        // 위/왼쪽 화살표 키 처리
        if (keyEvent->key() == Qt::Key_Up || keyEvent->key() == Qt::Key_Left) {
            if (blockNumber == ui->textEdit->document()->blockCount() - 1) {
                if (positionInBlock <= 8) {
                    return true; // 커서가 "kernel> " 앞쪽으로 이동하지 않도록 막음
                }
                return QWidget::eventFilter(obj, event);
            } else {
                return true; // 커서가 이전 줄로 이동하지 않도록 막음
            }
        }

        // Backspace 키 처리
        if (keyEvent->key() == Qt::Key_Backspace) {
            if (blockNumber == ui->textEdit->document()->blockCount() - 1) {
                if (positionInBlock <= 8) {
                    return true; // "kernel> " 앞쪽의 텍스트 삭제를 막음
                }
            }
        }
    }
    return QWidget::eventFilter(obj, event);
}


/*
 * @brief 명령어 제출 처리
 * @details Enter 키가 눌렸을 때, 사용자가 입력한 명령어를 처리합니다.
 */
void CmdWindow::on_submitButton_clicked() {
    // 현재 텍스트를 가져옴
    QString command = ui->textEdit->toPlainText().split("\n").last().replace("kernel> ", "");

    handleCommand(command);

    // 새로운 명령어를 위한 프롬프트 추가 (한 줄 개행만)
    ui->textEdit->moveCursor(QTextCursor::End);
    ui->textEdit->insertPlainText("\nkernel> ");
    ui->textEdit->ensureCursorVisible();
}

void CmdWindow::createProcessWithMessage(const QString &message) {
    QProcess *process = new QProcess(this);

    // 모달 창으로 생성한 프로세스의 진행 상황을 표시
    QDialog *modalDialog = new QDialog(this);
    QVBoxLayout *layout = new QVBoxLayout();
    QLabel *label = new QLabel("Process running with message: " + message, modalDialog);
    layout->addWidget(label);
    modalDialog->setLayout(layout);
    modalDialog->show();

    process->start("echo", QStringList() << message);
    connect(process, &QProcess::readyReadStandardOutput, [=]() {
        QString output = process->readAllStandardOutput();
        label->setText("Process Output: " + output);
    });

    connect(process, QOverload<int, QProcess::ExitStatus>::of(&QProcess::finished), [=]() {
        modalDialog->accept();
        process->deleteLater();  // 프로세스 종료 후 자동으로 메모리 해제
    });
}


/*
 * @brief 명령어 처리 함수
 * @param command 사용자가 입력한 명령어
 * @details 사용자가 입력한 명령어를 분석하고, 해당 명령어에 맞는 동작을 수행합니다.
 */
// void CmdWindow::handleCommand(const QString &command) {
// static SmartPtr sp;  // 기존의 스마트 포인터 전역 변수
// static SmartPointerDialog *dialog = nullptr;  // 모달 창은 한 번만 생성
// static QMap<int*, SmartPtr> smartPointers;    // 여러 스마트 포인터를 관리하기 위한 맵

// if (!dialog) {
//     // 모달 창이 없으면 생성하여 표시 (부모를 nullptr로 설정)
//     dialog = new SmartPointerDialog(nullptr);
//     dialog->setWindowFlag(Qt::Window);  // 독립된 창으로 설정
//     dialog->show();
// }

// if (command == "exit") {
//     close();
// }
// else if (command == "help") {
// ui->textEdit->append("Available commands:");
// ui->textEdit->append("  create <process_name>       - Create a new process with the given name");
// ui->textEdit->append("  create printf(\"message\")  - Print a message");
// ui->textEdit->append("  printf(\"message\")  - Print a message using kernel_printf");
// ui->textEdit->append("  create_smart_ptr");
// ui->textEdit->append("  retain_smart_ptr");
// ui->textEdit->append("  release_smart_ptr");
// ui->textEdit->append("  show_smart_ptr_value");
// ui->textEdit->append("  show_ref_count");
// ui->textEdit->append("  kill_ptr <pointer_value>    - Kill a specific smart pointer");
// ui->textEdit->append("  multithreading              - Run a multithreading test");
// ui->textEdit->append("  semaphore                  - Run a semaphore test");
// ui->textEdit->append("  mutex                      - Run a mutex test");
// ui->textEdit->append("  help_modal                 - Show help in a new modal window");
// ui->textEdit->append("  printf_test : kp_test");
// ui->textEdit->append("  asm <number>               - Run the kernel_asm_<number> function.");
// ui->textEdit->append("  kill <process_name>         - Kill the process with the given name");
// ui->textEdit->append("  list                        - List all processes");
// ui->textEdit->append("  show_memory_usage           - Show memory usage of smart pointers");
// ui->textEdit->append("  copy_smart_ptr              - Copy a smart pointer");
// ui->textEdit->append("  create_smart_ptr_with_timer <seconds> - Create a smart pointer with auto-release");
// ui->textEdit->append("  clear                       - Clear the screen");
// ui->textEdit->append("  exit                        - Exit the shell");
// }


// else if (command.startsWith("kill_ptr ")) {
//     QStringList parts = command.split(" ");
//     if (parts.size() == 2) {
//         bool ok;
//         int ptrValue = parts[1].toInt(&ok);
//         if (ok) {
//             int* foundPtr = nullptr;

//             // 스마트 포인터 맵에서 값을 찾기
//             for (int* ptr : smartPointers.keys()) {
//                 if (ptr == reinterpret_cast<int*>(ptrValue)) {  // 포인터 값을 직접 참조
//                     foundPtr = ptr;
//                     break;
//                 }
//             }

//             if (foundPtr) {
//                 SmartPtr sp = smartPointers[foundPtr];
//                 release(&sp);
//                 if (*(sp.ref_count) == 0) {
//                     dialog->removeSmartPointer(*foundPtr);  // 모달 창에서 삭제
//                     smartPointers.remove(foundPtr);         // 맵에서 삭제
//                     delete foundPtr;                        // 메모리 해제
//                     ui->textEdit->append("Smart pointer deleted.");
//                 } else {
//                     ui->textEdit->append("Pointer reference count is not zero.");
//                 }
//             } else {
//                 ui->textEdit->append("Pointer value not found.");
//             }
//         } else {
//             ui->textEdit->append("Invalid pointer value.");
//         }
//     } else {
//         ui->textEdit->append("Usage: kill_ptr <pointer_value>");
//     }
// }


// else if (command == "help_modal") {
//     // 새로운 모달 창을 열어 모든 명령어 표시
//     QDialog *helpDialog = new QDialog(this);
//     QVBoxLayout *layout = new QVBoxLayout(helpDialog);
//     QTextEdit *helpText = new QTextEdit(helpDialog);
//     helpText->setText("Available commands:\n"
//                       "  create <process_name>       - Create a new process with the given name\n"
//                       "  create printf(\"message\")  - Print a message\n"
//                       "  printf(\"message\")         - Print a message using kernel_printf\n"
//                       "  create_smart_ptr            - Create a smart pointer\n"
//                       "  retain_smart_ptr            - Retain a smart pointer\n"
//                       "  release_smart_ptr           - Release a smart pointer\n"
//                       "  show_smart_ptr_value        - Show the value of a smart pointer\n"
//                       "  show_ref_count              - Show reference count of a smart pointer\n"
//                       "  kill_ptr <pointer_value>    - Kill a specific smart pointer\n"
//                       "  multithreading              - Run a multithreading test\n"
//                       "  semaphore                  - Run a semaphore test\n"
//                       "  mutex                      - Run a mutex test\n"
//                       "  help_modal                 - Show help in a new modal window\n"
//                       "  printf_test : kp_test       - Test printf functionality\n"
//                       "  asm <number>               - Run the kernel_asm_<number> function\n"
//                       "  kill <process_name>         - Kill the process with the given name\n"
//                       "  list                       - List all processes\n"
//                       "  show_memory_usage           - Show memory usage of smart pointers\n"
//                       " copy_smart_ptr              - Copy a smart pointer\n"
//                       " create_smart_ptr_with_timer <seconds> - Create a smart pointer with auto-release\n"
//                       "  clear                      - Clear the screen\n"
//                       "  exit                       - Exit the shell");
//     helpText->setReadOnly(true);
//     layout->addWidget(helpText);
//     helpDialog->setLayout(layout);
//     helpDialog->setWindowTitle("Help Commands");
//     helpDialog->show();
// }

// else if (command == "show_memory_usage") {
// // 스마트 포인터 메모리 사용량을 추적
// int totalMemory = 0;
// for (int* ptr : smartPointers.keys()) {
//     totalMemory += sizeof(*ptr);  // 할당된 포인터 크기를 합산
// }
// ui->textEdit->append("Total memory used by smart pointers: " + QString::number(totalMemory) + " bytes.");
// }

// else if (command == "copy_smart_ptr") {
//     if (!smartPointers.isEmpty()) {
//         SmartPtr originalSp = smartPointers.begin().value();
//         SmartPtr copiedSp = originalSp;  // 스마트 포인터 복사
//         ui->textEdit->append("Smart pointer copied. New reference count: " + QString::number(*(copiedSp.ref_count)));
//     }
// }

// else if (command.startsWith("create_smart_ptr_with_timer ")) {
// QStringList parts = command.split(" ");
// if (parts.size() == 2) {
//     bool ok;
//     int timeout = parts[1].toInt(&ok);
//     if (ok) {
//         // 타이머를 이용한 스마트 포인터 생성 및 자동 삭제
//         int* testData = new int;
//         *testData = rand() % 1000;
//         sp = create_smart_ptr(*testData, 1, nullptr);
//         smartPointers[testData] = sp;

//         // 타이머 시작
//         QTimer::singleShot(timeout * 1000, this, [=]() {
//             if (*(sp.ref_count) == 1) {  // 참조 카운트가 1일 때만 해제
//                 release(&sp);
//                 ui->textEdit->append("Smart pointer auto-released after " + QString::number(timeout) + " seconds.");
//                 dialog->removeSmartPointer(*testData);  // 모달에서 삭제
//                 smartPointers.remove(testData);         // 맵에서 삭제
//                 delete testData;
//             } else {
//                 ui->textEdit->append("Smart pointer was not released due to non-zero reference count.");
//             }
//         });

//         ui->textEdit->append("Smart pointer created with auto-release in " + QString::number(timeout) + " seconds.");
//     } else {
//         ui->textEdit->append("Invalid timer value.");
//     }
// } else {
//     ui->textEdit->append("Usage: create_smart_ptr_with_timer <seconds>");
// }
// }


// else if (command == "create_smart_ptr") {
// // 포인터 생성 및 스마트 포인터 할당
// int* testData = new int;
// *testData = rand() % 1000;  // 0~999 사이의 랜덤 값
// sp = create_smart_ptr(*testData, 1, nullptr);
// smartPointers[testData] = sp;  // 생성된 스마트 포인터를 맵에 추가

// // CMD 창에 메시지 출력
// ui->textEdit->append(QString("Smart pointer created with value: %1").arg(*testData));
// kernel_printf("Smart pointer created with value: %d\n", *testData);

//     // 모달 창에 포인터 추가
//     dialog->addSmartPointer(*testData, *(sp.ref_count));
// }

// else if (command == "retain_smart_ptr") {
//     if (!smartPointers.isEmpty()) {
//         // 맵에서 첫 번째 포인터를 참조 카운트 증가
//         SmartPtr &sp = smartPointers.begin().value();
//         retain(&sp);

//         ui->textEdit->append("Smart pointer retained.");
//         dialog->addSmartPointer(*((int*)sp.ptr), *(sp.ref_count));  // 모달 창에 업데이트
//     }
// }
// else if (command == "release_smart_ptr") {
//     if (!smartPointers.isEmpty()) {
//         // 맵에서 첫 번째 포인터를 참조 카운트 감소 및 해제
//         int* ptr = smartPointers.begin().key();
//         SmartPtr sp = smartPointers[ptr];
//         release(&sp);

//         ui->textEdit->append("Smart pointer released.");
//         if (*(sp.ref_count) == 0) {
//             dialog->removeSmartPointer(*ptr);  // 참조 카운트가 0이면 삭제
//             smartPointers.remove(ptr);         // 맵에서 포인터 삭제
//             delete ptr;                        // 포인터 메모리 해제
//         } else {
//             dialog->addSmartPointer(*ptr, *(sp.ref_count));  // 참조 카운트 업데이트
//         }
//     }
// }
// else if (command == "show_smart_ptr_value") {
//     if (!smartPointers.isEmpty()) {
//         int* value = (int*)smartPointers.begin().key();
//         ui->textEdit->append("Smart pointer value: " + QString::number(*value));
//         kernel_printf("Smart pointer value: %d\n", *value);
//     }
// }
// else if (command == "show_ref_count") {
//     if (!smartPointers.isEmpty()) {
//         SmartPtr sp = smartPointers.begin().value();
//         ui->textEdit->append("Smart pointer reference count: " + QString::number(*(sp.ref_count)));
//         kernel_printf("Smart pointer reference count: %d\n", *(sp.ref_count));
//     }
// }

// else if (command == "multithreading") {
//     runMultithreadingTest();
// }
// else if (command == "semaphore") {
//     runSemaphoreTest();
// }
// else if (command == "mutex") {
//     runMutexTest();
// }
// else if (command.startsWith("create printf(")) {
//     QRegularExpression re(R"raw(create printf\("(.*)"\))raw");
//     QRegularExpressionMatch match = re.match(command);
//     if (match.hasMatch()) {
//         QString message = match.captured(1);
//         // az_printf("\n%s", message.toStdString().c_str());
//         ui->textEdit->append("Create process with message >> "+message);
//         createProcessWithMessage(message);
//     } else {
//         ui->textEdit->append("Invalid command format. Use: create printf(\"message\")");
//     }
// }

//    else if (command.startsWith("asm ")) {
//        QStringList parts = command.split(" ");
//        if (parts.size() == 2) {
//            int number = parts[1].toInt();
//            switch (number) {
//            case 1:
//                ui->textEdit->append("Executed kernel_asm_1.");
//                main_1();  // kernel_asm_1.a의 main_1 함수 호출
//                break;
//            case 2:
//                ui->textEdit->append("Executed kernel_asm_2.");
//                main_2();  // kernel_asm_2.a의 main_2 함수 호출
//                break;
//            case 3:
//                ui->textEdit->append("Executed kernel_asm_3.");
//                main_3();  // kernel_asm_3.a의 main_3 함수 호출
//                break;
//            case 4:
//                ui->textEdit->append("Executed kernel_asm_4.");
//                main_4();  // kernel_asm_4.a의 main_4 함수 호출
//                break;
//            case 5:
//                ui->textEdit->append("Executed kernel_asm_5.");
//                main_5();  // kernel_asm_5.a의 main_5 함수 호출
//                break;
//            case 6:
//                ui->textEdit->append("Executed kernel_asm_6.");
//                main_6();  // kernel_asm_6.a의 main_6 함수 호출
//                break;
//            case 7:
//                ui->textEdit->append("Executed kernel_asm_7.");
//                main_7();  // kernel_asm_7.a의 main_7 함수 호출
//                break;
//            default:
//                ui->textEdit->append("Invalid number. Please enter a number between 1 and 7.");
//            }
//        } else {
//            ui->textEdit->append("Usage: asm <number>");
//        }
//    }

// else if (command.startsWith("printf(")) {
//     QRegularExpression re1(R"raw(printf\("([^"]*)"\))raw");
//     QRegularExpression re2(R"raw(printf\("([^"]*)"\s*,\s*(.*)\))raw");

//     QRegularExpressionMatch match1 = re1.match(command);
//     QRegularExpressionMatch match2 = re2.match(command);

//     if (match1.hasMatch()) {
//         // 단순 메시지 출력
//         QString message = match1.captured(1);
//         QString debugMessage = message;
//         debugMessage.replace("\\n", "\n");
//         qDebug().noquote() << debugMessage;
//         ui->textEdit->append(message);
//         kernel_printf("%s", message.toStdString().c_str());
//         ui->textEdit->append("");

//         /** test **/
//         kernel_printf("\n\n******** Test kernel_printf Function ********\n");
//         kernel_printf("Hello world!\n");
//         kernel_printf("A single character : %c \n", 'T');
//         kernel_printf("An integer : %d \n", 37);
//         kernel_printf("An integer : %d \n", 299);
//         kernel_printf("5-4 = %d\n", 1);

//         int a = 1;
//         int b = 2;

//         kernel_printf("%d + %d = %d\n", a, b, a + b);
//         kernel_printf("%d\t\t\t String.\n", 12345678);
//         kernel_printf("-650\n");
//         kernel_printf("%+d\n", 430);
//         kernel_printf("%+1d\n", 650);
//         kernel_printf("%+10d\n", 499);
//         kernel_printf("% 3d\n", 1230);
//         kernel_printf("%08d\n", 342);
//         kernel_printf("%+03d\n", -430);
//         kernel_printf("%3d\n", -43);
//         kernel_printf("%u\n", 23919293929392);
//         kernel_printf("%+-u\n", 12345);
//         kernel_printf("%+10u\n", 12345);
//         kernel_printf("%-4s\n", "Az");
//         kernel_printf("%o\n", 333);
//         kernel_printf("%-0#+10o\n", 2048);
//         kernel_printf("%X\n", 12345678);
//         kernel_printf("%#+x\n", 12345678);
//         kernel_printf("\n\nfunction call Test\n");
//         kernel_printf("%d + %d = %d", 5, 6, function_Test(5, 6));
//         kernel_printf("\n");

//         /** 정적 라이브러리를 갖고 오는 방법은 두가지임. 위의 kernel_printf는 kernel_printf.h헤더이며 아래는 kernel_lib.h임. **/
//         /** 따라서 이 코드 파일에서 kernel_printf.h만 했기 때문에 아래도 헤더파일로 선언해야하는데 **/
//         /** 공부목적을 위해서 헤더 파일이 아닌 extern 메크로를 사용함 **/
//         /** 선언을 하니 워닝이 사라짐 **/
//         /*** kernel_putchar ***/
//         kernel_putchar('H');
//         kernel_putchar('E');
//         kernel_putchar('L');
//         kernel_putchar('L');
//         kernel_putchar('O');
//         kernel_putchar('\n');

//         kernel_putchar('V');
//         kernel_putchar('E');
//         kernel_putchar('D');
//         kernel_putchar('A');
//         kernel_putchar('\n');

//         kernel_printf("-\n");
//         kernel_printf(".\n");
//         kernel_printf("/\n");
//         kernel_printf("'()*+,-./\n");

//     }
//     else if (match2.hasMatch()) {
//         // 포맷 문자열과 가변 인자 처리
//         QString format = match2.captured(1);
//         QString arguments = match2.captured(2).trimmed();

//         qDebug() << "\nFormat:" << format;
//         qDebug() << "Arguments:" << arguments;

//         ui->textEdit->append(format);

//         if (!arguments.isEmpty()) {
//             QVariantList args = parseArguments(arguments);
//             executeKernelPrintf(format, args);
//         } else {
//             kernel_printf(format.toStdString().c_str());
//         }

//         ui->textEdit->append("");
//     }
//     else {
//         ui->textEdit->append("Invalid command format. Use: kernel_printf(\"message\") or kernel_printf(\"format\", values)");
//     }
// }
// else if (command == "kp_test") {
//     test_kernel_printf();
// }
// else if (command.startsWith("create ")) {
//     QString process_name = command.mid(7);
//     bool success = kernel_create_process(process_name.toStdString().c_str());
//     if (!success) {
//         ui->textEdit->append("Failed to create process: " + process_name);
//     }
// }
// else if (command.startsWith("kill ")) {
//     QString process_name = command.mid(5);
//     bool success = kernel_kill_process(process_name.toStdString().c_str());
//     if (!success) {
//         ui->textEdit->append("No running process found with name: " + process_name);
//     }
// }
// else if (command == "list") {
//     kernel_list_processes();
// }
// else if (command == "clear") {
//     ui->textEdit->clear();
// }
// else {
//     ui->textEdit->append("Unknown command. Type 'help' for a list of commands.");
// }
// }

/*
 * @brief 명령어 처리 함수
 * @param command 사용자가 입력한 명령어
 */
void CmdWindow::handleCommand(const QString &command) {

    if (command == "exit") {
        close();
    } else if (command == "help") {
        ui->textEdit->append("Available commands:");
        ui->textEdit->append("  pwd                        - Print working directory");
        ui->textEdit->append("  ls                         - List files and directories");
        ui->textEdit->append("  cd <directory>             - Change directory");
        ui->textEdit->append("  cp <source> <destination>   - Copy a file");
        ui->textEdit->append("  mv <source> <destination>   - Move a file");
        ui->textEdit->append("  df -h                      - Show disk usage");
        ui->textEdit->append("  du <directory>             - Show directory size");
        ui->textEdit->append("  printf(\"message\")         - Print a message");
        ui->textEdit->append("  history                    - Show command history");
        ui->textEdit->append("  mkdir <directory>          - Create a directory");
        ui->textEdit->append("  rmdir <directory>          - Remove a directory");
        ui->textEdit->append("  clear                      - Clear the screen");
        ui->textEdit->append("  ifconfig                   - Show network interfaces");
        ui->textEdit->append("  netstat                    - Show network connections");
        ui->textEdit->append("  help_modal                 - Show help in a new modal window");
        ui->textEdit->append("  exit                       - Exit the shell");
        ui->textEdit->append("  shutdown || poweroff       - Shutdown the system");
    }

    if (command == "help_modal") {
        static SmartPointerDialog *dialog = nullptr;

        // 새로운 모달 창을 열어 모든 명령어 표시
        QDialog *helpDialog = new QDialog(this);
        QVBoxLayout *layout = new QVBoxLayout(helpDialog);
        QTextEdit *helpText = new QTextEdit(helpDialog);

        // 명령어 목록 설정
        helpText->setText("Available commands:\n"
                          "  pwd                        - Print working directory\n"
                          "  ls                         - List files and directories\n"
                          "  cd <directory>             - Change directory\n"
                          "  cp <source> <destination>   - Copy a file\n"
                          "  mv <source> <destination>   - Move a file\n"
                          "  df -h                      - Show disk usage\n"
                          "  du <directory>             - Show directory size\n"
                          "  printf(\"message\")         - Print a message\n"
                          "  history                    - Show command history\n"
                          "  mkdir <directory>          - Create a directory\n"
                          "  rmdir <directory>          - Remove a directory\n"
                          "  clear                      - Clear the screen\n"
                          "  ifconfig                   - Show network interfaces\n"
                          "  netstat                    - Show network connections\n"
                          "  help_modal                 - Show help in a new modal window\n"
                          "  exit                       - Exit the shell\n"
                          "  shutdown  || poweroff      - Shutdown the system\n");

        // 텍스트 편집기를 읽기 전용으로 설정
        helpText->setReadOnly(true);

        // 레이아웃에 텍스트 편집기 추가
        layout->addWidget(helpText);

        // 모달 창의 레이아웃 설정
        helpDialog->setLayout(layout);
        helpDialog->setWindowTitle("Help Commands");

        // 모달 창 표시
        helpDialog->exec();  // exec()를 사용하여 모달로 열림
    }


    else if (command.startsWith("printf(")) {
        QRegularExpression re1(R"raw(printf\("([^"]*)"\))raw");
        QRegularExpression re2(R"raw(printf\("([^"]*)"\s*,\s*(.*)\))raw");

        QRegularExpressionMatch match1 = re1.match(command);
        QRegularExpressionMatch match2 = re2.match(command);

        if (match1.hasMatch()) {
            // 단순 메시지 출력
            QString message = match1.captured(1);
            QString debugMessage = message;
            debugMessage.replace("\\n", "\n");
            qDebug().noquote() << debugMessage;
            ui->textEdit->append(message);
            kernel_printf("%s", message.toStdString().c_str());
            ui->textEdit->append("");
        }
        else if (match2.hasMatch()) {
            // 포맷 문자열과 가변 인자 처리
            QString format = match2.captured(1);
            QString arguments = match2.captured(2).trimmed();

            qDebug() << "\nFormat:" << format;
            qDebug() << "Arguments:" << arguments;

            ui->textEdit->append(format);

            if (!arguments.isEmpty()) {
                QVariantList args = parseArguments(arguments);
                executeKernelPrintf(format, args);
            } else {
                kernel_printf(format.toStdString().c_str());
            }

            ui->textEdit->append("");
        }
        else {
            ui->textEdit->append("Invalid command format. Use: kernel_printf(\"message\") or kernel_printf(\"format\", values)");
        }
    }

    else if (command.startsWith("cp ")) {
        QStringList parts = command.split(" ");
        if (parts.size() == 3) {
            QStringList args;
            args << parts[0] << parts[1] << parts[2];  // QList<QVariant> -> QStringList
            runUnifiedProcess(args);
        } else {
            ui->textEdit->append("Usage: cp <source> <destination>");
        }
    }

    else if (command.startsWith("mv ")) {
        QStringList parts = command.split(" ");
        if (parts.size() == 3) {
            QStringList args;
            args << parts[0] << parts[1] << parts[2];
            runUnifiedProcess(args);
        } else {
            ui->textEdit->append("Usage: mv <source> <destination>");
        }
    }

    else if (command == "clear") {
        ui->textEdit->clear();
        //on_submitButton_clicked();
        //ui->textEdit->append("kernel> ");
    }

    else if (command == "pwd") {
        runUnifiedProcess(QStringList() << "pwd");
    }

    else if (command == "ls") {
        runUnifiedProcess(QStringList() << "ls" << "-alc");
    }

    else if (command.startsWith("cd ")) {
        QString directory = command.mid(3);
        QDir::setCurrent(directory);
        ui->textEdit->append("Changed directory to: " + directory);
    }

    else if (command == "history") {
        for (const QString &cmd : commandHistory) {
            ui->textEdit->append(cmd);
        }
    }

    else if (command.startsWith("mkdir ")) {
        QString directory = command.mid(6);
        QDir().mkdir(directory);
        ui->textEdit->append("Directory created: " + directory);
    }

    else if (command.startsWith("rmdir ")) {
        QString directory = command.mid(6);
        QDir().rmdir(directory);
        ui->textEdit->append("Directory removed: " + directory);
    }

    else if (command == "df -h") {
        // QProcess를 이용해 시스템 명령어 실행
        QProcess process;
        process.start("df", QStringList() << "-h");

        // 프로세스가 완료될 때까지 기다림
        process.waitForFinished();

        // 프로세스의 출력 결과를 읽음
        QString output = process.readAllStandardOutput();

        // 출력 결과를 ui->textEdit에 추가 (노란색으로 표시됨)
        ui->textEdit->append(output);
    }

    else if (command.startsWith("du ")) {
        QStringList parts = command.split(" ");
        if (parts.size() == 2) {
            runUnifiedProcess(QStringList() << "du" << parts[1]);  // QList<QVariant> -> QStringList
        } else {
            ui->textEdit->append("Usage: du <directory>");
        }
    }

    else if (command.startsWith("echo ")) {
        QString message = command.mid(5).trimmed(); // echo 이후의 메시지 추출
        ui->textEdit->append(message); // 화면에 출력
        kernel_printf("%s\n", message.toStdString().c_str()); // 콘솔에 출력
    }

    else if (command == "ifconfig") {
        QProcess process;
        process.start("ifconfig");
        process.waitForFinished();
        QString output = process.readAllStandardOutput();
        ui->textEdit->append(output);  // 결과를 UI에 출력
    }

    else if (command == "top") {
        QProcess process;
        process.start("top");
        process.waitForFinished();
        QString output = process.readAllStandardOutput();
        ui->textEdit->append(output);  // 결과를 UI에 출력
    }

    else if (command == "netstat") {
        QProcess process;
        process.start("netstat", QStringList() << "-tuln");
        process.waitForFinished();
        QString output = process.readAllStandardOutput();
        ui->textEdit->append(output);  // 결과를 UI에 출력
    }

    else if (command == "shutdown" || command == "poweroff") {
        ui->textEdit->append("Shutting down...");
        QApplication::quit();  // 애플리케이션 종료
    }

    else {
        ui->textEdit->append("Unknown command. Type 'help' for a list of commands.");
    }

    commandHistory.append(command);
}

/*
 * @brief 가변 인자 대신 QList<QVariant>를 사용하여 명령어를 처리하는 함수
 * @param args 명령어와 인자를 QList<QVariant>로 전달
 */
/*
 * @brief 가변 인자 대신 QStringList를 사용하여 명령어를 처리하는 함수
 *        프로세스 생성 시 스마트 포인터를 사용하여 메모리를 관리합니다.
 * @param args 명령어와 인자를 QStringList로 전달
 */
void CmdWindow::runUnifiedProcess(const QStringList &args) {
    static SmartPtr sp;  // 기존의 스마트 포인터 전역 변수
    static QMap<int*, SmartPtr> smartPointers;    // 여러 스마트 포인터를 관리하기 위한 맵

    if (args.isEmpty()) {
        kernel_printf("No command provided.\n");
        return;
    }

    QString command = args[0];
    commandHistory.append(command);  // 명령어를 히스토리에 추가

    kernel_printf("Executing command: %s\n", command.toStdString().c_str());
    ui->textEdit->append("Executing command: " + command);

    // 포인터 생성 및 스마트 포인터 할당
    int* testData = new int;
    *testData = rand() % 1000;  // 0~999 사이의 랜덤 값
    sp = create_smart_ptr(*testData, 1, nullptr);
    smartPointers[testData] = sp;  // 생성된 스마트 포인터를 맵에 추가

    ui->textEdit->append(QString("Smart pointer created with value: %1").arg(*testData));
    kernel_printf("Smart pointer created with value: %d\n", *testData);
    ui->textEdit->append("Smart pointer created with value: " + QString::number(*testData));

    // 스마트 포인터로 프로세스 데이터를 관리
    auto processData = std::make_shared<int>(rand() % 1000);
    kernel_printf("Process Smart Pointer Created With Value : %d\n", *processData);
    ui->textEdit->append("Process Smart Pointer Created With Value : " + QString::number(*processData));

    QProcess process;
    QString output;

    // 명령어 처리
    // 파일 및 디렉토리 명령어
    if (command == "pwd") {
        process.start("pwd");
    }

    if (command == "cd" && args.size() == 2) {
        QString directory = command.mid(3).trimmed();

        if (QDir::setCurrent(directory)) {
            ui->textEdit->append("Changed directory to: " + QDir::currentPath());
            kernel_printf("Directory changed to: %s\n", QDir::currentPath().toStdString().c_str());
        } else {
            ui->textEdit->append("Failed to change directory: " + directory);
            kernel_printf("Error: Failed to change directory to: %s\n", directory.toStdString().c_str());
        }
    }

    if (command == "ls") {
        process.start("ls", QStringList() << "-alc");
    }

    // history
    if(args[0] == "history") {
        kernel_printf("Command history:\n");
        ui->textEdit->append("Command history:");
        for(int i=0; i<commandHistory.size(); i++) {
            QString historyEntry = QString::number(i+1) + ": " + commandHistory[i];
            kernel_printf("%s\n", historyEntry.toStdString().c_str());
            ui->textEdit->append(historyEntry);
        }
    }

    if (command == "mkdir" && args.size() == 2) {
        QDir().mkdir(args[1]);
        kernel_printf("Directory %s created\n", args[1].toStdString().c_str());
        return;
    }

    if (command == "rmdir" && args.size() == 2) {
        QDir().rmdir(args[1]);
        kernel_printf("Directory %s removed\n", args[1].toStdString().c_str());
        return;
    }

    if (command == "cp" && args.size() == 3) {
        QString src = args[1];
        QString dst = args[2];

        // 소스 파일 확인
        if (!QFile::exists(src)) {
            kernel_printf("Error: Source file does not exist: %s\n", src.toStdString().c_str());
            ui->textEdit->append("Error: Source file does not exist: " + src);
            return;
        }

        // 목적지 파일 경로 유효성 확인 (목적지 폴더가 존재하는지)
        QFileInfo dstInfo(dst);
        if (!dstInfo.dir().exists()) {
            kernel_printf("Error: Destination directory does not exist: %s\n", dstInfo.dir().absolutePath().toStdString().c_str());
            ui->textEdit->append("Error: Destination directory does not exist: " + dstInfo.dir().absolutePath());
            return;
        }

        // 파일 복사
        if (QFile::copy(src, dst)) {
            kernel_printf("File copied from %s to %s\n", src.toStdString().c_str(), dst.toStdString().c_str());
            ui->textEdit->append("File copied from " + src + " to " + dst);
        } else {
            kernel_printf("Error: Failed to copy file. Please check permissions or if the file already exists.\n");
            ui->textEdit->append("Error: Failed to copy file. Please check permissions or if the file already exists.");
        }
    }

    if (command == "mv" && args.size() == 3) {
        QString src = args[1];
        QString dst = args[2];

        if (!QFile::exists(src)) {
            kernel_printf("Error: Source file does not exist: %s\n", src.toStdString().c_str());
            ui->textEdit->append("Error: Source file does not exist: " + src);
            return;
        }

        if (!QFile::rename(src, dst)) {
            kernel_printf("Error: Failed to move file.\n");
            ui->textEdit->append("Error: Failed to move file.");
        } else {
            kernel_printf("File moved from %s to %s\n", src.toStdString().c_str(), dst.toStdString().c_str());
            ui->textEdit->append("File moved from " + src + " to " + dst);
        }
    }

    if (command == "rm" && args.size() == 2) {
        QFile::remove(args[1]);
        kernel_printf("File %s removed\n", args[1].toStdString().c_str());
        return;
    }

    if (command.startsWith("echo ")) {
        QString message = command.mid(5).trimmed(); // echo 이후의 메시지 추출
        ui->textEdit->append(message); // 화면에 출력
        kernel_printf("%s\n", message.toStdString().c_str()); // 콘솔에 출력
    }

    // 디스크 정보 및 로그 관리
    if (command == "df" && args[1] == "-h") {
        QProcess process;
        process.start("df", QStringList() << "-h");
        process.waitForFinished();
        QString output = process.readAllStandardOutput();
        kernel_printf("%s", output.toStdString().c_str());
        ui->textEdit->append(output);
    } else if (command == "du" && args.size() == 2) {
        QString dir = args[1];
        QProcess process;
        process.start("du", QStringList() << "-sh" << dir);
        process.waitForFinished();
        QString output = process.readAllStandardOutput();
        kernel_printf("%s", output.toStdString().c_str());
        ui->textEdit->append(output);
    }

    if (command.startsWith("tail") && args.size() == 2) {
        process.start("tail", QStringList() << "-f" << args[1]);
    }

    if (command == "kill smartptr") {
        resetSmartPointer();
    }

    else {
        kernel_printf("Unknown command: %s\n", command.toStdString().c_str());
    }


    process.waitForFinished();
    output = process.readAllStandardOutput();
    kernel_printf("%s", output.toStdString().c_str());
    ui->textEdit->append(output);  // 명령어 결과를 GUI에 출력

    // 스마트 포인터 메모리 사용량을 추적
    int totalMemory = 0;
    for (int* ptr : smartPointers.keys()) {
        totalMemory += sizeof(*ptr);  // 할당된 포인터 크기를 합산
    }
    kernel_printf("Total memory used by smart pointers: %d bytes.\n", totalMemory);
    ui->textEdit->append("Total memory used by smart pointers: " + QString::number(totalMemory) + " bytes.");

    // 스마트 포인터 참조 카운트 체크 후 자동 해제
    kernel_printf("Smart pointer reference count: %ld\n", processData.use_count());
    ui->textEdit->append("Smart pointer reference count: " + QString::number(processData.use_count()));

    kernel_printf("Smart pointer will be automatically released when out of scope.\n");
    ui->textEdit->append("Smart pointer will be automatically released when out of scope.");
}

/*
 * @brief Qt 콘솔 출력 함수
 * @param str 출력할 문자열
 * @details C 코드에서 호출되는 출력 함수로, CMD 창 UI에 메시지를 표시합니다.
 */
void CmdWindow::qt_print(const char *str) {
    if (globalProgressLog) {
        globalProgressLog->append(QString::fromUtf8(str));
        QCoreApplication::processEvents();  // UI 업데이트
    }
}

/*
 * @brief 스마트 포인터를 해제하는 함수
 */
void CmdWindow::resetSmartPointer() {
    static QMap<int*, SmartPtr> smartPointers;
    ui->textEdit->append("Releasing smart pointer...");

    if (!smartPointers.isEmpty()) {
        int* ptr = smartPointers.begin().key();
        SmartPtr sp = smartPointers[ptr];
        release(&sp);

        ui->textEdit->append("Smart pointer reference count after release: " + QString::number(*(sp.ref_count)));

        // 참조 카운트가 0이면 메모리 해제
        if (*(sp.ref_count) == 0) {
            smartPointers.remove(ptr);  // 스마트 포인터 맵에서 제거
            delete ptr;                 // 실제 포인터 메모리 해제
            ui->textEdit->append("Smart pointer memory released and pointer deleted.");
            kernel_printf("Smart pointer memory released and pointer deleted.\n");
        } else {
            ui->textEdit->append("Smart pointer not deleted because reference count is not zero.");
        }
    } else {
        ui->textEdit->append("No smart pointers to release.");
    }
}

/*
 * @brief 멀티스레딩 테스트 실행
 * @details 사용자로부터 스레드 수와 세마포어 사용 여부를 입력받아, 멀티스레딩 테스트를 실행합니다.
 */
void CmdWindow::runMultithreadingTest() {
    bool ok;
    int num_threads = QInputDialog::getInt(this, tr("Multithreading Test"),
                                           tr("Enter the number of threads:"), 3, 1, 100, 1, &ok);
    if (!ok) return;

    int use_semaphore = QInputDialog::getInt(this, tr("Synchronization Method"),
                                             tr("Use semaphore? (0: Mutex, 1: Semaphore)"), 0, 0, 1, 1, &ok);
    if (!ok || (use_semaphore != 0 && use_semaphore != 1)) return;

    QDialog *threadDialog = new QDialog(this);
    QVBoxLayout *layout = new QVBoxLayout(threadDialog);
    QTextEdit *progressLog = new QTextEdit(threadDialog);
    progressLog->setStyleSheet("background-color: black; color: green; font-family: 'Courier'; font-size: 12px;");
    progressLog->setReadOnly(true);
    layout->addWidget(progressLog);
    threadDialog->setLayout(layout);
    threadDialog->show();

    // 전역 출력 로그 설정
    globalProgressLog = progressLog;

    // register_print_function을 사용하여 출력 함수를 등록
    register_print_function(qt_print);

    // 진행 상황을 실시간으로 UI에 표시
    progressLog->append("Starting multithreading test with " + QString::number(num_threads) + " threads...");
    QCoreApplication::processEvents();
    kernel_printf("Starting multithreading test with %d threads...\n", num_threads);
    kernel_printf("Using %s\n", use_semaphore ? "semaphore" : "mutex");
    kernel_printf("\n");

    // 멀티스레드 실행
    run_multithreading(num_threads, use_semaphore);

    progressLog->append("Multithreading test completed.");
    QCoreApplication::processEvents();
    kernel_printf("Multithreading test completed.\n");
    kernel_printf("\n");

    // 테스트가 끝난 후 전역 로그 해제
    globalProgressLog = nullptr;
}

/*
 * @brief 세마포어 테스트 실행
 * @details 사용자로부터 스레드 수를 입력받아, 세마포어 테스트를 실행합니다.
 */
void CmdWindow::runSemaphoreTest() {
    bool ok;
    int num_threads = QInputDialog::getInt(this, tr("Semaphore Test"),
                                           tr("Enter the number of threads:"), 3, 1, 100, 1, &ok);
    if (!ok) return;

    QDialog *semDialog = new QDialog(this);
    QVBoxLayout *layout = new QVBoxLayout(semDialog);
    QTextEdit *progressLog = new QTextEdit(semDialog);
    progressLog->setStyleSheet("background-color: black; color: green; font-family: 'Courier'; font-size: 12px;");
    progressLog->setReadOnly(true);
    layout->addWidget(progressLog);
    semDialog->setLayout(layout);
    semDialog->show();

    // 전역 출력 로그 설정
    globalProgressLog = progressLog;

    // register_print_function을 사용하여 출력 함수를 등록
    register_print_function(qt_print);

    // 진행 상황을 실시간으로 UI에 표시
    progressLog->append("Starting semaphore test with " + QString::number(num_threads) + " threads...");
    QCoreApplication::processEvents();
    kernel_printf("Starting semaphore test with %d threads...\n", num_threads);
    kernel_printf("\n");

    // 세마포어 테스트 진행
    run_multithreading(num_threads, 1);  // 세마포어 사용

    progressLog->append("Semaphore test completed.");
    QCoreApplication::processEvents();
    kernel_printf("Semaphore test completed.\n");
    kernel_printf("\n");

    // 테스트가 끝난 후 전역 로그 해제
    globalProgressLog = nullptr;
}

/*
 * @brief 뮤텍스 테스트 실행
 * @details 사용자로부터 스레드 수를 입력받아, 뮤텍스 테스트를 실행합니다.
 */
void CmdWindow::runMutexTest() {
    bool ok;
    int num_threads = QInputDialog::getInt(this, tr("Mutex Test"),
                                           tr("Enter the number of threads:"), 3, 1, 100, 1, &ok);
    if (!ok) return;

    QDialog *mutexDialog = new QDialog(this);
    QVBoxLayout *layout = new QVBoxLayout(mutexDialog);
    QTextEdit *progressLog = new QTextEdit(mutexDialog);
    progressLog->setStyleSheet("background-color: black; color: green; font-family: 'Courier'; font-size: 12px;");
    progressLog->setReadOnly(true);
    layout->addWidget(progressLog);
    mutexDialog->setLayout(layout);
    mutexDialog->show();

    // 전역 출력 로그 설정
    globalProgressLog = progressLog;

    // register_print_function을 사용하여 출력 함수를 등록
    register_print_function(qt_print);

    // 진행 상황을 실시간으로 UI에 표시
    progressLog->append("Starting mutex test with " + QString::number(num_threads) + " threads...");
    QCoreApplication::processEvents();
    kernel_printf("Starting mutex test with %d threads...\n", num_threads);
    kernel_printf("\n");

    // 뮤텍스 테스트 진행
    run_multithreading(num_threads, 0);  // 뮤텍스 사용

    progressLog->append("Mutex test completed.");
    QCoreApplication::processEvents();
    kernel_printf("Mutex test completed.\n");
    kernel_printf("\n");

    // 테스트가 끝난 후 전역 로그 해제
    globalProgressLog = nullptr;
}

/*
 * @brief kernel_printf와 함께 가변 인자를 처리하는 함수
 * @param format 포맷 문자열
 * @param args 가변 인자 리스트
 * @details 이 함수는 QString 포맷 문자열과 QVariantList 인자를 받아, kernel_printf를 호출하여 C 스타일로 포맷된 출력을 수행합니다.
 */
void CmdWindow::executeKernelPrintf(const QString &format, const QVariantList &args) {
    QString modifiedFormat = format;
    modifiedFormat.replace("\\n", "\n"); // 개행 처리
    QByteArray formatArray = modifiedFormat.toUtf8();
    const char *cFormat = formatArray.data();

    if (args.isEmpty()) {
        kernel_printf(cFormat);
        return;
    }

    switch (args.size()) {
    case 1:
        if (format.contains("%f")) {
            kernel_printf(cFormat, args[0].toDouble());
        } else if (format.contains("%c")) {
            kernel_printf(cFormat, args[0].toChar().toLatin1());
        } else {
            kernel_printf(cFormat, args[0].toInt());
        }
        break;
    case 2:
        if (format.contains("%f")) {
            kernel_printf(cFormat, args[0].toDouble(), args[1].toDouble());
        } else {
            kernel_printf(cFormat, args[0].toInt(), args[1].toInt());
        }
        break;
    case 3:
        if (format.contains("%f")) {
            kernel_printf(cFormat, args[0].toDouble(), args[1].toDouble(), args[2].toDouble());
        } else {
            kernel_printf(cFormat, args[0].toInt(), args[1].toInt(), args[2].toInt());
        }
        break;
    default:
        kernel_printf("Error: Unsupported number of arguments.\n");
        break;
    }
}


/*
 * @brief 가변 인자 문자열을 파싱하는 함수
 * @param arguments 콤마로 구분된 문자열 인자 목록
 * @return QVariantList 파싱된 인자 리스트
 * @details 이 함수는 문자열로 전달된 인자 목록을 파싱하여 QVariantList로 반환합니다.
 */
QVariantList CmdWindow::parseArguments(const QString &arguments) {
    QVariantList parsedArgs;
    QStringList argsList = arguments.split(',');

    for (QString &arg : argsList) {
        arg = arg.trimmed();
        if (arg.endsWith('U')) {
            parsedArgs.append(arg.left(arg.length() - 1).toUInt());
        } else if (arg.contains(QRegularExpression("^-?\\d*\\.\\d+$"))) {  // 소수점을 포함하는 경우
            parsedArgs.append(arg.toDouble());
        } else if (arg.contains(QRegularExpression("\\d+"))) {
            parsedArgs.append(evaluateSimpleExpression(arg));
        } else if (arg.startsWith("'") && arg.endsWith("'") && arg.length() == 3) {
            parsedArgs.append(QChar(arg[1]));
        } else {
            parsedArgs.append(arg);
        }
    }
    return parsedArgs;
}



/*
 * @brief 간단한 산술 표현식을 평가하는 함수
 * @param expression 평가할 표현식
 * @return int 평가된 결과
 * @details 이 함수는 간단한 산술 표현식을 평가하고, 결과를 정수로 반환합니다.
 */
int CmdWindow::evaluateSimpleExpression(const QString &expression) {
    QStack<int> values; // 숫자 값을 저장할 스택
    QStack<QChar> ops;  // 연산자를 저장할 스택
    QString token;

    for (int i = 0; i < expression.length(); ++i) {
        QChar ch = expression[i];

        if (ch.isDigit() || (ch == '-' && token.isEmpty())) {
            token += ch; // 숫자라면 토크에 저장
        } else if (ch == '+' || ch == '-' || ch == '*' || ch == '/') {
            if (!token.isEmpty()) {
                // 숫자를 스택에 푸시
                values.push(token.toInt());
                token.clear();
            }
            while (!ops.isEmpty() && precedence(ops.top()) >= precedence(ch)) {
                if (values.size() < 2) {
                    qDebug() << "Error: insufficient values for operation";
                    return 0;
                }
                int val2 = values.pop();
                int val1 = values.pop();
                QChar op = ops.pop();
                values.push(applyOp(val1, val2, op));
            }
            ops.push(ch);
        }
    }

    if (!token.isEmpty()) {
        values.push(token.toInt());
    }

    while (!ops.isEmpty()) {
        if (values.size() < 2) {
            qDebug() << "Error: insufficient values for operation";
            return 0;
        }
        int val2 = values.pop();
        int val1 = values.pop();
        QChar op = ops.pop();
        values.push(applyOp(val1, val2, op));
    }

    return values.isEmpty() ? 0 : values.top();
}


/*
 * @brief 연산자의 우선순위를 결정하는 함수
 * @param op 연산자
 * @return int 연산자의 우선순위
 * @details 이 함수는 산술 연산자의 우선순위를 반환합니다.
 */
int CmdWindow::precedence(QChar op) {
    if (op == '+' || op == '-') return 1;
    if (op == '*' || op == '/') return 2;
    return 0;
}


/*
 * @brief 연산자를 적용하는 함수
 * @param a 첫 번째 피연산자
 * @param b 두 번째 피연산자
 * @param op 연산자
 * @return int 연산 결과
 * @details 이 함수는 두 개의 피연산자와 연산자를 받아서 연산을 수행하고 결과를 반환합니다.
 */
int CmdWindow::applyOp(int a, int b, QChar op) {
    switch (op.toLatin1()) {
    case '+': return a + b;
    case '-': return a - b;
    case '*': return a * b;
    case '/': return a / b;
    }
    return 0;
}



/*
 * @brief 간단한 테스트를 실행하는 함수
 * @details 이 함수는 다양한 입력 값을 테스트하기 위해 실행됩니다.
 * @details 테스트시에는 생성자 함수에 넣으면 됩니다.
 */
void CmdWindow::runTests() {
    // 1. 단순 문자열 출력
    handleCommand("kernel_printf(\"hello world!\\n\")");

    // 2. 정수 출력
    handleCommand("kernel_printf(\"An integer : %d\\n\", 37)");
    handleCommand("kernel_printf(\"A large integer : %d\\n\", 299)");

    // 3. 간단한 덧셈
    handleCommand("kernel_printf(\"5 + 4 = %d\\n\", 5+4)");

    // 4. 변수 연산 결과 출력
    int a = 10;
    int b = 20;
    handleCommand(QString("kernel_printf(\"%d + %d = %d\\n\", %1, %2, %1 + %2)").arg(a).arg(b));


    // 5. 플로트 출력
    handleCommand("kernel_printf(\"A float : %f\\n\", 3.14)");
    handleCommand("kernel_printf(\"A float with precision: %.2f\\n\", 3.14159)");
    handleCommand("kernel_printf(\"A float with precision: %.5f\\n\", 3.14159)");
    handleCommand("kernel_printf(\"Complex float: %+08.3f\\n\", 3.14159)");
    handleCommand("kernel_printf(\"Complex float: %10.2f\\n\", 3.14159)");
    handleCommand("kernel_printf(\"Complex float: %-10.2f\\n\", 3.14159)");

    // 6. 문자 출력
    handleCommand("kernel_printf(\"A character : %c\\n\", 'A')");

    // 7. 서식있는 정수 출력
    handleCommand("kernel_printf(\"Formatted number: %+08d\\n\", -123)");
    handleCommand("kernel_printf(\"Unsigned int: %u\\n\", 4294967295U)");

    // 8. 다양한 정수 포맷
    handleCommand("kernel_printf(\"Hexadecimal: %x\\n\", 255)");
    handleCommand("kernel_printf(\"Octal: %o\\n\", 511)");

    // 9. 문자열 출력
    handleCommand("kernel_printf(\"%-10s\\n\", \"LeftAligned\")");

    // // 10. 혼합 테스트
    // handleCommand("kernel_printf(\"Mix: %d, %f, %c, %x\\n\", 100, 2.718, 'B', 0x7F)");

    // // 11. 복잡한 서식 지정
    // handleCommand("kernel_printf(\"%-0#+10o\\n\", 2048)");
    // handleCommand("kernel_printf(\"%+10u\\n\", 12345)");
    // handleCommand("kernel_printf(\"%#+x\\n\", 12345678)");
}

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

#include "cmdwindow.h"
#include "ui_cmdwindow.h"
#include "kernel_print.h"

extern "C" {
    bool kernel_create_process(const char *process_name);
    void kernel_list_processes();
    bool kernel_kill_process(const char *process_name);
    void register_print_function(void (*print_function)(const char *str));
    void az_printf(const char *format, ...);
}

/*
 * @brief CMD 창 생성자
 * @param parent 부모 위젯, 기본값은 nullptr
 * @details CMD 창을 초기화하고 스타일 및 초기 설정을 적용합니다.
 */
CmdWindow::CmdWindow(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::cmdWindow)
{
    ui->setupUi(this);

    // textEdit의 배경을 검은색, 텍스트를 녹색으로 설정하여 CMD 스타일로 변경
    ui->textEdit->setStyleSheet("background-color: black; color: green; font-family: 'Courier'; font-size: 12px;");
    ui->textEdit->installEventFilter(this); // 이벤트 필터 설치

    // C 코드에서 사용할 출력 함수 등록
    register_print_function(qt_print);

    // 초기 명령어 프롬프트
    ui->textEdit->append("kernel> ");
}

/*
 * @brief CMD 창 소멸자
 * @details CMD 창에서 사용한 자원을 해제합니다.
 */
CmdWindow::~CmdWindow()
{
    delete ui;
}

/*
 * @brief 키보드 입력 이벤트 처리
 * @param event 키보드 이벤트 객체
 * @details Enter 키를 누를 경우, 명령어 입력을 처리합니다.
 */
void CmdWindow::keyPressEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_Return || event->key() == Qt::Key_Enter) {
        on_submitButton_clicked();
    } else {
        QWidget::keyPressEvent(event);  // 다른 키는 기본 처리
    }
}

/*
 * @brief 이벤트 필터
 * @param obj 이벤트가 발생한 객체
 * @param event 처리할 이벤트
 * @return bool 이벤트를 처리했는지 여부
 * @details CMD 창에서 특정 키 입력(Enter, 화살표, Backspace)에 대한 동작을 커스터마이징합니다.
 */
bool CmdWindow::eventFilter(QObject *obj, QEvent *event)
{
    if (obj == ui->textEdit && event->type() == QEvent::KeyPress) {
        QKeyEvent *keyEvent = static_cast<QKeyEvent*>(event);
        QTextCursor cursor = ui->textEdit->textCursor();
        int blockNumber = cursor.blockNumber();
        int positionInBlock = cursor.positionInBlock();

        // 엔터 키 처리
        if (keyEvent->key() == Qt::Key_Return || keyEvent->key() == Qt::Key_Enter) {
            on_submitButton_clicked();
            return true; // 엔터 키 이벤트를 처리했음을 알림
        }

        // 위/왼쪽 화살표 키가 입력된 경우
        if (keyEvent->key() == Qt::Key_Up || keyEvent->key() == Qt::Key_Left) {
            if (blockNumber == ui->textEdit->document()->blockCount() - 1) {
                // 커서가 마지막 줄에 있는 경우, "kernel> " 이후로만 이동 가능하게 제한
                if (positionInBlock <= 8) {
                    return true; // "kernel> " 이전으로 이동하지 못하게 막음
                }
                return QWidget::eventFilter(obj, event);  // 마지막 줄에서는 기본 동작 허용
            } else {
                return true; // 커서가 이전 줄로 이동하지 못하게 막음
            }
        }

        // 백스페이스 키가 입력된 경우
        if (keyEvent->key() == Qt::Key_Backspace) {
            if (blockNumber == ui->textEdit->document()->blockCount() - 1) {
                if (positionInBlock <= 8) {
                    return true; // "kernel> " 이후로만 삭제 가능하게 막음
                }
            }
        }
    }
    return QWidget::eventFilter(obj, event); // 다른 이벤트는 기본 처리
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
}

/*
 * @brief 명령어 처리 함수
 * @param command 사용자가 입력한 명령어
 * @details 사용자가 입력한 명령어를 분석하고, 해당 명령어에 맞는 동작을 수행합니다.
 */
void CmdWindow::handleCommand(const QString &command) {
    if (command == "exit") {
        close();
    }

    else if (command == "help") {
        ui->textEdit->append("Available commands:");
        ui->textEdit->append("  create <process_name>       - Create a new process with the given name");
        ui->textEdit->append("  create printf(\"message\")  - Print a message");
        ui->textEdit->append("  kernel_printf(\"message\")  - Print a message using kernel_printf");
        ui->textEdit->append("  kernel_printf_test : kp_test");
        ui->textEdit->append("  kill <process_name>         - Kill the process with the given name");
        ui->textEdit->append("  list                        - List all processes");
        ui->textEdit->append("  clear                       - Clear the screen");
        ui->textEdit->append("  help                        - Show this help message");
        ui->textEdit->append("  exit                        - Exit the shell");
    }

    else if (command.startsWith("create printf(")) {
        QRegularExpression re(R"raw(create printf\("(.*)"\))raw");
        QRegularExpressionMatch match = re.match(command);
        if (match.hasMatch()) {
            QString message = match.captured(1);
            az_printf("\n%s", message.toStdString().c_str());  // 메시지 출력
            ui->textEdit->append("");  // 새로운 라인 추가
        } else {
            ui->textEdit->append("Invalid command format. Use: create printf(\"message\")");
        }
    }

    else if (command.startsWith("kernel_printf(")) {
        QRegularExpression re(R"raw(kernel_printf\("(.*)"\))raw");
        QRegularExpressionMatch match = re.match(command);
        if (match.hasMatch()) {
            QString message = match.captured(1);

            // Qt 콘솔에 출력
            QString debugMessage = message;
            debugMessage.replace("\\n\n", "\n");
            qDebug().noquote() << debugMessage;

            // CMD 창 UI에 표시
            ui->textEdit->append(message);  // 메시지를 textEdit에 추가
            kernel_printf("Console Test Print :::: %s\n", message.toStdString().c_str());  // 메시지 출력
            ui->textEdit->append("");  // 새로운 라인 추가
        } else {
            ui->textEdit->append("Invalid command format. Use: kernel_printf(\"message\")");
        }
    }

    else if (command == "kp_test") {
        test_kernel_printf();
    }

    else if (command.startsWith("create ")) {
        QString process_name = command.mid(7);
        bool success = kernel_create_process(process_name.toStdString().c_str());
        if (success) {
            // ui->textEdit->append("Created process: " + process_name);
        } else {
            ui->textEdit->append("Failed to create process: " + process_name);
        }
    }

    else if (command.startsWith("kill ")) {
        QString process_name = command.mid(5);
        bool success = kernel_kill_process(process_name.toStdString().c_str());
        if (success) {
            // ui->textEdit->append("Killed process: " + process_name);
        } else {
            ui->textEdit->append("No running process found with name: " + process_name);
        }
    }

    else if (command == "list") {
        kernel_list_processes();
    }

    else if (command == "clear") {
        ui->textEdit->clear(); // 화면을 비움
        ui->textEdit->append("kernel> ");
    }

    else {
        ui->textEdit->append("Unknown command. Type 'help' for a list of commands.");
    }
}

/*
 * @brief Qt 콘솔 출력 함수
 * @param str 출력할 문자열
 * @details C 코드에서 호출되는 출력 함수로, CMD 창 UI에 메시지를 표시합니다.
 */
void CmdWindow::qt_print(const char *str)
{
    CmdWindow *window = qobject_cast<CmdWindow *>(qApp->activeWindow());
    if (window && window->ui && window->ui->textEdit) {
        window->ui->textEdit->moveCursor(QTextCursor::End);  // 커서를 끝으로 이동
        window->ui->textEdit->insertPlainText(QString::fromUtf8(str));  // 메시지 출력
        // window->ui->textEdit->append("");  // 새로운 라인 추가
        // window->ui->textEdit->insertPlainText("\n");  // 개행 추가
    }
}

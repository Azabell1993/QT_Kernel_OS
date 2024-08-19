#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QKeyEvent>
#include <QString>
#include <QRegularExpression>

// #include "kernel_lib.h"

// C 함수들을 선언
extern "C" {
    bool kernel_create_process(const char *process_name);
    void kernel_list_processes();
    bool kernel_kill_process(const char *process_name);
    void register_print_function(void (*print_function)(const char *str));
    void az_printf(const char *format, ...);
}

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->textEdit->installEventFilter(this); // 이벤트 필터 설치

    // C 코드에서 사용할 출력 함수 등록
    register_print_function(qt_print);

    // 초기 명령어 프롬프트
    ui->textEdit->append("kernel> ");
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::keyPressEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_Return || event->key() == Qt::Key_Enter) {
        on_submitButton_clicked();
    } else {
        QMainWindow::keyPressEvent(event);  // 다른 키는 기본 처리
    }
}

bool MainWindow::eventFilter(QObject *obj, QEvent *event)
{
    if (obj == ui->textEdit && event->type() == QEvent::KeyPress) {
        QKeyEvent *keyEvent = static_cast<QKeyEvent*>(event);
        if (keyEvent->key() == Qt::Key_Return || keyEvent->key() == Qt::Key_Enter) {
            on_submitButton_clicked();
            return true; // 엔터 키 이벤트를 처리했음을 알림
        }
    }
    return QMainWindow::eventFilter(obj, event); // 다른 이벤트는 기본 처리
}

void MainWindow::on_submitButton_clicked() {
    // 현재 텍스트를 가져옴
    QString command = ui->textEdit->toPlainText().split("\n").last().replace("kernel> ", "");

    handleCommand(command);

    // 새로운 명령어를 위한 프롬프트 추가 (한 줄 개행만)
    ui->textEdit->moveCursor(QTextCursor::End);
    ui->textEdit->insertPlainText("\nkernel> ");
}

void MainWindow::handleCommand(const QString &command) {
    if (command == "exit") {
        close();
    } else if (command == "help") {
        ui->textEdit->append("\nAvailable commands:");
        ui->textEdit->append("  create <process_name>       - Create a new process with the given name");
        ui->textEdit->append("  create printf(\"message\")  - Print a message");
        ui->textEdit->append("  kill <process_name>         - Kill the process with the given name");
        ui->textEdit->append("  list                        - List all processes");
        ui->textEdit->append("  clear                       - Clear the screen");
        ui->textEdit->append("  help                        - Show this help message");
        ui->textEdit->append("  exit                        - Exit the shell");
    } else if (command.startsWith("create printf(")) {
        QRegularExpression re(R"raw(create printf\("(.*)"\))raw");
        QRegularExpressionMatch match = re.match(command);
        if (match.hasMatch()) {
            QString message = match.captured(1);
            az_printf("\n%s\n", message.toStdString().c_str());
        } else {
            ui->textEdit->append("\nInvalid command format. Use: create printf(\"message\")");
        }
    } else if (command.startsWith("create ")) {
        QString process_name = command.mid(7);
        bool success = kernel_create_process(process_name.toStdString().c_str());
        if (success) {
            ui->textEdit->append("\nCreated process: " + process_name);
        } else {
            ui->textEdit->append("\nFailed to create process: " + process_name);
        }
    } else if (command.startsWith("kill ")) {
        QString process_name = command.mid(5);
        bool success = kernel_kill_process(process_name.toStdString().c_str());
        if (success) {
            ui->textEdit->append("\nKilled process: " + process_name);
        } else {
            ui->textEdit->append("\nNo running process found with name: " + process_name);
        }
    } else if (command == "list") {
        kernel_list_processes();
    } else if (command == "clear") {
        ui->textEdit->clear(); // 화면을 비움
        ui->textEdit->append("kernel> ");
    } else {
        ui->textEdit->append("\nUnknown command. Type 'help' for a list of commands.");
    }
}

// C 코드에서 호출할 출력 함수 구현
void MainWindow::qt_print(const char *str)
{
    MainWindow *window = qobject_cast<MainWindow *>(qApp->activeWindow());
    if (window && window->ui && window->ui->textEdit) {
        window->ui->textEdit->moveCursor(QTextCursor::End);  // 커서를 끝으로 이동
        window->ui->textEdit->insertPlainText("\n");  // 다음 줄로 이동 후 출력
        window->ui->textEdit->insertPlainText(QString::fromUtf8(str));
    }
}

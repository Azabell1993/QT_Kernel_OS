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
#include <QStack>       // 간단한 수식 평가를 위한 스택
#include <QChar>        // 개별 문자를 처리하기 위한 QChar
#include "cmdwindow.h"
#include "ui_cmdwindow.h"

// C 정적 라이브러리
#include "kernel_print.h"

// 직접 하드코딩된 C 함수
extern "C" {
bool kernel_create_process(const char *process_name);
void kernel_list_processes();
bool kernel_kill_process(const char *process_name);
void register_print_function(void (*print_function)(const char *str));
void az_printf(const char *format, ...);
void kernel_putchar(char c);

int main_1();
int main_2();
int main_3();
int main_4();
int main_5();
int main_6();
int main_7();
}


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

    // CMD 스타일로 배경을 검은색, 텍스트를 녹색으로 설정
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
CmdWindow::~CmdWindow() {
    delete ui;
}


/*
 * @brief 키보드 입력 이벤트 처리
 * @param event 키보드 이벤트 객체
 * @details Enter 키를 누를 경우, 명령어 입력을 처리합니다.
 */
void CmdWindow::keyPressEvent(QKeyEvent *event) {
    if (event->key() == Qt::Key_Return || event->key() == Qt::Key_Enter) {
        on_submitButton_clicked();
    } else {
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
        ui->textEdit->append("  printf(\"message\")  - Print a message using kernel_printf");
        ui->textEdit->append("  printf_test : kp_test");
        ui->textEdit->append("  run_asm <number>        - Run the kernel_asm_<number> function.");
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
            az_printf("\n%s", message.toStdString().c_str());
            ui->textEdit->append("");
        } else {
            ui->textEdit->append("Invalid command format. Use: create printf(\"message\")");
        }
    }

    else if (command.startsWith("asm ")) {
        QStringList parts = command.split(" ");
        if (parts.size() == 2) {
            int number = parts[1].toInt();
            switch (number) {
            case 1:
                ui->textEdit->append("Executed kernel_asm_1.");
                main_1();  // kernel_asm_1.a의 main_1 함수 호출
                break;
            case 2:
                ui->textEdit->append("Executed kernel_asm_2.");
                main_2();  // kernel_asm_2.a의 main_2 함수 호출
                break;
            case 3:
                ui->textEdit->append("Executed kernel_asm_3.");
                main_3();  // kernel_asm_3.a의 main_3 함수 호출
                break;
            case 4:
                ui->textEdit->append("Executed kernel_asm_4.");
                main_4();  // kernel_asm_4.a의 main_4 함수 호출
                break;
            case 5:
                ui->textEdit->append("Executed kernel_asm_5.");
                main_5();  // kernel_asm_5.a의 main_5 함수 호출
                break;
            case 6:
                ui->textEdit->append("Executed kernel_asm_6.");
                main_6();  // kernel_asm_6.a의 main_6 함수 호출
                break;
            case 7:
                ui->textEdit->append("Executed kernel_asm_7.");
                main_7();  // kernel_asm_7.a의 main_7 함수 호출
                break;
            default:
                ui->textEdit->append("Invalid number. Please enter a number between 1 and 7.");
            }
        } else {
            ui->textEdit->append("Usage: asm <number>");
        }
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
    else if (command == "kp_test") {
        test_kernel_printf();
    }
    else if (command.startsWith("create ")) {
        QString process_name = command.mid(7);
        bool success = kernel_create_process(process_name.toStdString().c_str());
        if (!success) {
            ui->textEdit->append("Failed to create process: " + process_name);
        }
    }
    else if (command.startsWith("kill ")) {
        QString process_name = command.mid(5);
        bool success = kernel_kill_process(process_name.toStdString().c_str());
        if (!success) {
            ui->textEdit->append("No running process found with name: " + process_name);
        }
    }
    else if (command == "list") {
        kernel_list_processes();
    }
    else if (command == "clear") {
        ui->textEdit->clear();
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
        window->ui->textEdit->moveCursor(QTextCursor::End);
        window->ui->textEdit->insertPlainText(QString::fromUtf8(str));
        // Qt 이벤트 루프를 강제로 실행하여 즉시 업데이트
        QCoreApplication::processEvents();
    }
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

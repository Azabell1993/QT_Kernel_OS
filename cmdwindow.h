#ifndef CMDWINDOW_H
#define CMDWINDOW_H

#include <QDialog>
#include <QVariantList>
#include <QList>

namespace Ui {
class cmdWindow;
}

class CmdWindow : public QDialog
{
    Q_OBJECT

public:
    explicit CmdWindow(QWidget *parent = nullptr);
    ~CmdWindow();

protected:
    void keyPressEvent(QKeyEvent *event) override;
    bool eventFilter(QObject *obj, QEvent *event) override;

private slots:
    void on_submitButton_clicked();  // 슬롯 함수로 남겨두기
    void handleCommand(const QString &command);  // 슬롯 함수로 남겨두기

private:  // 슬롯이 아닌 일반 함수는 여기로 이동
    static void qt_print(const char *str);

    QVariantList parseArguments(const QString &arguments);
    void executeKernelPrintf(const QString &format, const QVariantList &args);

    int evaluateSimpleExpression(const QString &expression);
    int precedence(QChar op);
    int applyOp(int a, int b, QChar op);
    void appendWithWhiteText(const QString &text);
    void runTests();
    void createProcessWithMessage(const QString &message);

    void runSemaphoreTest();
    void runMultithreadingTest();
    void runMutexTest();
    void runUnifiedProcess(const QStringList &args);

    QList<QString> commandHistory;  // 명령어 기록을 저장하는 리스트

private:
    Ui::cmdWindow *ui;
};

#endif // CMDWINDOW_H

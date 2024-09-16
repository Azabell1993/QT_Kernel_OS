#ifndef CMDWINDOW_H
#define CMDWINDOW_H

#include <QDialog>
#include <QVariantList>

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
    void on_submitButton_clicked();
    void handleCommand(const QString &command);
    static void qt_print(const char *str);

    // 함수 선언
    QVariantList parseArguments(const QString &arguments);
    void executeKernelPrintf(const QString &format, const QVariantList &args);

    int evaluateSimpleExpression(const QString &expression);

    int precedence(QChar op);
    int applyOp(int a, int b, QChar op);

    void runTests();

    void createProcessWithMessage(const QString &message);
    void runSemaphoreTest();
    void runMultithreadingTest();
    void runMutexTest();

    void runUnifiedProcess(const QStringList &args);
private:
    Ui::cmdWindow *ui;
};

#endif // CMDWINDOW_H

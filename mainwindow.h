#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QObject>
#include <QTextEdit>
#include "kernel_lib.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

protected:
    void keyPressEvent(QKeyEvent *event) override;

private slots:
    void on_submitButton_clicked();

private:
    Ui::MainWindow *ui;

    void handleCommand(const QString &command);
    bool eventFilter(QObject *obj, QEvent *event) override;

    // C 코드에서 호출할 출력 함수
    static void qt_print(const char *str);
};

#endif // MAINWINDOW_H

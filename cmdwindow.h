// cmdwindow.h
/*
 * CMD Window
 *
 * Maintainer: Park Jiwoo
 *
 * Copyright (C) 2024 Park-Jiwoo
 *
 */
#ifndef CMDWINDOW_H
#define CMDWINDOW_H

#include <QDialog>

QT_BEGIN_NAMESPACE
namespace Ui { class cmdWindow; }
QT_END_NAMESPACE

class CmdWindow : public QDialog
{
    Q_OBJECT

public:
    explicit CmdWindow(QWidget *parent = nullptr);
    ~CmdWindow();

protected:
    void keyPressEvent(QKeyEvent *event) override;

private slots:
    void on_submitButton_clicked();

private:
    Ui::cmdWindow *ui;

    void handleCommand(const QString &command);
    bool eventFilter(QObject *obj, QEvent *event) override;

    // C 코드에서 호출할 출력 함수
    static void qt_print(const char *str);
};

#endif // CMDWINDOW_H

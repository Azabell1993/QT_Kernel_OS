/*
 * Main Window
 *
 * Maintainer: Park Jiwoo
 *
 * Copyright (C) 2024 Park-Jiwoo
 *
 */
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPoint>
#include <QLabel>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void updateTime();
    void openCmdWindow();
    void showContextMenu(const QPoint &pos);
    void showIconContextMenu(const QPoint &pos);
    void action1Triggered();
    void action2Triggered();
    void openTriggered();
    void deleteTriggered();
    void showImplementationPopup();
    void updateClock(QLabel *timeLabel);

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H

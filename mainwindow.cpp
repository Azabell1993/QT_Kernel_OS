// mainwindow.cpp
/*
 * Main Window
 *
 * Maintainer: Park Jiwoo
 *
 * Copyright (C) 2024 Park-Jiwoo
 *
 */
#include "mainwindow.h"
#include "cmdwindow.h"
#include "ui_mainwindow.h"
#include <QDateTime>
#include <QToolButton>
#include <QPushButton>
#include <QTimer>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // 스타일 설정
    ui->startButton->setStyleSheet("QToolButton { background-color: #0078D7; color: white; border-radius: 5px; padding: 5px; }"
                                   "QToolButton:hover { background-color: #005A9E; }");

    ui->timeLabel->setStyleSheet("QLabel { color: white; font-weight: bold; }");

    // 데스크바 스타일 적용
    ui->statusbar->setStyleSheet("QStatusBar { background-color: #333; color: white; }");

    // 시간 업데이트를 위한 타이머 설정
    QTimer *timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &MainWindow::updateTime);
    timer->start(1000); // 매초 업데이트

    updateTime(); // 초기 시간 설정

    // 시작 버튼에 메뉴 설정
    QMenu *startMenu = new QMenu(this);
    QAction *cmdAction = new QAction(QIcon(":/cmd.png"), "cmd", this);
    startMenu->addAction(cmdAction);
    ui->startButton->setMenu(startMenu);
    ui->startButton->setPopupMode(QToolButton::InstantPopup);

    // cmd 메뉴 클릭 시 CmdWindow 열기
    connect(cmdAction, &QAction::triggered, this, &MainWindow::openCmdWindow);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::updateTime()
{
    QDateTime currentTime = QDateTime::currentDateTime();
    QString timeString = currentTime.toString("hh:mm:ss");
    ui->timeLabel->setText(timeString);
}

void MainWindow::openCmdWindow()
{
    CmdWindow *cmdWindow = new CmdWindow(this);
    cmdWindow->show();
}

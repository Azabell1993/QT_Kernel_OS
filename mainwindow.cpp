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
#include <QLabel>
#include <QToolButton>
#include <QPushButton>
#include <QVBoxLayout>
#include <QMessageBox>
#include <QHBoxLayout>
#include <QTimer>
#include <QTime>
#include <QPixmap>
#include <QPoint>
#include <QMenu>
#include <QContextMenuEvent>

/*
 * @brief MainWindow 생성자
 * @param parent 부모 위젯, 기본값은 nullptr
 */
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setStyleSheet("background-color: #c0c0c0;");

    // 이미지 로고 추가
    QLabel *logoLabel = new QLabel(this);
    QPixmap logoPixmap(":/site_logo.png");
    if (!logoPixmap.isNull()) {
        logoLabel->setPixmap(logoPixmap.scaled(700, 400, Qt::KeepAspectRatio, Qt::SmoothTransformation)); // 이미지 크기 조정
        logoLabel->setAlignment(Qt::AlignCenter); // 이미지 정중앙 정렬
        logoLabel->setGeometry(0, 0, 800, 550); // 창 크기에 맞게 배치
    }

    // 바탕화면 아이콘 생성 및 이미지 적용
    QToolButton *computerIcon = new QToolButton(this);
    QPixmap computerPixmap(":/mypc.png"); // 이미지 경로 확인
    if (!computerPixmap.isNull()) {
        computerIcon->setIcon(QIcon(computerPixmap));
        computerIcon->setIconSize(QSize(64, 64)); // 이미지 크기 조정
    }
    computerIcon->setGeometry(20, 20, 100, 100);
    computerIcon->setStyleSheet("background: transparent; font-size: 12px;");
    computerIcon->setText("My Computer");
    computerIcon->setToolButtonStyle(Qt::ToolButtonTextUnderIcon); // 아이콘 아래 텍스트 배치

    // 쓰레기통 아이콘 생성 및 이미지 적용
    QToolButton *recycleBinIcon = new QToolButton(this);
    QPixmap recycleBinPixmap(":/trashcan.png"); // 이미지 경로 확인
    if (!recycleBinPixmap.isNull()) {
        recycleBinIcon->setIcon(QIcon(recycleBinPixmap));
        recycleBinIcon->setIconSize(QSize(64, 64)); // 이미지 크기 조정
    }
    recycleBinIcon->setGeometry(20, 140, 100, 100);
    recycleBinIcon->setStyleSheet("background: transparent; font-size: 12px;");
    recycleBinIcon->setText("Recycle Bin");
    recycleBinIcon->setToolButtonStyle(Qt::ToolButtonTextUnderIcon); // 아이콘 아래 텍스트 배치

    // 여기다가 메모장이나 다른 기타 프로그램을 만들어서 이미지를 적용하고 호출 할 수 있음

    // "My Computer" 아이콘 클릭 시 팝업창 표시
    connect(computerIcon, &QToolButton::clicked, this, &MainWindow::showImplementationPopup);

    // "Recycle Bin" 아이콘 클릭 시 팝업창 표시
    connect(recycleBinIcon, &QToolButton::clicked, this, &MainWindow::showImplementationPopup);

    // 작업 표시줄 생성
    QWidget *taskBar = new QWidget(this);
    taskBar->setGeometry(0, height() - 40, width(), 40);

    // 주황색으로 작업 표시줄 배경 색상 설정
    taskBar->setStyleSheet("background-color: #FFA500;");

    // Start 버튼 생성 및 이미지 적용
    QToolButton *startButton = new QToolButton(taskBar);
    QPixmap startPixmap(":/start.png"); // 이미지 경로 확인
    if (!startPixmap.isNull()) {
        startButton->setIcon(QIcon(startPixmap));
        // startButton->setText("Start"); // 텍스트 생략
        startButton->setIconSize(QSize(30, 30)); // 아이콘 크기 설정
        // startButton->setGeometry(10, 5, 75, 30);
        // startButton->setStyleSheet("background-color: #c0c0c0; color: black;");
    }

    // Start 메뉴 설정
    QMenu *startMenu = new QMenu(this);
    QAction *cmdAction = new QAction(QIcon(":/cmd.png"), "cmd", this);
    QAction *exitAction = new QAction(QIcon(":/exit.png"), "Exit", this);

    startMenu->addAction(cmdAction);
    startMenu->addAction(exitAction);

    startButton->setMenu(startMenu);
    startButton->setPopupMode(QToolButton::InstantPopup);

    connect(cmdAction, &QAction::triggered, this, &MainWindow::openCmdWindow);
    connect(exitAction, &QAction::triggered, this, &QApplication::quit);

    // 시작 버튼 클릭 시 메뉴가 위로 열리도록 설정
    connect(startButton, &QToolButton::clicked, [this, startButton, startMenu]() {
        QRect buttonGeometry = startButton->geometry();
        QPoint menuPos = startButton->mapToGlobal(buttonGeometry.topLeft());
        startMenu->popup(menuPos - QPoint(0, startMenu->sizeHint().height())); // 위로 펼쳐지도록 위치 조정
    });

    // 작업 표시줄 오른쪽에 시계 표시
    QLabel *timeLabel = new QLabel(taskBar);
    timeLabel->setGeometry(width() - 110, 5, 100, 30);
    timeLabel->setStyleSheet("color: white; font-weight: bold;");

    // 시계 업데이트를 위한 타이머 설정
    QTimer *timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, [this, timeLabel]() {
        updateClock(timeLabel);
    });
    timer->start(1000);

    // 시계 초기화
    updateClock(timeLabel);
}

/*
 * @brief 소멸자
 */
MainWindow::~MainWindow()
{
    delete ui;
}

/*
 * @brief CMD 창을 여는 함수
 * @details CMD 창을 열어주는 역할을 수행합니다.
 */
void MainWindow::openCmdWindow()
{
    CmdWindow *cmdWindow = new CmdWindow(this);
    cmdWindow->show();
}

/*
 * @brief "구현중" 팝업 표시
 * @details 클릭된 아이콘에 대해 구현 중이라는 팝업을 띄웁니다.
 */
void MainWindow::showImplementationPopup()
{
    QMessageBox::information(this, "구현중", "이 기능은 구현 중입니다.");
}

/*
 * @brief 시계 업데이트 함수
 * @param timeLabel 시계를 표시할 QLabel 객체
 * @details 현재 시간을 업데이트하여 주어진 QLabel에 표시합니다.
 */
void MainWindow::updateClock(QLabel *timeLabel)
{
    timeLabel->setText(QDateTime::currentDateTime().toString("hh:mm:ss"));
}

/*
 * @brief MainWindow에서 시간을 업데이트하는 함수
 * @details 이 함수는 타이머 또는 다른 이벤트에 의해 호출되어 시간을 업데이트합니다.
 */
void MainWindow::updateTime() {
    // 시간 업데이트에 관련된 코드
}

/*
 * @brief MainWindow에서 "Open" 액션이 트리거될 때 호출되는 함수
 * @details 이 함수는 "Open" 메뉴 항목이나 버튼이 클릭되었을 때 실행됩니다.
 */
void MainWindow::openTriggered() {
    // "Open" 동작에 관련된 코드
}

/*
 * @brief MainWindow에서 "Delete" 액션이 트리거될 때 호출되는 함수
 * @details 이 함수는 "Delete" 메뉴 항목이나 버튼이 클릭되었을 때 실행됩니다.
 */
void MainWindow::deleteTriggered() {
    // "Delete" 동작에 관련된 코드
}

/*
 * @brief MainWindow에서 컨텍스트 메뉴를 표시하는 함수
 * @param pos 컨텍스트 메뉴를 표시할 위치
 * @details 이 함수는 사용자가 우클릭했을 때 실행되며, 지정된 위치에 컨텍스트 메뉴를 표시합니다.
 */
void MainWindow::showContextMenu(const QPoint &pos) {
    // 컨텍스트 메뉴 표시 코드
}

/*
 * @brief MainWindow에서 "Action 1"이 트리거될 때 호출되는 함수
 * @details 이 함수는 특정 액션이 실행되었을 때 실행됩니다.
 */
void MainWindow::action1Triggered() {
    // "Action 1"에 대한 동작 코드
}

/*
 * @brief MainWindow에서 "Action 2"가 트리거될 때 호출되는 함수
 * @details 이 함수는 또 다른 특정 액션이 실행되었을 때 실행됩니다.
 */
void MainWindow::action2Triggered() {
    // "Action 2"에 대한 동작 코드
}

/*
 * @brief MainWindow에서 아이콘에 대한 컨텍스트 메뉴를 표시하는 함수
 * @param pos 컨텍스트 메뉴를 표시할 위치
 * @details 이 함수는 아이콘을 우클릭했을 때 실행되며, 해당 위치에 컨텍스트 메뉴를 표시합니다.
 */
void MainWindow::showIconContextMenu(const QPoint &pos) {
    // 아이콘 컨텍스트 메뉴 표시 코드
}


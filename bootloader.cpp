// bootloader.cpp
/*
 * Boot Loader
 *
 * Maintainer: Park Jiwoo
 *
 * Copyright (C) 2024 Park-Jiwoo
 *
 */
#include "bootloader.h"
#include "mainwindow.h"
#include <QKeyEvent>
#include <QApplication>
#include <QTimer>
#include <QLabel>
#include <QVBoxLayout>
#include <QThread>
#include <QSpacerItem>
#include <QTextEdit>

/*
 * @brief Bootloader 생성자
 * @param parent 부모 위젯, 기본값은 nullptr
 * @details 부트로더 UI를 초기화하고 부팅 시뮬레이션을 시작합니다.
 */
Bootloader::Bootloader(QWidget *parent)
    : QWidget(parent)
{
    this->setStyleSheet("background-color: black; color: green;");
    this->setWindowTitle("VEDA OS Bootloader");
    this->setFixedSize(800, 600);

    QVBoxLayout *layout = new QVBoxLayout(this);

    // 이미지 로고 추가
    QLabel *logoLabel = new QLabel(this);
    QPixmap logoPixmap(":/booting_img.jpg"); // 경로를 실제 이미지 경로로 변경
    logoLabel->setPixmap(logoPixmap.scaled(400, 400, Qt::KeepAspectRatio, Qt::SmoothTransformation)); // 이미지 크기 조정
    logoLabel->setAlignment(Qt::AlignCenter); // 이미지 정중앙 정렬

    // 부팅 상태 메시지 추가
    QLabel *label = new QLabel("VEDA OS Booting...", this);
    label->setAlignment(Qt::AlignCenter);

    // 레이아웃 구성
    layout->addStretch();
    layout->addWidget(logoLabel);
    layout->addWidget(label);
    layout->addStretch();

    setLayout(layout);

    // 부팅 시뮬레이션 시작
    QTimer::singleShot(500, this, &Bootloader::simulateBootProcess);
}

/*
 * @brief Bootloader 소멸자
 * @details 부트로더에서 사용한 자원을 해제합니다.
 */
Bootloader::~Bootloader()
{
}

/*
 * @brief 키 입력 이벤트 처리
 * @param event 키보드 이벤트 객체
 * @details Enter 키가 눌리면 OS 부팅을 시작합니다.
 */
void Bootloader::keyPressEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_Return || event->key() == Qt::Key_Enter) {
        startOS();
    }
}

/*
 * @brief 부팅 시뮬레이션 함수
 * @details 부팅 과정을 시뮬레이션하여 텍스트로 표시합니다.
 */
void Bootloader::simulateBootProcess()
{
    // 부팅 시뮬레이션을 위한 텍스트 창 생성
    QTextEdit *bootScreen = new QTextEdit(this);
    bootScreen->setStyleSheet("background-color: black; color: green; font-family: 'Courier'; font-size: 12px;");
    bootScreen->setReadOnly(true);

    QVBoxLayout *layout = qobject_cast<QVBoxLayout*>(this->layout());
    layout->addWidget(bootScreen);

    QString progress_bar = QString("          "); // 10단계의 진행 바
    bootScreen->append("Loading kernel... [" + progress_bar + "]");

    QTextCursor cursor = bootScreen->textCursor(); // 현재 커서 위치를 가져옴
    cursor.movePosition(QTextCursor::PreviousBlock, QTextCursor::MoveAnchor); // 첫 번째 줄로 이동

    for (int i = 0; i < 10; ++i) {
        QThread::msleep(1000); // 각 단계마다 1초 대기
        progress_bar[i * 1] = '#'; // 진행 상황 표시

        cursor.movePosition(QTextCursor::StartOfLine); // 현재 줄의 시작으로 이동
        cursor.select(QTextCursor::LineUnderCursor); // 현재 줄을 선택
        cursor.removeSelectedText(); // 선택된 줄의 내용을 지우기
        cursor.insertText("Loading kernel... [" + progress_bar + "]"); // 새로운 진행 상태 출력

        bootScreen->setTextCursor(cursor); // 업데이트된 커서를 설정
        QCoreApplication::processEvents(); // GUI 업데이트
    }

    bootScreen->append("\nKernel loaded.");
    bootScreen->append("Initializing subsystems...");
    QThread::msleep(1000); // 추가 딜레이
    bootScreen->append("Subsystems initialized.");
    bootScreen->append("\nPress Enter to continue...");
    bootScreen->moveCursor(QTextCursor::End);
}

/*
 * @brief OS 부팅 시작 함수
 * @details 부팅이 완료되면 메인 윈도우를 표시하고 부트로더 창을 닫습니다.
 */
void Bootloader::startOS()
{
    // 메인 윈도우로 전환
    MainWindow *mainWindow = new MainWindow();
    mainWindow->show();
    this->close();
}

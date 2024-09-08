#include "smartpointerdialog.h"
#include "ui_smartpointerdialog.h"
#include <QLabel>

SmartPointerDialog::SmartPointerDialog(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::SmartPointerDialog)
{
    ui->setupUi(this);
    layout = new QVBoxLayout(this);  // QVBoxLayout 생성
    this->setLayout(layout);         // 다이얼로그에 레이아웃 설정
}

SmartPointerDialog::~SmartPointerDialog()
{
    delete ui;
}

void SmartPointerDialog::addSmartPointer(int ptrValue, int refCount) {
    // 스마트 포인터 정보를 위한 라벨 생성
    QLabel *label = new QLabel(QString("Pointer: %1, Ref Count: %2").arg(ptrValue).arg(refCount), this);
    layout->addWidget(label);
    pointerWidgets[ptrValue] = label;  // 포인터 값에 해당하는 라벨 저장
}

void SmartPointerDialog::removeSmartPointer(int ptrValue) {
    if (pointerWidgets.contains(ptrValue)) {
        QLabel *label = qobject_cast<QLabel*>(pointerWidgets[ptrValue]);
        layout->removeWidget(label);
        delete label;
        pointerWidgets.remove(ptrValue);  // 포인터 값에 해당하는 위젯 삭제
    }
}

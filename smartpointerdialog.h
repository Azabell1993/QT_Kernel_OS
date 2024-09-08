#ifndef SMARTPOINTERDIALOG_H
#define SMARTPOINTERDIALOG_H

#include <QWidget>
#include <QVBoxLayout>
#include <QMap>

namespace Ui {
class SmartPointerDialog;
}

class SmartPointerDialog : public QWidget
{
    Q_OBJECT

public:
    explicit SmartPointerDialog(QWidget *parent = nullptr);
    ~SmartPointerDialog();

    void addSmartPointer(int ptrValue, int refCount);
    void removeSmartPointer(int ptrValue);

private:
    Ui::SmartPointerDialog *ui;
    QVBoxLayout *layout;
    QMap<int, QWidget*> pointerWidgets;  // 포인터 값과 연결된 위젯을 저장
};

#endif // SMARTPOINTERDIALOG_H

#ifndef BOOTLOADER_H
#define BOOTLOADER_H

#include <QWidget>
#include <QTextEdit>

class Bootloader : public QWidget
{
    Q_OBJECT

public:
    explicit Bootloader(QWidget *parent = nullptr);
    ~Bootloader();

protected:
    void keyPressEvent(QKeyEvent *event) override;

private slots:
    void startOS();
    void simulateBootProcess();
    // void handleCommand(const QString &command);
    // void appendPrompt();
    // void appendToBiosScreen(const QString &text, bool withPrompt = true);

private:
    static QTextEdit *global_output_widget;

    // 이 함수는 가변 인자를 처리하는 메서드로 사용
    void az_printf(const char *format, ...);
};

#endif // BOOTLOADER_H

// bootloader.h
/*
 * Boot Loader
 *
 * Maintainer: Park Jiwoo
 *
 * Copyright (C) 2024 Park-Jiwoo
 *
 */
#ifndef BOOTLOADER_H
#define BOOTLOADER_H

#include <QWidget>

class Bootloader : public QWidget
{
    Q_OBJECT

public:
    explicit Bootloader(QWidget *parent = nullptr);
    ~Bootloader();

protected:
    void keyPressEvent(QKeyEvent *event) override;

private:
    void simulateBootProcess();
    void startOS();
};

#endif // BOOTLOADER_H

// main.cpp
#include "bootloader.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    Bootloader bootloader;
    bootloader.show();

    return a.exec();
}

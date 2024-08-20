// main.cpp
/*
 * Main
 *
 * Maintainer: Park Jiwoo
 *
 * Copyright (C) 2024 Park-Jiwoo
 *
 */
#include "bootloader.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    Bootloader bootloader;
    bootloader.show();

    return a.exec();
}

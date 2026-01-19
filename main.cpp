#include "mainwindow.h"

#include <QApplication>
#include <QDebug>
#include <QFile>
#include <iostream>

// Windows-specific console allocation for debugging
#ifdef _WIN32
#ifndef NOMINMAX
#define NOMINMAX
#endif
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#endif

int main(int argc, char *argv[])
{
#ifdef _WIN32
    // Allocate a console for debug output (Windows only)
    AllocConsole();
    FILE* fp;
    freopen_s(&fp, "CONOUT$", "w", stdout);
    freopen_s(&fp, "CONOUT$", "w", stderr);
#endif
    
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();
}

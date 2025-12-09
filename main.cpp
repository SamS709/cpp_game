#include "mainwindow.h"

#include <QApplication>
#include <QDebug>
#include <QFile>
#include <iostream>
#include <windows.h>

int main(int argc, char *argv[])
{
    // Allocate a console for debug output
    AllocConsole();
    FILE* fp;
    freopen_s(&fp, "CONOUT$", "w", stdout);
    freopen_s(&fp, "CONOUT$", "w", stderr);

    
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();
}

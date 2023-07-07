#include "mainwindow.h"
#include "register.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow mainWindow;
    mainWindow.show();
    Register registerMenu;
    registerMenu.show();
    return a.exec();
}

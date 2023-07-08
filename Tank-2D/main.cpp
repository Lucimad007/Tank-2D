#include "mainwindow.h"
#include "register.h"
#include <QApplication>
#include <QDir>
Register* registerMenu;
int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
//    MainWindow mainWindow;
//    mainWindow.show();
    registerMenu = new Register();
    registerMenu->show();

    return app.exec();
}

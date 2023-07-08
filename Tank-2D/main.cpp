#include "mainwindow.h"
#include "register.h"
#include "game.h"
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
    Game game;
    game.show();

    return app.exec();
}

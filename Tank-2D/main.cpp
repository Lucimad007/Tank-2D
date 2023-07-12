#include "mainwindow.h"
#include "register.h"
#include "game.h"
#include <QApplication>
#include <QDir>
#include <QTimer>
QTimer *timer;
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
    timer = new QTimer();
    QTimer::connect(timer, &QTimer::timeout, [&](){
        game.clear();
        game.updateLogic();
        game.render();
    });
    timer->start(((float)1/game.getFPS() * 1000));

    return app.exec();
}

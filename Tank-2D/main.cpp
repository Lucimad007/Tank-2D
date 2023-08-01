#include "register.h"
#include "game.h"
#include <QApplication>
#include <QDir>
#include <QTimer>

Register* registerMenu;
QApplication* app;

int main(int argc, char *argv[])
{
    srand(NULL);    //NULL IS DEFINED ZERO
    app = new QApplication(argc, argv);
    registerMenu = new Register();
    registerMenu->show();

    return app->exec();
}

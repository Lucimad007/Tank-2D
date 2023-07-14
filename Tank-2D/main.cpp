#include "register.h"
#include "game.h"
#include <QApplication>
#include <QDir>
#include <QTimer>

Register* registerMenu;

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    registerMenu = new Register();
    registerMenu->show();

    return app.exec();
}

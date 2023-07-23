#include "construction.h"
#include "ui_construction.h"
#include <QDir>

Construction::Construction(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::construction)
{
    ui->setupUi(this);
    for(int i = 0; i < 25; i++)
        for(int j = 0; j < 20; j++)
            blocks[i][j] = 'X';
    loadIcon();
    this->setWindowTitle("Tank Battle City");
    this->setFixedSize(QSize(this->width(), this->height()));
}

void Construction::loadIcon(){
    QFileInfo info = QFileInfo(QDir::currentPath());
    QString path = info.dir().path();
    path += "/Tank-2D/Arts/signUpBack.png";
    QIcon icon(path);
    if(!icon.isNull())
    {
        this->setWindowIcon(icon);
    } else
    {
        qDebug() << "Icon not found.";
    }
}

Construction::~Construction()
{
    delete ui;
}

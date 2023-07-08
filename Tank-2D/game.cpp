#include "game.h"
#include "ui_game.h"
#include <QPixmap>
#include <QGraphicsPixmapItem>
#include <QDir>

Game::Game(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Game)
{
    ui->setupUi(this);
    this->setWindowTitle("Tank Battle City");
    loadIcon();
    this->setFixedSize(this->width(), this->height());
    scene = new QGraphicsScene();
    ui->backgroundView->setScene(scene);
    //we should ensure that scene fills the whole QGraphicsView
    scene->setSceneRect(ui->backgroundView->rect());

    //disable scrollbars if you don't want them to appear
    ui->backgroundView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    ui->backgroundView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

//    GameObject tank1("armored-tank-up.png", 0, 0, 1, 1);
//    GameObject tank2("armored-random-tank-down.png", 200, 50, 1, 1);
//    objects.push_back(tank1);
//    objects.push_back(tank2);
//    render();
}

void Game::loadIcon(){
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

void Game::clear(){
    scene->clear();
    scene->update();
}

void Game::render(){
    for(auto it = objects.begin(); it != objects.end(); ++it)
    {
        QIcon sprite(it->getSpritePath());
        qDebug() << it->getSpritePath();
        QPixmap pixmap = sprite.pixmap(ui->backgroundView->width(), ui->backgroundView->height());
        QGraphicsPixmapItem* test = new QGraphicsPixmapItem(pixmap);
        test->setPos(it->getX(), it->getY());
        scene->addItem(test);
    }
    scene->update();
}

Game::~Game()
{
    delete ui;
    delete scene;
}

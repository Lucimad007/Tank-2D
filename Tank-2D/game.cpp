#include "game.h"
#include "ui_game.h"
#include <QPixmap>
#include <QGraphicsPixmapItem>
#include <QDir>
#include <QKeyEvent>

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

    GameObject tank1(ARMORED_TANK, "yellow-tank-up.png", 0, 0, 1, 1);
    GameObject tank2(ARMORED_RANDOM_TANK, "armored-random-tank-down.png", 200, 50, 1, 1);
    tanks.push_back(tank2);
    player = tank1;
    render();
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

void Game::updateLogic(){

}

void Game::render(){
    //rendering player
    QImage sprite(player.getSpritePath());
    QPixmap pixmap = QPixmap::fromImage(sprite);
    pixmap = pixmap.scaled(QSize(player.getWIDTH(), player.getHEIGHT()));
    QGraphicsPixmapItem* item = new QGraphicsPixmapItem(pixmap);

    item->setPos(player.getX(), player.getY());
    scene->addItem(item);

    //rendering tanks
    for(auto it = tanks.begin(); it != tanks.end(); ++it)
    {
        QImage sprite(it->getSpritePath());
        qDebug() << it->getSpritePath();
        QPixmap pixmap =  QPixmap::fromImage(sprite);
        pixmap = pixmap.scaled(QSize(player.getWIDTH(), player.getHEIGHT()));
        QGraphicsPixmapItem* item = new QGraphicsPixmapItem(pixmap);
        item->setPos(it->getX(), it->getY());
        scene->addItem(item);
    }


    scene->update();
}

void Game::keyPressEvent(QKeyEvent* event){
    if (event->key() == Qt::Key_A)
    {
        player.setX(player.getX() - 2);
        player.setSpritePath("yellow-tank-left.png");
    } else if (event->key() == Qt::Key_D)
    {
        player.setX(player.getX() + 2);
        player.setSpritePath("yellow-tank-right.png");
    } else if (event->key() == Qt::Key_W)
    {
        player.setY(player.getY() - 2);
        player.setSpritePath("yellow-tank-up.png");
    } else if (event->key() == Qt::Key_S)
    {
        player.setY(player.getY() + 2);
        player.setSpritePath("yellow-tank-down.png");
    }


    // Call the base class implementation
    QWidget::keyPressEvent(event);
}

int Game::getFPS() const
{
    return FPS;
}

Game::~Game()
{
    delete ui;
    delete scene;
}

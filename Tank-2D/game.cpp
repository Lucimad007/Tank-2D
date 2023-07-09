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

    GameObject tank1(PLAYER, "yellow-tank-up.png", 0, 0, 1, 1);
    GameObject tank2(ARMORED_RANDOM_TANK, "armored-random-tank-down.png", 200, 50, 1, 1);
    GameObject tank3(ARMORED_TANK, "common-tank-up.png", 350, 50, 1, 1);
    tanks.push_front(tank2);
    tanks.push_back(tank3);
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
    //random movements of tanks
    QRect before, after;
    for(auto it = tanks.begin(); it != tanks.end(); ++it)
    {
        before = QRect(it->getX(), it->getY(), it->getWIDTH(), it->getHEIGHT());

        it->counter++;
        if(it->counter % it->steps == 0)
            it->randomNumber = rand();

        if(it->randomNumber % 4 == 0)
        {
            after = QRect(it->getX() + 4, it->getY(), it->getWIDTH(), it->getHEIGHT());
            it->setDirection(RIGHT);
            if(!haveCollision(before, after))
                it->setX(it->getX() + 4);
        } else if(it->randomNumber % 4 == 1)
        {
            after = QRect(it->getX() - 4, it->getY(), it->getWIDTH(), it->getHEIGHT());
            it->setDirection(LEFT);
            if(!haveCollision(before, after))
                it->setX(it->getX() - 4);
        } else if(it->randomNumber % 4 == 2)
        {
            after = QRect(it->getX(), it->getY() + 4, it->getWIDTH(), it->getHEIGHT());
            it->setDirection(DOWN);
            if(!haveCollision(before, after))
                it->setY(it->getY() + 4);

        } else if(it->randomNumber % 4 == 3)
        {
            after = QRect(it->getX(), it->getY() - 4, it->getWIDTH(), it->getHEIGHT());
            it->setDirection(UP);
            if(!haveCollision(before, after))
                it->setY(it->getY() - 4);
        }
    }

    //updating sprite paths
    for(auto it = tanks.begin(); it != tanks.end(); ++it)
    {
        if(it->getType() == COMMON_TANK)
        {
            if(it->getDirection() == UP)
                it->setSpritePath("common-tank-up.png");
            else if(it->getDirection() == DOWN)
                it->setSpritePath("common-tank-down.png");
            else if(it->getDirection() == LEFT)
                it->setSpritePath("common-tank-left.png");
            else if(it->getDirection() == RIGHT)
                it->setSpritePath("common-tank-right.png");
        } else if(it->getType() == ARMORED_TANK)
        {
            if(it->getDirection() == UP)
                it->setSpritePath("armored-tank-up.png");
            else if(it->getDirection() == DOWN)
                it->setSpritePath("armored-tank-down.png");
            else if(it->getDirection() == LEFT)
                it->setSpritePath("armored-tank-left.png");
            else if(it->getDirection() == RIGHT)
                it->setSpritePath("armored-tank-right.png");
        } else if(it->getType() == RANDOM_TANK)
        {
            if(it->getDirection() == UP)
                it->setSpritePath("random-tank-up.png");
            else if(it->getDirection() == DOWN)
                it->setSpritePath("random-tank-down.png");
            else if(it->getDirection() == LEFT)
                it->setSpritePath("random-tank-left.png");
            else if(it->getDirection() == RIGHT)
                it->setSpritePath("random-tank-right.png");
        } else if(it->getType() == ARMORED_RANDOM_TANK)
        {
            if(it->getDirection() == UP)
                it->setSpritePath("armored-random-tank-up.png");
            else if(it->getDirection() == DOWN)
                it->setSpritePath("armored-random-tank-down.png");
            else if(it->getDirection() == LEFT)
                it->setSpritePath("armored-random-tank-left.png");
            else if(it->getDirection() == RIGHT)
                it->setSpritePath("armored-random-tank-right.png");
        }
    }

    updateHitBoxes();

    limitObjects();     //we should ensure that none of the game objects are out of the scene
}

void Game::limitObjects(){
    //restricting tanks
    for(auto it = tanks.begin(); it != tanks.end(); ++it)
    {
        //left
        if(it->getX() < 0)
            it->setX(0);
        //right
        if(this->width() - it->getWIDTH() < it->getX())
            it->setX(this->width() - it->getWIDTH());
        //up
        if(it->getY() < 0)
            it->setY(0);
        //down
        if(this->height() - it->getHEIGHT() < it->getY())
            it->setY(this->height() - it->getHEIGHT());
    }

    //restricting player
    //left
    if(player.getX() < 0)
        player.setX(0);
    //right
    if(this->width() - player.getWIDTH() < player.getX())
        player.setX(this->width() - player.getWIDTH());
    //up
    if(player.getY() < 0)
        player.setY(0);
    //down
    if(this->height() - player.getHEIGHT() < player.getY())
        player.setY(this->height() - player.getHEIGHT());
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

void Game::updateHitBoxes(){
    hitBoxes.clear();
    for(auto it = tanks.begin(); it != tanks.end(); ++it)
    {
        hitBoxes.push_back(QRect(it->getX() , it->getY() , it->getWIDTH(), it->getHEIGHT()));
    }
}

bool Game::haveCollision(QRect before, QRect after){
    for(auto it = hitBoxes.begin(); it!= hitBoxes.end(); ++it)
    {
        if(before == *it)   //we should not compare the object to itself
            continue;
        if(it->intersects(after))
            return true;
    }

    return false;
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

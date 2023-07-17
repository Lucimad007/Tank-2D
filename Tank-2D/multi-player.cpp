#include <QGraphicsScene>
#include <QGraphicsView>
#include <QDir>
#include "multi-player.h"
#include "ui_multi-player.h"
#include "game-object.h"

MultiPlayer::MultiPlayer(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MultiPlayer)
{
    spriteLoader = new SpriteLoader();
    ui->setupUi(this);
    this->setWindowTitle("Tank Battle City");
    loadIcon();
    this->setFixedSize(this->width(), this->height());
    scene = new QGraphicsScene();
    backgroundView = new QGraphicsView(this);
    backgroundView->setFixedSize(this->size());
    backgroundView->setStyleSheet("background-color: black;");
    backgroundView->setScene(scene);
    //we should ensure that scene fills the whole QGraphicsView
    scene->setSceneRect(backgroundView->rect());

    //disable scrollbars if you don't want them to appear
    backgroundView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    backgroundView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    loadMap();
    render();
}

void MultiPlayer::loadIcon(){
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

void MultiPlayer::loadMap(){

    QString positions[25][20];
    //loading file
    QFileInfo info = QFileInfo(QDir::currentPath());
    QString path = info.dir().path();
    path += "/Tank-2D/levels/multiplayer.txt";
    QFile file(path);
    if(file.exists())
    {
        QString data;
        QByteArray temp;
        file.open(QIODevice::OpenModeFlag::ReadOnly);
        while(!file.atEnd())
        {
           char c;
           file.getChar(&c);
           if((c == '\n') || (c == '\b') || (c == '\r'))
               continue;
           data += c;
        }

        for(int i = 0; i < 20; i++)
            for(int j = 0; j < 25; j++)
            {
                positions[j][i] = data.at(25 * i + j);
            }
    } else
    {
        qDebug() << "File not found.";
        return;
    }
    //end of loading file

    //loading map
    //p : player1 ,P : player2, B : brick, M : stone, W : water, f: flag1, F : flag2, X : nothing
    for(int i = 0; i < 20; i++)
        for(int j = 0; j < 25; j++)
        {
            if(positions[j][i] == 'B')
            {
                GameObject brick(BRICK, spriteLoader->getBrick(), j * cellSize, i * cellSize);
                walls.push_back(brick);
            } else if(positions[j][i] == 'W')
            {
                GameObject water(WATER, spriteLoader->getWater(), j * cellSize, i * cellSize);
                walls.push_back(water);
            } else if(positions[j][i] == 'M')
            {
                GameObject stone(STONE, spriteLoader->getStone(), j * cellSize, i * cellSize);
                walls.push_back(stone);
            } else if(positions[j][i] == 'f')
            {
                GameObject flag(FLAG, spriteLoader->getFlag(), j * cellSize, i * cellSize);
                this->flag1 = flag;
            } else if(positions[j][i] == 'F')
            {
                GameObject flag(FLAG, spriteLoader->getFlag(), j * cellSize, i * cellSize);
                this->flag2 = flag;

            } else if(positions[j][i] == 'p')
            {
                GameObject player(PLAYER, spriteLoader->getYellow_tank_up(), j * cellSize, i * cellSize, DOWN);
                this->player1 = player;
            } else if(positions[j][i] == 'P')
            {
                GameObject player(PLAYER, spriteLoader->getYellow_tank_down(), j * cellSize, i * cellSize, DOWN);
                this->player2 = player;
            }
        }
    //end of loading map
}

void MultiPlayer::render(){

    //rendering walls
    for(auto it = walls.begin(); it != walls.end(); ++it)
    {
        QPixmap sprite(it->getSprite());
        QGraphicsPixmapItem* item = new QGraphicsPixmapItem(sprite);
        item->setPos(it->getX(), it->getY());
        scene->addItem(item);
    }

    //rendering missiles
    for(auto it = player1Missiles.begin(); it != player1Missiles.end(); ++it)
    {
        QPixmap sprite(it->getSprite());
        sprite = sprite.scaled(GameObject::getSMALL_WIDTH(), GameObject::getSMALL_HEIGHT());    //making missiles small
        QGraphicsPixmapItem* item = new QGraphicsPixmapItem(sprite);
        item->setPos(it->getX(), it->getY());
        scene->addItem(item);
    }
    for(auto it = player2Missiles.begin(); it != player2Missiles.end(); ++it)
    {
        QPixmap sprite(it->getSprite());
        sprite = sprite.scaled(GameObject::getSMALL_WIDTH(), GameObject::getSMALL_HEIGHT());    //making missiles small
        QGraphicsPixmapItem* item = new QGraphicsPixmapItem(sprite);
        item->setPos(it->getX(), it->getY());
        scene->addItem(item);
    }


    //flags
    QPixmap sprite1(flag1.getSprite());
    QGraphicsPixmapItem* flagItem1 = new QGraphicsPixmapItem(sprite1);
    flagItem1->setPos(flag1.getX(), flag1.getY());
    scene->addItem(flagItem1);

    QPixmap sprite2(flag2.getSprite());
    QGraphicsPixmapItem* flagItem2 = new QGraphicsPixmapItem(sprite2);
    flagItem2->setPos(flag2.getX(), flag2.getY());
    scene->addItem(flagItem2);

    //rendering players
    QPixmap spriteP1(player1.getSprite());
    QGraphicsPixmapItem* flagItemP1 = new QGraphicsPixmapItem(spriteP1);
    flagItemP1->setPos(player1.getX(), player1.getY());
    scene->addItem(flagItemP1);

    QPixmap spriteP2(player2.getSprite());
    QGraphicsPixmapItem* flagItemP2 = new QGraphicsPixmapItem(spriteP2);
    flagItemP2->setPos(player2.getX(), player2.getY());
    scene->addItem(flagItemP2);

    scene->update();
}

MultiPlayer::~MultiPlayer()
{
    delete ui;
}

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
    backgroundView->setFixedSize(QSize(WIDTH, HEIGHT));
    backgroundView->setStyleSheet("background-color: black;");
    backgroundView->setScene(scene);
    //we should ensure that scene fills the whole QGraphicsView
    scene->setSceneRect(backgroundView->rect());

    //disable scrollbars if you don't want them to appear
    backgroundView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    backgroundView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    loadMap();
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
                GameObject player(PLAYER, spriteLoader->getYellow_tank_up(), j * cellSize, i * cellSize, UP);
                this->player1 = player;
            } else if(positions[j][i] == 'P')
            {
                GameObject player(PLAYER, spriteLoader->getYellow_tank_down(), j * cellSize, i * cellSize, DOWN);
                this->player2 = player;
            }
        }
    //end of loading map
}

void MultiPlayer::clear(){
    scene->clear();
    scene->update();
}

void MultiPlayer::updateLogic(){
    //moving players
    movePlayers();

    //moving missiles
    moveMissiles();     //put this function first because early missiles shouldn't move

    //delete game objects which have 0 health
    deleteDeadObjects();

    deleteJunkMissiles();

    updateHitBoxes();

    limitObjects();     //we should ensure that none of the game objects are out of the scene

    detectMissileCollision();

    if(player1.counter)          //for restricting number of missiles being shot
        player1.counter--;
    if(player2.counter)          //for restricting number of missiles being shot
        player2.counter--;

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

void MultiPlayer::movePlayers(){
    //player1
    if(player1Direction == MOVE_UP)
    {
        QRect before, after;
        before = QRect(player1.getX(), player1.getY(), player1.getWIDTH(), player1.getHEIGHT());
        after = QRect(player1.getX(), player1.getY() - player1.getSpeed(), player1.getWIDTH(), player1.getHEIGHT());
        player1.setDirection(UP);
        player1.setSprite(spriteLoader->getYellow_tank_up());
        if(!haveCollision(before, after))
            player1.setY(player1.getY() - player1.getSpeed());
    } else if(player1Direction == MOVE_DOWN)
    {
        QRect before, after;
        before = QRect(player1.getX(), player1.getY(), player1.getWIDTH(), player1.getHEIGHT());
        after = QRect(player1.getX(), player1.getY() + player1.getSpeed(), player1.getWIDTH(), player1.getHEIGHT());
        player1.setDirection(DOWN);
        player1.setSprite(spriteLoader->getYellow_tank_down());
        if(!haveCollision(before, after))
            player1.setY(player1.getY() + player1.getSpeed());
    } else if(player1Direction == MOVE_LEFT){
        QRect before, after;
        before = QRect(player1.getX(), player1.getY(), player1.getWIDTH(), player1.getHEIGHT());
        after = QRect(player1.getX() - player1.getSpeed(), player1.getY(), player1.getWIDTH(), player1.getHEIGHT());
        player1.setDirection(LEFT);
        player1.setSprite(spriteLoader->getYellow_tank_left());
        if(!haveCollision(before, after))
            player1.setX(player1.getX() - player1.getSpeed());
    } else if(player1Direction == MOVE_RIGHT)
    {
        QRect before, after;
        before = QRect(player1.getX(), player1.getY(), player1.getWIDTH(), player1.getHEIGHT());
        after = QRect(player1.getX() + player1.getSpeed(), player1.getY(), player1.getWIDTH(), player1.getHEIGHT());
        player1.setDirection(RIGHT);
        player1.setSprite(spriteLoader->getYellow_tank_right());
        if(!haveCollision(before, after))
            player1.setX(player1.getX() + player1.getSpeed());
    }

    //player2
    if(player2Direction == MOVE_UP)
    {
        QRect before, after;
        before = QRect(player2.getX(), player2.getY(), player2.getWIDTH(), player2.getHEIGHT());
        after = QRect(player2.getX(), player2.getY() - player2.getSpeed(), player2.getWIDTH(), player2.getHEIGHT());
        player2.setDirection(UP);
        player2.setSprite(spriteLoader->getYellow_tank_up());
        if(!haveCollision(before, after))
            player2.setY(player2.getY() - player2.getSpeed());
    } else if(player2Direction == MOVE_DOWN)
    {
        QRect before, after;
        before = QRect(player2.getX(), player2.getY(), player2.getWIDTH(), player2.getHEIGHT());
        after = QRect(player2.getX(), player2.getY() + player2.getSpeed(), player2.getWIDTH(), player2.getHEIGHT());
        player2.setDirection(DOWN);
        player2.setSprite(spriteLoader->getYellow_tank_down());
        if(!haveCollision(before, after))
            player2.setY(player2.getY() + player2.getSpeed());
    } else if(player2Direction == MOVE_LEFT){
        QRect before, after;
        before = QRect(player2.getX(), player2.getY(), player2.getWIDTH(), player2.getHEIGHT());
        after = QRect(player2.getX() - player2.getSpeed(), player2.getY(), player2.getWIDTH(), player2.getHEIGHT());
        player2.setDirection(LEFT);
        player2.setSprite(spriteLoader->getYellow_tank_left());
        if(!haveCollision(before, after))
            player2.setX(player2.getX() - player2.getSpeed());
    } else if(player2Direction == MOVE_RIGHT)
    {
        QRect before, after;
        before = QRect(player2.getX(), player2.getY(), player2.getWIDTH(), player2.getHEIGHT());
        after = QRect(player2.getX() + player2.getSpeed(), player2.getY(), player2.getWIDTH(), player2.getHEIGHT());
        player2.setDirection(RIGHT);
        player2.setSprite(spriteLoader->getYellow_tank_right());
        if(!haveCollision(before, after))
            player2.setX(player2.getX() + player2.getSpeed());
    }
}

void MultiPlayer::keyPressEvent(QKeyEvent* event){

    if (event->key() == Qt::Key_A)
    {
        player1Direction = MOVE_LEFT;
    } else if (event->key() == Qt::Key_D)
    {
        player1Direction = MOVE_RIGHT;
    } else if (event->key() == Qt::Key_W)
    {
        player1Direction = MOVE_UP;
    } else if (event->key() == Qt::Key_S)
    {
        player1Direction = MOVE_DOWN;
    } else if ((event->key() == Qt::Key_T) && (player1.counter == 0))
    {
        player1.counter += 10;    //for restricting number of missiles being shot
        if(player1.getDirection() == UP)
        {
            GameObject missile(MISSILE, spriteLoader->getMissile_up(), player1.getX() + GameObject::getSMALL_WIDTH()/2 ,player1.getY(), UP);
            player1Missiles.push_back(missile);
        } else if(player1.getDirection() == DOWN)
        {
            GameObject missile(MISSILE, spriteLoader->getMissile_down(), player1.getX() + GameObject::getSMALL_WIDTH()/2, player1.getY(), DOWN);
            player1Missiles.push_back(missile);
        } else if(player1.getDirection() == RIGHT)
        {
            GameObject missile(MISSILE, spriteLoader->getMissile_right(), player1.getX(),player1.getY() + GameObject::getSMALL_HEIGHT()/2, RIGHT);
            player1Missiles.push_back(missile);
        } else if(player1.getDirection() == LEFT)
        {
            GameObject missile(MISSILE, spriteLoader->getMissile_left(), player1.getX(), player1.getY() + GameObject::getSMALL_HEIGHT()/2, LEFT);
            player1Missiles.push_back(missile);
        }
    } else if (event->key() == Qt::Key_4)
    {
        player2Direction = MOVE_LEFT;
    } else if (event->key() == Qt::Key_6)
    {
        player2Direction = MOVE_RIGHT;
    } else if (event->key() == Qt::Key_8)
    {
        player2Direction = MOVE_UP;
    } else if (event->key() == Qt::Key_5)
    {
        player2Direction = MOVE_DOWN;
    } else if ((event->key() == Qt::Key_P) && (player2.counter == 0))
    {
        player2.counter += 10;    //for restricting number of missiles being shot
        if(player2.getDirection() == UP)
        {
            GameObject missile(MISSILE, spriteLoader->getMissile_up(), player2.getX() + GameObject::getSMALL_WIDTH()/2 ,player2.getY(), UP);
            player2Missiles.push_back(missile);
        } else if(player2.getDirection() == DOWN)
        {
            GameObject missile(MISSILE, spriteLoader->getMissile_down(), player2.getX() + GameObject::getSMALL_WIDTH()/2, player2.getY(), DOWN);
            player2Missiles.push_back(missile);
        } else if(player2.getDirection() == RIGHT)
        {
            GameObject missile(MISSILE, spriteLoader->getMissile_right(), player2.getX(),player2.getY() + GameObject::getSMALL_HEIGHT()/2, RIGHT);
            player2Missiles.push_back(missile);
        } else if(player2.getDirection() == LEFT)
        {
            GameObject missile(MISSILE, spriteLoader->getMissile_left(), player2.getX(), player2.getY() + GameObject::getSMALL_HEIGHT()/2, LEFT);
            player2Missiles.push_back(missile);
        }
    }
    // Call the base class implementation
    QWidget::keyPressEvent(event);
}

void MultiPlayer::keyReleaseEvent(QKeyEvent* event){
    //Note : second conditions are for preventing player movement from delay which is caused by pressing and releasing
    // keys approximately simultaneous

    if(event->key() == Qt::Key_A && player1.getDirection() == LEFT)
        player1Direction = NONE;
    else if(event->key() == Qt::Key_S && player1.getDirection() == DOWN)
        player1Direction = NONE;
    else if(event->key() == Qt::Key_D && player1.getDirection() == RIGHT)
        player1Direction = NONE;
    else if(event->key() == Qt::Key_W && player1.getDirection() == UP)
        player1Direction = NONE;

    if(event->key() == Qt::Key_4 && player2.getDirection() == LEFT)
        player2Direction = NONE;
    else if(event->key() == Qt::Key_5 && player2.getDirection() == DOWN)
        player2Direction = NONE;
    else if(event->key() == Qt::Key_6 && player2.getDirection() == RIGHT)
        player2Direction = NONE;
    else if(event->key() == Qt::Key_8 && player2.getDirection() == UP)
        player2Direction = NONE;

    // Call the base class implementation
    QWidget::keyReleaseEvent(event);
}

void MultiPlayer::updateHitBoxes(){

    //hitboxes of walls
    for(auto it = walls.begin(); it != walls.end(); ++it)
    {
        it->setHitbox(QRect(it->getX() , it->getY() , it->getWIDTH(), it->getHEIGHT()));
    }

    //hitboxes of the players
    player1.setHitbox(QRect(player1.getX() , player1.getY() , player1.getWIDTH(), player1.getHEIGHT()));
    player2.setHitbox(QRect(player2.getX() , player2.getY() , player2.getWIDTH(), player2.getHEIGHT()));

    //hitboxes of the missiles
    for(auto it = player1Missiles.begin(); it != player1Missiles.end(); ++it)
    {
        it->setHitbox(QRect(it->getX() , it->getY() , it->getSMALL_WIDTH(), it->getSMALL_HEIGHT()));
    }
    for(auto it = player2Missiles.begin(); it != player2Missiles.end(); ++it)
    {
        it->setHitbox(QRect(it->getX() , it->getY() , it->getSMALL_WIDTH(), it->getSMALL_HEIGHT()));
    }
}

void MultiPlayer::limitObjects(){

    //restricting players
    //left
    if(player1.getX() < 0)
        player1.setX(0);
    if(player2.getX() < 0)
        player2.setX(0);
    //right
    if(WIDTH - player1.getWIDTH() < player1.getX())
        player1.setX(WIDTH - player1.getWIDTH());
    if(WIDTH - player2.getWIDTH() < player2.getX())
        player2.setX(WIDTH - player2.getWIDTH());
    //up
    if(player1.getY() < 0)
        player1.setY(0);
    if(player2.getY() < 0)
        player2.setY(0);
    //down
    if(HEIGHT - player1.getHEIGHT() < player1.getY())
        player1.setY(HEIGHT - player1.getHEIGHT());
    if(HEIGHT - player2.getHEIGHT() < player2.getY())
        player2.setY(HEIGHT - player2.getHEIGHT());
}

void MultiPlayer::deleteJunkMissiles(){
    for(auto it = player1Missiles.begin(); it != player1Missiles.end(); it++)
    {
        if(it->getX() > WIDTH)
            player1Missiles.erase(it);
        else if(it->getX() + it->getSMALL_WIDTH() < 0)
            player1Missiles.erase(it);
        else if(it->getY() + it->getSMALL_HEIGHT() < 0)
            player1Missiles.erase(it);
        else if(it->getY() > HEIGHT)
            player1Missiles.erase(it);
    }
    for(auto it = player2Missiles.begin(); it != player2Missiles.end(); it++)
    {
        if(it->getX() > WIDTH)
            player2Missiles.erase(it);
        else if(it->getX() + it->getSMALL_WIDTH() < 0)
            player2Missiles.erase(it);
        else if(it->getY() + it->getSMALL_HEIGHT() < 0)
            player2Missiles.erase(it);
        else if(it->getY() > HEIGHT)
            player2Missiles.erase(it);
    }
}

void MultiPlayer::deleteDeadObjects(){

    //walls
    for(auto it = walls.begin(); it != walls.end(); ++it)
    {
        if(it->getType() == STONE)  //we can not destroy stone in game
            continue;
        if(it->getHealth() <= 0)
            walls.erase(it);
    }

    //players
    if(player1.getHealth() <= 0)
        qDebug() << "Player 2 Won!";
    if(player2.getHealth() <= 0)
        qDebug() << "Player 1 Won!";

    //flags
    if(flag1.getHealth() <= 0)
        qDebug() << "Player 2 Won!";
    if(flag2.getHealth() <= 0)
        qDebug() << "Player 1 Won!";
}

void MultiPlayer::moveMissiles(){
    for(auto it = player1Missiles.begin(); it != player1Missiles.end(); ++it)
    {
        if(it->getDirection() == UP)
            it->setY(it->getY() - it->getSpeed());
        else if(it->getDirection() == DOWN)
            it->setY(it->getY() + it->getSpeed());
        else if(it->getDirection() == RIGHT)
            it->setX(it->getX() + it->getSpeed());
        else if(it->getDirection() == LEFT)
            it->setX(it->getX() - it->getSpeed());
    }

    for(auto it = player2Missiles.begin(); it != player2Missiles.end(); ++it)
    {
        if(it->getDirection() == UP)
            it->setY(it->getY() - it->getSpeed());
        else if(it->getDirection() == DOWN)
            it->setY(it->getY() + it->getSpeed());
        else if(it->getDirection() == RIGHT)
            it->setX(it->getX() + it->getSpeed());
        else if(it->getDirection() == LEFT)
            it->setX(it->getX() - it->getSpeed());
    }
}

bool MultiPlayer::haveCollision(QRect before, QRect after){

    //walls
    for(auto it = walls.begin(); it!= walls.end(); ++it)
    {
        if(before == it->getHitbox())   //we should not compare the object to itself
            continue;
        if(it->getHitbox().intersects(after))
            return true;
    }

    //players
    if(player1.getHitbox() != before)
        if(player1.getHitbox().intersects(after))
            return true;

    if(player2.getHitbox() != before)
        if(player2.getHitbox().intersects(after))
            return true;

    //flags
    if(flag1.getHitbox() != before)
        if(flag1.getHitbox().intersects(after))
            return true;

    if(flag2.getHitbox() != before)
        if(flag2.getHitbox().intersects(after))
            return true;


    return false;
}

void MultiPlayer::detectMissileCollision()
{
    //walls and missiles
    for(auto it = walls.begin(); it != walls.end(); ++it)
    {
        for(auto itm = player1Missiles.begin(); itm != player1Missiles.end(); ++itm)
        {
            if(it->getHitbox().intersects(itm->getHitbox()))
            {
                //delete objects (not hitboxes only)
                //do sth cool
                it->setHealth(it->getHealth() - itm->getDamage());
                player1Missiles.erase(itm);
            }
        }
    }
    for(auto it = walls.begin(); it != walls.end(); ++it)
    {
        for(auto itm = player2Missiles.begin(); itm != player2Missiles.end(); ++itm)
        {
            if(it->getHitbox().intersects(itm->getHitbox()))
            {
                //delete objects (not hitboxes only)
                //do sth cool
                it->setHealth(it->getHealth() - itm->getDamage());      // - 1
                player2Missiles.erase(itm);
            }
        }
    }

    //players and missiles
    for(auto it = player1Missiles.begin(); it != player1Missiles.end(); ++it)
    {
        if(player2.getHitbox().intersects(it->getHitbox()))
        {
            player1Missiles.erase(it);
            player2.setHealth(player2.getHealth() - it->getDamage());
            player2.setX(player2.getRespawnX());
            player2.setY(player2.getRespawnY());
            player2.setDamage(1);    //resetting to default damage
        }
    }
    for(auto it = player2Missiles.begin(); it != player2Missiles.end(); ++it)
    {
        if(player1.getHitbox().intersects(it->getHitbox()))
        {
            player2Missiles.erase(it);
            player1.setHealth(player1.getHealth() - it->getDamage());
            player1.setX(player1.getRespawnX());
            player1.setY(player1.getRespawnY());
            player1.setDamage(1);    //resetting to default damage
        }
    }

    //flags and missiles
    for(auto it = player1Missiles.begin(); it != player1Missiles.end(); ++it)
    {
        if(flag1.getHitbox().intersects(it->getHitbox()))
        {
            player1Missiles.erase(it);
            flag1.setHealth(flag1.getHealth() - it->getDamage());
            flag1.setX(-1000);   //making it disapear
            flag1.setY(-1000);
        }
        else if(flag2.getHitbox().intersects(it->getHitbox()))
        {
            player1Missiles.erase(it);
            flag2.setHealth(flag2.getHealth() - it->getDamage());
            flag2.setX(-1000);   //making it disapear
            flag2.setY(-1000);
        }
    }
    for(auto it = player2Missiles.begin(); it != player2Missiles.end(); ++it)
    {
        if(flag1.getHitbox().intersects(it->getHitbox()))
        {
            player2Missiles.erase(it);
            flag1.setHealth(flag1.getHealth() - it->getDamage());
            flag1.setX(-1000);   //making it disapear
            flag1.setY(-1000);
        }
        else if(flag2.getHitbox().intersects(it->getHitbox()))
        {
            player2Missiles.erase(it);
            flag2.setHealth(flag2.getHealth() - it->getDamage());
            flag2.setX(-1000);   //making it disapear
            flag2.setY(-1000);
        }
    }
}

MultiPlayer::~MultiPlayer()
{
    delete ui;
}

int MultiPlayer::getFPS() const
{
    return FPS;
}

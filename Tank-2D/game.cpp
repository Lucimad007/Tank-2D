#include "game.h"
#include "ui_game.h"
#include "sprite-loader.h"
#include <QPixmap>
#include <QGraphicsPixmapItem>
#include <QDir>
#include <QKeyEvent>

Game::Game(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Game)
{
    spriteLoader = new SpriteLoader();
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

    loadLevel(1);
}

void Game::loadLevel(int level){

    QString number = QString::number(level);
    QString positions[25][20];
    //loading file
    QFileInfo info = QFileInfo(QDir::currentPath());
    QString path = info.dir().path();
    path += "/Tank-2D/levels/level" + number  + ".txt";
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
    for(int i = 0; i < 20; i++)
        for(int j = 0; j < 25; j++)
        {
            if(positions[j][i] == 'X')
            {
                GameObject brick(BRICK, spriteLoader->getBrick(), j * cellSize, i * cellSize, 1, 0);
                walls.push_back(brick);
            }
        }
    //end of loading map
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
    randomMovementsOfTanks();

    tanksShooting();

    //updating sprites
    updateSprites();

    //moving missiles
    moveMissiles();

    deleteJunkMissiles();

    updateHitBoxes();

    limitObjects();     //we should ensure that none of the game objects are out of the scene

    detectMissileCollision();

    if(player.counter)          //for restricting number of missiles being shot
        player.counter--;

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

void Game::deleteJunkMissiles(){
    for(auto it = missiles.begin(); it != missiles.end(); it++)
    {
        if(it->getX() > this->width())
            missiles.erase(it);
        else if(it->getX() + it->getSMALL_WIDTH() < 0)
            missiles.erase(it);
        else if(it->getY() + it->getSMALL_HEIGHT() < 0)
            missiles.erase(it);
        else if(it->getY() > this->height())
            missiles.erase(it);
    }
}


void Game::detectMissileCollision()
{
    //tanks and missiles
    for(auto it = tanks.begin(); it != tanks.end(); ++it)
    {
        for(auto itm = missiles.begin(); itm != missiles.end(); ++itm)
        {
            if(it->getHitbox().intersects(itm->getHitbox()))
            {
                //delete objects (not hitboxes only)
                //do sth cool
                tanks.erase(it);
                missiles.erase(itm);
            }
        }
    }

    //walls and missiles
    for(auto it = walls.begin(); it != walls.end(); ++it)
    {
        for(auto itm = missiles.begin(); itm != missiles.end(); ++itm)
        {
            if(it->getHitbox().intersects(itm->getHitbox()))
            {
                //delete objects (not hitboxes only)
                //do sth cool
                walls.erase(it);
                missiles.erase(itm);
            }
        }
    }

    //player and missiles
    for(auto it = missiles.begin(); it != missiles.end(); ++it)
    {
        if(player.getHitbox().intersects(it->getHitbox()))
        {
            missiles.erase(it);
            player.setHealth(player.getHealth() - 1);
        }
    }
}

void Game::render(){
    //rendering player
    QGraphicsPixmapItem* item = new QGraphicsPixmapItem(player.getSprite());
    item->setPos(player.getX(), player.getY());
    scene->addItem(item);

    //rendering tanks
    for(auto it = tanks.begin(); it != tanks.end(); ++it)
    {
        QPixmap pixmap(it->getSprite());
        pixmap = pixmap.scaled(QSize(player.getWIDTH(), player.getHEIGHT()));
        QGraphicsPixmapItem* item = new QGraphicsPixmapItem(pixmap);
        item->setPos(it->getX(), it->getY());
        scene->addItem(item);
    }

    //rendering walls
    for(auto it = walls.begin(); it != walls.end(); ++it)
    {
        QPixmap sprite(it->getSprite());
        QGraphicsPixmapItem* item = new QGraphicsPixmapItem(sprite);
        item->setPos(it->getX(), it->getY());
        scene->addItem(item);
    }

    //rendering missiles
    for(auto it = missiles.begin(); it != missiles.end(); ++it)
    {
        QPixmap sprite(it->getSprite());
        sprite = sprite.scaled(GameObject::getSMALL_WIDTH(), GameObject::getSMALL_HEIGHT());    //making missiles small
        QGraphicsPixmapItem* item = new QGraphicsPixmapItem(sprite);
        item->setPos(it->getX(), it->getY());
        scene->addItem(item);
    }

    //bonus
    for(auto it = bonus.begin(); it != bonus.end(); ++it)
    {
        QPixmap sprite(it->getSprite());
        QGraphicsPixmapItem* item = new QGraphicsPixmapItem(sprite);
        item->setPos(it->getX(), it->getY());
        scene->addItem(item);
    }

    //flag
    QPixmap sprite(flag.getSprite());
    QGraphicsPixmapItem* flagItem = new QGraphicsPixmapItem(sprite);
    flagItem->setPos(flag.getX(), flag.getY());
    scene->addItem(flagItem);

    scene->update();
}

void Game::updateHitBoxes(){
    //hitboxes of tanks
    for(auto it = tanks.begin(); it != tanks.end(); ++it)
    {
        it->setHitbox(QRect(it->getX() , it->getY() , it->getWIDTH(), it->getHEIGHT()));
    }

    //hitboxes of walls
    for(auto it = walls.begin(); it != walls.end(); ++it)
    {
        it->setHitbox(QRect(it->getX() , it->getY() , it->getWIDTH(), it->getHEIGHT()));
    }

    //hitbox of the player
    player.setHitbox(QRect(player.getX() , player.getY() , player.getWIDTH(), player.getHEIGHT()));

    //hitboxes of the missiles
    for(auto it = missiles.begin(); it != missiles.end(); ++it)
    {
        it->setHitbox(QRect(it->getX() , it->getY() , it->getSMALL_WIDTH(), it->getSMALL_HEIGHT()));
    }
}

void Game::randomMovementsOfTanks(){
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

}

void Game::updateSprites(){
    for(auto it = tanks.begin(); it != tanks.end(); ++it)
    {
        if(it->getType() == COMMON_TANK)
        {
            if(it->getDirection() == UP)
                it->setSprite(spriteLoader->getCommon_tank_up());
            else if(it->getDirection() == DOWN)
                it->setSprite(spriteLoader->getCommon_tank_down());
            else if(it->getDirection() == LEFT)
                it->setSprite(spriteLoader->getCommon_tank_left());
            else if(it->getDirection() == RIGHT)
                it->setSprite(spriteLoader->getCommon_tank_right());
        } else if(it->getType() == ARMORED_TANK)
        {
            if(it->getDirection() == UP)
                it->setSprite(spriteLoader->getArmored_tank_up());
            else if(it->getDirection() == DOWN)
                it->setSprite(spriteLoader->getArmored_tank_down());
            else if(it->getDirection() == LEFT)
                it->setSprite(spriteLoader->getArmored_tank_left());
            else if(it->getDirection() == RIGHT)
                it->setSprite(spriteLoader->getArmored_tank_right());
        } else if(it->getType() == RANDOM_TANK)
        {
            if(it->getDirection() == UP)
                it->setSprite(spriteLoader->getRandom_tank_up());
            else if(it->getDirection() == DOWN)
                it->setSprite(spriteLoader->getRandom_tank_down());
            else if(it->getDirection() == LEFT)
                it->setSprite(spriteLoader->getRandom_tank_left());
            else if(it->getDirection() == RIGHT)
                it->setSprite(spriteLoader->getRandom_tank_right());
        } else if(it->getType() == ARMORED_RANDOM_TANK)
        {
            if(it->getDirection() == UP)
                it->setSprite(spriteLoader->getArmored_random_tank_up());
            else if(it->getDirection() == DOWN)
                it->setSprite(spriteLoader->getArmored_random_tank_down());
            else if(it->getDirection() == LEFT)
                it->setSprite(spriteLoader->getArmored_random_tank_left());
            else if(it->getDirection() == RIGHT)
                it->setSprite(spriteLoader->getArmored_random_tank_right());
        }
    }
}

void Game::moveMissiles(){
    for(auto it = missiles.begin(); it != missiles.end(); ++it)
    {
        if(it->getDirection() == UP)
            it->setY(it->getY() - 4);
        else if(it->getDirection() == DOWN)
            it->setY(it->getY() + 4);
        else if(it->getDirection() == RIGHT)
            it->setX(it->getX() + 4);
        else if(it->getDirection() == LEFT)
            it->setX(it->getX() - 4);
    }
}

void Game::tanksShooting(){
    for(auto it = tanks.begin(); it != tanks.end(); ++it)
    {
        if(it->counter % FPS == 0)      //every second
        {
            if(it->getDirection() == UP)
            {
                GameObject missile(MISSILE, spriteLoader->getMissile_up(), it->getX() + GameObject::getSMALL_WIDTH()/2 ,it->getY() - GameObject::getSMALL_HEIGHT(), 0, 0, UP);
                missiles.push_back(missile);
            } else if(it->getDirection() == DOWN)
            {
                GameObject missile(MISSILE, spriteLoader->getMissile_down(), it->getX() + GameObject::getSMALL_WIDTH()/2, it->getY() + GameObject::getHEIGHT(), 0, 0, DOWN);
                missiles.push_back(missile);
            } else if(it->getDirection() == RIGHT)
            {
                GameObject missile(MISSILE, spriteLoader->getMissile_right(), it->getX() + GameObject::getWIDTH() , it->getY() + GameObject::getSMALL_HEIGHT()/2, 0, 0, RIGHT);
                missiles.push_back(missile);
            } else if(it->getDirection() == LEFT)
            {
                GameObject missile(MISSILE, spriteLoader->getMissile_left(), it->getX() - GameObject::getSMALL_WIDTH() , it->getY() + GameObject::getSMALL_HEIGHT()/2 , 0, 0, LEFT);
                missiles.push_back(missile);
            }
        }
    }
}

bool Game::haveCollision(QRect before, QRect after){
    //tanks
    for(auto it = tanks.begin(); it!= tanks.end(); ++it)
    {
        if(before == it->getHitbox())   //we should not compare the object to itself
            continue;
        if(it->getHitbox().intersects(after))
            return true;
    }

    //walls
    for(auto it = walls.begin(); it!= walls.end(); ++it)
    {
        if(before == it->getHitbox())   //we should not compare the object to itself
            continue;
        if(it->getHitbox().intersects(after))
            return true;
    }

    //player
    if(player.getHitbox() != before)
        if(player.getHitbox().intersects(after))
            return true;

    return false;
}

void Game::keyPressEvent(QKeyEvent* event){

    if (event->key() == Qt::Key_A)
    {
        QRect before, after;
        before = QRect(player.getX(), player.getY(), player.getWIDTH(), player.getHEIGHT());
        after = QRect(player.getX() - 4, player.getY(), player.getWIDTH(), player.getHEIGHT());
        player.setDirection(LEFT);
        player.setSprite(spriteLoader->getYellow_tank_left());
        if(!haveCollision(before, after))
            player.setX(player.getX() - 4);
    } else if (event->key() == Qt::Key_D)
    {
        QRect before, after;
        before = QRect(player.getX(), player.getY(), player.getWIDTH(), player.getHEIGHT());
        after = QRect(player.getX() + 4, player.getY(), player.getWIDTH(), player.getHEIGHT());
        player.setDirection(RIGHT);
        player.setSprite(spriteLoader->getYellow_tank_right());
        if(!haveCollision(before, after))
            player.setX(player.getX() + 4);
    } else if (event->key() == Qt::Key_W)
    {
        QRect before, after;
        before = QRect(player.getX(), player.getY(), player.getWIDTH(), player.getHEIGHT());
        after = QRect(player.getX(), player.getY() - 4, player.getWIDTH(), player.getHEIGHT());
        player.setDirection(UP);
        player.setSprite(spriteLoader->getYellow_tank_up());
        if(!haveCollision(before, after))
            player.setY(player.getY() - 4);
    } else if (event->key() == Qt::Key_S)
    {
        QRect before, after;
        before = QRect(player.getX(), player.getY(), player.getWIDTH(), player.getHEIGHT());
        after = QRect(player.getX(), player.getY() + 4, player.getWIDTH(), player.getHEIGHT());
        player.setDirection(DOWN);
        player.setSprite(spriteLoader->getYellow_tank_down());
        if(!haveCollision(before, after))
            player.setY(player.getY() + 4);
    } else if ((event->key() == Qt::Key_T) && (player.counter == 0))
    {
        player.counter += 10;    //for restricting number of missiles being shot
        if(player.getDirection() == UP)
        {
            GameObject missile(MISSILE, spriteLoader->getMissile_up(), player.getX() + GameObject::getSMALL_WIDTH()/2 ,player.getY() - GameObject::getSMALL_HEIGHT(), 0, 0, UP);
            missiles.push_back(missile);
        } else if(player.getDirection() == DOWN)
        {
            GameObject missile(MISSILE, spriteLoader->getMissile_down(), player.getX() + GameObject::getSMALL_WIDTH()/2, player.getY() + GameObject::getHEIGHT(), 0, 0, DOWN);
            missiles.push_back(missile);
        } else if(player.getDirection() == RIGHT)
        {
            GameObject missile(MISSILE, spriteLoader->getMissile_right(), player.getX() + GameObject::getWIDTH() ,player.getY() + GameObject::getSMALL_HEIGHT()/2, 0, 0, RIGHT);
            missiles.push_back(missile);
        } else if(player.getDirection() == LEFT)
        {
            GameObject missile(MISSILE, spriteLoader->getMissile_left(), player.getX() - GameObject::getSMALL_WIDTH() ,player.getY() + GameObject::getSMALL_HEIGHT()/2 , 0, 0, LEFT);
            missiles.push_back(missile);
        }
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

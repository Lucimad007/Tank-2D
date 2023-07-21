#include "game.h"
#include "ui_game.h"
#include "sprite-loader.h"
#include "register.h"
#include "user.h"
#include <QPixmap>
#include <QGraphicsPixmapItem>
#include <QDir>
#include <QKeyEvent>
#include <QTimer>
#include <QUiLoader>
#include <QVBoxLayout>
#include <QGraphicsView>
#include <QPushButton>
#include <QLabel>

extern QTimer *timer;
extern Register* registerMenu;

Game::Game(int currentLevel, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Game)
{
    user = registerMenu->getUser();
    this->currentLevel = currentLevel;
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

    loadLevel(currentLevel);
}

void Game::loadLevel(int level){
    if(level >= 11){
        registerMenu->show();
        this->close();
        return;
    }
    remainingTanks = 2 + level * 4;     //tanks out of the scene
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
    //P : player, O : common tank, A : armored tank, c : random tank, C : armored random tank, B : brick, M : stone,
    //W : water, F : flag, X : nothing
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
            } else if(positions[j][i] == 'F')
            {
                GameObject flag(FLAG, spriteLoader->getFlag(), j * cellSize, i * cellSize);
                this->flag = flag;
            } else if(positions[j][i] == 'C')
            {
                GameObject tank(ARMORED_RANDOM_TANK, spriteLoader->getArmored_random_tank_down(), j * cellSize, i * cellSize, DOWN);
                tanks.push_back(tank);
                spawnPoints.push_back(tank.getHitbox());
                numberOfTanks++;
            } else if(positions[j][i] == 'c')
            {
                GameObject tank(RANDOM_TANK, spriteLoader->getRandom_tank_down(), j * cellSize, i * cellSize, DOWN);
                tanks.push_back(tank);
                spawnPoints.push_back(tank.getHitbox());
                numberOfTanks++;
            } else if(positions[j][i] == 'A')
            {
                GameObject tank(ARMORED_TANK, spriteLoader->getArmored_tank_down(), j * cellSize, i * cellSize, DOWN);
                tanks.push_back(tank);
                spawnPoints.push_back(tank.getHitbox());
                numberOfTanks++;
            } else if(positions[j][i] == 'O')
            {
                GameObject tank(COMMON_TANK, spriteLoader->getCommon_tank_down(), j * cellSize, i * cellSize, DOWN);
                tanks.push_back(tank);
                spawnPoints.push_back(tank.getHitbox());
                numberOfTanks++;
            } else if(positions[j][i] == 'P')
            {
                GameObject player(PLAYER, spriteLoader->getYellow_tank_down(), j * cellSize, i * cellSize, DOWN);
                this->player = player;
            }
        }
    //end of loading map

    //loading heart
    QPixmap pixmap = spriteLoader->getHeart();
    pixmap = pixmap.scaled(ui->heartView->width(), ui->heartView->height());
    QGraphicsPixmapItem* item = new QGraphicsPixmapItem(pixmap);
    QGraphicsScene* tempScene = new QGraphicsScene();
    tempScene->addItem(item);
    ui->heartView->setScene(tempScene);
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
    //moving player
    movePlayer();

    //moving missiles
    moveMissiles();     //put this function first because early missiles shouldn't move

    //delete game objects which have 0 health
    deleteDeadObjects();

    //random movements of tanks
    randomMovementsOfTanks();

    tanksShooting();

    //updating sprites
    updateSprites();

    deleteJunkMissiles();

    updateHitBoxes();

    limitObjects();     //we should ensure that none of the game objects are out of the scene

    detectMissileCollision();

    detectBonusCollision();

    spawnTanks();

    checkWinning();

    if(player.counter)          //for restricting number of missiles being shot
        player.counter--;
    if(timeStopCounter)
        timeStopCounter--;

    for(auto it = bonus.begin(); it != bonus.end(); ++it)
    {
        it->counter--;      //managing lifespan of bonus items
    }

}

void Game::limitObjects(){
    //restricting tanks
    for(auto it = tanks.begin(); it != tanks.end(); ++it)
    {
        //left
        if(it->getX() < 0)
            it->setX(0);
        //right
        if(WIDTH - it->getWIDTH() < it->getX())
            it->setX(WIDTH - it->getWIDTH());
        //up
        if(it->getY() < 0)
            it->setY(0);
        //down
        if(HEIGHT - it->getHEIGHT() < it->getY())
            it->setY(HEIGHT - it->getHEIGHT());
    }

    //restricting player
    //left
    if(player.getX() < 0)
        player.setX(0);
    //right
    if(WIDTH - player.getWIDTH() < player.getX())
        player.setX(WIDTH - player.getWIDTH());
    //up
    if(player.getY() < 0)
        player.setY(0);
    //down
    if(HEIGHT - player.getHEIGHT() < player.getY())
        player.setY(HEIGHT - player.getHEIGHT());
}

void Game::deleteJunkMissiles(){
    for(auto it = missiles.begin(); it != missiles.end(); it++)
    {
        if(it->getX() > WIDTH)
            missiles.erase(it);
        else if(it->getX() + it->getSMALL_WIDTH() < 0)
            missiles.erase(it);
        else if(it->getY() + it->getSMALL_HEIGHT() < 0)
            missiles.erase(it);
        else if(it->getY() > HEIGHT)
            missiles.erase(it);
    }
    for(auto it = enemyMissiles.begin(); it != enemyMissiles.end(); it++)
    {
        if(it->getX() > WIDTH)
            enemyMissiles.erase(it);
        else if(it->getX() + it->getSMALL_WIDTH() < 0)
            enemyMissiles.erase(it);
        else if(it->getY() + it->getSMALL_HEIGHT() < 0)
            enemyMissiles.erase(it);
        else if(it->getY() > HEIGHT)
            enemyMissiles.erase(it);
    }
}

void Game::deleteDeadObjects(){
    //tanks
    for(auto it = tanks.begin(); it != tanks.end(); ++it)
    {
        if(it->getHealth() <= 0)
        {
            if((it->getType() == ARMORED_RANDOM_TANK) || (it->getType() == RANDOM_TANK))
            {
                int rnd = rand();
                GameObject reward;
                if(rnd%3 == 0)
                    reward = GameObject(STAR, spriteLoader->getStar(), it->getX(), it->getY());
                else if(rnd%3 == 1)
                    reward = GameObject(TANKI, spriteLoader->getTanki(), it->getX(), it->getY());
                else if(rnd%3 == 2)
                    reward = GameObject(CLOCK, spriteLoader->getClock(), it->getX(), it->getY());
                reward.counter = this->FPS * 4;     //for 4 seconds lifespan
                bonus.push_back(reward);
            }
            if((it->getType() == ARMORED_RANDOM_TANK) || it->getType() == ARMORED_TANK)
            {
                score += 200;
            } else
            {
                score += 100;
            }
            tanks.erase(it);
            numberOfTanks--;

        }
    }

    //walls
    for(auto it = walls.begin(); it != walls.end(); ++it)
    {
        if(it->getType() == STONE)  //we can not destroy stone in game
            continue;
        if(it->getHealth() <= 0)
            walls.erase(it);
    }

    //player
    if(player.getHealth() <= 0)
        gameOver();

    //flag
    if(flag.getHealth() <= 0)
        gameOver();

    //bonus items
    for(auto it = bonus.begin(); it != bonus.end(); ++it)
    {
        if(it->counter <= 0)
            bonus.erase(it);
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
                it->setHealth(it->getHealth() - player.getDamage());
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
                it->setHealth(it->getHealth() - player.getDamage());
                missiles.erase(itm);
            }
        }
    }
    for(auto it = walls.begin(); it != walls.end(); ++it)
    {
        for(auto itm = enemyMissiles.begin(); itm != enemyMissiles.end(); ++itm)
        {
            if(it->getHitbox().intersects(itm->getHitbox()))
            {
                //delete objects (not hitboxes only)
                //do sth cool
                it->setHealth(it->getHealth() - itm->getDamage());      // - 1
                enemyMissiles.erase(itm);
            }
        }
    }

    //player and enemy missiles
    for(auto it = enemyMissiles.begin(); it != enemyMissiles.end(); ++it)
    {
        if(player.getHitbox().intersects(it->getHitbox()))
        {
            missiles.erase(it);
            player.setHealth(player.getHealth() - 1);
            player.setX(player.getRespawnX());
            player.setY(player.getRespawnY());
            player.setDamage(1);    //resetting to default damage
        }
    }

    //flag and missiles
    for(auto it = enemyMissiles.begin(); it != enemyMissiles.end(); ++it)
    {
        if(flag.getHitbox().intersects(it->getHitbox()))
        {
            enemyMissiles.erase(it);
            flag.setHealth(flag.getHealth() - it->getDamage());
            flag.setX(-1000);   //making it disapear
            flag.setY(-1000);
        }
    }
    for(auto it = missiles.begin(); it != missiles.end(); ++it)
    {
        if(flag.getHitbox().intersects(it->getHitbox()))
        {
            missiles.erase(it);
            flag.setHealth(flag.getHealth() - it->getDamage());
            flag.setX(-1000);   //making it disapear
            flag.setY(-1000);
        }
    }
}

void Game::detectBonusCollision(){
    if(bonus.empty())
        return;

    for(auto it = bonus.begin(); it != bonus.end(); ++it)
    {
        if(it->getHitbox().intersects(player.getHitbox()))
        {
            if(it->getType() == STAR)
            {
                player.setDamage(player.getDamage() + 1);
            } else if(it->getType() == TANKI)
            {
                player.setHealth(player.getHealth() + 1);
            } else if(it->getType() == CLOCK)
            {
                timeStopCounter += 4 * FPS;     //to block random movements for 4 seconds
            }
            bonus.erase(it);
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
    for(auto it = enemyMissiles.begin(); it != enemyMissiles.end(); ++it)
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

    //rendering player state
    updateSidebar();
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
    for(auto it = enemyMissiles.begin(); it != enemyMissiles.end(); ++it)
    {
        it->setHitbox(QRect(it->getX() , it->getY() , it->getSMALL_WIDTH(), it->getSMALL_HEIGHT()));
    }

    //hitboxes of bonus items
    for(auto it = bonus.begin(); it != bonus.end(); ++it)
    {
        it->setHitbox(QRect(it->getX() , it->getY() , it->getWIDTH(), it->getHEIGHT()));
    }
}

void Game::randomMovementsOfTanks(){
    if(timeStopCounter)
        return;
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
                it->setX(it->getX() + it->getSpeed());
        } else if(it->randomNumber % 4 == 1)
        {
            after = QRect(it->getX() - 4, it->getY(), it->getWIDTH(), it->getHEIGHT());
            it->setDirection(LEFT);
            if(!haveCollision(before, after))
                it->setX(it->getX() - it->getSpeed());
        } else if(it->randomNumber % 4 == 2)
        {
            after = QRect(it->getX(), it->getY() + 4, it->getWIDTH(), it->getHEIGHT());
            it->setDirection(DOWN);
            if(!haveCollision(before, after))
                it->setY(it->getY() + it->getSpeed());

        } else if(it->randomNumber % 4 == 3)
        {
            after = QRect(it->getX(), it->getY() - 4, it->getWIDTH(), it->getHEIGHT());
            it->setDirection(UP);
            if(!haveCollision(before, after))
                it->setY(it->getY() - it->getSpeed());
        }
    }

}

void Game::spawnTanks(){
    if(numberOfTanks >= 4)
        return;
    else if(remainingTanks <= 0)
        return;

    for(auto it = spawnPoints.begin(); it != spawnPoints.end(); ++it)
    {
        bool isSkipped = false;
        //checking if player is in that respawn point
        if(it->intersects(player.getHitbox()))
            continue;
        //checking if other tanks are in that respawn point
        for(auto itm = tanks.begin(); itm != tanks.end(); ++itm)
        {
            if(it->intersects(itm->getHitbox()))
            {
                isSkipped = true;
                continue;
            }
        }
        if(isSkipped)
            continue;

        int number = rand();
        if(number % 4 != 0)     //just for distributing tanks
            continue;
        GameObject tank;
        int rnd = rand();
        if(rnd % 4 == 0)
        {
            tank = GameObject(COMMON_TANK, spriteLoader->getCommon_tank_down(), it->x(), it->y(), DOWN);
        } else if(rnd % 4 == 1)
        {
            tank = GameObject(ARMORED_TANK, spriteLoader->getArmored_tank_down(), it->x(), it->y(), DOWN);
        } else if(rnd % 4 == 2)
        {
            tank = GameObject(RANDOM_TANK, spriteLoader->getRandom_tank_down(), it->x(), it->y(), DOWN);
        } else if(rnd % 4 == 3)
        {
            tank = GameObject(ARMORED_RANDOM_TANK, spriteLoader->getArmored_random_tank_down(), it->x(), it->y(), DOWN);
        }

        numberOfTanks++;
        remainingTanks--;
        tanks.push_back(tank);
        return;
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
            it->setY(it->getY() - it->getSpeed());
        else if(it->getDirection() == DOWN)
            it->setY(it->getY() + it->getSpeed());
        else if(it->getDirection() == RIGHT)
            it->setX(it->getX() + it->getSpeed());
        else if(it->getDirection() == LEFT)
            it->setX(it->getX() - it->getSpeed());
    }

    for(auto it = enemyMissiles.begin(); it != enemyMissiles.end(); ++it)
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

void Game::tanksShooting(){
    if(timeStopCounter)
        return;

    for(auto it = tanks.begin(); it != tanks.end(); ++it)
    {
        if(it->counter % FPS == 0)      //every second
        {
            int damage;
            if((it->getType() == ARMORED_RANDOM_TANK) || it->getType() == ARMORED_TANK)
                damage = 2;
            else if((it->getType() == RANDOM_TANK) || (it->getType() == COMMON_TANK))
                damage = 1;
            if(it->getDirection() == UP)
            {
                GameObject missile(MISSILE, spriteLoader->getMissile_up(), it->getX() + GameObject::getSMALL_WIDTH()/2 ,it->getY(), UP);
                missile.setDamage(damage);
                enemyMissiles.push_back(missile);
            } else if(it->getDirection() == DOWN)
            {
                GameObject missile(MISSILE, spriteLoader->getMissile_down(), it->getX() + GameObject::getSMALL_WIDTH()/2, it->getY(), DOWN);
                missile.setDamage(damage);
                enemyMissiles.push_back(missile);
            } else if(it->getDirection() == RIGHT)
            {
                GameObject missile(MISSILE, spriteLoader->getMissile_right(), it->getX() , it->getY() + GameObject::getSMALL_HEIGHT()/2, RIGHT);
                missile.setDamage(damage);
                enemyMissiles.push_back(missile);
            } else if(it->getDirection() == LEFT)
            {
                GameObject missile(MISSILE, spriteLoader->getMissile_left(), it->getX() , it->getY() + GameObject::getSMALL_HEIGHT()/2 , LEFT);
                missile.setDamage(damage);
                enemyMissiles.push_back(missile);
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

    //flag
    if(flag.getHitbox() != before)
        if(flag.getHitbox().intersects(after))
            return true;


    return false;
}

void Game::movePlayer(){
    if(playerDirection == MOVE_UP)
    {
        QRect before, after;
        before = QRect(player.getX(), player.getY(), player.getWIDTH(), player.getHEIGHT());
        after = QRect(player.getX(), player.getY() - player.getSpeed(), player.getWIDTH(), player.getHEIGHT());
        player.setDirection(UP);
        player.setSprite(spriteLoader->getYellow_tank_up());
        if(!haveCollision(before, after))
            player.setY(player.getY() - player.getSpeed());
    } else if(playerDirection == MOVE_DOWN)
    {
        QRect before, after;
        before = QRect(player.getX(), player.getY(), player.getWIDTH(), player.getHEIGHT());
        after = QRect(player.getX(), player.getY() + player.getSpeed(), player.getWIDTH(), player.getHEIGHT());
        player.setDirection(DOWN);
        player.setSprite(spriteLoader->getYellow_tank_down());
        if(!haveCollision(before, after))
            player.setY(player.getY() + player.getSpeed());
    } else if(playerDirection == MOVE_LEFT){
        QRect before, after;
        before = QRect(player.getX(), player.getY(), player.getWIDTH(), player.getHEIGHT());
        after = QRect(player.getX() - player.getSpeed(), player.getY(), player.getWIDTH(), player.getHEIGHT());
        player.setDirection(LEFT);
        player.setSprite(spriteLoader->getYellow_tank_left());
        if(!haveCollision(before, after))
            player.setX(player.getX() - player.getSpeed());
    } else if(playerDirection == MOVE_RIGHT)
    {
        QRect before, after;
        before = QRect(player.getX(), player.getY(), player.getWIDTH(), player.getHEIGHT());
        after = QRect(player.getX() + player.getSpeed(), player.getY(), player.getWIDTH(), player.getHEIGHT());
        player.setDirection(RIGHT);
        player.setSprite(spriteLoader->getYellow_tank_right());
        if(!haveCollision(before, after))
            player.setX(player.getX() + player.getSpeed());
    }
}

void Game::keyPressEvent(QKeyEvent* event){

    if (event->key() == Qt::Key_A)
    {
        playerDirection = MOVE_LEFT;
    } else if (event->key() == Qt::Key_D)
    {
        playerDirection = MOVE_RIGHT;
    } else if (event->key() == Qt::Key_W)
    {
        playerDirection = MOVE_UP;
    } else if (event->key() == Qt::Key_S)
    {
        playerDirection = MOVE_DOWN;
    } else if ((event->key() == Qt::Key_T) && (player.counter == 0))
    {
        player.counter += 10;    //for restricting number of missiles being shot
        if(player.getDirection() == UP)
        {
            GameObject missile(MISSILE, spriteLoader->getMissile_up(), player.getX() + GameObject::getSMALL_WIDTH()/2 ,player.getY(), UP);
            missiles.push_back(missile);
        } else if(player.getDirection() == DOWN)
        {
            GameObject missile(MISSILE, spriteLoader->getMissile_down(), player.getX() + GameObject::getSMALL_WIDTH()/2, player.getY(), DOWN);
            missiles.push_back(missile);
        } else if(player.getDirection() == RIGHT)
        {
            GameObject missile(MISSILE, spriteLoader->getMissile_right(), player.getX(),player.getY() + GameObject::getSMALL_HEIGHT()/2, RIGHT);
            missiles.push_back(missile);
        } else if(player.getDirection() == LEFT)
        {
            GameObject missile(MISSILE, spriteLoader->getMissile_left(), player.getX(), player.getY() + GameObject::getSMALL_HEIGHT()/2, LEFT);
            missiles.push_back(missile);
        }
    }


    // Call the base class implementation
    QWidget::keyPressEvent(event);
}

void Game::keyReleaseEvent(QKeyEvent* event) {

    //Note : second conditions are for preventing player movement from delay which is caused by pressing and releasing
    // keys approximately simultaneous

    if(event->key() == Qt::Key_A && player.getDirection() == LEFT)
        playerDirection = NONE;
    else if((event->key() == Qt::Key_S) && player.getDirection() == DOWN)
        playerDirection = NONE;
    else if(event->key() == Qt::Key_D && player.getDirection() == RIGHT)
        playerDirection = NONE;
    else if(event->key() == Qt::Key_W && player.getDirection() == UP)
        playerDirection = NONE;

    // Call the base class implementation
    QWidget::keyReleaseEvent(event);
}

int Game::getFPS() const
{
    return FPS;
}

void Game::gameOver(){
    timer->stop();
    QFileInfo info = QFileInfo(QDir::currentPath());
    QString path = info.dir().path();
    path += "/Tank-2D/Arts/gameOver.png";
    QImage sprite(path);
    QPixmap pixmap = QPixmap::fromImage(sprite);
    pixmap = pixmap.scaled(WIDTH, HEIGHT);
    QGraphicsPixmapItem* item = new QGraphicsPixmapItem(pixmap);
    item->setPos(0 , 0);
    item->setZValue(1);     //make it to be external layer
    scene->addItem(item);
    scene->update();
}

void Game::checkWinning(){
    if(remainingTanks <= 0)
        if(numberOfTanks <= 0)
            winner();
}

void Game::winner(){
    if(user.getHighScore() < score)
        user.setHighScore(score);
    user.setGamesPlayed(user.getGamesPlayed() + 1);
    registerMenu->setUser(user);
    registerMenu->saveUserInfo();

    timer->stop();
    ui->setupUi(this);
    this->setWindowTitle("Tank Battle City");
    scene->clear();
    scene->deleteLater();
    backgroundView->repaint();
    backgroundView->deleteLater();

    QUiLoader loader;
    QFileInfo info = QFileInfo(QDir::currentPath());
    QString path = info.dir().path();
    path += "/Tank-2D/winner.ui";
    QFile file(path);
    if(file.exists())
    {
        file.open(QIODevice::OpenModeFlag::ReadOnly);
        QWidget* widget = loader.load(&file, this);

        //loading flag logo
        QGraphicsView* flagView = widget->findChild<QGraphicsView*>("flagView", Qt::FindChildrenRecursively);
        flagView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        flagView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        flagView->setStyleSheet("border:transparent;");
        QPixmap pixmap = spriteLoader->getFlag();
        pixmap = pixmap.scaled(flagView->width(), flagView->height());
        QGraphicsPixmapItem* pixItem = new QGraphicsPixmapItem(pixmap);
        QGraphicsScene* tempScene = new QGraphicsScene();
        tempScene->addItem(pixItem);
        flagView->setScene(tempScene);

        //loading info
        QLabel* playerName = widget->findChild<QLabel*>("playerName", Qt::FindChildrenRecursively);
        playerName->setText(user.getUsername());
        QLabel* playerScore = widget->findChild<QLabel*>("playerScore", Qt::FindChildrenRecursively);
        playerScore->setText(QString::number(score));
        QLabel* highestScore = widget->findChild<QLabel*>("highestScore", Qt::FindChildrenRecursively);
        highestScore->setText(QString::number(user.getHighScore()));

        //connecting continue button
        QPushButton* continueButton = widget->findChild<QPushButton*>("continueButton", Qt::FindChildrenRecursively);
        connect(continueButton, &QPushButton::clicked, this, &Game::on_continueButton_clicked);

        //setting widget
        QVBoxLayout* layout = new QVBoxLayout();
        layout->addWidget(widget);
        layout->setContentsMargins(0 ,0 ,0 ,0);     //now it fills the whole background
        this->setLayout(layout);
        this->setFixedSize(WIDTH, HEIGHT);     //resizing window
    } else
    {
        qDebug() << "File not found.";
    }
}

void Game::clearGameObjects(){
    tanks.clear();
    walls.clear();
    bonus.clear();
    missiles.clear();
    enemyMissiles.clear();
    spawnPoints.clear();
}

void Game::on_continueButton_clicked(){
    clearGameObjects();     //it is important to get rid of previous game objects
    ui->setupUi(this);
    this->setWindowTitle("Tank Battle City");
    scene = new QGraphicsScene();
    backgroundView = new QGraphicsView();
    backgroundView->setFixedSize(QSize(WIDTH, HEIGHT));
    backgroundView->setStyleSheet("background-color: black;");
    backgroundView->setScene(scene);
    //we should ensure that scene fills the whole QGraphicsView
    scene->setSceneRect(backgroundView->rect());

    //disable scrollbars if you don't want them to appear
    backgroundView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    backgroundView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    QVBoxLayout* layout = new QVBoxLayout();
    layout->setContentsMargins(0 ,0 ,0 ,0);     //now it fills the whole background
    layout->addWidget(backgroundView);
    delete this->layout();
    this->setLayout(layout);

    currentLevel++;
    loadLevel(currentLevel);
    timer->start();
}

void Game::updateSidebar(){
    //player health
    int health = player.getHealth() >= 0 ? player.getHealth() : 0;
    ui->healthNumberLabel->setText(QString::number(health));
}

Game::~Game()
{
    delete ui;
    delete scene;
}

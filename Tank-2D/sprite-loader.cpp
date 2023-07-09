#include "sprite-loader.h"
#include <QDir>
#include <QGraphicsPixmapItem>
#include <QGraphicsScene>
#include <QGraphicsView>
#include "game-object.h"

SpriteLoader::SpriteLoader()
{
    QFileInfo info = QFileInfo(QDir::currentPath());
    QString path = info.dir().path() + "/Tank-2D/Arts/";
    QImage sprite(path + "yellow-tank-up.png");
    QPixmap pixmap = QPixmap::fromImage(sprite);
    yellow_tank_up = pixmap.scaled(GameObject::getWIDTH(), GameObject::getHEIGHT());

    sprite = QImage(path + "yellow-tank-down.png");
    pixmap = QPixmap::fromImage(sprite);
    yellow_tank_down = pixmap.scaled(GameObject::getWIDTH(), GameObject::getHEIGHT());

    sprite = QImage(path + "yellow-tank-left.png");
    pixmap = QPixmap::fromImage(sprite);
    yellow_tank_left = pixmap.scaled(GameObject::getWIDTH(), GameObject::getHEIGHT());

    sprite = QImage(path + "yellow-tank-right.png");
    pixmap = QPixmap::fromImage(sprite);
    yellow_tank_right = pixmap.scaled(GameObject::getWIDTH(), GameObject::getHEIGHT());

    sprite = QImage(path + "common-tank-up.png");
    pixmap = QPixmap::fromImage(sprite);
    common_tank_up = pixmap.scaled(GameObject::getWIDTH(), GameObject::getHEIGHT());

    sprite = QImage(path + "common-tank-down.png");
    pixmap = QPixmap::fromImage(sprite);
    common_tank_down = pixmap.scaled(GameObject::getWIDTH(), GameObject::getHEIGHT());

    sprite = QImage(path + "common-tank-left.png");
    pixmap = QPixmap::fromImage(sprite);
    common_tank_left = pixmap.scaled(GameObject::getWIDTH(), GameObject::getHEIGHT());

    sprite = QImage(path + "common-tank-right.png");
    pixmap = QPixmap::fromImage(sprite);
    common_tank_right = pixmap.scaled(GameObject::getWIDTH(), GameObject::getHEIGHT());

    sprite = QImage(path + "armored-tank-up.png");
    pixmap = QPixmap::fromImage(sprite);
    armored_tank_up = pixmap.scaled(GameObject::getWIDTH(), GameObject::getHEIGHT());

    sprite = QImage(path + "armored-tank-down.png");
    pixmap = QPixmap::fromImage(sprite);
    armored_tank_down = pixmap.scaled(GameObject::getWIDTH(), GameObject::getHEIGHT());

    sprite = QImage(path + "armored-tank-left.png");
    pixmap = QPixmap::fromImage(sprite);
    armored_tank_left = pixmap.scaled(GameObject::getWIDTH(), GameObject::getHEIGHT());

    sprite = QImage(path + "armored-tank-right.png");
    pixmap = QPixmap::fromImage(sprite);
    armored_tank_right = pixmap.scaled(GameObject::getWIDTH(), GameObject::getHEIGHT());

    sprite = QImage(path + "random-tank-up.png");
    pixmap = QPixmap::fromImage(sprite);
    random_tank_up = pixmap.scaled(GameObject::getWIDTH(), GameObject::getHEIGHT());

    sprite = QImage(path + "random-tank-down.png");
    pixmap = QPixmap::fromImage(sprite);
    random_tank_down = pixmap.scaled(GameObject::getWIDTH(), GameObject::getHEIGHT());

    sprite = QImage(path + "random-tank-left.png");
    pixmap = QPixmap::fromImage(sprite);
    random_tank_left = pixmap.scaled(GameObject::getWIDTH(), GameObject::getHEIGHT());

    sprite = QImage(path + "random-tank-right.png");
    pixmap = QPixmap::fromImage(sprite);
    random_tank_right = pixmap.scaled(GameObject::getWIDTH(), GameObject::getHEIGHT());

    sprite = QImage(path + "armored-random-tank-up.png");
    pixmap = QPixmap::fromImage(sprite);
    armored_random_tank_up = pixmap.scaled(GameObject::getWIDTH(), GameObject::getHEIGHT());

    sprite = QImage(path + "armored-random-tank-down.png");
    pixmap = QPixmap::fromImage(sprite);
    armored_random_tank_down = pixmap.scaled(GameObject::getWIDTH(), GameObject::getHEIGHT());

    sprite = QImage(path + "armored-random-tank-left.png");
    pixmap = QPixmap::fromImage(sprite);
    armored_random_tank_left = pixmap.scaled(GameObject::getWIDTH(), GameObject::getHEIGHT());

    sprite = QImage(path + "armored-random-tank-right.png");
    pixmap = QPixmap::fromImage(sprite);
    armored_random_tank_right = pixmap.scaled(GameObject::getWIDTH(), GameObject::getHEIGHT());

    sprite = QImage(path + "Tanki.png");
    pixmap = QPixmap::fromImage(sprite);
    tanki = pixmap.scaled(GameObject::getWIDTH(), GameObject::getHEIGHT());

    sprite = QImage(path + "Star.png");
    pixmap = QPixmap::fromImage(sprite);
    star = pixmap.scaled(GameObject::getWIDTH(), GameObject::getHEIGHT());

    sprite = QImage(path + "Clock.png");
    pixmap = QPixmap::fromImage(sprite);
    clock = pixmap.scaled(GameObject::getWIDTH(), GameObject::getHEIGHT());

    sprite = QImage(path + "flag.png");
    pixmap = QPixmap::fromImage(sprite);
    flag = pixmap.scaled(GameObject::getWIDTH(), GameObject::getHEIGHT());

    sprite = QImage(path + "brick.png");
    pixmap = QPixmap::fromImage(sprite);
    brick = pixmap.scaled(GameObject::getWIDTH(), GameObject::getHEIGHT());

    sprite = QImage(path + "Stone.png");
    pixmap = QPixmap::fromImage(sprite);
    stone = pixmap.scaled(GameObject::getWIDTH(), GameObject::getHEIGHT());

    sprite = QImage(path + "water.png");
    pixmap = QPixmap::fromImage(sprite);
    water = pixmap.scaled(GameObject::getWIDTH(), GameObject::getHEIGHT());

    sprite = QImage(path + "missile-up.gif");
    pixmap = QPixmap::fromImage(sprite);
    missile_up = pixmap.scaled(GameObject::getWIDTH(), GameObject::getHEIGHT());

    sprite = QImage(path + "missile-down.gif");
    pixmap = QPixmap::fromImage(sprite);
    missile_down = pixmap.scaled(GameObject::getWIDTH(), GameObject::getHEIGHT());

    sprite = QImage(path + "missile-left.gif");
    pixmap = QPixmap::fromImage(sprite);
    missile_left = pixmap.scaled(GameObject::getWIDTH(), GameObject::getHEIGHT());

    sprite = QImage(path + "missile-right.gif");
    pixmap = QPixmap::fromImage(sprite);
    missile_right = pixmap.scaled(GameObject::getWIDTH(), GameObject::getHEIGHT());
}

QPixmap SpriteLoader::getYellow_tank_up() const
{
    return yellow_tank_up;
}

const QPixmap &SpriteLoader::getYellow_tank_down() const
{
    return yellow_tank_down;
}

const QPixmap &SpriteLoader::getYellow_tank_left() const
{
    return yellow_tank_left;
}

const QPixmap &SpriteLoader::getYellow_tank_right() const
{
    return yellow_tank_right;
}

const QPixmap &SpriteLoader::getCommon_tank_up() const
{
    return common_tank_up;
}

const QPixmap &SpriteLoader::getCommon_tank_down() const
{
    return common_tank_down;
}

const QPixmap &SpriteLoader::getCommon_tank_left() const
{
    return common_tank_left;
}

const QPixmap &SpriteLoader::getCommon_tank_right() const
{
    return common_tank_right;
}

const QPixmap &SpriteLoader::getArmored_tank_up() const
{
    return armored_tank_up;
}

const QPixmap &SpriteLoader::getArmored_tank_down() const
{
    return armored_tank_down;
}

const QPixmap &SpriteLoader::getArmored_tank_left() const
{
    return armored_tank_left;
}

const QPixmap &SpriteLoader::getArmored_tank_right() const
{
    return armored_tank_right;
}

const QPixmap &SpriteLoader::getRandom_tank_up() const
{
    return random_tank_up;
}

const QPixmap &SpriteLoader::getRandom_tank_down() const
{
    return random_tank_down;
}

const QPixmap &SpriteLoader::getRandom_tank_left() const
{
    return random_tank_left;
}

const QPixmap &SpriteLoader::getRandom_tank_right() const
{
    return random_tank_right;
}

const QPixmap &SpriteLoader::getArmored_random_tank_up() const
{
    return armored_random_tank_up;
}

const QPixmap &SpriteLoader::getArmored_random_tank_down() const
{
    return armored_random_tank_down;
}

const QPixmap &SpriteLoader::getArmored_random_tank_left() const
{
    return armored_random_tank_left;
}

const QPixmap &SpriteLoader::getArmored_random_tank_right() const
{
    return armored_random_tank_right;
}

const QPixmap &SpriteLoader::getTanki() const
{
    return tanki;
}

const QPixmap &SpriteLoader::getStar() const
{
    return star;
}

const QPixmap &SpriteLoader::getClock() const
{
    return clock;
}

const QPixmap &SpriteLoader::getFlag() const
{
    return flag;
}

const QPixmap &SpriteLoader::getBrick() const
{
    return brick;
}

const QPixmap &SpriteLoader::getStone() const
{
    return stone;
}

const QPixmap &SpriteLoader::getWater() const
{
    return water;
}

const QPixmap &SpriteLoader::getMissile_up() const
{
    return missile_up;
}

const QPixmap &SpriteLoader::getMissile_down() const
{
    return missile_down;
}

const QPixmap &SpriteLoader::getMissile_left() const
{
    return missile_left;
}

const QPixmap &SpriteLoader::getMissile_right() const
{
    return missile_right;
}

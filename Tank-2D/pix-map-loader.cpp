#include "pix-map-loader.h"
#include "game-object.h"
#include <QDir>
#include <QIcon>

PixMapLoader::PixMapLoader()
{
    int width = GameObject::getWIDTH();
    int height = GameObject::getHEIGHT();
    QFileInfo info = QFileInfo(QDir::currentPath());
    QString path = info.dir().path() + "/Tank-2D/Arts/";
    QImage sprite(path + "yellow-tank-up.png");
    QPixmap pixmap = QPixmap::fromImage(sprite);
    pixmap = pixmap.scaled(width, height);
    YELLOW_TANK_UP = QGraphicsPixmapItem(pixmap);
}

QGraphicsPixmapItem PixMapLoader::getYELLOW_TANK_UP() const
{
    return YELLOW_TANK_UP;
}

const QGraphicsPixmapItem &PixMapLoader::getYELLOW_TANK_DOWN() const
{
    return YELLOW_TANK_DOWN;
}

const QGraphicsPixmapItem &PixMapLoader::getYELLOW_TANK_LEFT() const
{
    return YELLOW_TANK_LEFT;
}

const QGraphicsPixmapItem &PixMapLoader::getYELLOW_TANK_RIGHT() const
{
    return YELLOW_TANK_RIGHT;
}

QGraphicsPixmapItem *PixMapLoader::getCOMMOM_TANK_UP() const
{
    return COMMOM_TANK_UP;
}

const QGraphicsPixmapItem &PixMapLoader::getCOMMOM_TANK_DOWN() const
{
    return COMMOM_TANK_DOWN;
}

const QGraphicsPixmapItem &PixMapLoader::getCOMMOM_TANK_LEFT() const
{
    return COMMOM_TANK_LEFT;
}

const QGraphicsPixmapItem &PixMapLoader::getCOMMOM_TANK_RIGHT() const
{
    return COMMOM_TANK_RIGHT;
}

QGraphicsPixmapItem *PixMapLoader::getARMORED_TANK_UP() const
{
    return ARMORED_TANK_UP;
}

const QGraphicsPixmapItem &PixMapLoader::getARMORED_TANK_DOWN() const
{
    return ARMORED_TANK_DOWN;
}

const QGraphicsPixmapItem &PixMapLoader::getARMORED_TANK_LEFT() const
{
    return ARMORED_TANK_LEFT;
}

const QGraphicsPixmapItem &PixMapLoader::getARMORED_TANK_RIGHT() const
{
    return ARMORED_TANK_RIGHT;
}

QGraphicsPixmapItem *PixMapLoader::getRANDOM_TANK_UP() const
{
    return RANDOM_TANK_UP;
}

const QGraphicsPixmapItem &PixMapLoader::getRANDOM_TANK_DOWN() const
{
    return RANDOM_TANK_DOWN;
}

const QGraphicsPixmapItem &PixMapLoader::getRANDOM_TANK_RIGHT() const
{
    return RANDOM_TANK_RIGHT;
}

const QGraphicsPixmapItem &PixMapLoader::getRANDOM_TANK_LEFT() const
{
    return RANDOM_TANK_LEFT;
}

QGraphicsPixmapItem *PixMapLoader::getARMORED_RANDOM_TANK_UP() const
{
    return ARMORED_RANDOM_TANK_UP;
}

const QGraphicsPixmapItem &PixMapLoader::getARMORED_RANDOM_TANK_DOWN() const
{
    return ARMORED_RANDOM_TANK_DOWN;
}

const QGraphicsPixmapItem &PixMapLoader::getARMORED_RANDOM_TANK_RIGHT() const
{
    return ARMORED_RANDOM_TANK_RIGHT;
}

const QGraphicsPixmapItem &PixMapLoader::getARMORED_RANDOM_TANK_LEFT() const
{
    return ARMORED_RANDOM_TANK_LEFT;
}

QGraphicsPixmapItem *PixMapLoader::getSTONE() const
{
    return STONE;
}

const QGraphicsPixmapItem &PixMapLoader::getBRICK() const
{
    return BRICK;
}

const QGraphicsPixmapItem &PixMapLoader::getWATER() const
{
    return WATER;
}

QGraphicsPixmapItem *PixMapLoader::getTANKI() const
{
    return TANKI;
}

const QGraphicsPixmapItem &PixMapLoader::getSTAR() const
{
    return STAR;
}

const QGraphicsPixmapItem &PixMapLoader::getCLOCK() const
{
    return CLOCK;
}

QGraphicsPixmapItem *PixMapLoader::getFLAG() const
{
    return FLAG;
}

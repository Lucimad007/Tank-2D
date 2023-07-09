#ifndef PIXMAPLOADER_H
#define PIXMAPLOADER_H
#include <QGraphicsPixmapItem>

class PixMapLoader
{
public:
    PixMapLoader();

    QGraphicsPixmapItem getYELLOW_TANK_UP() const;

    const QGraphicsPixmapItem &getYELLOW_TANK_DOWN() const;

    const QGraphicsPixmapItem &getYELLOW_TANK_LEFT() const;

    const QGraphicsPixmapItem &getYELLOW_TANK_RIGHT() const;

    QGraphicsPixmapItem *getCOMMOM_TANK_UP() const;

    const QGraphicsPixmapItem &getCOMMOM_TANK_DOWN() const;

    const QGraphicsPixmapItem &getCOMMOM_TANK_LEFT() const;

    const QGraphicsPixmapItem &getCOMMOM_TANK_RIGHT() const;

    QGraphicsPixmapItem *getARMORED_TANK_UP() const;

    const QGraphicsPixmapItem &getARMORED_TANK_DOWN() const;

    const QGraphicsPixmapItem &getARMORED_TANK_LEFT() const;

    const QGraphicsPixmapItem &getARMORED_TANK_RIGHT() const;

    QGraphicsPixmapItem *getRANDOM_TANK_UP() const;

    const QGraphicsPixmapItem &getRANDOM_TANK_DOWN() const;

    const QGraphicsPixmapItem &getRANDOM_TANK_RIGHT() const;

    const QGraphicsPixmapItem &getRANDOM_TANK_LEFT() const;

    QGraphicsPixmapItem *getARMORED_RANDOM_TANK_UP() const;

    const QGraphicsPixmapItem &getARMORED_RANDOM_TANK_DOWN() const;

    const QGraphicsPixmapItem &getARMORED_RANDOM_TANK_RIGHT() const;

    const QGraphicsPixmapItem &getARMORED_RANDOM_TANK_LEFT() const;

    QGraphicsPixmapItem *getSTONE() const;

    const QGraphicsPixmapItem &getBRICK() const;

    const QGraphicsPixmapItem &getWATER() const;

    QGraphicsPixmapItem *getTANKI() const;

    const QGraphicsPixmapItem &getSTAR() const;

    const QGraphicsPixmapItem &getCLOCK() const;

    QGraphicsPixmapItem *getFLAG() const;

private:
    QGraphicsPixmapItem YELLOW_TANK_UP, YELLOW_TANK_DOWN, YELLOW_TANK_LEFT, YELLOW_TANK_RIGHT;
    QGraphicsPixmapItem* COMMOM_TANK_UP, COMMOM_TANK_DOWN, COMMOM_TANK_LEFT, COMMOM_TANK_RIGHT;
    QGraphicsPixmapItem* ARMORED_TANK_UP, ARMORED_TANK_DOWN, ARMORED_TANK_LEFT, ARMORED_TANK_RIGHT;
    QGraphicsPixmapItem* RANDOM_TANK_UP, RANDOM_TANK_DOWN, RANDOM_TANK_RIGHT, RANDOM_TANK_LEFT;
    QGraphicsPixmapItem* ARMORED_RANDOM_TANK_UP, ARMORED_RANDOM_TANK_DOWN, ARMORED_RANDOM_TANK_RIGHT, ARMORED_RANDOM_TANK_LEFT;
    QGraphicsPixmapItem* STONE, BRICK, WATER;
    QGraphicsPixmapItem* TANKI, STAR, CLOCK;
    QGraphicsPixmapItem* FLAG;
};

#endif // PIXMAPLOADER_H

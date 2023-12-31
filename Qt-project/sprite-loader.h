#ifndef SPRITELOADER_H
#define SPRITELOADER_H
#include <QPixmap>

class SpriteLoader
{
public:
    SpriteLoader();
    QPixmap getYellow_tank_up() const;

    const QPixmap &getYellow_tank_down() const;

    const QPixmap &getYellow_tank_left() const;

    const QPixmap &getYellow_tank_right() const;

    const QPixmap &getCommon_tank_up() const;

    const QPixmap &getCommon_tank_down() const;

    const QPixmap &getCommon_tank_left() const;

    const QPixmap &getCommon_tank_right() const;

    const QPixmap &getArmored_tank_up() const;

    const QPixmap &getArmored_tank_down() const;

    const QPixmap &getArmored_tank_left() const;

    const QPixmap &getArmored_tank_right() const;

    const QPixmap &getRandom_tank_up() const;

    const QPixmap &getRandom_tank_down() const;

    const QPixmap &getRandom_tank_left() const;

    const QPixmap &getRandom_tank_right() const;

    const QPixmap &getArmored_random_tank_up() const;

    const QPixmap &getArmored_random_tank_down() const;

    const QPixmap &getArmored_random_tank_left() const;

    const QPixmap &getArmored_random_tank_right() const;

    const QPixmap &getTanki() const;

    const QPixmap &getStar() const;

    const QPixmap &getClock() const;

    const QPixmap &getFlag() const;

    const QPixmap &getBrick() const;

    const QPixmap &getStone() const;

    const QPixmap &getWater() const;

    const QPixmap &getMissile_up() const;

    const QPixmap &getMissile_down() const;

    const QPixmap &getMissile_left() const;

    const QPixmap &getMissile_right() const;

    const QPixmap &getHeart() const;

    const QPixmap &getCross() const;
    void setCross(const QPixmap &newCross);

    const QPixmap &getBlack() const;
    void setBlack(const QPixmap &newBlack);

    const QPixmap &getExplosion() const;
    void setExplosion(const QPixmap &newExplosion);

    const QPixmap &getExplosion2() const;
    void setExplosion2(const QPixmap &newExplosion2);

    const QPixmap &getExplosion3() const;
    void setExplosion3(const QPixmap &newExplosion3);

    const QPixmap &getExplosion4() const;
    void setExplosion4(const QPixmap &newExplosion4);

    const QPixmap &getExplosion5() const;
    void setExplosion5(const QPixmap &newExplosion5);

    const QPixmap &getBrokenBrick() const;
    void setBrokenBrick(const QPixmap &newBrokenBrick);

private:
    QPixmap yellow_tank_up, yellow_tank_down, yellow_tank_left, yellow_tank_right;
    QPixmap common_tank_up, common_tank_down, common_tank_left, common_tank_right;
    QPixmap armored_tank_up, armored_tank_down, armored_tank_left, armored_tank_right;
    QPixmap random_tank_up, random_tank_down, random_tank_left, random_tank_right;
    QPixmap armored_random_tank_up, armored_random_tank_down, armored_random_tank_left, armored_random_tank_right;
    QPixmap tanki, star, clock;
    QPixmap flag;
    QPixmap brick, brokenBrick, stone, water;
    QPixmap missile_up, missile_down, missile_left, missile_right;
    QPixmap heart;
    QPixmap cross;
    QPixmap black;
    QPixmap explosion, explosion2, explosion3, explosion4 , explosion5;
};

#endif // SPRITELOADER_H

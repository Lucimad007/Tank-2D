#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H
#include <QString>

enum Type {PLAYER, ARMORED_RANDOM_TANK, ARMORED_TANK, COMMON_TANK, RANDOM_TANK, YELLOW_TANK, STAR, TANKI, CLOCK, FLAG
          , WATER, STONE , BRICK};
enum Direction {LEFT , RIGHT, UP, DOWN};

class GameObject
{
public:
    GameObject();
    GameObject(const Type& type, const QString& spritePath, const int& x, const int& y, const int& health, const int& damage, const Direction& direction = UP);

    const QString &getSpritePath() const;
    void setSpritePath(const QString &newSpritePath);

    int getY() const;
    void setY(int newY);

    int getX() const;
    void setX(int newX);

    int getDamage() const;
    void setDamage(int newDamage);

    int getHealth() const;
    void setHealth(int newHealth);

    static int getHEIGHT();

    static int getWIDTH();

    Type getType() const;
    void setType(Type newType);

private:
    static int WIDTH;
    static int HEIGHT;
    int health;
    int damage;
    int x;
    int y;
    QString spritePath;
    QString defaultPath;
    Type type;
    Direction direction;
public:
    int counter = 0;
    int randomNumber = 0;
    int steps = WIDTH / 4;
    Direction getDirection() const;
    void setDirection(Direction newDirection);
};

#endif // GAMEOBJECT_H

#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H
#include <QString>

class GameObject
{
public:
    GameObject();
    GameObject(const QString& spritePath, const int& x, const int& y, const int& health, const int& damage);

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

    int getHEIGHT() const;

    int getWIDTH() const;

private:
    int WIDTH;
    int HEIGHT;
    int health;
    int damage;
    int x;
    int y;
    QString spritePath;
    QString defaultPath;
};

#endif // GAMEOBJECT_H

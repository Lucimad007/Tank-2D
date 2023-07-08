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

signals:
    void xChanged();

private:
    const int WIDTH = 32;
    const int HEIGHT = 32;
    int health;
    int damage;
    int x;
    int y;
    QString spritePath;
    QString defaultPath;
    Q_PROPERTY(int x READ getX WRITE setX NOTIFY xChanged)
};

#endif // GAMEOBJECT_H

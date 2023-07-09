#include "game-object.h"
#include <QDir>

int GameObject::WIDTH = 32;
int GameObject::HEIGHT = 32;

GameObject::GameObject()
{
    QFileInfo info = QFileInfo(QDir::currentPath());
    defaultPath = info.dir().path() + "/Tank-2D/Arts/";
    spritePath = defaultPath;
    x = 0;
    y = 0;
    health = 0;
    damage = 0;
}

GameObject::GameObject(const Type& type, const QString& spritePath, const int& x, const int& y, const int& health, const int& damage, const Direction& direction){
    this->type = type;
    QFileInfo info = QFileInfo(QDir::currentPath());
    defaultPath = info.dir().path() + "/Tank-2D/Arts/";
    this->spritePath = defaultPath + spritePath;
    this->x = x;
    this->y = y;
    this->health = health;
    this->damage = damage;
    this->direction = direction;
}

const QString &GameObject::getSpritePath() const
{
    return spritePath;
}

void GameObject::setSpritePath(const QString &newSpritePath)
{
    spritePath = defaultPath;
    spritePath += newSpritePath;
}

int GameObject::getY() const
{
    return y;
}

void GameObject::setY(int newY)
{
    y = newY;
}

int GameObject::getX() const
{
    return x;
}

void GameObject::setX(int newX)
{
    x = newX;
}

int GameObject::getDamage() const
{
    return damage;
}

void GameObject::setDamage(int newDamage)
{
    damage = newDamage;
}

int GameObject::getHealth() const
{
    return health;
}

void GameObject::setHealth(int newHealth)
{
    health = newHealth;
}

int GameObject::getHEIGHT()
{
    return HEIGHT;
}

int GameObject::getWIDTH()
{
    return WIDTH;
}

Type GameObject::getType() const
{
    return type;
}

void GameObject::setType(Type newType)
{
    type = newType;
}

Direction GameObject::getDirection() const
{
    return direction;
}

void GameObject::setDirection(Direction newDirection)
{
    direction = newDirection;
}

bool GameObject::getChanged() const
{
    return changed;
}

void GameObject::setChanged(bool newChanged)
{
    changed = newChanged;
}

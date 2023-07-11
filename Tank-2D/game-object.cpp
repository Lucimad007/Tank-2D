#include "game-object.h"
#include <QDir>
#include <QRect>

int GameObject::WIDTH = 32;
int GameObject::HEIGHT = 32;
int GameObject::SMALL_WIDTH = 16;
int GameObject::SMALL_HEIGHT = 16;

GameObject::GameObject()
{
    x = 0;
    y = 0;
    health = 0;
    damage = 0;
    hitbox = QRect(0, 0, WIDTH, HEIGHT);
}

GameObject::GameObject(const Type& type, const QPixmap& sprite, const int& x, const int& y, const int& health, const int& damage, const Direction& direction){
    this->type = type;
    this->sprite = sprite;
    this->x = x;
    this->y = y;
    this->health = health;
    this->damage = damage;
    this->direction = direction;
    if(type == MISSILE)
        hitbox = QRect(x, y, SMALL_WIDTH, SMALL_HEIGHT);
    else
        hitbox = QRect(x, y, WIDTH, HEIGHT);
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

int GameObject::getSMALL_WIDTH()
{
    return SMALL_WIDTH;
}

void GameObject::setHitbox(const QRect &newHitbox)
{
    hitbox = newHitbox;
}

const QRect &GameObject::getHitbox() const
{
    return hitbox;
}

int GameObject::getSMALL_HEIGHT()
{
    return SMALL_HEIGHT;
}

QPixmap GameObject::getSprite() const
{
    return sprite;
}

void GameObject::setSprite(QPixmap newSprite)
{
    sprite = newSprite;
}

Direction GameObject::getDirection() const
{
    return direction;
}

void GameObject::setDirection(Direction newDirection)
{
    direction = newDirection;
}

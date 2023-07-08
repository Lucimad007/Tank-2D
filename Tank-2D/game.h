#ifndef GAME_H
#define GAME_H
#include <QList>
#include <QWidget>
#include <QGraphicsScene>
#include "game-object.h"

namespace Ui {
class Game;
}

class Game : public QWidget
{
    Q_OBJECT

public:
    explicit Game(QWidget *parent = nullptr);
    void loadIcon();
    void clear();
    void render();
    ~Game();

private:
    Ui::Game *ui;
    QGraphicsScene* scene = nullptr;
    QList<GameObject> objects;
};

#endif // GAME_H

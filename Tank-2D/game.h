#ifndef GAME_H
#define GAME_H
#include <QVector>
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
    void loadLevel();
    void loadIcon();
    void clear();
    void updateLogic();
    void limitObjects();
    void render();
    void updateHitBoxes();
    bool haveCollision(QRect before, QRect after);
    ~Game();

    int getFPS() const;

protected:
    void keyPressEvent(QKeyEvent* event) override;

private:
    Ui::Game *ui;
    int FPS = 60;
    QGraphicsScene* scene = nullptr;
    GameObject player;
    GameObject flag;
    QVector<GameObject> tanks;
    QVector<GameObject> walls;
    QVector<GameObject> bonus;
    QVector<GameObject> missiles;
    QVector<QRect> hitBoxes;
};

#endif // GAME_H

#ifndef GAME_H
#define GAME_H
#include <QVector>
#include <QWidget>
#include <QGraphicsScene>
#include "game-object.h"
#include "sprite-loader.h"

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
    void deleteJunkMissiles();
    void render();
    void updateHitBoxes();
    bool haveCollision(QRect before, QRect after);  //before object is for authentication. due to our code the objects can be in the same posision unless they are the same object.
    void detectMissileCollision();
    ~Game();

    int getFPS() const;

protected:
    void keyPressEvent(QKeyEvent* event) override;

private:
    Ui::Game *ui;
    SpriteLoader* spriteLoader;     //we read files once to improve code performance critically
    int FPS = 60;
    QGraphicsScene* scene = nullptr;
    GameObject player;
    GameObject flag;
    std::list<GameObject> tanks;
    std::list<GameObject> walls;
    std::list<GameObject> bonus;
    std::list<GameObject> missiles;     //QList has a bug when we are erasing the last element
};

#endif // GAME_H

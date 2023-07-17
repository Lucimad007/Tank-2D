#ifndef MULTIPLAYER_H
#define MULTIPLAYER_H

#include <QWidget>
#include <QGraphicsView>
#include "sprite-loader.h"
#include "game-object.h"

namespace Ui {
class MultiPlayer;
}

class MultiPlayer : public QWidget
{
    Q_OBJECT

public:
    explicit MultiPlayer(QWidget *parent = nullptr);
    void loadIcon();
    void loadMap();
    void clear();
    void updateLogic();
    void render();
    ~MultiPlayer();

    int getFPS() const;

private:
    Ui::MultiPlayer *ui;
    SpriteLoader* spriteLoader;     //we read files once to improve code performance critically
    int FPS = 60;
    const int cellSize = 32;
    QGraphicsScene* scene = nullptr;
    QGraphicsView* backgroundView = nullptr;
    GameObject player1, player2, flag1, flag2;
    std::list<GameObject> walls;
    std::list<GameObject> player1Missiles;     //QList has a bug when we are erasing the last element
    std::list<GameObject> player2Missiles;
};

#endif // MULTIPLAYER_H

#ifndef MULTIPLAYER_H
#define MULTIPLAYER_H

#include <QWidget>
#include <QKeyEvent>
#include <QGraphicsView>
#include "sprite-loader.h"
#include "game-object.h"
#include "game.h"       //to access PlayerMovementDirection enum;

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
    void updateHitBoxes();
    void limitObjects();
    void deleteJunkMissiles();
    void deleteDeadObjects();
    void moveMissiles();
    bool haveCollision(QRect before, QRect after);  //before object is for authentication. due to our code the objects can be in the same posision unless they are the same object.
    void detectMissileCollision();
    void movePlayers();
    int getFPS() const;
    void clearGameObjects();
    void updateSidebar();
    void player2Winner();
    void player1Winner();
    ~MultiPlayer();

private slots:
    void keyPressEvent(QKeyEvent* event) override;
    void keyReleaseEvent(QKeyEvent* event) override;

    void on_pauseButton_clicked();

    void on_menuButton_clicked();

    void on_restartButton_clicked();

private:
    Ui::MultiPlayer *ui;
    SpriteLoader* spriteLoader;     //we read files once to improve code performance critically
    int FPS = 60;
    const int WIDTH = 800;
    const int HEIGHT = 640;
    const int cellSize = 32;
    QGraphicsScene* scene = nullptr;
    QGraphicsView* backgroundView = nullptr;
    GameObject player1, player2, flag1, flag2;
    std::list<GameObject> walls;
    std::list<GameObject> player1Missiles;     //QList has a bug when we are erasing the last element
    std::list<GameObject> player2Missiles;
    PlayerMovementDirection player1Direction = NONE;
    PlayerMovementDirection player2Direction = NONE;
};

#endif // MULTIPLAYER_H

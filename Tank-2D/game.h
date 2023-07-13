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
    explicit Game(int currentLevel,QWidget *parent = nullptr);
    void loadLevel(int level);
    void loadIcon();
    void clear();
    void updateLogic();
    void limitObjects();
    void deleteJunkMissiles();
    void deleteDeadObjects();
    void render();
    void updateHitBoxes();
    void randomMovementsOfTanks();
    void updateSprites();
    void moveMissiles();
    void tanksShooting();
    bool haveCollision(QRect before, QRect after);  //before object is for authentication. due to our code the objects can be in the same posision unless they are the same object.
    void detectMissileCollision();
    void detectBonusCollision();
    void gameOver();
    void checkWinning();
    void winner();
    void spawnTanks();
    void clearGameObjects();
    ~Game();

    int getFPS() const;

protected:
    void keyPressEvent(QKeyEvent* event) override;

private slots:
    void on_continueButton_clicked();

private:
    Ui::Game *ui;
    SpriteLoader* spriteLoader;     //we read files once to improve code performance critically
    int FPS = 60;
    int timeStopCounter = 0;
    int numberOfTanks = 0;  //tanks which are in the scene already
    int remainingTanks = 0;    //tanks which are now spawned yet
    const int cellSize = 32;
    int currentLevel;
    QGraphicsScene* scene = nullptr;
    QGraphicsView* backgroundView = nullptr;
    GameObject player;
    GameObject flag;
    std::list<GameObject> tanks;
    std::list<GameObject> walls;
    std::list<GameObject> bonus;
    std::list<GameObject> missiles;     //QList has a bug when we are erasing the last element
    std::list<GameObject> enemyMissiles;
    std::list<QRect> spawnPoints;
};

#endif // GAME_H

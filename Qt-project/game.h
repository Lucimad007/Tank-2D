#ifndef GAME_H
#define GAME_H
#include <QVector>
#include <QWidget>
#include <QGraphicsScene>
#include "game-object.h"
#include "sprite-loader.h"
#include "user.h"

enum PlayerMovementDirection {MOVE_UP, MOVE_DOWN , MOVE_LEFT, MOVE_RIGHT, NONE};
enum LevelState {SINGLE_PLAYER_LEVEL, CUSTOM_LEVEL, NONE_LEVEL};

namespace Ui {
class Game;
}

class Game : public QWidget
{
    Q_OBJECT

public:
    explicit Game(int currentLevel,QWidget *parent = nullptr);
    explicit Game(QString levelName,QWidget *parent = nullptr);
    void loadLevel(int level);
    void loadLevel(QString name);
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
    void movePlayer();
    void clearGameObjects();
    void updateSidebar();
    ~Game();

    int getFPS() const;

    bool getHasClock() const;
    void setHasClock(bool newHasClock);

    bool getHasStar() const;
    void setHasStar(bool newHasStar);

    bool getHasTanki() const;
    void setHasTanki(bool newHasTanki);

protected:
    void keyPressEvent(QKeyEvent* event) override;
    void keyReleaseEvent(QKeyEvent* event) override;

private slots:
    void on_continueButton_clicked();

    void on_pauseButton_clicked();

    void on_menuButton_clicked();

    void on_restartButton_clicked();

private:
    Ui::Game *ui;
    QWidget* originalUI = nullptr;
    SpriteLoader* spriteLoader;     //we read files once to improve code performance critically
    int FPS = 60;
    const int WIDTH = 800;
    const int HEIGHT = 640;
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
    std::list<GameObject> explosions;
    std::list<QRect> spawnPoints;
    User user;
    int score = 0;
    PlayerMovementDirection playerDirection = NONE;
    LevelState levelState = NONE_LEVEL;
    bool hasTanki = true;
    bool hasStar = true;
    bool hasClock = true;
    QString customLevelName = "";
};

#endif // GAME_H

#ifndef CONSTRUCTION_H
#define CONSTRUCTION_H

#include <QWidget>
#include <QGraphicsScene>
#include "sprite-loader.h"
#include "user.h"

namespace Ui {
class construction;
}

class Construction : public QWidget
{
    Q_OBJECT

public:
    explicit Construction(QWidget *parent = nullptr);
    void loadIcon();
    ~Construction();

private:
    Ui::construction *ui;
    SpriteLoader* spriteLoader;     //we read files once to improve code performance critically
    int FPS = 60;
    const int WIDTH = 800;
    const int HEIGHT = 640;
    int remainingTanks = 0;    //tanks which are now spawned yet
    const int cellSize = 32;
    QGraphicsScene* scene = nullptr;
    QGraphicsView* backgroundView = nullptr;
    User user;
    QString blocks[25][20];
};

#endif // CONSTRUCTION_H

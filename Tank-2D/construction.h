#ifndef CONSTRUCTION_H
#define CONSTRUCTION_H

#include <QWidget>
#include <QGraphicsScene>
#include "sprite-loader.h"
#include "user.h"
#include "game-object.h"

namespace Ui {
class construction;
}

class Construction : public QWidget
{
    Q_OBJECT

public:
    explicit Construction(QWidget *parent = nullptr);
    void loadIcon();
    void saveToFile(QString name);
    void inline setDefaultCursor();
    ~Construction();

    GameObject getCurrentObject() const;
    void setCurrentObject(GameObject newCurrentObject);

protected:
    void mousePressEvent(QMouseEvent* event) override;

private slots:
    void on_saveButton_clicked();

    void on_menuButton_clicked();

    void on_tankiCheckBox_stateChanged(int arg1);

    void on_starCheckBox_stateChanged(int arg1);

    void on_clockCheckBox_stateChanged(int arg1);

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
    GameObject currentObject;
};

#endif // CONSTRUCTION_H

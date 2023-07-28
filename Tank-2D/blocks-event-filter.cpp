#include "blocks-event-filter.h"
#include "sprite-loader.h"
#include "construction.h"
#include <QMouseEvent>
#include <QApplication>

extern QApplication* app;
extern Construction* construction;

BlocksEventFilter::BlocksEventFilter()
{

}

bool BlocksEventFilter::eventFilter(QObject* obj, QEvent* event){
    SpriteLoader spriteLoader;
    QGraphicsView* view = static_cast<QGraphicsView*>(obj);
    if (event->type() == QEvent::MouseButtonPress)
    {
        // Mouse button press event
        QMouseEvent* mouseEvent = static_cast<QMouseEvent*>(event);
        if (mouseEvent->button() == Qt::LeftButton)
        {
            if(view->objectName() == "playerView"){
                QCursor cursor(spriteLoader.getYellow_tank_up());
                app->setOverrideCursor(cursor);
                construction->setCurrentObject(GameObject(PLAYER,spriteLoader.getYellow_tank_up()));
            } else if(view->objectName() == "flagView"){
                QCursor cursor(spriteLoader.getFlag());
                app->setOverrideCursor(cursor);
                construction->setCurrentObject(GameObject(FLAG,spriteLoader.getFlag()));
            } else if(view->objectName() == "randomCommonTankView"){
                QCursor cursor(spriteLoader.getRandom_tank_up());
                app->setOverrideCursor(cursor);
                construction->setCurrentObject(GameObject(RANDOM_TANK,spriteLoader.getRandom_tank_up()));
            } else if(view->objectName() == "commonTankView"){
                QCursor cursor(spriteLoader.getCommon_tank_up());
                app->setOverrideCursor(cursor);
                construction->setCurrentObject(GameObject(COMMON_TANK,spriteLoader.getCommon_tank_up()));
            } else if(view->objectName() == "randomArmoredTankView"){
                QCursor cursor(spriteLoader.getArmored_random_tank_up());
                app->setOverrideCursor(cursor);
                construction->setCurrentObject(GameObject(ARMORED_RANDOM_TANK,spriteLoader.getArmored_random_tank_up()));
            } else if(view->objectName() == "armoredTankView"){
                QCursor cursor(spriteLoader.getArmored_tank_up());
                app->setOverrideCursor(cursor);
                construction->setCurrentObject(GameObject(ARMORED_TANK,spriteLoader.getArmored_tank_up()));
            } else if(view->objectName() == "brickView"){
                QCursor cursor(spriteLoader.getBrick());
                app->setOverrideCursor(cursor);
                construction->setCurrentObject(GameObject(BRICK,spriteLoader.getBrick()));
            } else if(view->objectName() == "waterView"){
                QCursor cursor(spriteLoader.getWater());
                app->setOverrideCursor(cursor);
                construction->setCurrentObject(GameObject(WATER,spriteLoader.getWater()));
            } else if(view->objectName() == "stoneView"){
                QCursor cursor(spriteLoader.getStone());
                app->setOverrideCursor(cursor);
                construction->setCurrentObject(GameObject(STONE,spriteLoader.getStone()));
            }
        }
    }
    return QObject::eventFilter(obj, event);
}

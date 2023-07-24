#include "blocks-event-filter.h"
#include "sprite-loader.h"
#include <QMouseEvent>
#include <QApplication>

extern QApplication* app;

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
            } else if(view->objectName() == "flagView"){
                QCursor cursor(spriteLoader.getFlag());
                app->setOverrideCursor(cursor);
            } else if(view->objectName() == "randomCommonTankView"){
                QCursor cursor(spriteLoader.getRandom_tank_up());
                app->setOverrideCursor(cursor);
            } else if(view->objectName() == "commonTankView"){
                QCursor cursor(spriteLoader.getCommon_tank_up());
                app->setOverrideCursor(cursor);
            } else if(view->objectName() == "randomArmoredTankView"){
                QCursor cursor(spriteLoader.getArmored_random_tank_up());
                app->setOverrideCursor(cursor);
            } else if(view->objectName() == "armoredTankView"){
                QCursor cursor(spriteLoader.getArmored_tank_up());
                app->setOverrideCursor(cursor);
            } else if(view->objectName() == "brickView"){
                QCursor cursor(spriteLoader.getBrick());
                app->setOverrideCursor(cursor);
            } else if(view->objectName() == "waterView"){
                QCursor cursor(spriteLoader.getWater());
                app->setOverrideCursor(cursor);
            } else if(view->objectName() == "stoneView"){
                QCursor cursor(spriteLoader.getStone());
                app->setOverrideCursor(cursor);
            }
        }
    }
    return QObject::eventFilter(obj, event);
}

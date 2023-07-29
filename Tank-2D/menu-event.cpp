#include "menu-event.h"
#include "register.h"
#include "multi-player.h"
#include "construction.h"
#include <QMouseEvent>
#include <QTimer>

extern Register* registerMenu;
extern MultiPlayer* multiPlayer;
extern QTimer *multiPlayerTimer;
extern Construction* construction;

MenuEvent::MenuEvent()
{

}

bool MenuEvent::eventFilter(QObject* obj, QEvent* event){
    QGraphicsView* view = static_cast<QGraphicsView*>(obj);
    if (event->type() == QEvent::MouseButtonPress)
    {
        // Mouse button press event
        QMouseEvent* mouseEvent = static_cast<QMouseEvent*>(event);
        if (mouseEvent->button() == Qt::LeftButton)
        {
            if(view->objectName() == "player1View")
            {
                registerMenu->setSelectLevelUI();
            } else if(view->objectName() == "player2View")
            {
                multiPlayer = new MultiPlayer();
                registerMenu->hide();
                multiPlayer->show();
                multiPlayerTimer = new QTimer();
                QTimer::connect(multiPlayerTimer, &QTimer::timeout, [&](){
                    multiPlayer->clear();
                    multiPlayer->updateLogic();
                    multiPlayer->render();
                });
                multiPlayerTimer->start(((float)1/multiPlayer->getFPS() * 1000));
            } else if(view->objectName() == "constructionView")
            {
                registerMenu->setConstructUI();
                registerMenu->loadCustomLevelPrototypes();
            }
        }
    }

    return QObject::eventFilter(obj, event);
}

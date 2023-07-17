#include "menu_event.h"
#include "register.h"
#include "multi-player.h"
#include <QMouseEvent>

extern Register* registerMenu;
extern MultiPlayer* multiPlayer;

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
            } else if(view->objectName() == "constructionView")
            {
                qDebug() << "construction mode";
            }
        }
    }

    return QObject::eventFilter(obj, event);
}

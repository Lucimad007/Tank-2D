#ifndef MENUEVENT_H
#define MENUEVENT_H
#include <QGraphicsView>

class MenuEvent  : public QGraphicsView
{
public:
    MenuEvent();
protected:
    bool eventFilter(QObject* obj, QEvent* event) override ;
};

#endif // MENUEVENT_H

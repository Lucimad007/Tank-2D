#ifndef BLOCKSEVENTFILTER_H
#define BLOCKSEVENTFILTER_H
#include <QGraphicsView>

class BlocksEventFilter : public QGraphicsView
{
public:
    BlocksEventFilter();
protected:
    bool eventFilter(QObject* obj, QEvent* event) override ;
};

#endif // BLOCKSEVENTFILTER_H

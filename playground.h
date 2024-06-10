#ifndef PLAYGROUND_H
#define PLAYGROUND_H
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QVector>
#include "Card.h"

class PlayGround : public QGraphicsView
{
    Q_OBJECT
public:
    PlayGround(QWidget *parent = nullptr);
private:
    QGraphicsScene* display;
    QVector<Card*> cards;

    void createCards();
};

#endif // PLAYGROUND_H

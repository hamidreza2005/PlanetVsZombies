#ifndef PLAYGROUND_H
#define PLAYGROUND_H

#include <QGraphicsView>
#include <QGraphicsScene>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QWidget>
#include "Card.h"

class PlayGround : public QWidget
{
    Q_OBJECT
public:
    explicit PlayGround(QWidget *parent = nullptr);
private:
    QGraphicsView* graphicsView;
    QGraphicsScene* scene;
    QVector<Card*> cards;
    QLabel* playerName;
    QLabel* brainCount;
    QLabel* remainingTime;

    void createCards();
    void setupPlayerInfo();
    void setupLayout();
};

#endif // PLAYGROUND_H

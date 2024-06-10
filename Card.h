//
// Created by hrhas on 6/10/2024.
//

#ifndef PLANETVSZOMBIES_CARD_H
#define PLANETVSZOMBIES_CARD_H
#include "QGraphicsPixmapItem"
#include "QObject"

class Card : public QObject,public QGraphicsPixmapItem{
    Q_OBJECT
public:
    explicit Card(int width = 100,int height = 100);
    void setImage(const QString& src);
private:
    int width = 100;
    int height = 100;
    void setBorders();
};


#endif //PLANETVSZOMBIES_CARD_H

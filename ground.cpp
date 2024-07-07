#include "ground.h"
#include <QPixmap>

Ground::Ground(QObject *parent)
    : QGraphicsPixmapItem(), QObject(parent)
{
    QPixmap image_ground(":/resources/images/field.png");
    QPixmap Scaled_image_ground = image_ground.scaled(1080,540,Qt::IgnoreAspectRatio,Qt::SmoothTransformation);
    setPixmap(Scaled_image_ground);
}

double Ground::getYForNewEntity(int y) {
    double finalY;

    if (y <= 0) {
        finalY = 0;
    }
    else if (y <= 78) {
        finalY = 77.6;
    }
    else if (y <= 156) {
        finalY = 155.2;
    }
    else if (y <= 233) {
        finalY = 232.8;
    }
    else if (y <= 311) {
        finalY = 310.4;
    }
    else if (y <=389) {
        finalY = 388;
    }
    else{
        finalY = 465;
    }
    finalY -= 77.6;

    return finalY;
}

double Ground::getXForPlants(int x) {
    double finalX;

    if (x <= 30) {
        finalX = 0;
    }
    else if (x <= 109) {
        finalX = 70;
    }
    else if (x <= 184) {
        finalX = 146.5;
    }
    else if (x <= 262) {
        finalX = 223;
    }
    else if (x <= 339) {
        finalX = 300.5;
    }
    else if (x <=417) {
        finalX = 378;
    }
    else{
        finalX = 453.5;
    }
    finalX -= 207;
    return finalX;
}
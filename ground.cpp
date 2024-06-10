#include "ground.h"
#include <QPixmap>

Ground::Ground(QObject *parent)
    : QGraphicsPixmapItem()
{
    QPixmap image_ground(":/resources/images/field.png");
    QPixmap Scaled_image_ground = image_ground.scaled(1080,540,Qt::IgnoreAspectRatio,Qt::SmoothTransformation);
    setPixmap(Scaled_image_ground);
}

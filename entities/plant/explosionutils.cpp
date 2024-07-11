#include "explosionutils.h"

void showExplosionImage(QGraphicsScene* scene, QPointF position) {
    QGraphicsPixmapItem* explosionImage = new QGraphicsPixmapItem(QPixmap(":/resources/images/fireshooter.png").scaled(80, 80, Qt::KeepAspectRatio, Qt::SmoothTransformation));
    explosionImage->setPos(position);
    scene->addItem(explosionImage);

    QTimer::singleShot(3000, [scene, explosionImage]() {
        scene->removeItem(explosionImage);
        delete explosionImage;
    });
}

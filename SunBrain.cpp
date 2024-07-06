#include "SunBrain.h"
#include <QGraphicsSceneMouseEvent>
#include <QPropertyAnimation>
#include <QRandomGenerator>
#include <QGraphicsScene>
#include <QCursor>

SunBrain::SunBrain(const QString &imagePath, int value, QObject *parent)
    : QObject(parent), QGraphicsPixmapItem(nullptr), value(value) {
    setPixmap(QPixmap(imagePath).scaled(50, 50, Qt::KeepAspectRatio, Qt::SmoothTransformation));
    setCursor(QCursor(Qt::PointingHandCursor));


    int fallSpeed = QRandomGenerator::global()->bounded(1000, 3000);
    QTimer::singleShot(fallSpeed, this, &SunBrain::fall);


    disappearTimer = new QTimer(this);
    connect(disappearTimer, &QTimer::timeout, this, &SunBrain::disappear);
    disappearTimer->start(5000); // 5 seconds
}

void SunBrain::fall() {
    int x = pos().x();
    int y = pos().y();
    int endY = QRandomGenerator::global()->bounded(0, 450);

    QPropertyAnimation *animation = new QPropertyAnimation(this, "y");
    animation->setDuration(2000);
    animation->setStartValue(y);
    animation->setEndValue(endY);
    animation->start(QAbstractAnimation::DeleteWhenStopped);
}

void SunBrain::disappear() {
    scene()->removeItem(this);
    delete this;
}

void SunBrain::mousePressEvent(QGraphicsSceneMouseEvent *event) {
    Q_UNUSED(event)
    emit collected(value);
    scene()->removeItem(this);
    delete this;
}

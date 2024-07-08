#include "SunBrain.h"
#include <QGraphicsSceneMouseEvent>
#include <QPropertyAnimation>
#include <QRandomGenerator>
#include <QGraphicsScene>
#include <QCursor>
#include <QGraphicsView>
#include <QWidget>

SunBrain::SunBrain(const QString &imagePath, int value, QProgressBar *targetBar, bool isZombieMode, QObject *parent)
    : QObject(parent), QGraphicsPixmapItem(nullptr), value(value), targetBar(targetBar), isZombieMode(isZombieMode) {
    setPixmap(QPixmap(imagePath).scaled(50, 50, Qt::KeepAspectRatio, Qt::SmoothTransformation));
    setCursor(QCursor(Qt::PointingHandCursor));

    disappearTimer = new QTimer(this);
    connect(disappearTimer, &QTimer::timeout, this, &SunBrain::disappear);
    disappearTimer->start(5000); // 5 seconds
}

void SunBrain::fall() {
    int endY = QRandomGenerator::global()->bounded(100, 400);

    QPropertyAnimation *animation = new QPropertyAnimation(this, "pos");
    animation->setDuration(QRandomGenerator::global()->bounded(1000, 3000));
    animation->setStartValue(pos());
    animation->setEndValue(QPointF(pos().x(), endY));
    animation->start(QAbstractAnimation::DeleteWhenStopped);
}

void SunBrain::disappear() {
    scene()->removeItem(this);
    delete this;
}

void SunBrain::mousePressEvent(QGraphicsSceneMouseEvent *event) {
    Q_UNUSED(event)
    emit collected(value);

    QPropertyAnimation* moveAnimation = new QPropertyAnimation(this, "pos");
    moveAnimation->setDuration(500);

    QPointF targetPos = targetBar->mapToGlobal(targetBar->rect().center());
    QGraphicsView* view = scene()->views().first();
    QPoint targetViewPos = view->mapFromGlobal(targetPos.toPoint());
    QPointF targetScenePos = view->mapToScene(targetViewPos);

    moveAnimation->setEndValue(targetScenePos);
    connect(moveAnimation, &QPropertyAnimation::finished, this, &SunBrain::disappear);
    moveAnimation->start(QAbstractAnimation::DeleteWhenStopped);
}

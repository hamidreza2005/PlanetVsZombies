#include "SunBrain.h"
#include <QGraphicsSceneMouseEvent>
#include <QRandomGenerator>
#include <QGraphicsScene>
#include <QCursor>
#include <QGraphicsView>

SunBrain::SunBrain(const QString &imagePath, int value, QProgressBar *targetBar, bool isZombieMode, QGraphicsItem *rotatingItem, QObject *parent)
    : QObject(parent), QGraphicsPixmapItem(nullptr), value(value), targetBar(targetBar), isZombieMode(isZombieMode), rotatingItem(rotatingItem), isCollected(false) {
    setPixmap(QPixmap(imagePath).scaled(50, 50, Qt::KeepAspectRatio, Qt::SmoothTransformation));
    setCursor(QCursor(Qt::PointingHandCursor));

    calculateTargetPosition();

    disappearTimer = new QTimer(this);
    connect(disappearTimer, &QTimer::timeout, this, &SunBrain::disappear);

    fallTimer = new QTimer(this);
    connect(fallTimer, &QTimer::timeout, this, &SunBrain::fall);

    colorizeTimer = new QTimer(this);
    connect(colorizeTimer, &QTimer::timeout, this, &SunBrain::colorizeRed);

    delayTimer = new QTimer(this);
    connect(delayTimer, &QTimer::timeout, this, &SunBrain::startTimers);

    delayTimer->start(3000);

    colorEffect = new QGraphicsColorizeEffect();
}

void SunBrain::calculateTargetPosition() {
    int x = QRandomGenerator::global()->bounded(isZombieMode ? 500 : 200, isZombieMode ? 700 : 400);
    int y = QRandomGenerator::global()->bounded(100, 400);
    targetPosition = QPointF(x, y);
}

void SunBrain::fall() {
    if (y() < targetPosition.y()) {
        setY(y() + 2);
    } else {
        fallTimer->stop();
        emit landed(); // Emit landed signal
        disappearTimer->start(5000);
        colorizeTimer->start(2000);
    }
}

void SunBrain::disappear() {
    scene()->removeItem(this);
    delete this;
}

void SunBrain::mousePressEvent(QGraphicsSceneMouseEvent *event) {
    Q_UNUSED(event)
    isCollected = true;
    emit collected(value);

    moveTimer = new QTimer(this);
    connect(moveTimer, &QTimer::timeout, this, &SunBrain::moveToTarget);
    moveTimer->start(15);
}

void SunBrain::moveToTarget() {
    if (!isCollected) return;

    QPointF targetPos = QPointF(480, 490); // Updated to move to specific coordinates
    if (pos().x() < targetPos.x()) setX(x() + 3);
    if (pos().y() < targetPos.y()) setY(y() + 10);
    if (pos().x() >= targetPos.x() && pos().y() >= targetPos.y()) {
        moveTimer->stop();
        disappear();
    }
}

void SunBrain::colorizeRed() {
    colorEffect->setColor(Qt::red);
    setGraphicsEffect(colorEffect);
    colorizeTimer->stop();
}

void SunBrain::startTimers() {
    fallTimer->start(30);
    delayTimer->stop();
}

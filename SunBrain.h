#ifndef PLANETVSZOMBIES_SUNBRAIN_H
#define PLANETVSZOMBIES_SUNBRAIN_H

#include <QGraphicsPixmapItem>
#include <QObject>
#include <QTimer>
#include <QProgressBar>
#include <QGraphicsColorizeEffect>

class SunBrain : public QObject, public QGraphicsPixmapItem {
    Q_OBJECT
public:
    explicit SunBrain(const QString &imagePath, int value, QProgressBar *targetBar, bool isZombieMode, QGraphicsItem *rotatingItem, QObject *parent = nullptr);

signals:
    void collected(int value);
    void landed();

public slots:
    void fall();
    void disappear();
    void moveToTarget();
    void startTimers();
    void colorizeRed();

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;

private:
    int value;
    QTimer *disappearTimer;
    QTimer *fallTimer;
    QTimer *moveTimer;
    QTimer *colorizeTimer;
    QTimer *delayTimer;
    QProgressBar *targetBar;
    bool isZombieMode;
    bool isCollected;
    QPointF targetPosition;
    QGraphicsItem *rotatingItem;
    QGraphicsColorizeEffect *colorEffect;

    void calculateTargetPosition();
};

#endif // PLANETVSZOMBIES_SUNBRAIN_H

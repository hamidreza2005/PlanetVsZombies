#ifndef PLANETVSZOMBIES_SUNBRAIN_H
#define PLANETVSZOMBIES_SUNBRAIN_H

#include <QGraphicsPixmapItem>
#include <QObject>
#include <QTimer>
#include <QProgressBar>

class SunBrain : public QObject, public QGraphicsPixmapItem {
    Q_OBJECT
public:
    explicit SunBrain(const QString &imagePath, int value, QProgressBar *targetBar, bool isZombieMode, QObject *parent = nullptr);

signals:
    void collected(int value);

public slots:
    void fall();
    void disappear();

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;

private:
    int value;
    QTimer *disappearTimer;
    QProgressBar *targetBar;
    bool isZombieMode;
};

#endif // PLANETVSZOMBIES_SUNBRAIN_H

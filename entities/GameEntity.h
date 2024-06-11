#ifndef PLANETVSZOMBIES_GAMEENTITY_H
#define PLANETVSZOMBIES_GAMEENTITY_H

#include "QObject"
#include "QGraphicsPixmapItem"

class GameEntity: public QObject,public QGraphicsPixmapItem{
    Q_OBJECT
    Q_PROPERTY(qreal x READ x WRITE setX)
public:
    explicit GameEntity(int health);

    int getHealth() const;
    void setHealth(int newHealth);

    // Abstract Methods
    virtual QString getName() = 0;
    [[nodiscard]] virtual QString getPicturePath() const = 0;
    virtual ~GameEntity() = default;

protected:
    int health;

    void setImage();
};


#endif //PLANETVSZOMBIES_GAMEENTITY_H

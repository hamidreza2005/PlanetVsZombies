#ifndef PLANETVSZOMBIES_GAMEENTITY_H
#define PLANETVSZOMBIES_GAMEENTITY_H

#include "QObject"
#include "QGraphicsPixmapItem"

class GameEntity: public QObject, public QGraphicsPixmapItem {
    Q_OBJECT
    Q_PROPERTY(qreal x READ x WRITE setX)
public:
    explicit GameEntity(int health, int cost);

    int getHealth() const;
    int getCost() const;
    void setHealth(int newHealth);
    void setCost(int newCost);

    virtual QString getName() = 0;
    [[nodiscard]] virtual QString getPicturePath() const = 0;
    [[nodiscard]] virtual QString getCardPicturePath() const = 0;

    virtual ~GameEntity() = default;

protected:
    int health;
    int cost;
    void setImage();
};

#endif //PLANETVSZOMBIES_GAMEENTITY_H

#include "playground.h"
#include "ground.h"

PlayGround::PlayGround(QWidget *parent)
    : QGraphicsView(parent) {
    this->setFixedSize(900,600);
    auto* ground = new Ground();
    display = new QGraphicsScene(this);
    setScene(display);
    setSceneRect(0, 0, 850, 550);
    display->setBackgroundBrush(QBrush(Qt::white));
    display->addItem(ground);
    this->createCards();
}


void PlayGround::createCards(){
    QVector<QString> zombiePathes = {
        ":/resources/images/zombies/astronaut zombie.png",
        ":/resources/images/zombies/tall zombie.png",
        ":/resources/images/zombies/Bucket head zombie.png",
        ":/resources/images/zombies/leaf hair zombie.png",
        ":/resources/images/zombies/regular zombie.png",
        ":/resources/images/zombies/purple hair zombie.png",
    };

    QVector<QString> plantPathes = {
        ":/resources/images/plants/boomrang.png",
        ":/resources/images/plants/jalapino.png",
        ":/resources/images/plants/peashooter.png",
        ":/resources/images/plants/plum mine.png",
        ":/resources/images/plants/two_peashooter.png",
        ":/resources/images/plants/walnut.png",
    };

    for(int i = 0; i<6; i++){
        auto* card = new Card();
        card->setImage(zombiePathes[i]);
        card->setX((i+1)*110);
        card->setY(450);
        this->cards.push_back(card);
        scene()->addItem(card);
    }
}

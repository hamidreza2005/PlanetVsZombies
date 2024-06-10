#include "playground.h"
#include "ground.h"
#include <QFont>

PlayGround::PlayGround(QWidget *parent) : QWidget(parent) {
    this->setFixedSize(1000, 700);

    graphicsView = new QGraphicsView(this);
    graphicsView->setFixedSize(1000, 500);
    scene = new QGraphicsScene(this);
    graphicsView->setScene(scene);
    graphicsView->setSceneRect(0, 0, 650, 450);

    auto* ground = new Ground();
    ground->setPos(-260, -40);
    scene->addItem(ground);

    createCards();

    setupPlayerInfo();
    setupLayout();
}

void PlayGround::setupPlayerInfo() {
    playerName = new QLabel("Player Name: ", this);
    brainCount = new QLabel("Brain: 0", this);
    remainingTime = new QLabel("Time: 00:00", this);

    QFont font("Arial", 10, QFont::Bold);
    playerName->setFont(font);
    brainCount->setFont(font);
    remainingTime->setFont(font);

    playerName->setStyleSheet("QLabel { color : blue; }");
    brainCount->setStyleSheet("QLabel { color : green; }");
    remainingTime->setStyleSheet("QLabel { color : red; }");
}

void PlayGround::setupLayout() {
    QVBoxLayout* mainLayout = new QVBoxLayout(this);

    QHBoxLayout* infoLayout = new QHBoxLayout();
    infoLayout->addWidget(playerName);
    infoLayout->addWidget(brainCount);
    infoLayout->addWidget(remainingTime);

    mainLayout->addLayout(infoLayout, 1);

    mainLayout->addWidget(graphicsView, 4);

    QGraphicsView* cardView = new QGraphicsView(this);
    QGraphicsScene* cardScene = new QGraphicsScene(this);
    cardView->setScene(cardScene);
    cardView->setFixedHeight(150);

    mainLayout->addWidget(cardView, 1);

    setLayout(mainLayout);

    for (auto* card : cards) {
        cardScene->addItem(card);
    }
}

void PlayGround::createCards() {
    QVector<QString> zombiePathes = {
        ":/resources/images/zombies/astronaut zombie.png",
        ":/resources/images/zombies/tall zombie.png",
        ":/resources/images/zombies/Bucket head zombie.png",
        ":/resources/images/zombies/leaf hair zombie.png",
        ":/resources/images/zombies/regular zombie.png",
        ":/resources/images/zombies/purple hair zombie.png",
    };

    for(int i = 0; i < 6; i++) {
        auto* card = new Card();
        card->setImage(zombiePathes[i]);
        card->setPos(i * 150, 0);
        cards.push_back(card);
    }
}

// QVector<QString> plantPathes = {
//     ":/resources/images/plants/boomrang.png",
//     ":/resources/images/plants/jalapino.png",
//     ":/resources/images/plants/peashooter.png",
//     ":/resources/images/plants/plum mine.png",
//     ":/resources/images/plants/two_peashooter.png",
//     ":/resources/images/plants/walnut.png",
// };

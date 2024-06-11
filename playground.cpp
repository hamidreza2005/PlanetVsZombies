#include "playground.h"
#include "ground.h"
#include <QFont>
#include <cstdlib>
#include <ctime>
#include "entities/zombie/RegularZombie.h"
#include "entities/plant/PeaShooter.h"

PlayGround::PlayGround(QWidget *parent) : QWidget(parent) {
    srand(static_cast<unsigned int>(time(0)));
   // isZombie = rand() % 2 == 0;
    isZombie =true;

    this->setFixedSize(1000, 700);

    graphicsView = new QGraphicsView(this);
    graphicsView->setFixedSize(1000, 500);
    scene = new QGraphicsScene(this);
    graphicsView->setScene(scene);
    graphicsView->setSceneRect(0, 0, 650, 450);

    setupGround();
    if (isZombie) {
        createZombieCards();
        setupPlayerZombieInfo();
    } else {
        createPlantCards();
        setupPlayerPlantInfo();
    }

    setupLayout();

    auto* z1 = new RegularZombie();
    z1->setPos(200, 153);
    scene->addItem(z1);

    auto* p1 = new PeaShooter();
    p1->setPos(50,158);
    scene->addItem(p1);
}

void PlayGround::setupPlayerZombieInfo() {
    playerZombieName = new QLabel("zombie Player: ", this);
    brainCount = new QLabel("Brains: 0", this);
    remainingZombieTime = new QLabel("Time: 00:00", this);

    QFont font("Arial", 10, QFont::Bold);
    playerZombieName->setFont(font);
    brainCount->setFont(font);
    remainingZombieTime->setFont(font);

    playerZombieName->setStyleSheet("QLabel { color : blue; }");
    brainCount->setStyleSheet("QLabel { color : green; }");
    remainingZombieTime->setStyleSheet("QLabel { color : red; }");
}

void PlayGround::setupPlayerPlantInfo() {
    playerPlantName = new QLabel("Plant Player: ", this);
    sunCount = new QLabel("Sun: 0", this);
    remainingPlantTime = new QLabel("Time: 00:00", this);

    QFont font("Arial", 10, QFont::Bold);
    playerPlantName->setFont(font);
    sunCount->setFont(font);
    remainingPlantTime->setFont(font);

    playerPlantName->setStyleSheet("QLabel { color : blue; }");
    sunCount->setStyleSheet("QLabel { color : green; }");
    remainingPlantTime->setStyleSheet("QLabel { color : red; }");
}

void PlayGround::setupGround() {
    this->ground = new Ground();
    ground->setPos(-260, -40);
    scene->addItem(ground);
}

void PlayGround::setupLayout() {
    QVBoxLayout* mainLayout = new QVBoxLayout(this);

    QHBoxLayout* infoLayout = new QHBoxLayout();
    if (isZombie) {
        infoLayout->addWidget(playerZombieName);
        infoLayout->addWidget(brainCount);
        infoLayout->addWidget(remainingZombieTime);
    } else {
        infoLayout->addWidget(playerPlantName);
        infoLayout->addWidget(sunCount);
        infoLayout->addWidget(remainingPlantTime);
    }

    mainLayout->addLayout(infoLayout, 1);
    mainLayout->addWidget(graphicsView, 4);

    QGraphicsView* cardView = new QGraphicsView(this);
    QGraphicsScene* cardScene = new QGraphicsScene(this);
    cardView->setScene(cardScene);
    cardView->setFixedHeight(150);

    mainLayout->addWidget(cardView, 1);
    setLayout(mainLayout);

    QVector<Card*>& cards = isZombie ? zombieCards : plantCards;
    for (auto* card : cards) {
        cardScene->addItem(card);
    }
}

void PlayGround::createZombieCards() {
    QVector<std::function<GameEntity*()>> zombies = {
            [](){return new RegularZombie;},
            [](){return new RegularZombie;},
            [](){return new RegularZombie;},
            [](){return new RegularZombie;},
            [](){return new RegularZombie;},
            [](){return new RegularZombie;},
    };

    for(int i = 0; i < 6; i++) {
        auto* card = new Card(zombies[i]);
        card->setPos(i * 150, 0);
        QObject::connect(card, &Card::createEntity, this, &PlayGround::onCreateEntity);
        zombieCards.push_back(card);
    }
}

void PlayGround::createPlantCards() {
    QVector<std::function<GameEntity*()>> plants = {
            [](){return new PeaShooter;},
            [](){return new PeaShooter;},
            [](){return new PeaShooter;},
            [](){return new PeaShooter;},
            [](){return new PeaShooter;},
            [](){return new PeaShooter;},
    };

    for(int i = 0; i < 6; i++) {
        auto* card = new Card(plants[i]);
        card->setPos(i * 150, 0);
        QObject::connect(card, &Card::createEntity, this, &PlayGround::onCreateEntity);
        plantCards.push_back(card);
    }
}

void PlayGround::onCreateEntity(Card *card) {
    auto* newEntity = card->getEntityFactory()();
    newEntity->setPos(300, 153);
    scene->addItem(newEntity);
}

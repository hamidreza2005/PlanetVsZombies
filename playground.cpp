#include "playground.h"
#include "ground.h"
#include <QFont>
#include <cstdlib>
#include <ctime>
#include "Zombie/RegularZombie.h"

PlayGround::PlayGround(QWidget *parent) : QWidget(parent), remainingSeconds(210), brainCount(0), sunCount(0) {
    srand(static_cast<unsigned int>(time(0)));
    isZombie = rand() % 2 == 0;

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
<<<<<<< Updated upstream
    } else {
=======
    }
    else {
        setupPlantGround();
>>>>>>> Stashed changes
        createPlantCards();
        setupPlayerPlantInfo();
    }

    setupLayout();

<<<<<<< Updated upstream
//    auto* z1 = new RegularZombie();
//    z1->setPos(200, 158);
//    scene->addItem(z1);
=======
    timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &PlayGround::updateTimer);
    timer->start(1000);
>>>>>>> Stashed changes
}

void PlayGround::setupPlayerZombieInfo() {
    playerZombieName = new QLabel("Zombie Player", this);
    //set name of the player instead of "Zombie Player"
    remainingZombieTime = new QLabel("Time: 03:30", this);
    brainBar = new QProgressBar(this);
    brainBar->setRange(0, 100);
    brainBar->setValue(0);
    brainBar->setFormat("%v");

    QFont font("Arial", 10, QFont::Bold);
    playerZombieName->setFont(font);
    remainingZombieTime->setFont(font);
    brainBar->setFont(font);

    playerZombieName->setStyleSheet("QLabel { color : blue; }");
    remainingZombieTime->setStyleSheet("QLabel { color : green; }");
    brainBar->setStyleSheet("QProgressBar { text-align: center; } QProgressBar::chunk { background-color: red; }");
}

void PlayGround::setupPlayerPlantInfo() {
    playerPlantName = new QLabel("Plant Player", this);
    //set name of the player instead of "Plant Player"
    remainingPlantTime = new QLabel("Time: 03:30", this);
    sunBar = new QProgressBar(this);
    sunBar->setRange(0, 100);
    sunBar->setValue(0);
    sunBar->setFormat("%v");

    QFont font("Arial", 10, QFont::Bold);
    playerPlantName->setFont(font);
    remainingPlantTime->setFont(font);
    sunBar->setFont(font);

    playerPlantName->setStyleSheet("QLabel { color : blue; }");
    remainingPlantTime->setStyleSheet("QLabel { color : green; }");
    sunBar->setStyleSheet("QProgressBar { text-align: center; } QProgressBar::chunk { background-color: yellow; }");
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
        infoLayout->addWidget(brainBar);
        infoLayout->addWidget(remainingZombieTime);
    }
    else {
        infoLayout->addWidget(playerPlantName);
        infoLayout->addWidget(sunBar);
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
        zombieCards.push_back(card);
    }
}

void PlayGround::createPlantCards() {
    QVector<QString> plantPathes = {
        ":/resources/images/plants/boomrang.png",
        ":/resources/images/plants/jalapino.png",
        ":/resources/images/plants/peashooter.png",
        ":/resources/images/plants/plum mine.png",
        ":/resources/images/plants/two_peashooter.png",
        ":/resources/images/plants/walnut.png",
    };

    for(int i = 0; i < 6; i++) {
        auto* card = new Card();
        card->setImage(plantPathes[i]);
        card->setPos(i * 150, 0);
        plantCards.push_back(card);
    }
}

void PlayGround::updateTimer() {
    if (remainingSeconds > 0) {
        remainingSeconds--;
        int minutes = remainingSeconds / 60;
        int seconds = remainingSeconds % 60;
        QString timeString = QString("Time: %1:%2")
                                 .arg(minutes, 2, 10, QLatin1Char('0'))
                                 .arg(seconds, 2, 10, QLatin1Char('0'));
        if (isZombie) {
            remainingZombieTime->setText(timeString);
        }
        else {
            remainingPlantTime->setText(timeString);
        }
    }
    else {
        timer->stop();
    }
}

void PlayGround::updateBrainCount(int amount) {
    brainCount += amount;
    brainBar->setValue(brainCount / 5);
    brainBar->setFormat(QString("Brains: %1").arg(brainCount));
}

void PlayGround::updateSunCount(int amount) {
    sunCount += amount;
    sunBar->setValue(sunCount / 5);
    sunBar->setFormat(QString("Sun: %1").arg(sunCount));
}

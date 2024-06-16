#include "playground.h"
#include "ground.h"
#include <QFont>
#include <cstdlib>
#include <ctime>
#include <QGraphicsPixmapItem>
#include <QMouseEvent>
#include "entities/zombie/RegularZombie.h"
#include "entities/zombie/BucketHeadZombie.h"
#include "entities/zombie/TallZombie.h"
#include "entities/zombie/LeafHeadZombie.h"
#include "entities/zombie/PurpleHairZombie.h"
#include "entities/zombie/AstronautZombie.h"
#include "entities/plant/PeaShooter.h"

QVector<std::function<GameEntity*()>> PlayGround::zombies = {
    [](){return new RegularZombie;},
    [](){return new BucketHeadZombie;},
    [](){return new TallZombie;},
    [](){return new LeafHeadZombie;},
    [](){return new PurpleHairZombie;},
    [](){return new AstronautZombie;},
    };

QVector<std::function<GameEntity*()>> PlayGround::plants = {
    [](){return new PeaShooter;},
    [](){return new PeaShooter;},
    [](){return new PeaShooter;},
    [](){return new PeaShooter;},
    [](){return new PeaShooter;},
    [](){return new PeaShooter;},
    };


PlayGround::PlayGround(QWidget *parent) : QWidget(parent), remainingSeconds(210), brainCount(0), sunCount(0), selectedCard(nullptr) {
    srand(static_cast<unsigned int>(time(0)));
    //isZombie = rand() % 2 == 0;
    isZombie=true;

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

    graphicsView->viewport()->installEventFilter(this);

    timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &PlayGround::updateTimer);
    timer->start(1000);
}

void PlayGround::setupPlayerZombieInfo() {
    playerZombieName = new QLabel("Zombie Player", this);
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
    remainingZombieTime->setStyleSheet("QLabel { color : black; }");
    brainBar->setStyleSheet("QProgressBar { text-align: center; } QProgressBar::chunk { background-color: red; }");
}

void PlayGround::setupPlayerPlantInfo() {
    playerPlantName = new QLabel("Plant Player", this);
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
    remainingPlantTime->setStyleSheet("QLabel { color : black; }");
    sunBar->setStyleSheet("QProgressBar { text-align: center; } QProgressBar::chunk { background-color: yellow; }");
}

void PlayGround::setupGround() {
    auto* ground = new Ground();
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
    } else {
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
    for(int i = 0; i < 6; i++) {
        auto* card = new Card(this->zombies[i]);
        card->setPos(i * 150, 0);
        QObject::connect(card, &Card::selectEntity, this, &PlayGround::selectCard);
        zombieCards.push_back(card);
    }
}

void PlayGround::createPlantCards() {
    for(int i = 0; i < 6; i++) {
        auto* card = new Card(this->plants[i]);
        card->setPos(i * 150, 0);
        QObject::connect(card, &Card::selectEntity, this, &PlayGround::selectCard);
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
        } else {
            remainingPlantTime->setText(timeString);
        }
    } else {
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

void PlayGround::selectCard(Card *card) {
    if(this->selectedCard == card){
        this->selectedCard->unselect();
        this->selectedCard = nullptr;
        return;
    }

    if(this->selectedCard)
        this->selectedCard->unselect();
    this->selectedCard = card;
    this->selectedCard->select();
}

bool PlayGround::eventFilter(QObject* obj, QEvent* event) {
    if (event->type() == QEvent::MouseButtonPress) {
        auto* mouseEvent = dynamic_cast<QMouseEvent*>(event);
        addEntity(mouseEvent->pos());
    }
    return QWidget::eventFilter(obj, event);
}

void PlayGround::addEntity(QPointF point) {
    if (!selectedCard || this->isOutOfGround(&point)) {
        return;
    }

    int y = point.y();

    int finalX = 750;
    int finalY;

    if (y <= 0) {
        finalY = 0;
    }
    else if (y <= 78) {
        finalY = 77.6;
    }
    else if (y <= 156) {
        finalY = 155.2;
    }
    else if (y <= 233) {
        finalY = 232.8;
    }
    else if (y <= 311) {
        finalY = 310.4;
    }
    else if (y <=389) {
        finalY = 388;
    }
    else{
        finalY = 465;
    }
    finalY-=77.6;
    auto* newEntity = selectedCard->getEntityFactory()();
    newEntity->setPos(finalX, finalY);
    scene->addItem(newEntity);
}

bool PlayGround::isOutOfGround(const QPointF* point) {
    return point->x() < 485 || point->x() > 1000 || point->y() < -80 || point->y() > 470;
}

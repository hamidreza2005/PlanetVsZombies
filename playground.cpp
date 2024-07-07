#include "playground.h"
#include "ground.h"
#include <QFont>
#include <QGraphicsPixmapItem>
#include <QMouseEvent>
#include <QMessageBox>
#include "entities/zombie/RegularZombie.h"
#include "entities/zombie/BucketHeadZombie.h"
#include "entities/zombie/TallZombie.h"
#include "entities/zombie/LeafHeadZombie.h"
#include "entities/zombie/PurpleHairZombie.h"
#include "entities/zombie/AstronautZombie.h"
#include "entities/plant/PeaShooter.h"
#include "entities/plant/TwoPeaShooter.h"
#include "entities/plant/Boomrang.h"
#include "entities/plant/Jalapeno.h"
#include "entities/plant/Walnut.h"
#include "entities/plant/PlumMine.h"
#include "core/Cookie.h"

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
    [](){return new TwoPeaShooter;},
    [](){return new Walnut;},
    [](){return new Boomerang;},
    [](){return new Jalapeno;},
    [](){return new PlumMine;},
    };

PlayGround::PlayGround(ClientSocket* clientSocket,QWidget *parent) : Window(clientSocket,parent), remainingSeconds(210), brainCount(50), sunCount(50), selectedCard(nullptr) {
    this->setFixedSize(1000, 700);
    graphicsView = new QGraphicsView(this);
    graphicsView->setFixedSize(1000, 500);
    scene = new QGraphicsScene(this);
    graphicsView->setScene(scene);
    graphicsView->setSceneRect(0, 0, 650, 450);
    graphicsView->viewport()->installEventFilter(this);

    timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &PlayGround::updateTimer);
}

void PlayGround::play() {
    this->connectDataListener();
    connect(socket,&ClientSocket::connectionLost,this,&PlayGround::connectionLost);
    isZombie = Cookie::getInstance()->loggedInPlayer->getRole() == "zombie";
    if (isZombie) {
        createZombieCards();
        setupPlayerZombieInfo();
    } else {
        createPlantCards();
        setupPlayerPlantInfo();
    }
    setupLayout();
    this->setupGround();
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
    remainingZombieTime->setStyleSheet("QLabel { color : red; }");
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
    remainingPlantTime->setStyleSheet("QLabel { color : red; }");
    sunBar->setStyleSheet("QProgressBar { text-align: center; } QProgressBar::chunk { background-color: yellow; }");
}

void PlayGround::setupGround() {
    ground = new Ground(this);
    ground->setPos(-260, -40);
    scene->addItem(ground);
}

void PlayGround::setupLayout() {
    mainLayout = new QVBoxLayout(this);

    infoLayout = new QHBoxLayout(this);
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

    auto cardView = new QGraphicsView(this);
    auto cardScene = new QGraphicsScene(this);
    cardView->setScene(cardScene);
    cardView->setFixedHeight(150);

    mainLayout->addWidget(cardView, 1);
    setLayout(mainLayout);

    QVector<Card*>& cards = isZombie ? zombieCards : plantCards;
    for (auto* card : cards) {
        cardScene->addItem(card);
    }
}


void PlayGround::createPlantCards() {
    for (int i = 0; i < plants.size(); i++) {
        int cost;
        if (i == 0) cost = 100; // PeaShooter
        else if (i == 1) cost = 200; // TwoPeaShooter
        else if (i == 2) cost = 50; // Walnut
        else if (i == 3) cost = 125; // Boomerang
        else if (i == 4) cost = 150; // Jalapeno
        else cost = 175; // PlumMine

        auto* card = new Card(this->plants[i], 100, 100, cost);
        card->setPos(i * 150, 0);
        QObject::connect(card, &Card::selectEntity, this, &PlayGround::selectCard);
        plantCards.push_back(card);
        scene->addItem(card);
    }
}

void PlayGround::createZombieCards() {
    for (int i = 0; i < zombies.size(); i++) {
        int cost;
        if (i == 0) cost = 100; // RegularZombie
        else if (i == 1) cost = 200; // BucketHeadZombie
        else if (i == 2) cost = 150; // TallZombie
        else if (i == 3) cost = 120; // LeafHeadZombie
        else if (i == 4) cost = 180; // PurpleHairZombie
        else cost = 200; // AstronautZombie

        auto* card = new Card(this->zombies[i], 100, 100, cost);
        card->setPos(i * 150, 0);
        QObject::connect(card, &Card::selectEntity, this, &PlayGround::selectCard);
        zombieCards.push_back(card);
        scene->addItem(card);
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
    qDebug() << "it is not out of the ground";

    int y = point.y();
    int finalX, finalY;

    if(isZombie) {
        finalX = 750;
    } else {
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
        finalY -= 77.6;

        int x = point.x();
        if (x <= 30) {
            finalX = 0;
        }
        else if (x <= 109) {
            finalX = 70;
        }
        else if (x <= 184) {
            finalX = 146.5;
        }
        else if (x <= 262) {
            finalX = 223;
        }
        else if (x <= 339) {
            finalX = 300.5;
        }
        else if (x <=417) {
            finalX = 378;
        }
        else{
            finalX = 453.5;
        }
        finalX -= 207;

        qDebug() << "Zombie got added";
        QPointF finalPosition(finalX, finalY);
        if (isPositionOccupied(finalPosition)) {
            qDebug() << "Location is occupied";
            return;
        }

        qDebug() << "Zombie got added";
        auto* newEntity = selectedCard->getEntityFactory()();
        newEntity->setPos(finalX, finalY);
        scene->addItem(newEntity);
    }
}

bool PlayGround::isOutOfGround(const QPointF* point) {
    if(isZombie){
        return point->x() < 485 || point->x() > 1000 || point->y() < -80 || point->y() > 470;
    }
    else{
        return point->x() < 30 || point->x() > 490 || point->y() < -80 || point->y() > 470;
    }
}

bool PlayGround::isPositionOccupied(QPointF point) {
    QList<QGraphicsItem*> items = scene->items();
    for (QGraphicsItem* item : items) {
        if (dynamic_cast<Plant*>(item) && item->pos() == point) {
            qDebug() << "Position occupied by another plant.";
            return true;
        }
    }
    qDebug() << "Position available.";
    return false;
}

void PlayGround::connectDataListener() {
    dataListener = connect(socket, &ClientSocket::dataReceived, this, &PlayGround::handleServerResponse);
}

void PlayGround::handleServerResponse(const QJsonObject &data) {
    qDebug() << "hi";
    qDebug() << data;
    if (!data.contains("state")){
        return;
    }

    if(data.value("state") == "opponentLeft"){
        QMessageBox::critical(this, "Problem", "Your Opponent Left The game",
                              QMessageBox::Yes);
        this->endTheGame();
        return;
    }
}

void PlayGround::endTheGame() {
    emit this->goToDashboardPage(this);
}

void PlayGround::connectionLost() {
    QMessageBox::critical(this, "Problem", "Your Connection Got Lost",
                          QMessageBox::Yes);
    this->endTheGame();
}

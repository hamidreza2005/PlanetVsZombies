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
#include <QDebug>
#include <QRandomGenerator>
#include "core/Cookie.h"
#include <QGraphicsRotation>
#include <QGraphicsItemAnimation>
#include <QTimeLine>
#include <QPropertyAnimation>

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

PlayGround::PlayGround(ClientSocket* clientSocket, QWidget *parent)
    : Window(clientSocket, parent), remainingSeconds(210), brainCount(0), sunCount(0), selectedCard(nullptr) {
    this->setFixedSize(1000, 700);
    graphicsView = new QGraphicsView(this);
    graphicsView->setFixedSize(1000, 500);
    scene = new QGraphicsScene(this);
    graphicsView->setScene(scene);
    graphicsView->setSceneRect(0, 0, 650, 450);
    graphicsView->viewport()->installEventFilter(this);

    timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &PlayGround::updateTimer);

    cardStateTimer = new QTimer(this);
    connect(cardStateTimer, &QTimer::timeout, this, &PlayGround::checkCardStates);
}

void PlayGround::play() {
    this->connectDataListener();
    connect(socket, &ClientSocket::connectionLost, this, &PlayGround::connectionLost);
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

    sunBrainTimer = new QTimer(this);
    connect(sunBrainTimer, &QTimer::timeout, this, &PlayGround::spawnSunBrain);
    sunBrainTimer->start(10000);

    cardStateTimer->start(1000);
}

void PlayGround::setupPlayerZombieInfo() {
    playerZombieName = new QLabel("Zombie Player", this);
    remainingZombieTime = new QLabel("Time: 03:30", this);
    brainBar = new QProgressBar(this);
    brainBar->setRange(0, 1000);
    brainBar->setValue(0);
    brainBar->setFormat("%v");

    QFont font("Arial", 10, QFont::Bold);
    playerZombieName->setFont(font);
    remainingZombieTime->setFont(font);
    brainBar->setFont(font);

    brainBar->setFixedHeight(20);
    brainBar->setFixedWidth(200);

    playerZombieName->setStyleSheet("QLabel { color : blue; }");
    remainingZombieTime->setStyleSheet("QLabel { color : red; }");
    brainBar->setStyleSheet("QProgressBar { text-align: center; } QProgressBar::chunk { background-color: red; }");
}

void PlayGround::setupPlayerPlantInfo() {
    playerPlantName = new QLabel("Plant Player", this);
    remainingPlantTime = new QLabel("Time: 03:30", this);
    sunBar = new QProgressBar(this);
    sunBar->setRange(0, 1000);
    sunBar->setValue(0);
    sunBar->setFormat("%v");

    QFont font("Arial", 10, QFont::Bold);
    playerPlantName->setFont(font);
    remainingPlantTime->setFont(font);
    sunBar->setFont(font);

    sunBar->setFixedHeight(20);
    sunBar->setFixedWidth(200);

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

    int cardWidth = 100;
    int gapBetweenCards = 2;
    int rotatingImageSize = 100;

    QVector<Card*>& cards = isZombie ? zombieCards : plantCards;
    for (int i = 0; i < 3; ++i) {
        cards[i]->setPos(i * (cardWidth + gapBetweenCards), 0);
        cardScene->addItem(cards[i]);
    }

    setupRotatingImage(isZombie ? ":/resources/images/Brain.png" : ":/resources/images/sun.png", cardScene);

    for (int i = 3; i < 6; ++i) {
        cards[i]->setPos((i + 1) * (cardWidth + gapBetweenCards), 0);
        cardScene->addItem(cards[i]);
    }
}

void PlayGround::createZombieCards() {
    for (int i = 0; i < zombies.size(); i++) {
        int cost;
        if (i == 0) cost = 100;
        else if (i == 1) cost = 200;
        else if (i == 2) cost = 150;
        else if (i == 3) cost = 120;
        else if (i == 4) cost = 180;
        else cost = 200;

        auto* card = new Card(this->zombies[i], 100, 100, cost);
        card->setPos((i * 100) , 0);
        QObject::connect(card, &Card::selectEntity, this, &PlayGround::selectCard);
        zombieCards.push_back(card);
        scene->addItem(card);
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
        card->setPos((i * 100), 0);
        QObject::connect(card, &Card::selectEntity, this, &PlayGround::selectCard);
        plantCards.push_back(card);
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
    brainBar->setValue(brainCount);
    brainBar->setFormat(QString("Brains: %1").arg(brainCount));
}

void PlayGround::updateSunCount(int amount) {
    sunCount += amount;
    sunBar->setValue(sunCount);
    sunBar->setFormat(QString("Sun: %1").arg(sunCount));
}

void PlayGround::selectCard(Card *card) {
    if (this->selectedCard == card) {
        this->selectedCard->unselect();
        this->selectedCard = nullptr;
        return;
    }

    if (this->selectedCard)
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

    if (isZombie) {
        finalX = 750;
        if (y > 0 && y <= 78) {
            finalY = 77.6;
        } else if (y <= 156) {
            finalY = 155.2;
        } else if (y <= 233) {
            finalY = 232.8;
        } else if (y <= 311) {
            finalY = 310.4;
        } else if (y <= 389) {
            finalY = 388;
        } else {
            finalY = 465;
        }
        finalY -= 77.6;
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
        updateBrainCount(-selectedCard->getCost());

    } else {
        if (y > 0 && y <= 78) {
            finalY = 77.6;
        } else if (y <= 156) {
            finalY = 155.2;
        } else if (y <= 233) {
            finalY = 232.8;
        } else if (y <= 311) {
            finalY = 310.4;
        } else if (y <= 389) {
            finalY = 388;
        } else {
            finalY = 465;
        }
        finalY -= 77.6;

        int x = point.x();
        if (x > 29 && x <= 109) {
            finalX = 70;
        } else if (x <= 184) {
            finalX = 146.5;
        } else if (x <= 262) {
            finalX = 223;
        } else if (x <= 339) {
            finalX = 300.5;
        } else if (x <= 417) {
            finalX = 378;
        } else {
            finalX = 453.5;
        }
        finalX -= 207;

        QPointF finalPosition(finalX, finalY);
        if (isPositionOccupied(finalPosition)) {
            return;
        }

        auto* newEntity = selectedCard->getEntityFactory()();
        newEntity->setPos(finalX, finalY);
        scene->addItem(newEntity);
        updateSunCount(-selectedCard->getCost());
    }

    checkCardStates();
}

bool PlayGround::isOutOfGround(const QPointF* point) {
    if (isZombie) {
        return point->x() < 485 || point->x() > 1000 || point->y() < -80 || point->y() > 470;
    } else {
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

void PlayGround::spawnSunBrain() {
    QString imagePath = isZombie ? ":/resources/images/Brain.png" : ":/resources/images/sun.png";
    int value = 50;
    SunBrain *sunBrain = new SunBrain(imagePath, value, isZombie ? brainBar : sunBar, isZombie, rotatingItem);

    int x = QRandomGenerator::global()->bounded(isZombie ? 485 : 30, isZombie ? 750 : 490);
    int y = -100;

    sunBrain->setPos(x, y);

    connect(sunBrain, &SunBrain::collected, this, &PlayGround::collectSunBrain);
    connect(sunBrain, &SunBrain::landed, this, &PlayGround::handleLanded); // Handle when SunBrain lands
    scene->addItem(sunBrain);
}

void PlayGround::setupRotatingImage(const QString& imagePath, QGraphicsScene* cardScene) {
    rotatingItem = new QGraphicsPixmapItem(QPixmap(imagePath).scaled(100, 100, Qt::KeepAspectRatio, Qt::SmoothTransformation));
    rotatingItem->setTransformOriginPoint(rotatingItem->boundingRect().center());
    rotatingItem->setZValue(10);

    int xPos = 3 * (100 + 2);
    int yPos = 0;
    rotatingItem->setPos(xPos, yPos);

    QGraphicsRotation* rotation = new QGraphicsRotation;
    rotation->setAngle(0);
    rotation->setAxis(Qt::YAxis);
    rotation->setOrigin(QVector3D(rotatingItem->boundingRect().center().x(), rotatingItem->boundingRect().center().y(), 0));
    rotatingItem->setTransformations({rotation});
    cardScene->addItem(rotatingItem);

    auto rotationAnimation = new QPropertyAnimation(rotation, "angle");
    rotationAnimation->setDuration(2000);
    rotationAnimation->setStartValue(0);
    rotationAnimation->setEndValue(360);
    rotationAnimation->setLoopCount(-1);
    rotationAnimation->start();
}

void PlayGround::collectSunBrain(int value) {
    if (isZombie) {
        updateBrainCount(value);
    } else {
        updateSunCount(value);
    }
}

void PlayGround::connectDataListener() {
    dataListener = connect(socket, &ClientSocket::dataReceived, this, &PlayGround::handleServerResponse);
}

void PlayGround::handleServerResponse(const QJsonObject &data) {
    qDebug() << data;
    if (!data.contains("state")) {
        return;
    }

    if (data.value("state") == "opponentLeft") {
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

void PlayGround::checkCardStates() {
    int availablePoints = isZombie ? brainCount : sunCount;
    QVector<Card*>& cards = isZombie ? zombieCards : plantCards;

    for (Card* card : cards) {
        if (card->getCost() > availablePoints) {
            card->setEnabled(false);
            card->setOpacity(0.5); // Dim the card to show it's unavailable
            if (selectedCard == card) {
                card->unselect();
                selectedCard = nullptr;
            }
        } else {
            card->setEnabled(true);
            card->setOpacity(1.0); // Reset opacity to normal
        }
    }
}

void PlayGround::handleLanded() {
    sunBrainTimer->start(10000); // Restart the timer to spawn SunBrain after 10 seconds
}

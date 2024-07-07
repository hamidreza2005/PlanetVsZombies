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
#include <QRandomGenerator>
#include "core/Cookie.h"
#include "QMap"

QMap<QString,std::function<GameEntity*()>> PlayGround::zombies = {
        {"Regular Zombie",[](){return new RegularZombie;}},
        {"Bucket Head Zombie",[](){return new BucketHeadZombie;}},
        {"Tall Zombie",[](){return new TallZombie;}},
        {"Laef Head Zombie",[]() { return new LeafHeadZombie; }},
        {"Purple Head Zombie",[](){return new PurpleHairZombie;}},
        {"Astronaut Zombie",[](){return new AstronautZombie;}},
    };

QMap<QString,std::function<GameEntity*()>> PlayGround::plants = {
        {"PeaShooter Plant",[]() { return new PeaShooter; }},
        {"TwoPeaShooter Plant",[]() { return new TwoPeaShooter; }},
        {"Walnut Plant",[]() { return new Walnut; }},
        {"Boomerang Plant",[]() { return new Boomerang; }},
        {"Jalapeno Plant",[](){return new Jalapeno;}},
        {"Plum Mine Plant",[](){return new PlumMine;}},
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

    // Set up timer to spawn SunBrain items
    sunBrainTimer = new QTimer(this);
    connect(sunBrainTimer, &QTimer::timeout, this, &PlayGround::spawnSunBrain);
    sunBrainTimer->start(5000); // Spawn every 5 seconds
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

void PlayGround::createZombieCards() {
    for(int i = 0; i < 6; i++) {
        auto* card = new Card(this->zombies.values()[i]);
        card->setPos(i * 150, 0);
        QObject::connect(card, &Card::selectEntity, this, &PlayGround::selectCard);
        zombieCards.push_back(card);
    }
}

void PlayGround::createPlantCards() {
    for(int i = 0; i < 6; i++) {
        auto* card = new Card(this->plants.values()[i]);
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

    int finalX, finalY;
    finalX = isZombie ? 750 : ground->getXForPlants(point.x());
    finalY = ground->getYForNewEntity(point.y());

    QPointF finalPosition(finalX, finalY);
    if (isPositionOccupied(finalPosition)) {
        return;
    }

    auto* newEntity = selectedCard->getEntityFactory()();
    newEntity->setPos(finalX, finalY);
    scene->addItem(newEntity);
    this->sendAddRequest(newEntity->getName(),finalX,finalY);
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

void PlayGround::spawnSunBrain() {
    QString imagePath = isZombie ? ":/resources/images/Brain.png" : ":/resources/images/sun.png";
    int value = 50;
    SunBrain *sunBrain = new SunBrain(imagePath, value);

    int x = QRandomGenerator::global()->bounded(isZombie ? 485 : 30, isZombie ? 1000 : 490);
    int y = -50; // Start above the scene
    sunBrain->setPos(x, y);

    connect(sunBrain, &SunBrain::collected, this, &PlayGround::collectSunBrain);
    scene->addItem(sunBrain);
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
    if (!data.contains("state")){
        return;
    }

    if(data.value("state") == "opponentLeft"){
        QMessageBox::critical(this, "Problem", "Your Opponent Left The game",
                              QMessageBox::Yes);
        this->endTheGame();
        return;
    }

    if(data.value("state") == "add"){
        GameEntity* newEntity;
        QString entityName = data.value("entity").toString();
        if(this->zombies.contains(entityName)){
            newEntity = this->zombies.value(entityName)();
        }else if(this->plants.contains(entityName)){
            newEntity = this->plants.value(entityName)();
        }else{
            return;
        }
        newEntity->setPos(data.value("x").toInt(), data.value("y").toInt());
        scene->addItem(newEntity);
    }
}

void PlayGround::endTheGame() {
    QList<QGraphicsItem*> items = scene->items();
    for (QGraphicsItem* item : items) {
        delete item;
    }
    timer->stop();
    sunBrainTimer->stop();
    delete infoLayout;
    delete mainLayout;
    delete playerPlantName;
    delete playerZombieName;
    delete remainingPlantTime;
    delete remainingZombieTime;
    delete brainBar;
    delete sunBar;
    emit this->goToDashboardPage(this);
}

void PlayGround::connectionLost() {
    QMessageBox::critical(this, "Problem", "Your Connection Got Lost",
                          QMessageBox::Yes);
    this->endTheGame();
}

void PlayGround::sendAddRequest(const QString& name,int x,int y) {
    QJsonObject response;
    response["state"] = "add";
    response["entity"] = name;
    response["x"] = x;
    response["y"] = y;
    this->socket->sendJson("gameRoom",response);
}

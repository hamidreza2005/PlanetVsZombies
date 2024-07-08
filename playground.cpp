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
    connectionLostListener = connect(socket,&ClientSocket::connectionLost,this,&PlayGround::connectionLost);
    isZombie = Cookie::getInstance()->loggedInPlayer->getRole() == "zombie";
    if (isZombie) {
        createZombieCards();
        setupPlayerZombieInfo();
    } else {
        createPlantCards();
        setupPlayerPlantInfo();
    }
    this->setPlayerName();
    this->setupRemainingTimeInfo();
    this->setupLayout();
    this->setupGround();
    timer->start(1000);

    sunBrainTimer = new QTimer(this);
    connect(sunBrainTimer, &QTimer::timeout, this, &PlayGround::spawnSunBrain);
    sunBrainTimer->start(5000);
}

void PlayGround::setupPlayerZombieInfo() {
    brainBar = new QProgressBar(this);
    brainBar->setRange(0, 100);
    brainBar->setValue(0);
    brainBar->setFormat("%v");

    QFont font("Arial", 10, QFont::Bold);
    brainBar->setFont(font);

    brainBar->setStyleSheet("QProgressBar { text-align: center; } QProgressBar::chunk { background-color: red; }");
}

void PlayGround::setupPlayerPlantInfo() {
    sunBar = new QProgressBar(this);
    sunBar->setRange(0, 100);
    sunBar->setValue(0);
    sunBar->setFormat("%v");

    QFont font("Arial", 10, QFont::Bold);
    sunBar->setFont(font);

    sunBar->setStyleSheet("QProgressBar { text-align: center; } QProgressBar::chunk { background-color: yellow; }");
}

void PlayGround::setPlayerName() {
    playerName = new QLabel(Cookie::getInstance()->loggedInPlayer->getUsername(), this);
    QFont font("Arial", 10, QFont::Bold);
    playerName->setFont(font);
    playerName->setStyleSheet("QLabel { color : blue; }");
}

void PlayGround::setupRemainingTimeInfo() {
    remainingTime = new QLabel("Time: 03:30", this);
    QFont font("Arial", 10, QFont::Bold);
    remainingTime->setFont(font);
    remainingTime->setStyleSheet("QLabel { color : red; }");
}

void PlayGround::setupGround() {
    ground = new Ground(this);
    ground->setPos(-260, -40);
    scene->addItem(ground);
}

void PlayGround::setupLayout() {
    mainLayout = new QVBoxLayout(this);

    infoLayout = new QHBoxLayout(this);
    infoLayout->addWidget(playerName);
    if (isZombie) {
        infoLayout->addWidget(brainBar);
    } else {
        infoLayout->addWidget(sunBar);
    }
    infoLayout->addWidget(remainingTime);
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
        auto* card = new Card(PlayGround::zombies.values()[i]);
        card->setPos(i * 150, 0);
        QObject::connect(card, &Card::selectEntity, this, &PlayGround::selectCard);
        zombieCards.push_back(card);
    }
}

void PlayGround::createPlantCards() {
    for(int i = 0; i < 6; i++) {
        auto* card = new Card(PlayGround::plants.values()[i]);
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

        remainingTime->setText(timeString);
    } else {
        this->ranOutOfTime();
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
    if (!selectedCard || ground->isOutOfGround(isZombie,&point)) {
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
    if(auto zombie = dynamic_cast<Zombie*>(newEntity)){
        connect(zombie,&Zombie::zombieReachedToTheEnd,this,&PlayGround::AZombieReachedTheEnd);
    }
    this->sendAddRequest(newEntity->getName(),finalX,finalY);
}

bool PlayGround::isPositionOccupied(QPointF point) {
    QList<QGraphicsItem*> items = scene->items();
    for (QGraphicsItem* item : items) {
        if (dynamic_cast<Plant*>(item) && item->pos() == point) {
            return true;
        }
    }
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
        this->addNewEntityFromServer(data);
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
    delete playerName;
    delete remainingTime;
    delete brainBar;
    delete sunBar;
    disconnect(connectionLostListener);
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

void PlayGround::AZombieReachedTheEnd() {
    qDebug() << "a zombie reached the end";
}

void PlayGround::ranOutOfTime() {
    qDebug() << "We ran out of time";
}

void PlayGround::addNewEntityFromServer(QJsonObject entityData){
    GameEntity* newEntity;
    QString entityName = entityData.value("entity").toString();

    if(PlayGround::zombies.contains(entityName)){
        newEntity = PlayGround::zombies.value(entityName)();
    }else if(PlayGround::plants.contains(entityName)){
        newEntity = PlayGround::plants.value(entityName)();
    }else{
        return;
    }
    newEntity->setPos(entityData.value("x").toInt(), entityData.value("y").toInt());
    scene->addItem(newEntity);
}
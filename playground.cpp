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
#include <QGraphicsRotation>
#include <QTimeLine>
#include <QPropertyAnimation>
#include <QPushButton>

QMap<QString, std::function<GameEntity*()>> PlayGround::zombies = {
    {"Regular Zombie", []() { return new RegularZombie; }},
    {"Bucket Head Zombie", []() { return new BucketHeadZombie; }},
    {"Tall Zombie", []() { return new TallZombie; }},
    {"Leaf Head Zombie", []() { return new LeafHeadZombie; }},
    {"Purple Hair Zombie", []() { return new PurpleHairZombie; }},
    {"Astronaut Zombie", []() { return new AstronautZombie; }},
};

QMap<QString, std::function<GameEntity*()>> PlayGround::plants = {
    {"PeaShooter Plant", []() { return new PeaShooter; }},
    {"TwoPeaShooter Plant", []() { return new TwoPeaShooter; }},
    {"Walnut Plant", []() { return new Walnut; }},
    {"Boomerang Plant", []() { return new Boomerang; }},
    {"Jalapeno Plant", []() { return new Jalapeno; }},
    {"Plum Mine Plant", []() { return new PlumMine; }},
};

PlayGround::PlayGround(ClientSocket* clientSocket, QWidget *parent)
    : Window(clientSocket, parent), remainingSeconds(210), points(INITIAL_POINTS), selectedCard(nullptr) {
    this->setFixedSize(1200, 700);
    graphicsView = new QGraphicsView(this);
    graphicsView->setFixedSize(1000, 500);
    scene = new QGraphicsScene(this);
    graphicsView->setScene(scene);
    graphicsView->setSceneRect(0, 0, 650, 450);
    graphicsView->viewport()->installEventFilter(this);
    this->setupGround();

    timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &PlayGround::updateTimer);
}

void PlayGround::play() {
    isZombie = Cookie::getInstance()->loggedInPlayer->getRole() == "zombie";
    this->setUpPointsBar();
    this->setPlayerName();
    this->setupRemainingTimeInfo();
    this->setupLayout();
    Window::showPopupMessage("Round " + Cookie::getInstance()->playingRound, 1000);
    remainingSeconds = 210;
    timer->start(1000);
    sunBrainTimer = new QTimer(this);
    connect(sunBrainTimer, &QTimer::timeout, this, &PlayGround::spawnSunBrain);
    this->spawnSunBrain();
    sunBrainTimer->start(5000);
}

void PlayGround::setUpPointsBar() {
    pointsBar = new QProgressBar(this);
    pointsBar->setRange(0, 2000);
    pointsBar->setValue(0);
    pointsBar->setFormat("%v");

    QFont font("Arial", 10, QFont::Bold);
    pointsBar->setFont(font);
    pointsBar->setFixedHeight(20);
    pointsBar->setFixedWidth(200);
    pointsBar->setStyleSheet("QProgressBar { text-align: center; } QProgressBar::chunk { background-color: red; }");
}

void PlayGround::setPlayerName() {
    playerName = new QLabel(this);
    playerName->setText(Cookie::getInstance()->loggedInPlayer->getUsername() + " Vs " + Cookie::getInstance()->opponentUsername);
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
    mainLayout = new QVBoxLayout();
    this->setUpChatBox();
    infoLayout = new QHBoxLayout();
    infoLayout->addWidget(playerName);
    infoLayout->addWidget(pointsBar);
    infoLayout->addWidget(remainingTime);
    mainLayout->addLayout(infoLayout, 1);
    mainLayout->addWidget(graphicsView, 4);

    cardView = new QGraphicsView();
    cardsScene = new QGraphicsScene(this);
    cardView->setScene(cardsScene);
    cardView->setFixedHeight(150);
    cardView->setFixedWidth(1000);
    mainLayout->addWidget(cardView, 1);

    containerLayout = new QHBoxLayout(this);
    containerLayout->addLayout(mainLayout, 4);
    containerLayout->addLayout(chatLayout, 1);

    setLayout(containerLayout);

    this->createCards();
}

void PlayGround::setUpChatBox() {
    chatLayout = new QVBoxLayout();
    this->chatHandler = new Chat(chatLayout);
    connect(chatHandler,&Chat::sendMessageToOpponent,this,&PlayGround::sendMessageToOpponent);
    connect(chatHandler,&Chat::playerResigned,this,&PlayGround::playerWantsToResign);
}

void PlayGround::createCards() {
    QList<std::function<GameEntity*()>> entites = isZombie ? PlayGround::zombies.values() : PlayGround::plants.values();
    for(int i = 0; i < 6; i++) {
        auto *card = new Card(entites[i],100,100);
        connect(card, &Card::selectEntity, this, &PlayGround::selectCard);
        card->disable();
        cards.push_back(card);
    }

    int cardWidth = 100;
    int gapBetweenCards = 2;

    for (int i = 0; i < 3; ++i) {
        cards[i]->setPos(i * (cardWidth + gapBetweenCards), 0);
        cardsScene->addItem(cards[i]);
    }

    setupRotatingImage(isZombie ? ":/resources/images/Brain.png" : ":/resources/images/sun.png");

    for (int i = 3; i < 6; ++i) {
        cards[i]->setPos((i + 1) * (cardWidth + gapBetweenCards), 0);
        cardsScene->addItem(cards[i]);
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

void PlayGround::updatePoint(int amount) {
    this->points += amount;
    pointsBar->setValue(this->points);
    pointsBar->setFormat(QString(isZombie ? "Brains: %1" : "Sun: %1").arg(this->points));
    this->checkCardStates();
}

void PlayGround::selectCard(Card *card) {
    this->checkCardStates();
    if(card->getCost() > this->points){
        return;
    }

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
    if (!selectedCard || ground->isOutOfGround(isZombie, &point)) {
        return;
    }

    int finalX, finalY;
    finalX = isZombie ? 750 : ground->getXForPlants(point.x());
    finalY = ground->getYForNewEntity(point.y());

    QPointF finalPosition(finalX, finalY);
    if (this->isPositionOccupied(finalPosition)) {
        return;
    }

    auto* newEntity = selectedCard->getEntityFactory()();
    this->updatePoint(-1 * selectedCard->getCost());
    this->checkCardStates();
    newEntity->setPos(finalX, finalY);
    scene->addItem(newEntity);
    if (auto zombie = dynamic_cast<Zombie*>(newEntity)) {
        connect(zombie, &Zombie::zombieReachedToTheEnd, this, &PlayGround::AZombieReachedTheEnd);
    }
    this->sendAddRequest(newEntity->getName(), finalX, finalY);
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
    SunBrain* sunBrain = new SunBrain(imagePath, 100, pointsBar, isZombie, rotatingItem);

    int x = QRandomGenerator::global()->bounded(isZombie ? 485 : 30, isZombie ? 750 : 490);
    int y = -100;

    sunBrain->setPos(x, y);

    connect(sunBrain, &SunBrain::collected, this, &PlayGround::collectSunBrain);
    connect(sunBrain, &SunBrain::landed, this, &PlayGround::handleLanded);
    scene->addItem(sunBrain);
}

void PlayGround::setupRotatingImage(const QString& imagePath) {
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
    cardsScene->addItem(rotatingItem);

    auto rotationAnimation = new QPropertyAnimation(rotation, "angle");
    rotationAnimation->setDuration(2000);
    rotationAnimation->setStartValue(0);
    rotationAnimation->setEndValue(360);
    rotationAnimation->setLoopCount(-1);
    rotationAnimation->start();
}

void PlayGround::collectSunBrain(int value) {
    this->updatePoint(value);
}

void PlayGround::connectDataListener() {
    dataListener = connect(socket, &ClientSocket::dataReceived, this, &PlayGround::handleServerResponse);
}

void PlayGround::handleServerResponse(const QJsonObject &data) {
    if (!data.contains("state")) {
        return;
    }

    if (data.value("state") == "opponentLeft") {
        QMessageBox::critical(this, "Problem", "Your Opponent Left The game",
                              QMessageBox::Yes);
        this->endTheGame();
        return;
    }

    if (data.value("state") == "add") {
        this->addNewEntityFromServer(data);
        return;
    }

    if(data.value("state") == "chat"){
        this->chatHandler->addIconToChatBox(data.value("message").toString(),data.value("username").toString());
        return;
    }

    if(data.value("state") == "roundWinner"){
        Window::showPopupMessage(data.value("message").toString(),2000,this);
        Cookie::getInstance()->playingRound = data.value("round").toString();
        Cookie::getInstance()->loggedInPlayer->getRole() = data.value("role").toString();
        QTimer::singleShot(2000, [this]() {
            this->startARound();
            MediaPlayer::getInstance()->playRoundMusic(":/resources/musics/round2.mp3", ":/resources/musics/Grasswalk.mp3");
        });
        return;
    }

    if (data.value("state") == "GameEnded") {
        QString message = data.value("result").toString() + "\n";
        if (data.contains("winner")) {
            message += data.value("winner").toString() + " Won the Game";
        }
        Window::showPopupMessage(message, 2000, this);
        Cookie::getInstance()->playingRound = "0";
        QTimer::singleShot(2000, [this]() {
            this->endTheGame();
        });
        return;
    }
}

void PlayGround::cleanThePlayground() {
    this->selectedCard = nullptr;
    this->points = INITIAL_POINTS;
    QList<QGraphicsItem*> items = scene->items();
    for (QGraphicsItem* item : items) {
        if (dynamic_cast<Ground*>(item)) {
            continue;
        }
        delete item;
    }
    timer->stop();
    sunBrainTimer->stop();
    Cookie::getInstance()->zombiesCountThatReachedTheEnd = 0;
    delete infoLayout;
    delete mainLayout;
    delete chatHandler;
    delete chatLayout;
    delete containerLayout;
    delete playerName;
    delete remainingTime;
    this->removeAllCards();
    delete cardsScene;
    delete cardView;
    delete pointsBar;
    delete sunBrainTimer;
}

void PlayGround::connectionLost() {
    QMessageBox::critical(this, "Problem", "Your Connection Got Lost",
                          QMessageBox::Yes);
    this->endTheGame();
}

void PlayGround::sendAddRequest(const QString& name, int x, int y) {
    QJsonObject response;
    response["state"] = "add";
    response["entity"] = name;
    response["x"] = x;
    response["y"] = y;
    this->sendOverSocket(response);
}

void PlayGround::AZombieReachedTheEnd() {
    if (Cookie::getInstance()->zombiesCountThatReachedTheEnd > 1) {
        return;
    }
    QJsonObject response;
    response["state"] = "zombieReachedTheEnd";
    this->sendOverSocket(response);
    Cookie::getInstance()->zombiesCountThatReachedTheEnd++;
}

void PlayGround::ranOutOfTime() {
    QJsonObject response;
    response["state"] = "timeEnded";
    this->sendOverSocket(response);
}

void PlayGround::connectConnectionLostListener() {
    connectionLostListener = connect(socket, &ClientSocket::connectionLost, this, &PlayGround::connectionLost);
}

void PlayGround::disconnectConnectionLostListener() {
    if (connectionLostListener) {
        disconnect(connectionLostListener);
    }
}

void PlayGround::startARound() {
    this->cleanThePlayground();
    this->play();
}

void PlayGround::endTheGame() {
    this->cleanThePlayground();
    this->disconnectConnectionLostListener();
    emit this->goToDashboardPage(this);
}

void PlayGround::removeAllCards() {
    for (auto card : this->cards) {
        cardsScene->removeItem(card);
        delete card;
    }
    this->cards.erase(this->cards.begin(), this->cards.end());
}

void PlayGround::addNewEntityFromServer(const QJsonObject &entityData) {
    GameEntity* newEntity;
    QString entityName = entityData.value("entity").toString();

    if (PlayGround::zombies.contains(entityName)) {
        newEntity = PlayGround::zombies.value(entityName)();
    } else if (PlayGround::plants.contains(entityName)) {
        newEntity = PlayGround::plants.value(entityName)();
    } else {
        return;
    }
    newEntity->setPos(entityData.value("x").toInt(), entityData.value("y").toInt());
    scene->addItem(newEntity);
    checkCardStates();
}

void PlayGround::sendOverSocket(const QJsonObject &response) {
    try {
        this->socket->sendJson("gameRoom", response);
    } catch (...) {
    }
}

void PlayGround::checkCardStates() {
    for (auto card : this->cards) {
        if (card->getCost() > this->points) {
            if (this->selectedCard == card) {
                this->selectedCard->unselect();
                this->selectedCard = nullptr;
            }
            card->disable();
        } else {
            card->enable();
        }
    }
}

void PlayGround::handleLanded() {
    sunBrainTimer->start(10000);
}

void PlayGround::sendMessageToOpponent(const QString &message) {
    this->sendOverSocket({{"state","chat"},{"message",message},{"username",Cookie::getInstance()->loggedInPlayer->getUsername()}});
}

void PlayGround::playerWantsToResign() {
    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, "Confirm" , "Are You Sure that You Want To Resign?",
                                  QMessageBox::Yes | QMessageBox::No);
    if (reply == QMessageBox::Yes){
        this->sendOverSocket({{"state","playerResigned"}});
    }
}

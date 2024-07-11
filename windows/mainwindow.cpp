#include "mainwindow.h"

MainWindow::MainWindow(ClientSocket* clientSocket, QWidget *parent) :
    QMainWindow(parent),
    socket(clientSocket),
    stackedWidget(new QStackedWidget(this))
{
    this->createWindows();
    setCentralWidget(stackedWidget);
    this->setFixedSize(1200,700);
    stackedWidget->setFixedSize(1200,700);
    this->connectSignals();
    this->setWindowIcon(QIcon(":/resources/images/icon.png"));
    this->showHostConnectorWindow();
}

MainWindow::~MainWindow() {
}

void MainWindow::showRegisterWindow(Window* senderWindow) {
    if(senderWindow)
        senderWindow->disconnectDataListener();
    registerWindow->connectDataListener();
    stackedWidget->setCurrentWidget(registerWindow);
}

void MainWindow::showLoginWindow(Window* senderWindow) {
    if (senderWindow)
        senderWindow->disconnectDataListener();
    loginWindow->connectDataListener();
    stackedWidget->setCurrentWidget(loginWindow);
    this->playMenuMusic();
}

void MainWindow::showDashboardWindow(Window* senderWindow) {
    if (senderWindow)
        senderWindow->disconnectDataListener();
    dashboardWindow->connectDataListener();
    stackedWidget->setCurrentWidget(dashboardWindow);
}

void MainWindow::showResetPasswordWindow(Window *senderWindow) {
    if (senderWindow)
        senderWindow->disconnectDataListener();
    resetPasswordWindow->connectDataListener();
    stackedWidget->setCurrentWidget(resetPasswordWindow);
}

void MainWindow::showPlaygroundWindow(Window* senderWindow) {
    if (senderWindow)
        senderWindow->disconnectDataListener();
    playgroundWindow->connectDataListener();
    playgroundWindow->connectConnectionLostListener();
    stackedWidget->setCurrentWidget(playgroundWindow);
    playgroundWindow->play();
}

void MainWindow::showHistoryWindow(Window *senderWindow) {
    if (senderWindow)
        senderWindow->disconnectDataListener();
    historyWindow->connectDataListener();
    stackedWidget->setCurrentWidget(historyWindow);
}

void MainWindow::showCredentialsWindow(Window *senderWindow) {
    if (senderWindow)
        senderWindow->disconnectDataListener();
    credentialsWindow->connectDataListener();
    stackedWidget->setCurrentWidget(credentialsWindow);
}

void MainWindow::showHostConnectorWindow(Window *senderWindow) {
    if (senderWindow)
        senderWindow->disconnectDataListener();
    hostConnectorWindow->connectDataListener();
    stackedWidget->setCurrentWidget(hostConnectorWindow);
}

void MainWindow::connectSignals() {
    connect(loginWindow, &Login::goToRegisterPage, this, &MainWindow::showRegisterWindow);
    connect(loginWindow, &Login::goToDashboardPage, this, &MainWindow::showDashboardWindow);
    connect(loginWindow, &Login::goToResetPasswordPage, this, &MainWindow::showResetPasswordWindow);
    connect(loginWindow, &Login::goToHostConnectorPage, this, &MainWindow::showHostConnectorWindow);
    connect(resetPasswordWindow, &ResetPassword::goToLoginPage, this, &MainWindow::showLoginWindow);
    connect(registerWindow, &Register::goToLoginPage, this, &MainWindow::showLoginWindow);
    connect(dashboardWindow, &Dashboard::startTheGame, this, &MainWindow::showPlaygroundWindow);
    connect(dashboardWindow, &Dashboard::goToLoginPage, this, &MainWindow::showLoginWindow);
    connect(dashboardWindow, &Dashboard::goToHistoryPage, this, &MainWindow::showHistoryWindow);
    connect(dashboardWindow, &Dashboard::goToCredentialsPage, this, &MainWindow::showCredentialsWindow);
    connect(playgroundWindow, &PlayGround::goToDashboardPage, this, &MainWindow::showDashboardWindow);
    connect(playgroundWindow, &PlayGround::goToDashboardPage, this, &MainWindow::playAfterTheGameMusic);
    connect(historyWindow, &History::goToDashboard, this, &MainWindow::showDashboardWindow);
    connect(credentialsWindow, &UpdateCredentials::goToDashboardPage, this, &MainWindow::showDashboardWindow);
    connect(hostConnectorWindow, &HostConnector::connectionIsEstablished, this, &MainWindow::showLoginWindow);
}

void MainWindow::createWindows() {
    this->loginWindow = new Login(socket,this),
    this->registerWindow = new Register(socket,this);
    this->dashboardWindow = new Dashboard(socket,this);
    this->resetPasswordWindow = new ResetPassword(socket,this);
    this->playgroundWindow = new PlayGround(socket,this);
    this->historyWindow = new History(socket,this);
    this->credentialsWindow = new UpdateCredentials(socket,this);
    this->hostConnectorWindow = new HostConnector(socket,this);

    stackedWidget->addWidget(loginWindow);
    stackedWidget->addWidget(registerWindow);
    stackedWidget->addWidget(dashboardWindow);
    stackedWidget->addWidget(resetPasswordWindow);
    stackedWidget->addWidget(playgroundWindow);
    stackedWidget->addWidget(historyWindow);
    stackedWidget->addWidget(credentialsWindow);
    stackedWidget->addWidget(hostConnectorWindow);
}

void MainWindow::playMenuMusic() {
}

void MainWindow::playRoundMusic(const QString& roundMusic, const QString& backgroundMusic) {
}

void MainWindow::playAfterTheGameMusic(Window *senderWindow) {
    this->playMenuMusic();
}

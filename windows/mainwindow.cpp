#include "mainwindow.h"


MainWindow::MainWindow(ClientSocket* clientSocket,QWidget *parent) :
        QMainWindow(parent),
        socket(clientSocket),
        stackedWidget(new QStackedWidget(this)),
        loginWindow(new Login(clientSocket,this)),
        registerWindow(new Register(clientSocket,this)),
        dashboardWindow(new Dashboard(clientSocket,this)),
        resetPasswordWindow(new ResetPassword(clientSocket,this)),
        playgroundWindow(new PlayGround(clientSocket,this))
{
    stackedWidget->addWidget(loginWindow);
    stackedWidget->addWidget(registerWindow);
    stackedWidget->addWidget(dashboardWindow);
    stackedWidget->addWidget(resetPasswordWindow);
    stackedWidget->addWidget(playgroundWindow);
    setCentralWidget(stackedWidget);
    this->setFixedSize(1000,700);
    stackedWidget->setFixedSize(1000,700);
    // Connect signals to slots
    this->connectSignals();

    // Show the login window by default
    showLoginWindow();
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
    if(senderWindow)
        senderWindow->disconnectDataListener();
    loginWindow->connectDataListener();
    stackedWidget->setCurrentWidget(loginWindow);
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

void MainWindow::showPlaygroundWindow(Window *senderWindow) {
    if (senderWindow)
        senderWindow->disconnectDataListener();
    playgroundWindow->connectDataListener();
    stackedWidget->setCurrentWidget(playgroundWindow);
    playgroundWindow->play();
}

void MainWindow::connectSignals() {
    connect(loginWindow, &Login::goToRegisterPage, this, &MainWindow::showRegisterWindow);
    connect(loginWindow, &Login::goToDashboardPage, this, &MainWindow::showDashboardWindow);
    connect(loginWindow, &Login::goToResetPasswordPage, this, &MainWindow::showResetPasswordWindow);
    connect(resetPasswordWindow, &ResetPassword::goToLoginPage, this, &MainWindow::showLoginWindow);
    connect(registerWindow, &Register::goToLoginPage, this, &MainWindow::showLoginWindow);
    connect(dashboardWindow, &Dashboard::startTheGame, this, &MainWindow::showPlaygroundWindow);
    connect(dashboardWindow, &Dashboard::goToLoginPage, this, &MainWindow::showLoginWindow);
    connect(playgroundWindow, &PlayGround::goToDashboardPage, this, &MainWindow::showDashboardWindow);
}

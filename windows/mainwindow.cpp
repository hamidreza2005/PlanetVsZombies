#include "mainwindow.h"


MainWindow::MainWindow(ClientSocket* clientSocket,QWidget *parent) :
        QMainWindow(parent),
        socket(clientSocket),
        stackedWidget(new QStackedWidget(this)),
        loginWindow(new Login(clientSocket,this)),
        registerWindow(new Register(clientSocket,this)),
        dashboardWindow(new Dashboard(clientSocket,this))
{
    stackedWidget->addWidget(loginWindow);
    stackedWidget->addWidget(registerWindow);
    stackedWidget->addWidget(dashboardWindow);
    setCentralWidget(stackedWidget);
    stackedWidget->setFixedSize(500,500);
    // Connect signals to slots
    connect(loginWindow, &Login::goToRegisterPage, this, &MainWindow::showRegisterWindow);
    connect(loginWindow, &Login::goToDashboardPage, this, &MainWindow::showDashboardWindow);
//    connect(registerWindow, &RegisterWindow::showLogin, this, &MainWindow::showLoginWindow);

    // Show the login window by default
    showLoginWindow();
}

MainWindow::~MainWindow() {

}

void MainWindow::showRegisterWindow(Window* senderWindow) {
    if(senderWindow)
        senderWindow->disconnectDataListener();
    stackedWidget->setCurrentWidget(registerWindow);
}

void MainWindow::showLoginWindow(Window* senderWindow) {
    if(senderWindow)
        senderWindow->disconnectDataListener();
    stackedWidget->setCurrentWidget(loginWindow);
}

void MainWindow::showDashboardWindow(Window* senderWindow) {
    if (senderWindow)
        senderWindow->disconnectDataListener();
    stackedWidget->setCurrentWidget(dashboardWindow);
}

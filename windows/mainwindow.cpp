#include "mainwindow.h"


MainWindow::MainWindow(ClientSocket* clientSocket,QWidget *parent) :
        QMainWindow(parent),
        socket(clientSocket),
        stackedWidget(new QStackedWidget(this)),
        loginWindow(new Login(clientSocket,this)),
        registerWindow(new Register(clientSocket,this)),
        dashboardWindow(new Dashboard(clientSocket,this)),
        resetPasswordWindow(new ResetPassword(clientSocket,this))
{
    stackedWidget->addWidget(loginWindow);
    stackedWidget->addWidget(registerWindow);
    stackedWidget->addWidget(dashboardWindow);
    stackedWidget->addWidget(resetPasswordWindow);
    setCentralWidget(stackedWidget);
    stackedWidget->setFixedSize(500,500);
    // Connect signals to slots
    connect(loginWindow, &Login::goToRegisterPage, this, &MainWindow::showRegisterWindow);
    connect(loginWindow, &Login::goToDashboardPage, this, &MainWindow::showDashboardWindow);
    connect(loginWindow, &Login::goToResetPasswordPage, this, &MainWindow::showResetPasswordWindow);
    connect(resetPasswordWindow, &ResetPassword::goToLoginPage, this, &MainWindow::showLoginWindow);
    connect(registerWindow, &Register::goToLoginPage, this, &MainWindow::showLoginWindow);

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
    dashboardWindow->connectDataListener();
}

void MainWindow::showResetPasswordWindow(Window *senderWindow) {
    if (senderWindow)
        senderWindow->disconnectDataListener();
    stackedWidget->setCurrentWidget(resetPasswordWindow);
}

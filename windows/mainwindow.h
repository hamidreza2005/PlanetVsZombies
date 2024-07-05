#ifndef PLANETVSZOMBIES_MAINWINDOW_H
#define PLANETVSZOMBIES_MAINWINDOW_H

#include <QMainWindow>
#include <QStackedWidget>
#include "Login/login.h"
#include "Register/register.h"
#include "Dashboard/dashboard.h"
#include "Window.h"

class MainWindow : public QMainWindow {
Q_OBJECT

public:
    explicit MainWindow(ClientSocket* clientSocket,QWidget *parent = nullptr);

    ~MainWindow() override;

private:
    QStackedWidget *stackedWidget;
    Login *loginWindow;
    Register *registerWindow;
    Dashboard *dashboardWindow;
    ClientSocket* socket;

public slots:
    void showLoginWindow(Window* senderWindow = nullptr);
    void showRegisterWindow(Window* senderWindow = nullptr);
    void showDashboardWindow(Window* senderWindow = nullptr);
};


#endif //PLANETVSZOMBIES_MAINWINDOW_H

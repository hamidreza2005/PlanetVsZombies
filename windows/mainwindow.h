#ifndef PLANETVSZOMBIES_MAINWINDOW_H
#define PLANETVSZOMBIES_MAINWINDOW_H

#include <QMainWindow>
#include <QStackedWidget>
#include "Login/login.h"
#include "Register/register.h"
#include "Dashboard/dashboard.h"
#include "ResetPassword/resetpassword.h"
#include "window.h"
#include "Playground/playground.h"
#include "History/history.h"
#include "UpdateCredentials/updatecredentials.h"
#include "HostConnector/hostconnector.h"
#include "../core/mediaplayer.h"

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(ClientSocket* clientSocket, QWidget *parent = nullptr);
    ~MainWindow() override;

private:
    QStackedWidget *stackedWidget;
    Login *loginWindow;
    Register *registerWindow;
    Dashboard *dashboardWindow;
    ResetPassword *resetPasswordWindow;
    PlayGround* playgroundWindow;
    History* historyWindow;
    UpdateCredentials* credentialsWindow;
    HostConnector* hostConnectorWindow;
    ClientSocket* socket;

    void connectSignals();
    void createWindows();
    void playMenuMusic();
    void playRoundMusic(const QString &roundMusic, const QString &backgroundMusic);

public slots:
    void showLoginWindow(Window* senderWindow = nullptr);
    void showRegisterWindow(Window* senderWindow = nullptr);
    void showDashboardWindow(Window* senderWindow = nullptr);
    void showResetPasswordWindow(Window* senderWindow = nullptr);
    void showPlaygroundWindow(Window* senderWindow = nullptr);
    void showHistoryWindow(Window* senderWindow = nullptr);
    void showCredentialsWindow(Window* senderWindow = nullptr);
    void showHostConnectorWindow(Window* senderWindow = nullptr);

    void playAfterTheGameMusic(Window* senderWindow = nullptr);
};

#endif //PLANETVSZOMBIES_MAINWINDOW_H

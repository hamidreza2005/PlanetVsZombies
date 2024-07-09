#ifndef PLANETVSZOMBIES_DASHBOARD_H
#define PLANETVSZOMBIES_DASHBOARD_H

#include "../../core/ClientSocket.h"
#include "../Window.h"
#include "QTimer"

QT_BEGIN_NAMESPACE
namespace Ui { class Dashboard; }
QT_END_NAMESPACE

class Dashboard : public Window {
Q_OBJECT

private:
    void getOnlineUsers();
    void updateOnlineUsersBox(const QJsonArray &users);
public:
    explicit Dashboard(ClientSocket* clientSocket,QWidget *parent = nullptr);
    void handleServerResponse(const QJsonObject &data) override;
    void connectDataListener() override;
    void disconnectDataListener() override;
    ~Dashboard() override;

public slots:
    void on_ready_clicked();
    void on_exit_clicked();
    void on_refreshUsers_clicked();
    void on_history_clicked();
    void on_updateCredentials_clicked();
signals:
    void startTheGame(Window* sender);
    void goToLoginPage(Window* sender);
    void goToHistoryPage(Window *sender);
    void goToCredentialsPage(Window *sender);
private:
    Ui::Dashboard *ui;
    QTimer* updateOnlineUsersTimer;
    void verifyCurrentClientIsReadyToPlay(const QString & opponentUsername);
};


#endif //PLANETVSZOMBIES_DASHBOARD_H

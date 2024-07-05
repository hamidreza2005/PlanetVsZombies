#ifndef PLANETVSZOMBIES_LOGIN_H
#define PLANETVSZOMBIES_LOGIN_H

#include "../../core/ClientSocket.h"
#include "../window.h"
QT_BEGIN_NAMESPACE
namespace Ui { class Login; }
QT_END_NAMESPACE

class Login : public Window {
Q_OBJECT

public:
    explicit Login(ClientSocket* clientSocket,QWidget *parent = nullptr);
    ~Login() override;
    void disconnectDataListener() override;
    void handleServerResponse(const QJsonObject &data) override;
public slots:
    void on_submit_clicked();
    void on_registerLink_clicked();
signals:
    void goToRegisterPage(Window* sender);
    void goToDashboardPage(Window* sender);
private:
    Ui::Login *ui;
    bool fieldsAreNotEmpty();
};


#endif //PLANETVSZOMBIES_LOGIN_H

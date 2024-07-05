//
// Created by hrhas on 7/5/2024.
//

#ifndef PLANETVSZOMBIES_RESETPASSWORD_H
#define PLANETVSZOMBIES_RESETPASSWORD_H

#include "../Window.h"

QT_BEGIN_NAMESPACE
namespace Ui { class ResetPassword; }
QT_END_NAMESPACE

class ResetPassword : public Window {
Q_OBJECT

public:
    explicit ResetPassword(ClientSocket* clientSocket,QWidget *parent = nullptr);
    void handleServerResponse(const QJsonObject &data) override;
    void connectDataListener() override;
    ~ResetPassword() override;

public slots:
    void on_submit_clicked();
    void on_loginLink_clicked();
signals:
    void goToLoginPage(Window* sender);
private:
    Ui::ResetPassword *ui;

    bool fieldsAreNotEmpty();
};


#endif //PLANETVSZOMBIES_RESETPASSWORD_H

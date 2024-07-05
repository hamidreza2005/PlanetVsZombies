#ifndef PLANETVSZOMBIES_REGISTER_H
#define PLANETVSZOMBIES_REGISTER_H

#include <QWidget>
#include "../../core/ClientSocket.h"
#include "../window.h"

QT_BEGIN_NAMESPACE
namespace Ui { class Register; }
QT_END_NAMESPACE

class Register : public Window {
Q_OBJECT
public:
    explicit Register(ClientSocket* clientSocket,QWidget *parent = nullptr);

    ~Register() override;

    void handleServerResponse(const QJsonObject &data) override;
    void disconnectDataListener() override;
    void connectDataListener() override;
public slots:
    void on_submit_clicked();
    void on_loginLink_clicked();
signals:
    void goToLoginPage(Window* sender);
private:
    Ui::Register* ui;

    bool fieldsAreNotEmpty();
};


#endif //PLANETVSZOMBIES_REGISTER_H

#ifndef PLANETVSZOMBIES_HOSTCONNECTOR_H
#define PLANETVSZOMBIES_HOSTCONNECTOR_H

#include "../Window.h"


QT_BEGIN_NAMESPACE
namespace Ui { class HostConnector; }
QT_END_NAMESPACE

class HostConnector : public Window {
Q_OBJECT

public:
    explicit HostConnector(ClientSocket* clientSocket,QWidget *parent = nullptr);

    ~HostConnector() override;
    void handleServerResponse(const QJsonObject &data) override;
    void connectDataListener() override;

public slots:
    void on_connect_clicked();
private:
    Ui::HostConnector *ui;

signals:
    void connectionIsEstablished(Window* sender);
};


#endif //PLANETVSZOMBIES_HOSTCONNECTOR_H

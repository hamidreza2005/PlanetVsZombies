#ifndef PLANETVSZOMBIES_DASHBOARD_H
#define PLANETVSZOMBIES_DASHBOARD_H

#include "../../core/ClientSocket.h"
#include "../Window.h"

QT_BEGIN_NAMESPACE
namespace Ui { class Dashboard; }
QT_END_NAMESPACE

class Dashboard : public Window {
Q_OBJECT

public:
    explicit Dashboard(ClientSocket* clientSocket,QWidget *parent = nullptr);
    void handleServerResponse(const QJsonObject &data) override;
    void connectDataListener() override;
    ~Dashboard() override;

private:
    Ui::Dashboard *ui;
};


#endif //PLANETVSZOMBIES_DASHBOARD_H

#ifndef PLANETVSZOMBIES_DASHBOARD_H
#define PLANETVSZOMBIES_DASHBOARD_H

#include <QWidget>
#include "../../core/ClientSocket.h"


QT_BEGIN_NAMESPACE
namespace Ui { class Dashboard; }
QT_END_NAMESPACE

class Dashboard : public QWidget {
Q_OBJECT

public:
    explicit Dashboard(ClientSocket* clientSocket,QWidget *parent = nullptr);

    ~Dashboard() override;

private:
    Ui::Dashboard *ui;
};


#endif //PLANETVSZOMBIES_DASHBOARD_H

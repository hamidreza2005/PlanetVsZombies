#ifndef PLANETVSZOMBIES_HISTORY_H
#define PLANETVSZOMBIES_HISTORY_H

#include <QWidget>
#include "../Window.h"

QT_BEGIN_NAMESPACE
namespace Ui { class History; }
QT_END_NAMESPACE

class History : public Window {
Q_OBJECT

public:
    explicit History(ClientSocket* clientSocket,QWidget *parent = nullptr);

    ~History() override;
    void handleServerResponse(const QJsonObject &data) override;
    void connectDataListener() override;

    void getHistoryFromServer();
private:
    Ui::History *ui;
public slots:
    void on_dashboardLink_clicked();
signals:
    void goToDashboard(Window* sender);
};


#endif //PLANETVSZOMBIES_HISTORY_H

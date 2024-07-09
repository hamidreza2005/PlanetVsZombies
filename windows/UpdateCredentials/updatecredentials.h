#ifndef PLANETVSZOMBIES_UPDATECREDENTIALS_H
#define PLANETVSZOMBIES_UPDATECREDENTIALS_H

#include "../Window.h"

QT_BEGIN_NAMESPACE
namespace Ui { class UpdateCredentials; }
QT_END_NAMESPACE

class UpdateCredentials : public Window {
Q_OBJECT

public:
    explicit UpdateCredentials(ClientSocket* clientSocket,QWidget *parent = nullptr);

    ~UpdateCredentials() override;
    void handleServerResponse(const QJsonObject &data) override;
    void connectDataListener() override;
public slots:
    void on_dashboardLink_clicked();
    void on_submit_clicked();
private:
    Ui::UpdateCredentials *ui;
    bool fieldsAreNotEmpty();
    void getUserData();
    void fillInputsUsingUserData(const QJsonObject &data);
signals:
    void goToDashboardPage(Window* sender);
};


#endif //PLANETVSZOMBIES_UPDATECREDENTIALS_H

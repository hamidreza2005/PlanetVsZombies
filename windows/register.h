#ifndef PLANETVSZOMBIES_REGISTER_H
#define PLANETVSZOMBIES_REGISTER_H

#include <QWidget>
#include "../core/ClientSocket.h"

QT_BEGIN_NAMESPACE
namespace Ui { class Register; }
QT_END_NAMESPACE

class Register : public QWidget {
Q_OBJECT

public:
    Register(ClientSocket* clientSocket,QWidget *parent = nullptr);

    ~Register() override;

public slots:
    void on_submit_clicked();
    void handleServerResponse(const QJsonObject &data);
private:
    Ui::Register *ui;
    ClientSocket* socket;
    QMetaObject::Connection dataListener;

    bool fieldsAreNotEmpty();
    void showErrors(const QJsonObject &errors);
};


#endif //PLANETVSZOMBIES_REGISTER_H

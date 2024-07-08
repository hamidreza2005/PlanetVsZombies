#ifndef PLANETVSZOMBIES_WINDOW_H
#define PLANETVSZOMBIES_WINDOW_H

#include <QJsonObject>
#include "QWidget"
#include "../core/ClientSocket.h"

class Window: public QWidget {
    Q_OBJECT
protected:
    ClientSocket* socket;
    QMetaObject::Connection dataListener;
public:
    explicit Window(ClientSocket* clientSocket,QWidget *parent = nullptr);
    virtual void disconnectDataListener();
    virtual void connectDataListener() = 0;
    virtual void handleServerResponse(const QJsonObject &data) = 0;
    virtual ~Window();
    static void showValidationErrors(QWidget* window,const QJsonObject &errors);
    static void showConnectionLostError(QWidget* window);
    static void showPopupMessage(const QString &message, int duration = 2000, QWidget *parent = nullptr);
};


#endif //PLANETVSZOMBIES_WINDOW_H

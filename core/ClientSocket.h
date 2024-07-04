#ifndef PLANETVSZOMBIES_CLIENTSOCKET_H
#define PLANETVSZOMBIES_CLIENTSOCKET_H
#include "QObject"
#include "QTcpSocket"
#include "QJsonObject"
#include "QTimer"

class ClientSocket : public QObject
{
    Q_OBJECT
private:
    const static int PORT;
    const static QString HOST;
    QTimer* reconnectTimer;
protected:
    void connectToHost();
    void connectSiganlsToListeners();
protected slots:
    void socket_connected();
    void socket_disconnected();
    void read();
signals:
    void dataReceived(const QJsonObject &data);
public:
    QTcpSocket *socket = nullptr;
    ClientSocket();
    ~ClientSocket();

    void sendJson(const QString& route,const QJsonObject &json);
    void sendString(const QString &string);
    bool isConnected();
};


#endif //PLANETVSZOMBIES_CLIENTSOCKET_H

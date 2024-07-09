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
    static int PORT;
    static QString HOST;
    QTimer* reconnectTimer;
protected:
    void startSocket();
    void connectSiganlsToListeners();
protected slots:
    void socket_connected();
    void socket_disconnected();
    void read();
signals:
    void dataReceived(const QJsonObject &data);
    void connectionLost();
public:
    QTcpSocket *socket = nullptr;
    ClientSocket();
    ~ClientSocket();

    void sendJson(const QString& route,const QJsonObject &json);
    void sendString(const QString &string);
    bool isConnected();
    void connectToHost();
    static void setHost(const QString& ip,int port);
};


#endif //PLANETVSZOMBIES_CLIENTSOCKET_H

#ifndef PLANETVSZOMBIES_CLIENTSOCKET_H
#define PLANETVSZOMBIES_CLIENTSOCKET_H
#include "QObject"
#include "QTcpSocket"

class ClientSocket : public QObject
{
    Q_OBJECT
private:
    const static int PORT;
    const static QString HOST;
    void connectToHost();
protected slots:
    void socket_connected();
    void socket_disconnected();
    void read();
public:
    QTcpSocket *socket = nullptr;
    ClientSocket();
    ~ClientSocket();
};


#endif //PLANETVSZOMBIES_CLIENTSOCKET_H

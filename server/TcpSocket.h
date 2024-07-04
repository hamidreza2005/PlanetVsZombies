#ifndef SERVER_TCPSOCKET_H
#define SERVER_TCPSOCKET_H
#include "QTcpSocket"
#include "QObject"
#include <QString>

class TcpSocket: public QObject {
private:
    QTcpSocket* socket = nullptr;
public:
    TcpSocket(QTcpSocket* socket);
    void write(const QString& data);
    void write(QJsonObject & data);
};


#endif //SERVER_TCPSOCKET_H

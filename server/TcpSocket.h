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
    void writeOk(QJsonObject & data);
    void sendValidationError(QJsonValue errors,int statusCode = 419);
    void sendValidationError(QString field,QString error,int statusCode = 419);
    bool isConnected();
    QTcpSocket* getOriginalSocket();
};


#endif //SERVER_TCPSOCKET_H

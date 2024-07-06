#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QObject>
#include <QTcpSocket>
#include <QTcpServer>
#include <QList>
#include "Controllers/Controller.h"

class Bootstrap : public QObject
{
Q_OBJECT
private:
    const static int PORT;
    const static QString HOST;
    QTcpServer *server = nullptr;
    QList<QTcpSocket*> clients;
    Controller* controller;
    static Bootstrap* instance;
    void run();
public:
    Bootstrap();
    ~Bootstrap() override;
    static Bootstrap* getInstance();
    QList<QTcpSocket*> getClients();
public slots:
    void server_newConnection();
    void socket_connected();
    void socket_readyRead(QTcpSocket *_socket);
    void socket_bytesWritten(QTcpSocket *_socket);
    void socket_disconnected(QTcpSocket *_socket);
};
#endif // MAINWINDOW_H

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QObject>
#include <QTcpSocket>
#include <QTcpServer>
#include <QList>
#include "Controller.h"

class Bootstrap : public QObject
{
Q_OBJECT
private:
    const static int PORT;
    const static QString HOST;
    QTcpServer *server = nullptr;
    QList<QTcpSocket*> clients;
    Controller* controller;
    void run();
public:
    Bootstrap();
    ~Bootstrap();

public slots:
    void server_newConnection();
    void socket_connected();
    void socket_readyRead(QTcpSocket *_socket);
    void socket_bytesWritten(QTcpSocket *_socket);
    void socket_disconnected(QTcpSocket *_socket);

private slots:

    void on_disconnect_clicked();
};
#endif // MAINWINDOW_H

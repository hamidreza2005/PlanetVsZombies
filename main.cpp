#include "playground.h"
#include <QApplication>
#include "windows/mainwindow.h"
#include "core/ClientSocket.h"
int main(int argc, char *argv[]) {
    QApplication a(argc, argv);
    auto* socket = new ClientSocket();
//    auto* playground = new PlayGround();
    auto* mainWindow = new MainWindow(socket);
    mainWindow->show();
//    playground->show();
    return a.exec();
}

#include "windows/Playground/playground.h"
#include <QApplication>
#include "windows/mainwindow.h"
#include "core/ClientSocket.h"
int main(int argc, char *argv[]) {
    QApplication a(argc, argv);
    auto* socket = new ClientSocket();
    auto* mainWindow = new MainWindow(socket);
    mainWindow->show();
    return a.exec();
}

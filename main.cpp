#include "playground.h"
#include <QApplication>
#include "windows/register.h"
#include "core/ClientSocket.h"
int main(int argc, char *argv[]) {
    QApplication a(argc, argv);
    auto* socket = new ClientSocket();
    auto* playground = new PlayGround();
    auto* registerWindow = new Register(socket);
    registerWindow->show();
//    playground->show();
    return a.exec();
}

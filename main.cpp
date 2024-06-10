#include "playground.h"
#include <QApplication>

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);
    auto* playgroud = new PlayGround();
    playgroud->show();
    return a.exec();
}

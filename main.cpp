#include "playground.h"
#include <QApplication>

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);
    auto* playground = new PlayGround();
    playground->show();
    return a.exec();
}

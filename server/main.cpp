#include <QCoreApplication>
#include "bootstrap.h"

int main(int argc, char *argv[]) {
    QCoreApplication a(argc, argv);
    Bootstrap boot;
    return QCoreApplication::exec();
}

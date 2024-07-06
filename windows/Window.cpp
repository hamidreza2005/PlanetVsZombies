#include <QMessageBox>
#include "Window.h"

Window::Window(ClientSocket* clientSocket,QWidget *parent):
        QWidget(parent),socket(clientSocket)
{

}
void Window::showValidationErrors(QWidget* window,const QJsonObject &errors) {
    QString message = "";
    for(auto it = errors.constBegin();it != errors.constEnd();it++){
        message += it.key() + " : " + it.value()[0].toString() + "\n";
    }
    QMessageBox::critical(window,"Error",message);
}

Window::~Window() {
    delete socket;
}

void Window::showConnectionLostError(QWidget *window) {
    QMessageBox::information(window,"Error","Connection is not possible right now.");
}
void Window::disconnectDataListener() {
    disconnect(dataListener);
}

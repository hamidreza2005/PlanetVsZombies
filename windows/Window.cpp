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

void Window::showPopupMessage(const QString &message, int duration, QWidget *parent){
    auto messageBox = new QMessageBox(parent);
    messageBox->setText(message);
    messageBox->setStandardButtons(QMessageBox::NoButton);
    messageBox->setWindowFlag(Qt::FramelessWindowHint);
    QString styleSheet = R"(
        QMessageBox {
            background-color: #02a121;
            color: #FFFFFF;
            padding: 10px;
            font-size: 20px;
            border: 2px solid #027018;
            text-align:center;
        }
        QLabel {
            color: #FFFFFF;
            font-size: 20px;
        }
    )";
    messageBox->setStyleSheet(styleSheet);
    messageBox->show();

    QTimer::singleShot(duration, [messageBox]() {
        messageBox->close();
        delete messageBox;
    });
}

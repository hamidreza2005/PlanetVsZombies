#ifndef PLANETVSZOMBIES_CHAT_H
#define PLANETVSZOMBIES_CHAT_H
#include "QObject"
#include "QMap"
#include <QListWidget>
#include <QVBoxLayout>
#include <QPushButton>

class Chat : public QObject {
    Q_OBJECT
private:
    static QMap<QString,QString> allEmoji;
    QListWidget* messageBox;
    QVBoxLayout* layout;
    QPushButton* resignButton;
public:
    explicit Chat(QVBoxLayout* chatLayout);
    ~Chat() override;
    void addIconToChatBox(QString iconName,QString senderUsername);
signals:
    void sendMessageToOpponent(const QString &message);
    void playerResigned();
};


#endif //PLANETVSZOMBIES_CHAT_H

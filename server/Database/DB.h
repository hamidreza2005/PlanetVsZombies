#ifndef SERVER_DB_H
#define SERVER_DB_H
#include "QString"
#include "QJsonArray"
#include "QFile"

class DB {
private:
    static const QString usersDbPath;
    static const QString historyDbPath;
    QFile *usersFile;
    QFile *historyFile;
    static DB* instance;

    void writeToDB(QJsonArray &data,QFile* file);
public:
    DB();
    ~DB();
    QJsonArray getUsers();
    bool exists(const QString& field,const QString& value);
    void createUser(const QJsonObject &data);
    QJsonObject findUser(const QJsonObject &data);
    void updateUser(const QJsonObject &userData,const QJsonObject &newData);
    void deleteUser(const QJsonObject &userData);

    void saveInToHistory(QJsonObject data);
    QJsonArray getHistory();
    QJsonArray findHistory(const QString& username);

    static DB* getInstance();
    DB(DB const&) = delete;
    void operator=(DB const&) = delete;
};


#endif //SERVER_DB_H

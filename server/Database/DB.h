#ifndef SERVER_DB_H
#define SERVER_DB_H
#include "QString"
#include "QJsonArray"
#include "QFile"

class DB {
private:
    static const QString path;
    QFile *dbfile;
    static DB* instance;

    void writeToDB(QJsonArray &data);
public:
    DB();
    ~DB();
    QJsonArray getUsers();
    bool exists(const QString& field,const QString& value);
    void createUser(const QJsonObject &data);
    QJsonObject findUser(const QJsonObject &data);
    void updateUser(const QJsonObject &userData,const QJsonObject &newData);
    void deleteUser(const QJsonObject &userData);
    static DB* getInstance();
    DB(DB const&) = delete;
    void operator=(DB const&) = delete;
};


#endif //SERVER_DB_H

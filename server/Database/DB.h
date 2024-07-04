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
public:
    DB();
    ~DB();
    QJsonArray getUsers();
    bool exists(const QString& field,const QString& value);
    void createUser(const QJsonObject &data);

    static DB* getInstance();
    DB(DB const&) = delete;
    void operator=(DB const&) = delete;
};


#endif //SERVER_DB_H

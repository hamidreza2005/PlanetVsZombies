#include "DB.h"
#include "QJsonArray"
#include "QJsonObject"
#include "QJsonDocument"
#include "QFile"

const QString DB::path = "./db.json";
DB* DB::instance = nullptr;

DB::DB() {
    this->dbfile = new QFile(DB::path);
    if (!this->dbfile->open(QIODevice::ReadWrite)) {
        qWarning() << "Couldn't open file for reading.";
    }
}

DB::~DB() {
    dbfile->close();
    delete dbfile;
}

QJsonArray DB::getUsers() {
    this->dbfile->seek(0);
    QByteArray jsonData = this->dbfile->readAll();
    QJsonDocument jsonDoc = QJsonDocument::fromJson(jsonData);
    return jsonDoc.array();
}

void DB::createUser(const QJsonObject &data) {
    QJsonArray jsonArray = this->getUsers();
    jsonArray.append(data);

    QJsonDocument jsonDoc(jsonArray);
    QByteArray jsonData = jsonDoc.toJson();
    this->dbfile->seek(0);
    this->dbfile->resize(0);
    dbfile->write(jsonData);
    dbfile->flush();
}

DB* DB::getInstance() {
    if (!instance) {
        instance = new DB();
    }
    return instance;
}

bool DB::exists(const QString &field, const QString &value) {
    QJsonArray users = this->getUsers();
    for(auto && it : users){
        auto user = it.toObject();
        if (user.value(field) == value){
            return true;
        }
    }

    return false;
}
//bool AuthorizationService::userExists(QString username, QString password) {
//    QJsonArray users = this->getUsers();
//    string hashedPassword = AuthorizationService::hashPassword(password);
//    for(auto user: users){
//        auto userObj = user.toObject();
//        if(
//                userObj.value("username").toString() == username
//                && userObj.value("password").toString() == QString::fromStdString(hashedPassword)
//                ){
//            return true;
//        }
//    }
//    return false;
//}
//
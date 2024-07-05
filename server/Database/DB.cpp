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

QJsonObject DB::findUser(const QJsonObject &data) {
    if (data.isEmpty()){
        throw std::invalid_argument("Your Query Data is Empty");
    }
    auto users = this->getUsers();
    for(auto && it : users){
        auto user = it.toObject();
        int MatchedFields = 0;
        for(auto dataIt = data.constBegin();dataIt != data.constEnd();dataIt++){
            if (user.value(dataIt.key()) == dataIt.value()){
                MatchedFields++;
            }
        }
        if (MatchedFields == data.size()){
            return user;
        }
    }
    throw std::invalid_argument("User does not Exists");
}
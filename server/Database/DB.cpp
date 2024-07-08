#include "DB.h"
#include "QJsonArray"
#include "QJsonObject"
#include "QJsonDocument"
#include "QFile"
#include "../exceptions/ModelNotFoundException.h"

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

    this->writeToDB(jsonArray);
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
    throw ModelNotFoundException("User does not Exists");
}

void DB::updateUser(const QJsonObject &userData, const QJsonObject &newData) {
    auto users = this->getUsers();
    for(auto it = users.begin();it != users.end();it++){
        auto user = it->toObject();
        int MatchedFields = 0;
        for(auto dataIt = userData.constBegin();dataIt != userData.constEnd();dataIt++){
            if (user.value(dataIt.key()) == dataIt.value()){
                MatchedFields++;
            }
        }

        if (MatchedFields != userData.size()){
            continue;
        }

        for(auto newDataIt = newData.begin();newDataIt != newData.constEnd();newDataIt++){
            user[newDataIt.key()] = newDataIt.value();
        }
        *it = user;
        this->writeToDB(users);
        return;
    }

    throw ModelNotFoundException("User does not Exists");
}

void DB::deleteUser(const QJsonObject &userData) {
    auto users = this->getUsers();
    QJsonArray::iterator toBeDeletedUser = users.end();
    for(auto it = users.begin();it != users.end();it++){
        auto user = it->toObject();
        int MatchedFields = 0;
        for(auto dataIt = userData.constBegin();dataIt != userData.constEnd();dataIt++){
            if (user.value(dataIt.key()) == dataIt.value()){
                MatchedFields++;
            }
        }

        if (MatchedFields != userData.size()){
            continue;
        }

        toBeDeletedUser = it;
        break;
    }

    if(toBeDeletedUser == users.end()){
        throw new ModelNotFoundException("User does not Exists");
    }else{
        users.erase(toBeDeletedUser);
        this->writeToDB(users);
    }
}

void DB::writeToDB(QJsonArray &data) {
    QJsonDocument jsonDoc(data);
    QByteArray jsonData = jsonDoc.toJson();
    this->dbfile->seek(0);
    this->dbfile->resize(0);
    dbfile->write(jsonData);
    dbfile->flush();
}

void DB::saveInToHistory(QJsonObject data) {

}

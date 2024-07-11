#include "DB.h"
#include "QJsonArray"
#include "QJsonObject"
#include "QJsonDocument"
#include "QFile"
#include "../exceptions/ModelNotFoundException.h"
#include <QDateTime>
const QString DB::usersDbPath = "./db.json";
const QString DB::historyDbPath = "./history.json";
DB* DB::instance = nullptr;

DB::DB() {
    this->usersFile = new QFile(DB::usersDbPath);
    this->historyFile = new QFile(DB::historyDbPath);
    if (!this->usersFile->open(QIODevice::ReadWrite)) {
        qWarning() << "Couldn't open users file";
    }
    if(!this->historyFile->open(QIODevice::ReadWrite)){
        qWarning() << "Couldn't open history file";
    }
}

DB::~DB() {
    this->usersFile->close();
    this->historyFile->close();
    delete usersFile;
    delete historyFile;
}

QJsonArray DB::getUsers() {
    this->usersFile->seek(0);
    QByteArray jsonData = this->usersFile->readAll();
    QJsonDocument jsonDoc = QJsonDocument::fromJson(jsonData);
    return jsonDoc.array();
}

void DB::createUser(const QJsonObject &data) {
    QJsonArray jsonArray = this->getUsers();
    jsonArray.append(data);

    this->writeToDB(jsonArray,this->usersFile);
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
        this->writeToDB(users,this->usersFile);
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
        this->writeToDB(users,this->usersFile);
    }
}

void DB::writeToDB(QJsonArray &data,QFile* file) {
    QJsonDocument jsonDoc(data);
    QByteArray jsonData = jsonDoc.toJson();
    file->seek(0);
    file->resize(0);
    file->write(jsonData);
    file->flush();
}

void DB::saveInToHistory(QJsonObject data) {
    QJsonArray jsonArray = this->getHistory();
    QDateTime currentDateTime = QDateTime::currentDateTime();

    QString formattedDateTime = currentDateTime.toString("yyyy-MM-dd HH:mm:ss");
    data["played_on"] = formattedDateTime;
    jsonArray.append(data);

    this->writeToDB(jsonArray,this->historyFile);
}

QJsonArray DB::getHistory() {
    this->historyFile->seek(0);
    QByteArray jsonData = this->historyFile->readAll();
    QJsonDocument jsonDoc = QJsonDocument::fromJson(jsonData);
    return jsonDoc.array();
}

QJsonArray DB::findHistory(const QString &username) {
    auto allHistory = this->getHistory();
    QJsonArray result;
    for(auto && it : allHistory){
        auto history = it.toObject();
        if (history.value("firstPlayer") == username || history.value("secondPlayer") == username){
            result.append(history);
        }
    }
    return result;
}

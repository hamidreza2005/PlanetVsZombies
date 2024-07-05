#include "AuthorizationController.h"
#include "Validation/Validator.h"
#include "../Database/DB.h"
#include <QCryptographicHash>

void AuthorizationController::login(TcpSocket *socket, const QJsonObject &request) {
    Validator validator;
    validator.validate(request,{
            {"password",{"min:8"}},
            {"username",{}}
    });
    if (!validator.successfull()){
        socket->sendValidationError(validator.getErrorsAsJsonValue());
        return;
    }

    QJsonObject response;
    try{
        auto user = DB::getInstance()->findUser({
                {"username",request["username"]},
                {"password",AuthorizationController::hashString(request["password"].toString())},
        });
        response["status"] = "200";
        response["user"] = user;
        socket->write(response);
    }catch(const std::invalid_argument &e){
        QJsonObject errorBag;
        QJsonArray error;
        error.append(e.what());
        errorBag["username"] = error;
        socket->sendValidationError(QJsonValue(errorBag),404);
    }
}

void AuthorizationController::registration(TcpSocket *socket, const QJsonObject &request) {
    Validator validator;
    validator.validate(request,{
            {"email",{"email"}},
            {"password",{"min:8"}},
            {"phone",{"phoneNumber"}},
            {"name",{}},
            {"username",{}}
    });
    if (!validator.successfull()){
        socket->sendValidationError(validator.getErrorsAsJsonValue());
        return;
    }

    auto uniqueErrors = AuthorizationController::userUniqueDataErrors({
        {"email",request["email"]},
        {"username",request["username"]},
        {"phoneNumber",request["phone"]},
    });

    if (!uniqueErrors.isEmpty()){
        socket->sendValidationError(QJsonValue(uniqueErrors));
        return;
    }

    QJsonObject response;
    DB::getInstance()->createUser(AuthorizationController::getUserDataToRegister(request));
    response["status"] = "200";
    socket->write(response);
}

QString AuthorizationController::hashString(const QString &str) {
    QByteArray byteArray = str.toUtf8();

    QByteArray hash = QCryptographicHash::hash(byteArray, QCryptographicHash::Sha256);
    return QString::fromStdString(hash.toHex().toStdString());
}

QJsonObject AuthorizationController::getUserDataToRegister(const QJsonObject &request) {
    QString hashedPassword = AuthorizationController::hashString(request["password"].toString());
    return QJsonObject({
           {"name",request["name"]},
           {"email",request["email"]},
           {"username",request["username"]},
           {"phoneNumber",request["phone"]},
           {"password",hashedPassword}
    });
}

QJsonObject AuthorizationController::userUniqueDataErrors(const QJsonObject &data) {
    QJsonObject jsonObj;
    for (auto it = data.constBegin(); it != data.constEnd(); ++it) {
        if(DB::getInstance()->exists(it.key(),it.value().toString())){
            QJsonArray jsonArray;
            jsonArray.append(it.key() + " must be unique");
            jsonObj.insert(it.key(), jsonArray);
        }
    }
    return jsonObj;
}

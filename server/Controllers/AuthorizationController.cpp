#include "AuthorizationController.h"
#include "Validation/Validator.h"
#include "../Database/DB.h"
#include <QCryptographicHash>
#include "../exceptions/ModelNotFoundException.h"

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
    }catch(const ModelNotFoundException &e){
        socket->sendValidationError("username","Username Or Password is Incorrect",404);
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

void AuthorizationController::resetPassword(TcpSocket *socket, const QJsonObject &request) {
    Validator validator;
    validator.validate(request,{
            {"password",{"min:8"}},
            {"phone",{"phoneNumber"}},
    });
    if (!validator.successfull()){
        socket->sendValidationError(validator.getErrorsAsJsonValue());
        return;
    }

    try{
        QJsonObject response;
        DB::getInstance()->updateUser({
            {"phoneNumber",request["phone"]},
        },{{"password",AuthorizationController::hashString(request["password"].toString())}});

        response["status"] = "200";
        response["message"] = "Password Changed Successfully";
        socket->write(response);
    }catch(ModelNotFoundException &e){
        socket->sendValidationError("phoneNumber","phoneNumber does not exist",404);
    }
}

void AuthorizationController::changeCredentials(TcpSocket *socket, const QJsonObject &request) {
    QJsonObject req = request;
    Validator validator;
    validator.validate(req,{
            {"email",{"email"}},
            {"phoneNumber",{"phoneNumber"}},
            {"password",{}},
            {"name",{}},
            {"username",{}},
            {"currentUsername",{}},
    });
    if (!validator.successfull()){
        socket->sendValidationError(validator.getErrorsAsJsonValue());
        return;
    }
    if (req.value("password").toString() == ""){
        req.remove("password");
    }else{
        req["password"] = AuthorizationController::hashString(req["password"].toString());
    }

    QString currentUsername = req["currentUsername"].toString();
    req.remove("currentUsername");
    auto currentUser = DB::getInstance()->findUser({{"username",currentUsername}});

    auto uniqueErrors = AuthorizationController::checkIfUserNewDataIsUnique(req,currentUser);

    if (!uniqueErrors.isEmpty()){
        socket->sendValidationError(QJsonValue(uniqueErrors));
        return;
    }

    QJsonObject response;
    DB::getInstance()->updateUser({{"username",currentUsername}},req);
    response["status"] = "200";
    response["user"] = QJsonObject{{"username",req["username"].toString()}};
    socket->write(response);
}

void AuthorizationController::getUserData(TcpSocket *socket, const QJsonObject &request) {
    Validator validator;
    validator.validate(request,{
            {"username",{}},
    });
    if (!validator.successfull()){
        socket->sendValidationError(validator.getErrorsAsJsonValue());
        return;
    }

    QJsonObject response;
    try{
        auto user = DB::getInstance()->findUser({{"username",request["username"].toString()}});
        response["status"] = "200";
        response["currentUser"] = user;
        socket->write(response);
    }catch (const ModelNotFoundException& e){
        socket->sendValidationError("username","Username Or Password is Incorrect",404);
    }

}

QJsonObject AuthorizationController::checkIfUserNewDataIsUnique(QJsonObject &request,const QJsonObject& currentUser) {
    QJsonObject data = {
            {"email",request["email"]},
            {"username",request["username"]},
            {"phoneNumber",request["phoneNumber"]},
    };

    if(request["email"] == currentUser["email"]){
        request.remove("email");
        data.remove("email");
    }

    if(request["username"] == currentUser["username"]){
        request.remove("username");
        data.remove("username");
    }

    if(request["phoneNumber"] == currentUser["phoneNumber"]){
        request.remove("phoneNumber");
        data.remove("phoneNumber");
    }
    return AuthorizationController::userUniqueDataErrors(data);
}

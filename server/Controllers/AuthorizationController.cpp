#include "AuthorizationController.h"
#include "Validation/Validator.h"
void AuthorizationController::login(TcpSocket *socket, const QJsonObject &request) {
    socket->write("hi");
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
    QJsonObject response;
    if (!validator.successfull()){
        response["errors"] = validator.getErrorsAsJsonValue();
        response["status"] = 419;
        socket->write(response);
        return;
    }
    response["status"] = "200";
    socket->write(response);
}

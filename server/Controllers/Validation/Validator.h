#ifndef SERVER_VALIDATOR_H
#define SERVER_VALIDATOR_H

#include <QJsonObject>
#include "QList"
#include "QMap"

class Validator {
private:
    QMap<QString,QList<QString>> *errorBags = nullptr;

    void addErrorToBag(const QString &field,const QString &error);
public:
    Validator();
    ~Validator();
    void validate(const QJsonObject &data,const QMap<QString,QList<QString>> &rules);
    bool successfull();
    QMap<QString,QList<QString>> getErrors();
    QJsonValue getErrorsAsJsonValue();
};


#endif //SERVER_VALIDATOR_H

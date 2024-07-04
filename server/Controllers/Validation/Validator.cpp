#include <QJsonArray>
#include "Validator.h"
#include "Rules.h"

void Validator::validate(const QJsonObject &data, const QMap<QString,QList<QString>> &rules) {
    auto availableRules = Rules::availableRules;
    for (auto it = rules.begin(); it != rules.end() ; it++) {
        if(!data.contains(it.key())){
            this->addErrorToBag(it.key(),"field is required");
            continue;
        }
        QString fieldData = data.value(it.key()).toString();
        for (auto &rule : it.value()) {
            if (!availableRules.contains(rule)){
                throw std::invalid_argument("rule " + rule.toStdString() + "does not exist");
            }

            auto action = availableRules.value(rule).first;
            if (!action(fieldData)){
                this->addErrorToBag(it.key(),availableRules.value(rule).second);
            }

        }
    }
}

bool Validator::successfull() {
    return this->errorBags->isEmpty();
}

QMap<QString, QList<QString>> Validator::getErrors() {
    return *this->errorBags;
}

void Validator::addErrorToBag(const QString &field, const QString &error) {
    if (this->errorBags->contains(field)){
        this->errorBags->value(field).append(error);
    }else{
        this->errorBags->insert(field,{error});
    }
}

QJsonValue Validator::getErrorsAsJsonValue() {
    QJsonObject jsonObj;
    for (auto it = this->errorBags->constBegin(); it != this->errorBags->constEnd(); ++it) {
        QJsonArray jsonArray;
        for (const auto &str : it.value()) {
            jsonArray.append(str);
        }
        jsonObj.insert(it.key(), jsonArray);
    }
    return QJsonValue(jsonObj);
}

Validator::Validator() {
    this->errorBags = new QMap<QString,QList<QString>>();
}

Validator::~Validator() {
    delete errorBags;
}

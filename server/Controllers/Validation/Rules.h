#ifndef SERVER_RULES_H
#define SERVER_RULES_H
#include "QString"
#include "QMap"

class Rules {
public:
    static QMap<QString,QPair<std::function<bool(const QString&)>,QString>> availableRules;

    static bool isEmail(const QString& email);
    static bool isPhoneNumber(const QString& phone);
    static bool is8CharacterMin(const QString& field);
};


#endif //SERVER_RULES_H

#include "Rules.h"
#include "QRegularExpression"

QMap<QString,QPair<std::function<bool(const QString&)>,QString>> Rules::availableRules = {
        {"email",{Rules::isEmail,"Please enter a valid email"}},
        {"phoneNumber",{Rules::isPhoneNumber,"Please enter a valid phoneNumber"}},
        {"min:8",{Rules::is8CharacterMin,"the field must be at least 8 character"}},
};
bool Rules::isEmail(const QString &email) {
    QRegularExpression emailPattern(R"((\w+)(\.{1}\w+)*@(\w+)\.(\w+)(\.\w+)*)");
    QRegularExpressionMatch match = emailPattern.match(email);
    return match.hasMatch();
}

bool Rules::is8CharacterMin(const QString &field) {
    return field.size() >= 8;
}

bool Rules::isPhoneNumber(const QString &phone) {
    QRegularExpression phonePattern(R"(\+?(\d[\d-. ]+)?(\([\d-. ]+\))?[\d-. ]+\d)");
    QRegularExpressionMatch match = phonePattern.match(phone);
    return match.hasMatch();
}

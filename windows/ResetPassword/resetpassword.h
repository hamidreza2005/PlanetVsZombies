//
// Created by hrhas on 7/5/2024.
//

#ifndef PLANETVSZOMBIES_RESETPASSWORD_H
#define PLANETVSZOMBIES_RESETPASSWORD_H

#include <QWidget>


QT_BEGIN_NAMESPACE
namespace Ui { class ResetPassword; }
QT_END_NAMESPACE

class ResetPassword : public QWidget {
Q_OBJECT

public:
    explicit ResetPassword(QWidget *parent = nullptr);

    ~ResetPassword() override;

private:
    Ui::ResetPassword *ui;
};


#endif //PLANETVSZOMBIES_RESETPASSWORD_H

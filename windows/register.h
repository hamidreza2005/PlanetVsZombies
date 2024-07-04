//
// Created by hrhas on 7/4/2024.
//

#ifndef PLANETVSZOMBIES_REGISTER_H
#define PLANETVSZOMBIES_REGISTER_H

#include <QWidget>


QT_BEGIN_NAMESPACE
namespace Ui { class Register; }
QT_END_NAMESPACE

class Register : public QWidget {
Q_OBJECT

public:
    explicit Register(QWidget *parent = nullptr);

    ~Register() override;

private:
    Ui::Register *ui;
};


#endif //PLANETVSZOMBIES_REGISTER_H

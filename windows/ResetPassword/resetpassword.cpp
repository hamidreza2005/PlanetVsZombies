//
// Created by hrhas on 7/5/2024.
//

// You may need to build the project (run Qt uic code generator) to get "ui_ResetPassword.h" resolved

#include "resetpassword.h"
#include "ui_ResetPassword.h"


ResetPassword::ResetPassword(QWidget *parent) :
        QWidget(parent), ui(new Ui::ResetPassword) {
    ui->setupUi(this);
}

ResetPassword::~ResetPassword() {
    delete ui;
}

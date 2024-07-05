//
// Created by hrhas on 7/5/2024.
//

// You may need to build the project (run Qt uic code generator) to get "ui_Dashboard.h" resolved

#include "dashboard.h"
#include "ui_Dashboard.h"


Dashboard::Dashboard(QWidget *parent) :
        QWidget(parent), ui(new Ui::Dashboard) {
    ui->setupUi(this);
}

Dashboard::~Dashboard() {
    delete ui;
}

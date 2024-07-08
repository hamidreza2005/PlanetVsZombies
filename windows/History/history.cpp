//
// Created by hrhas on 7/8/2024.
//

// You may need to build the project (run Qt uic code generator) to get "ui_History.h" resolved

#include "history.h"
#include "ui_History.h"


History::History(QWidget *parent) :
        QWidget(parent), ui(new Ui::History) {
    ui->setupUi(this);
}

History::~History() {
    delete ui;
}

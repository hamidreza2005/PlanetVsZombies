//
// Created by hrhas on 7/5/2024.
//

#ifndef PLANETVSZOMBIES_DASHBOARD_H
#define PLANETVSZOMBIES_DASHBOARD_H

#include <QWidget>


QT_BEGIN_NAMESPACE
namespace Ui { class Dashboard; }
QT_END_NAMESPACE

class Dashboard : public QWidget {
Q_OBJECT

public:
    explicit Dashboard(QWidget *parent = nullptr);

    ~Dashboard() override;

private:
    Ui::Dashboard *ui;
};


#endif //PLANETVSZOMBIES_DASHBOARD_H

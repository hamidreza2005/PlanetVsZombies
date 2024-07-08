//
// Created by hrhas on 7/8/2024.
//

#ifndef PLANETVSZOMBIES_HISTORY_H
#define PLANETVSZOMBIES_HISTORY_H

#include <QWidget>


QT_BEGIN_NAMESPACE
namespace Ui { class History; }
QT_END_NAMESPACE

class History : public QWidget {
Q_OBJECT

public:
    explicit History(QWidget *parent = nullptr);

    ~History() override;

private:
    Ui::History *ui;
};


#endif //PLANETVSZOMBIES_HISTORY_H

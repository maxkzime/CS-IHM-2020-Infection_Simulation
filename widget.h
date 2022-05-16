// CS-IHM-2020/Infection-Simulation/widget.h
// BODIN Maxime C2
// 22/16/05

#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QGraphicsScene>
#include <QTimer>
#include <QMessageBox>
#include <QDebug>
#include <QIcon>

#include "disease.h"
//#include "person.h"

QT_BEGIN_NAMESPACE
namespace Ui { class Widget; }
QT_END_NAMESPACE

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);
    ~Widget();

private slots:
    void on_hsNbDiseaseDays_valueChanged(int value);
    void on_hSTransmissionRate_valueChanged(int value);
    void on_hSNbIncubationDays_valueChanged(int value);
    void on_hsTimerSpeed_valueChanged(int value);

    void on_pbStop_clicked();
    void on_pbStart_clicked();
    void on_pbStepByStep_clicked();
    void on_pbPause_clicked();

    void on_spinBox_wishingDate_valueChanged(int arg1);

private:
    Ui::Widget *ui;

    int itsPopulationSize = 40;
    QTimer * itsTimer = new QTimer(this);
    int timerSpeed = 1000;
    QGraphicsScene * itsMainScene = new QGraphicsScene(this);

    int itsDate = 0;
    bool itsTriggerTime = true;

    void start();
    void stop();
    void initialize();
    void initializeIncubationAtCenter();
    void stepByStep();
    void updateStats();
    void resetPopulation();

    Disease * itsDisease = nullptr;

    QIcon itsPauseIcon;
    QIcon itsStartIcon;

};
#endif // WIDGET_H

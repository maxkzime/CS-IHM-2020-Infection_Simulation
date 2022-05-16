// CS-IHM-2020/Infection-Simulation/widget.cpp
// BODIN Maxime C2
// 22/16/05

#include "widget.h"
#include "ui_widget.h"

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);

    // pb Pause settings
    itsPauseIcon.addPixmap(QPixmap(":/imgs/pause.png").scaled(QSize(156,156)));
    itsStartIcon.addPixmap(QPixmap(":/imgs/start.png").scaled(QSize(156,156)));
    ui->pbPause->setIcon(itsPauseIcon);
    ui->pbPause->setIconSize(QSize(25,25));

    // Scene settings
    itsMainScene->setSceneRect(0,0,itsPopulationSize*10,itsPopulationSize*10);


    initialize();

    // timer loop
    connect(itsTimer, &QTimer::timeout, this,[=](){
        stepByStep();
    });

    ui->graphicsView->setScene(itsMainScene);
}


/* Initialize statistics :
 * date, new disease, first cases in the middle of the scene,
 * people positions, grid lines, and update the scene after */
void Widget::initialize()
{

    // initialize date and update date statistic
    itsDate = 0;

    if(itsDisease == nullptr)
        itsDisease = new Disease();

    initializeIncubationAtCenter();

    updateStats();

    for(const vector <Person*> &v: itsDisease->getItsPopulation()){
        for(Person * p : v){
            itsMainScene->addItem(p);
            p->setPos(10*p->getItsCol(),10*p->getItsRow());
        }
    }

    // grid lines position
    for(int i = 0; i <=itsPopulationSize*10; i = i+10)
    {
        itsMainScene->addLine(0+i,0,0+i,itsPopulationSize*10,QPen(Qt::gray));
        itsMainScene->addLine(0,0+i,itsPopulationSize*10,0+i,QPen(Qt::gray));
    }

    itsMainScene->update(itsMainScene->sceneRect());
}


// Initialize incubation at the center (cross shaped)
void Widget::initializeIncubationAtCenter()
{
    int midPopSize = itsPopulationSize/2;
    for(const vector <Person*> &v: itsDisease->getItsPopulation()){
        for(Person * p : v){
            if((p->getItsCol() == midPopSize && p->getItsRow() == midPopSize) ||
                    (p->getItsCol() == midPopSize-1 && p->getItsRow() == midPopSize) ||
                    (p->getItsCol() == midPopSize && p->getItsRow() == midPopSize+1) ||
                    (p->getItsCol() == midPopSize && p->getItsRow() == midPopSize-1) ||
                    (p->getItsCol() == midPopSize+1 && p->getItsRow() == midPopSize))
            {
                p->updateState(INCUBATION,itsDate);
                p->draw();
            }
        }
    }
}


/* Call stop() when we click on the "stop" button */
void Widget::on_pbStop_clicked() { stop(); }


/* If there's a cycle running, stop the timer,
 * reset date, reset people states, initialize incubation at the center.
 * If there's no cycle running, print an error message.
 */
void Widget::stop()
{
    if(!itsTriggerTime)
    {
        itsTimer->stop();

        ui->pbPause->setIcon(itsPauseIcon);

        itsDate = 0;

        resetPopulation();

        updateStats();

        itsTriggerTime  = true; // Unlock start button
    }
    else
    {
        QMessageBox errorBox;
        errorBox.setWindowTitle("ERREUR : cycle inexistant");
        errorBox.setText("Vous devez lancer un cycle pour pouvoir le stopper");
        errorBox.setIcon(QMessageBox::Critical);
        errorBox.setStandardButtons(QMessageBox::Close);
        errorBox.setDefaultButton(QMessageBox::Close);
        errorBox.exec();
    }
}


/* Call start() when we click on the "start" button */
void Widget::on_pbStart_clicked() {  start(); }


/* If there's no cycle already running,
 * reset the date and the population, start the timer with the speed defined by the user.
 * If there's a cycle running, print an error message.
 */
void Widget::start()
{
    if(itsTriggerTime)
    {
        itsDate = 0;

        resetPopulation();

        itsTimer->start(timerSpeed);

        itsTriggerTime = false; // Unlock stop button
    }
    else
    {
        QMessageBox errorBox;
        errorBox.setWindowTitle("ERREUR : cycle existant");
        errorBox.setText("Vous devez stopper le cycle pour pouvoir en commencer un");
        errorBox.setIcon(QMessageBox::Critical);
        errorBox.setStandardButtons(QMessageBox::Close);
        errorBox.setDefaultButton(QMessageBox::Close);
        errorBox.exec();
    }
}


/* Call stepByStep() when we click on the "step by step" button */
void Widget::on_pbStepByStep_clicked() { stepByStep(); }


/* Step by step : update date,
 * and update population accordingly with their state,
 * also update propagation and updating the scene.
 */
void Widget::stepByStep()
{
    itsDate++;

    itsDisease->update(itsDate);
    itsDisease->draw();

    updateStats();

    itsMainScene->update(itsMainScene->sceneRect()); // Update the scene
}


/* Update statistics : date, infected number, recover number, susceptible number and incubed number */
void Widget::updateStats()
{
    // Population size statistic
    ui->label_popStats->setText(QString::fromStdString(std::to_string(itsPopulationSize)));

    // Update date
    ui->label_dateStats->setText(QString::fromStdString(std::to_string(itsDate)));

    // Update statistics
    ui->label_infectedStats->setText(QString::fromStdString(std::to_string(itsDisease->getItsStats().at(3))));
    ui->label_recoverStats->setText(QString::fromStdString(std::to_string(itsDisease->getItsStats().at(2))));
    ui->label_susceptibleStats->setText(QString::fromStdString(std::to_string(itsDisease->getItsStats().at(1))));
    ui->label_incubedStats->setText(QString::fromStdString(std::to_string(itsDisease->getItsStats().at(0))));
}


/* Reset population states and initialize incubation at the center. */
void Widget::resetPopulation()
{
    for(const vector <Person*> &v: itsDisease->getItsPopulation()){
        for(Person * p : v){
            if(p->getItsState() != SUSCEPTIBLE)
            {
                p->updateState(SUSCEPTIBLE,itsDate);
                p->draw();
            }
        }
    }

    initializeIncubationAtCenter();
}


/* Update number of sick days,
 * when the slider value is changed and if there's no cycle running
 * (else we print an error message).
 */
void Widget::on_hsNbDiseaseDays_valueChanged(int value)
{
    if(itsTriggerTime){
        // number of disease days (between 1 and 20)
        ui->labelResultNbDiseaseDays->setText(QString::fromStdString("\t" + std::to_string(value)));

        // Update disease statistics
        itsDisease->setItsDaysWithSymptoms(value);
    }
    else
    {
        QMessageBox errorBox;
        errorBox.setWindowTitle("ERREUR : cycle en cours");
        errorBox.setText("Vous devez stopper le cycle pour pouvoir modifier les statistiques");
        errorBox.setIcon(QMessageBox::Critical);
        errorBox.setStandardButtons(QMessageBox::Close);
        errorBox.setDefaultButton(QMessageBox::Close);
        errorBox.exec();
    }

}


/* Update the transmission rate,
 * when the slider value is changed and if there's no cycle running
 * (else we print an error message).
 */
void Widget::on_hSTransmissionRate_valueChanged(int value)
{
    if(itsTriggerTime){
        // Transmission rate (between 0 and 1)
        ui->labelResultTransmissionRate->setText(QString::fromStdString("\t" + std::to_string((double)value/10000)));

        // Update disease statistics
        itsDisease->setItsTransmissionRate((double)value/10000);
    }
    else
    {
        QMessageBox errorBox;
        errorBox.setWindowTitle("ERREUR : cycle en cours");
        errorBox.setText("Vous devez stopper le cycle pour pouvoir modifier les statistiques");
        errorBox.setIcon(QMessageBox::Critical);
        errorBox.setStandardButtons(QMessageBox::Close);
        errorBox.setDefaultButton(QMessageBox::Close);
        errorBox.exec();
    }
}


/* Update number of incubation days,
 * when the slider value is changed and if there's no cycle running
 * (else we print an error message).
 */
void Widget::on_hSNbIncubationDays_valueChanged(int value)
{
    if(itsTriggerTime){
        // number of incubation days (between 1 and 20)
        ui->labelResultNbIncubationDays->setText(QString::fromStdString("\t" + std::to_string(value)));

        // Update disease statistics
        itsDisease->setItsDaysInIncubation(value);
    }
    else
    {
        QMessageBox errorBox;
        errorBox.setWindowTitle("ERREUR : cycle en cours");
        errorBox.setText("Vous devez stopper le cycle pour pouvoir modifier les statistiques");
        errorBox.setIcon(QMessageBox::Critical);
        errorBox.setStandardButtons(QMessageBox::Close);
        errorBox.setDefaultButton(QMessageBox::Close);
        errorBox.exec();
    }
}


/* Update the timer speed,
 * when the slider value is changed and if there's no cycle running
 * (else we print an error message).
 */
void Widget::on_hsTimerSpeed_valueChanged(int value)
{
    if(itsTriggerTime){
        // Timer speed (between 10 and 1000 in ms)
        ui->labelResultTimerSpeed->setText(QString::fromStdString("\t" + std::to_string(value)));

        // Update timer speed
        timerSpeed = value;
    }
    else
    {
        QMessageBox errorBox;
        errorBox.setWindowTitle("ERREUR : cycle en cours");
        errorBox.setText("Vous devez stopper le cycle pour pouvoir modifier les statistiques");
        errorBox.setIcon(QMessageBox::Critical);
        errorBox.setStandardButtons(QMessageBox::Close);
        errorBox.setDefaultButton(QMessageBox::Close);
        errorBox.exec();
    }
}


/* When click on pause pb,if there's a cycle running,
 * and if the timer is running stop the timer and update the button icon,
 * else start the timer and update the button icon. Else we print an error message.
 */
void Widget::on_pbPause_clicked()
{
    if(!itsTriggerTime){
        if(itsTimer->isActive()){
            itsTimer->stop();
            ui->pbPause->setIcon(itsStartIcon);
        }else{
            itsTimer->start(timerSpeed);
            ui->pbPause->setIcon(itsPauseIcon);
        }
    }
    else
    {
        QMessageBox errorBox;
        errorBox.setWindowTitle("ERREUR : cycle inexistant");
        errorBox.setText("Vous devez lancer un cycle pour pouvoir le mettre en pause");
        errorBox.setIcon(QMessageBox::Critical);
        errorBox.setStandardButtons(QMessageBox::Close);
        errorBox.setDefaultButton(QMessageBox::Close);
        errorBox.exec();
    }
}


/* When the player enter a wishing day we reset the population,
 * then iterate to this day.
 */
void Widget::on_spinBox_wishingDate_valueChanged(int arg1)
{
    itsDate = 0;
    resetPopulation();

    for (int i = 0; i < arg1 ; i++) {
        stepByStep();
    }
}


Widget::~Widget()
{
    delete ui;
    delete itsDisease;
}

// CS-IHM-2020/Infection-Simulation/person.h
// BODIN Maxime C2
// 22/16/05

#ifndef PERSON_H
#define PERSON_H

#include <QObject>
#include <QGraphicsPixmapItem>
#include <QGraphicsScene>
#include <QGraphicsSceneMouseEvent>

#include "PersonState.h"

class Person : public QObject,  public QGraphicsPixmapItem
{
    Q_OBJECT

public:
    Person(QPixmap pixmap = QPixmap(":/imgs/ERROR.png"));

    void updateState(PersonState aNewState, int aDate);
    void draw();
    bool isInIncubation();

    /* Setters */
    void setItsRow(int newItsRow);
    void setItsCol(int newItsCol);
    void setItsAvatar(const QPixmap &newItsAvatar);

    /* Getters */
    int getItsRow() const;
    int getItsCol() const;
    int getItsUpdate() const;
    PersonState getItsState() const;

private:
    PersonState itsState = SUSCEPTIBLE;
    int itsUpdate = 0;
    QPixmap itsAvatar;

    int itsRow = 0;
    int itsCol = 0;

public slots:
    void mousePressEvent(QGraphicsSceneMouseEvent *event);
};

#endif // PERSON_H

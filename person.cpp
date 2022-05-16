// CS-IHM-2020/Infection-Simulation/person.cpp
// BODIN Maxime C2
// 22/16/05

#include "person.h"

Person::Person(QPixmap pixmap)
{
    setPixmap(pixmap);

    // Initialize avatar
    updateState(SUSCEPTIBLE,0);
    draw();
}

/* Update state and avatar accordingly */
void Person::updateState(PersonState aNewState, int aDate)
{
    itsUpdate = aDate;

    itsState = aNewState;

    switch (aNewState) {
    case SUSCEPTIBLE:
        setItsAvatar(QPixmap(":/imgs/SUSCEPTIBLE.png"));
        break;
    case INCUBATION:
        setItsAvatar(QPixmap(":/imgs/INCUBATION.png"));
        break;
    case INFECTED:
        setItsAvatar(QPixmap(":/imgs/INFECTED.png"));
        break;
    case RECOVER:
        setItsAvatar(QPixmap(":/imgs/RECOVER.png"));
        break;

    }
}


/* Update person's pixmap */
void Person::draw()
{
    setPixmap(itsAvatar);
}


// Return true if is in incubation
bool Person::isInIncubation()
{
    bool incubationValidation = false;
    if(itsState == INCUBATION)
        incubationValidation = true;

    return incubationValidation;
}


/* Mouse press button detection on case,
 * Right click : case is in incubation
 * Left click : case is susceptible
 */
void Person::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    if (event->button() != Qt::RightButton && itsState != INCUBATION)
    {
        updateState(INCUBATION,0);
        draw();
    }
    else if(event->button() != Qt::LeftButton && itsState != SUSCEPTIBLE)
    {
        updateState(SUSCEPTIBLE,0);
        draw();
    }
}


/* Getters */

int Person::getItsUpdate() const { return itsUpdate; }

PersonState Person::getItsState() const { return itsState; }

int Person::getItsRow() const { return itsRow; }

int Person::getItsCol() const { return itsCol; }


/* Setters */

void Person::setItsRow(int newItsRow) { itsRow = newItsRow; }

void Person::setItsCol(int newItsCol) { itsCol = newItsCol; }

void Person::setItsAvatar(const QPixmap &newItsAvatar) { itsAvatar = newItsAvatar; }



// CS-IHM-2020/Infection-Simulation/disease.h
// BODIN Maxime C2
// 22/16/05

#ifndef DISEASE_H
#define DISEASE_H

#include <vector>
#include <random> // random number
#include <chrono> // random generator seed

#include "person.h"

using std::vector;


class Disease
{
public:
    Disease();

    void update(int date);
    bool isNeighbourInIncubation(int aRow, int aCol);
    bool isInfected();
    void draw();


    /* Getters */
    double getItsTransmissionRate() const;
    int getItsDaysInIncubation() const;
    int getItsDaysWithSymptoms() const;
    const vector<vector<Person *> > &getItsPopulation() const;

    vector<int> getItsStats();

    /* Setters */
    void setItsPopulation(const vector<vector<Person *> > &newItsPopulation);
    void setItsTransmissionRate(double newItsTransmissionRate);
    void setItsDaysInIncubation(int newItsDaysInIncubation);
    void setItsDaysWithSymptoms(int newItsDaysWithSymptoms);

    std::string personStateToStr(PersonState aState);

private:
    double itsTransmissionRate = 0.1;
    int itsDaysInIncubation = 1;
    int itsDaysWithSymptoms = 1;

    vector<vector <Person *>> itsPopulation;


};

#endif // DISEASE_H

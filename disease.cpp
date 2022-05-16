// CS-IHM-2020/Infection-Simulation/disease.cpp
// BODIN Maxime C2
// 22/16/05

#include "disease.h"

Disease::Disease()
{
    /* Initialize population 40*40 */
    for(int i = 0; i < 40; i++)
    {
        vector<Person *>* row = new vector<Person*>;

        for(int j = 0; j < 40; j++)
        {
            Person * p = new Person();
            p->setItsCol(j);
            p->setItsRow(i);
            row->push_back(p);
        }

        itsPopulation.push_back(*row);

        row->clear();
    }
}


/* Update each person in the population,
 * accordingly with its state and the date.
 */
void Disease::update(int date)
{
    for(const vector <Person*> &v: itsPopulation){
        for(Person * p : v)
        {
            if(p->getItsState() == SUSCEPTIBLE &&
                    (isNeighbourInIncubation(p->getItsRow(),p->getItsCol()) &&
                     isInfected())){
                p->updateState(INCUBATION,date);
            }

            if(p->getItsState() == INCUBATION &&
                    p->getItsUpdate() == date-itsDaysInIncubation){
                p->updateState(INFECTED,date);
            }

            if(p->getItsState() == INFECTED &&
                    p->getItsUpdate() == date-itsDaysWithSymptoms){
                p->updateState(RECOVER,date);
            }
        }
    }
}


/* Translate a state to string */
std::string Disease::personStateToStr(PersonState aState){
    std::string str = "ERROR";
    switch (aState) {
    case SUSCEPTIBLE:str = "SUCESPTIBLE";
        break;
    case INCUBATION:str = "INCUBATION";
        break;
    case INFECTED:str = "INFECTED";
        break;
    case RECOVER:str = "RECOVER";
        break;
    }

    return str;
}


/* Return true if one the person's neighbour is in incubation, (up, down, left, right) */
bool Disease::isNeighbourInIncubation(int aRow, int aCol)
{
    bool neighbourIncubationValidation = false;
    for(const vector <Person*> &v: itsPopulation){
        for(Person * p : v){
            if((p->getItsRow() == aRow+1 && p->getItsCol() == aCol) ||
                    (p->getItsRow() == aRow-1 && p->getItsCol() == aCol) ||
                    (p->getItsRow() == aRow && p->getItsCol() == aCol+1) ||
                    (p->getItsRow() == aRow && p->getItsCol() == aCol-1))
            {
                if(p->isInIncubation()){
                    neighbourIncubationValidation = true;
                }
            }
        }
    }

    return neighbourIncubationValidation;
}


/* Return true if a random float is inferior or equal to the transmission rate */
bool Disease::isInfected()
{
    // random number generator
    auto seed = std::chrono::high_resolution_clock::now().time_since_epoch().count();
    static std::mt19937 gen(seed);

    std::uniform_real_distribution<> distribNumber(0.001,0.999);

    return distribNumber(gen) <= itsTransmissionRate;
}


/* Update the population pixmap */
void Disease::draw()
{
    for(const vector <Person*> &v: itsPopulation){
        for(Person * p : v){
            p->draw();
        }
    }
}



/* Getters */

double Disease::getItsTransmissionRate() const { return itsTransmissionRate; }

int Disease::getItsDaysInIncubation() const { return itsDaysInIncubation; }

int Disease::getItsDaysWithSymptoms() const { return itsDaysWithSymptoms; }

const vector<vector<Person *> > &Disease::getItsPopulation() const { return itsPopulation; }

/* Return the number of incubated, susceptible, infected and recovered people */
vector<int> Disease::getItsStats()
{
    vector<int> incubatedCounter(4,0);
    for(const vector <Person*> &v: itsPopulation){
        for(Person * p : v){
            if(p->isInIncubation()){
                incubatedCounter.at(0)++;
            }else if(p->getItsState() == SUSCEPTIBLE){
                incubatedCounter.at(1)++;
            }else if(p->getItsState() == RECOVER){
                incubatedCounter.at(2)++;
            }else if(p->getItsState() == INFECTED){
                incubatedCounter.at(3)++;
            }
        }
    }
    return incubatedCounter;
}


/* Setters */

void Disease::setItsTransmissionRate(double newItsTransmissionRate) { itsTransmissionRate = newItsTransmissionRate; }

void Disease::setItsDaysInIncubation(int newItsDaysInIncubation) { itsDaysInIncubation = newItsDaysInIncubation; }

void Disease::setItsDaysWithSymptoms(int newItsDaysWithSymptoms) { itsDaysWithSymptoms = newItsDaysWithSymptoms; }

void Disease::setItsPopulation(const vector<vector<Person *> > &newItsPopulation) { itsPopulation = newItsPopulation; }

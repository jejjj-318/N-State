#ifndef N_STATE_SIMULATION_H
#define N_STATE_SIMULATION_H

#include "../country/country.h"

class Simulation
{
public:
    static void advanceMonth(Country& country);
};

#endif // N_STATE_SIMULATION_H

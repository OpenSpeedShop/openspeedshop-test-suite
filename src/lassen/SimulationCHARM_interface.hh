
#ifndef SimulationCHARM_interface_hh_
#define SimulationCHARM_interface_hh_

#include "Simulation.hh"

class Initializer {
public:
   virtual void initialize( Lassen::Simulation *sim ) = 0;
};


void SimulationCHARM_start( int nchares, Initializer *init );


#endif

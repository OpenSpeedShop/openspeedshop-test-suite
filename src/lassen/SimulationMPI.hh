#ifndef SIMULATION_MPI
#define SIMULATION_MPI

#include "mpi.h"
#include "SimulationParallel.hh"
#include "Lassen.hh"

namespace Lassen {
   
class SimulationMPI : public SimulationParallel {
public:
   
   SimulationMPI( );
   ~SimulationMPI() {}
   
   void setComm( MPI_Comm _comm ) { comm = _comm; }
   
   void run();
   
protected:

   void step( StepState &stepState );
   void parallelInitialize();

   
   Real globalMin( Real local ) const; 
   Real globalMax( Real local ) const; 

   void synchronizeNodeData( );

   void communicateFront( StepState &stepState );

   
   // initialization functions
   void initializeNodeCommunication(const std::vector<int> &communicatingNodes);
   void initializeFacetCommunication(const std::vector<int> &boundaryNodes);


   void packFacet( const PlanarFront &planarFront, int facetIndex, std::vector<Real> &buffer) const;
   void unpackFacet( const std::vector<Real> &buffer, PlanarFront &planarFront);
      
protected:

   MPI_Comm comm;
};
   
};

#endif

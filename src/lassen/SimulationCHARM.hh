#ifndef SIMULATION_CHARM
#define SIMULATION_CHARM

#include "Lassen.hh"
#include <vector>
#include "pup_stl.h"
#include "SimulationParallel.hh"
#include "Simulation.hh"
#include "Input.hh"

#include "SimulationCHARM_interface.hh"
extern Initializer *initPtr;

// fixme:  find a better way to handle namespaces?
using namespace Lassen;

// PUP for Front objects
// Use PUPbytes macro for classes that are plain data
PUPbytes(BoundingBox);
PUPbytes(Point);
PUPbytes(ComputeErrors);

// Mesh PUP
inline void operator|(PUP::er &p, Mesh &mesh) {
   p| mesh.dim;
   
   p| mesh.nnodes;
   p| mesh.nodeToZoneOffset;
   p| mesh.nodeToZone;
   p| mesh.nodePoint;

   p| mesh.nzones;
   p| mesh.zoneToNodeCount;
   p| mesh.zoneToNode;

   p| mesh.nodeGlobalID;
   p| mesh.nLocalZones;
   p| mesh.nLocalNodes;
   
}

// Domain PUP
inline void operator|(PUP::er &p, Domain &domain) {
   p| *domain.mesh;
   p| domain.domainID;
   p| domain.numDomains;
   p| domain.neighborDomains;
   if (p.isUnpacking()) {
      domain.initializeGlobalToLocal();
   }
}


// PlanarFront PUP
inline void operator|(PUP::er &p, PlanarFront &front) {
   p| front.vertices;
   p| front.facetToVertexOffset;
   p| front.facetZone;
   p| front.facetCenter;
   p| front.facetVelocity;
   p| front.nfacets;
}


PUPbytes(NodeData);
PUPbytes(NodeState::Enum);
PUPbytes(PointSource);



#include "SimulationCHARM.decl.h"
#include "Output.hh"

extern CProxy_Interface interfaceProxy;
extern CProxy_SimulationCHARM simProxy;
extern CkReduction::reducerType reduce_CheckErrorType;


class Interface : public CBase_Interface {
public:
   Interface_SDAG_CODE;
   Interface( int numDomains );
   Interface( CkMigrateMessage *m) {
      __sdag_init();
   }

};


class SimulationCHARM : public CBase_SimulationCHARM, public SimulationParallel {
public:
   SimulationCHARM_SDAG_CODE;
   
   SimulationCHARM( );
   virtual ~SimulationCHARM( );
   SimulationCHARM( CkMigrateMessage *m );

   void setup( ) {
      if (initPtr) {
         initPtr->initialize( dynamic_cast<Simulation*>(this) );
         // Creating a simulation chare
         interfaceProxy.doneSetup();
      }
   }
   void pup( PUP::er &p);
             
   void ResumeFromSync() {
      thisProxy[thisIndex].doneAtSync();
   }


   void UserSetLBLoad(); // override virtual function
   
public:

   // for node communication
   struct initializeData {
      int imsg1, imsg2, imsg3, imsg4;
      std::vector<int> boundaryNodes;
      std::vector<int> communicatingNodes;
      std::vector< std::pair< int, int > > nodeToDomainPair;
      int numOverlap;
      BoundingBox boundingBox;
      std::vector<Point> boundaryNodePoints;
      std::vector<Point> zoneCenter;
      SpatialGrid *grid;
      Real cutoffDistance;
      std::vector< std::pair<int, int> > facetToDomainPair;
      bool checkErrors;
   } initData;
   
   struct runData {
      int imsg1;
      StepState stepState;
      std::vector<int> activeDomains;
      int done;
   } runData;

   struct CommunicateFrontData {
      int imsg;
   } commFront;

   struct LastReduction {
      Real value;
   } lastReduction;
   

};

////////////////////////////////////////////////////////////////////////////////



#endif

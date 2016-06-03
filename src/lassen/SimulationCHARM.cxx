
#include "SimulationCHARM.hh"
#include "Source.hh"
#include "Output.hh"

Initializer *initPtr = 0;

/// - reducer
CkReduction::reducerType reduce_CheckErrorType;

CkReductionMsg *reduce_CheckError(int nmsg, CkReductionMsg **msgs)
{
   ComputeErrors combine;
   
   for(int j = 0; j < nmsg; j++) {
      CkAssert(msgs[j]->getSize() == sizeof(ComputeErrors));
      ComputeErrors *tmp = (ComputeErrors *) msgs[j]->getData();
      combine.error1 += tmp->error1;
      combine.error2 += tmp->error2;
      combine.errorMax = std::max(combine.errorMax, tmp->errorMax );
      combine.errorNodeCount += tmp->errorNodeCount;
   }
   return CkReductionMsg::buildNew(sizeof(ComputeErrors), &combine);
}

void register_reduce_CheckError(){
   reduce_CheckErrorType = CkReduction::addReducer(reduce_CheckError);
}


// global readonly data
CProxy_Interface interfaceProxy;
CProxy_SimulationCHARM simProxy;
using namespace Lassen;


////////////////////////////////////////////////////////////////////////////////
// Interface

Interface::Interface( int numDomains)
{
   __sdag_init();
   interfaceProxy = thisProxy;
}


void SimulationCHARM_start( int numDomains, Initializer *init )
{
   initPtr = init;
   if (CkMyPe() == 0) {
      CProxy_Interface::ckNew(numDomains, 0);
      CkArrayOptions opts(numDomains);
      //CProxy_RRMap myMap = CProxy_RRMap::ckNew();
      //opts.setMap( myMap );
      opts.setAnytimeMigration(false);
      opts.setStaticInsertion(true);
      
      simProxy = CProxy_SimulationCHARM::ckNew( opts );
      simProxy.setup();
      interfaceProxy.run();
   }
   CsdScheduler(-1);
}


////////////////////////////////////////////////////////////////////////////////
/**
 */

SimulationCHARM::
SimulationCHARM()
{
   __sdag_init();
   // MIGRATE
   setMigratable(true);
   usesAtSync = true;
   usesAutoMeasure = false;
}

SimulationCHARM::
~SimulationCHARM()
{
}

SimulationCHARM::
SimulationCHARM( CkMigrateMessage *) 
{
   __sdag_init();
   // MIGRATE
   setMigratable(true);
   usesAtSync = true;
   usesAutoMeasure = false;
}

void
SimulationCHARM::
pup( PUP::er &p)
{
   CBase_SimulationCHARM::pup(p);
   __sdag_pup(p);

   // Simulation
   p| *domain;
   p| sources;
   p| narrowBandNodes;
   p| nodeState;
   p| nodeTimeReached;
   p| nodeLevel;
   p| nodeImagePoint;
   p| nodeImageVelocity;
   p| zoneVelocity;
   p| minVelocity;
   p| maxVelocity;
   p| minEdgeSize;
   p| narrowBandWidth;
   p| dt;
   p| time;
   p| cycle;
   p| sourcesComplete;
   p| numNodesUnreached;

   PUParray(p,(Real*)(&timers),sizeof(Timers)/sizeof(Real));
   
   // SimulationParallel
   p| nodeToDomainOffset;
   p| nodeToDomain;
   p| facetToDomainOffset;
   p| facetToDomain;
   p| facetNeighbors;
   p| numBusyDomains;  // Fixme - is this really state?  should be temporary
   p| numGlobalNarrowBandNodes; // Fixme, above

   
   if (p.isUnpacking()) {
      mesh = domain->mesh;
   }
}

void
SimulationCHARM::
UserSetLBLoad()
{
  if (numNodesUnreached == 0) {
     // Done
    setObjTime( 1.00 );
  }
  if (narrowBandNodes.size() > 0) {
    Real v = 50 + 100.0 * numNodesUnreached * 1.0 / mesh->nnodes;
    // On Narrowband.  
    setObjTime( v );
  }
  else {
    int nactive = 0;
    for (int i = 0 ; i <domain->numDomains; ++i) {
      if (runData.activeDomains[i] > 0) {
        nactive ++;
      }
    }
    if (nactive > 0) { 
      // Idle, but has active neighbors 
      setObjTime( 50.0 );
    }
    else {
      // Idle
      setObjTime( 20.0 );
    }
  }

}


// keep at the end of the file
#include "SimulationCHARM.def.h"

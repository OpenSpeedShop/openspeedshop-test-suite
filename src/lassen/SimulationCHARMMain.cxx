#include "Lassen.hh"
#include "Input.hh"
#include "SimulationCHARMMain.decl.h"
#include "SimulationCHARM.hh"
#include <algorithm>

using namespace Lassen;

class main : public CBase_main {
public:
   main( CkArgMsg *m);
};


main::main(CkArgMsg *m)
{
   // get the mesh specification
   int numIJKDomains[3];
   int numGlobalZones[3];
   Real zoneSizes[3];

   Point problemsize;
   if (!ProblemSize( m->argv[1], problemsize )) {
      CkAbort("Abort");
   }
   
   int dim;
   if (m->argc == 6 ) {
      dim = 2;
      numIJKDomains[0] = atoi(m->argv[2]);
      numIJKDomains[1] = atoi(m->argv[3]);
      numIJKDomains[2] = 1;
      numGlobalZones[0] = atoi(m->argv[4]);
      numGlobalZones[1] = atoi(m->argv[5]);;
      numGlobalZones[2] = 1;
   }
   else if (m->argc == 8) {
      dim = 3;
      numIJKDomains[0] = atoi(m->argv[2]);
      numIJKDomains[1] = atoi(m->argv[3]);
      numIJKDomains[2] = atoi(m->argv[4]);
      numGlobalZones[0] = atoi(m->argv[5]);
      numGlobalZones[1] = atoi(m->argv[6]);
      numGlobalZones[2] = atoi(m->argv[7]);
   }
   else {
      CkPrintf("Usage: %s domainsI domainsJ [domainsK] nx ny [nz]\n", m->argv[0]);
      CkAbort("Abort");
   }

   
   zoneSizes[0] = problemsize.x / numGlobalZones[0];
   zoneSizes[1] = problemsize.y / numGlobalZones[1];
   zoneSizes[2] = problemsize.z / numGlobalZones[2];

   CkAssert( numGlobalZones[0] >= numIJKDomains[0] );
   CkAssert( numGlobalZones[1] >= numIJKDomains[1] );
   CkAssert( numGlobalZones[2] >= numIJKDomains[2] );

   CkAssert( numGlobalZones[0] % numIJKDomains[0] == 0 );
   CkAssert( numGlobalZones[1] % numIJKDomains[1] == 0 );
   CkAssert( numGlobalZones[2] % numIJKDomains[2] == 0 );

   int numDomains = numIJKDomains[0] * numIJKDomains[1] * numIJKDomains[2];
   interfaceProxy = CProxy_Interface::ckNew(numDomains, 0);

   CkArrayOptions opts( numDomains );
   
   opts.setAnytimeMigration(false);
   opts.setStaticInsertion(true);
   
   simProxy = CProxy_SimulationCHARM::ckNew( opts );
   simProxy.setupRegular( m->argv[1], dim, numIJKDomains, numGlobalZones, zoneSizes);
   interfaceProxy.run();
}



// keep at the end of the file
#include "SimulationCHARMMain.def.h"


#include <iostream>
#include <vector>
#include <string>
#include "Output.hh"
#include "Source.hh"
#include "Input.hh"
#include "Lassen.hh"
#include <stdlib.h>

using namespace Lassen;


int main(int argc, char *argv[])
{
   Simulation sim;
   Domain *domain = sim.getDomain();
   
   if (argc < 4 || argc > 5) {
      std::cout << "Usage " << argv[0] << " initfile nx ny [nz] " << "\n";
      return 1;
   }

   Point problemsize;
   if (!ProblemSize( argv[1], problemsize )) {
      return 1;
   }
   std::cout << "PROBLEMSIZE: " << problemsize << "\n";
   // Gather command line arguments to specify the mesh
   int nx = 1;
   int ny = 1;
   int nz = 1;
   int dim = 2;
   if (argc == 4) {
      nx = atoi(argv[2]);
      ny = atoi(argv[3]);
      dim = 2;
   }
   if (argc == 5) {
      nx = atoi(argv[2]);
      ny = atoi(argv[3]);
      nz = atoi(argv[4]);
      dim = 3;
   }
   double dx = problemsize.x / nx;
   double dy = problemsize.y / ny;
   double dz = problemsize.z / nz;

   Real sourceRad;

   // Add the source
   if (dim == 2) {
      sourceRad = 2.0 * std::max(dx,dy);
   }
   else {
      sourceRad = 2.0 * std::max( dx, std::max(dy, dz) );
   }

   int numDomains[3] = {1,1,1};
   int numGlobalZones[3] = {nx,ny,nz};
   Real zoneSize[3] = {dx,dy,dz};

   
   MeshConstructor::MakeRegularDomain( domain, dim, 0, numDomains, numGlobalZones, zoneSize );

   ProblemSetup( argv[1], sim, sourceRad );

   sim.run();
#if HAVE_SILO
   OutputSilo silo( &sim, "lassen" );
   silo.writeSimulation();
#endif

   if (argv[1] == "default") {
      ComputeErrors err;
      err.computeLocalErrors( &sim );
      err.completeErrors();
      err.reportErrors();
   }
   
   return 0;
}


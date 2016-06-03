
#include <vector>
#include <string>
#include "Output.hh"
#include "Source.hh"
#include "Input.hh"
#include "SimulationMPI.hh"
#include <stdlib.h>
#include <stdio.h>


#include "mpi.h"

using namespace Lassen;

void computeErrors(const Simulation *sim );


                   
int main(int argc, char *argv[])
{

   MPI_Init(&argc, &argv);
   int commrank;
   int commsize;
   MPI_Comm_size( MPI_COMM_WORLD, &commsize);
   MPI_Comm_rank( MPI_COMM_WORLD, &commrank);

   int numDomains[3];
   int numGlobalZones[3];
   Real zoneSizes[3];
   int dim;

   if (argc != 6 && argc != 8) {
      std::cout << "Usage " << argv[0] << " initfile domainsI domainsJ [domainsK] nx ny [nz]\n";
      return 1;
   }

   Point problemsize;
   if (!ProblemSize( argv[1], problemsize )) {
      MPI_Finalize();
      return 1;
   }
   
   if (argc == 6 ) {
      dim = 2;
      numDomains[0] = atoi(argv[2]);
      numDomains[1] = atoi(argv[3]);
      numDomains[2] = 1;
      numGlobalZones[0] = atoi(argv[4]);
      numGlobalZones[1] = atoi(argv[5]);;
      numGlobalZones[2] = 1;
   }
   else if (argc == 8) {
      dim = 3;
      numDomains[0] = atoi(argv[2]);
      numDomains[1] = atoi(argv[3]);
      numDomains[2] = atoi(argv[4]);
      numGlobalZones[0] = atoi(argv[5]);
      numGlobalZones[1] = atoi(argv[6]);
      numGlobalZones[2] = atoi(argv[7]);
   }
   else {
      std::cout << argv[0] << " domainsI domainsJ [domainsK] nx ny [nz]\n";
      MPI_Abort(MPI_COMM_WORLD, 1);
      return 1;
   }
        
   zoneSizes[0] = problemsize.x / numGlobalZones[0];
   zoneSizes[1] = problemsize.y / numGlobalZones[1];
   zoneSizes[2] = problemsize.z / numGlobalZones[2];

   assert( numDomains[0] * numDomains[1] * numDomains[2] == commsize );
   assert( numGlobalZones[0] >= numDomains[0] );
   assert( numGlobalZones[1] >= numDomains[1] );
   assert( numGlobalZones[2] >= numDomains[2] );
   
   assert( numGlobalZones[0] % numDomains[0] == 0 );
   assert( numGlobalZones[1] % numDomains[1] == 0 );
   assert( numGlobalZones[2] % numDomains[2] == 0 );
   
   SimulationMPI sim;
   sim.setComm( MPI_COMM_WORLD );
   Domain *domain = sim.getDomain();
   Mesh *mesh = domain->mesh;
   
   MeshConstructor::MakeRegularDomain( domain, dim, commrank, numDomains, numGlobalZones, zoneSizes );

   Real sourceRad;
   if (dim == 2) {
      sourceRad = 2.0 * std::max(zoneSizes[0], zoneSizes[1]);
   }
   else {
      sourceRad = 2.0 * std::max( std::max(zoneSizes[0], zoneSizes[1]),  zoneSizes[2]);
   }


   ProblemSetup( argv[1], sim, sourceRad );
   sim.run();

#if HAVE_SILO
   OutputSilo silo( &sim, "lassenp" );
   silo.writeSimulation();
#endif
   
   Real maxtime = 0.0;
   for (size_t i = 0; i < sim.getNodeTimeReached().size(); ++i) {
      maxtime = std::max( maxtime, sim.getNodeTimeReached()[i] );
   }
   
   if (argv[1] == "default") {
      computeErrors( &sim );
   }

   MPI_Finalize();
   
}


void computeErrors(const Simulation *sim )
{
   ComputeErrors err;
   err.computeLocalErrors( sim );
   
   Real lerror1 = err.error1;
   Real lerror2 = err.error2;
   Real lerrorMax = err.errorMax;
   GlobalID  lerrorNodeCount = err.errorNodeCount;
   
   MPI_Allreduce( &lerror1, &err.error1, 1, MPI_DOUBLE, MPI_SUM, MPI_COMM_WORLD);
   MPI_Allreduce( &lerror2, &err.error2, 1, MPI_DOUBLE, MPI_SUM, MPI_COMM_WORLD);
   MPI_Allreduce( &lerrorMax, &err.errorMax, 1, MPI_DOUBLE, MPI_MAX, MPI_COMM_WORLD);
   // fixme - use the same LASSEN_MPI_GLOBALID that SimulationMPI uses
   MPI_Allreduce( &lerrorNodeCount, &err.errorNodeCount, 1, MPI_LONG, MPI_SUM, MPI_COMM_WORLD);

   err.completeErrors();
   if (sim->getDomain()->domainID == 0) {
      err.reportErrors( );
   }

}



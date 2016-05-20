#ifndef OUTPUT_H_
#define OUTPUT_H_

#include "Simulation.hh"
#include "Lassen.hh"
#include <string>
class DBfile;

namespace Lassen {
   
class Simulation;
class Domain;

#if HAVE_SILO
class OutputSilo {
public:

   // For now one file per domain.
   OutputSilo(const Simulation *sim, std::string basename);
   void writeDomain( );
   void writeSimulation( const StepState *stepState = NULL );
   
protected:

   // FIXME:  
   void open();
   void close();

   std::string getDomainFile( int domainID ) const;
   void writeMultiBlock( ); // only works on domainID = 0
   void writeSimulationVars( const StepState *stepState  );
   void writeMultiVar( DBfile *multidb, std::string name );
   
protected:
   const Simulation *sim;
   const Domain *domain;
   DBfile *dbfile;
   std::string basename;
   std::string multiname;
   
};
#endif
   

class OutputPlanarFront  {
public:
   
   OutputPlanarFront( std::string filename, int dim, const PlanarFront &planarFront );
   
};

// fixme:  for debugging. helper function to print the contents of the mesh to stdout
void PrintMesh(const Mesh *mesh);
   
};

#endif

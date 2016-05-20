
#include <iostream>
#include <stdio.h>


#include "Output.hh"
#include "Simulation.hh"

#if HAVE_SILO
#include "silo.h"

namespace Lassen {
   
////////////////////////////////////////////////////////////////////////////////
OutputSilo
::OutputSilo( const Simulation *sim, const std::string basename )
   : sim(sim),
     domain(sim->getDomain()),
     dbfile(NULL),
     basename(basename)
{
   int numDomains = domain->numDomains;
   
   // FIXME - bad bad bad
   char buf[128];
   sprintf(buf, "%s_d%05d_c%05d", basename.c_str(), numDomains, sim->getCycle() );
   multiname = std::string(buf);
}

////////////////////////////////////////////////////////////////////////////////
void
OutputSilo::
open( )
{
   int domainID = domain->domainID; 
   std::string filename = getDomainFile( domainID );
   dbfile = DBCreate(filename.c_str(), DB_CLOBBER, DB_LOCAL, NULL, DB_PDB);
}


////////////////////////////////////////////////////////////////////////////////
void
OutputSilo::
writeSimulation( const StepState *stepState  )
{
   open();
   writeDomain();
   writeSimulationVars( stepState );
   close();
   if (domain->domainID == 0) {
      writeMultiBlock();
   }
}

////////////////////////////////////////////////////////////////////////////////
std::string
OutputSilo::
getDomainFile( int domainID ) const
{
   // FIXME - bad bad bad
   char buf[128];
   sprintf(buf, "%s_i%05d.silo", multiname.c_str(), domainID );
   return std::string(buf);
}

////////////////////////////////////////////////////////////////////////////////
void
OutputSilo::
close( )
{
   DBClose(dbfile);
}

////////////////////////////////////////////////////////////////////////////////
void
OutputSilo::
writeDomain( )
{
   const Mesh *mesh = domain->mesh;
   int dim = mesh->dim;
   
   const char *coordnames[3] = {"x", "y", "z"};
   int nnodes = mesh->nLocalNodes;
   int nzones = mesh->nLocalZones;
   int datatype = DB_DOUBLE;

   // zonelist
   static const char *zlname = "zonelist";
   int origin = 0;
   int lo_offset = 0;
   int hi_offset = 0;
   int shapetype[1] = {(dim == 2) ? (DB_ZONETYPE_QUAD) : (DB_ZONETYPE_HEX)};
   int shapesize[1] = {mesh->zoneToNodeCount};
   int shapecnt[1]  = {nzones};
   int nshapes = 1;

   int zoneToNodeSize = mesh->zoneToNodeCount * nzones;
   if ((DBPutZonelist2(dbfile, zlname, nzones, dim,
                       const_cast<int*>(&mesh->zoneToNode[0]),
                       zoneToNodeSize, origin, lo_offset, hi_offset,
                       shapetype, shapesize, shapecnt, nshapes, 
                       NULL)) == -1) {
      std::cerr << "FAIL in DBPutZonelist2 " << DBErrString() << "\n";
   }

   // coords
   std::vector<Real> x(nnodes), y(nnodes), z(nnodes);
   for (int i = 0; i < nnodes; ++i) {
      x[i] = mesh->nodePoint[i].x;
      y[i] = mesh->nodePoint[i].y;
      z[i] = mesh->nodePoint[i].z;
   }
   Real *coords[3];
   coords[0] = const_cast<Real*>(&x[0]);
   coords[1] = const_cast<Real*>(&y[0]);
   coords[2] = (dim == 2) ? (static_cast<Real*>(0)) : (const_cast<Real*>(&z[0]));

   
   DBPutUcdmesh(dbfile, "mesh", dim,
                const_cast<char **>(coordnames),
                reinterpret_cast<float **>(coords),
                nnodes, nzones, zlname, NULL,
                datatype, NULL);

}


////////////////////////////////////////////////////////////////////////////////
 
void
OutputSilo::
writeSimulationVars( const StepState *stepState)
{
   if (sim->getCycle() < 1) {
      return;
   }
   
   int nnodes = domain->mesh->nLocalNodes;
   int nzones = domain->mesh->nLocalZones;
   
   std::vector<int> nodeStateInt(nnodes);
   std::vector<int> nodeGlobalIDInt(nnodes);
   const std::vector<NodeState::Enum> & nodeState = sim->getNodeState();
   for (int i = 0; i < nnodes; ++i) {
      nodeStateInt[i] = static_cast<int>( nodeState[i] );
      nodeGlobalIDInt[i] = static_cast<int>( domain->mesh->nodeGlobalID[i] );
   }
   int ret;

   DBoptlist *optlist = DBMakeOptlist(1) ;
   int blockOrigin = 0;
   DBAddOption(optlist, DBOPT_BLOCKORIGIN, &blockOrigin) ;
   
   ret = DBPutUcdvar1(dbfile, "nodeState", "mesh", reinterpret_cast<float*>(&nodeStateInt[0]),
                          nnodes, 0, 0, DB_INT, DB_NODECENT, NULL);
   assert(ret == 0);

   // FIXME = node global id: could be > int
   ret = DBPutUcdvar1(dbfile, "nodeGlobalID", "mesh",
                      reinterpret_cast<float*>(&nodeGlobalIDInt[0]),
                      nnodes, 0, 0, DB_INT, DB_NODECENT, NULL);
   assert(ret == 0);
   
   ret = DBPutUcdvar1(dbfile, "nodeTimeReached", "mesh",
                reinterpret_cast<float*>(const_cast<Real*>(&sim->getNodeTimeReached()[0])),
                nnodes, 0, 0, DB_DOUBLE, DB_NODECENT, optlist);
   assert(ret == 0);

   ret = DBPutUcdvar1(dbfile, "nodeLevel", "mesh",
                      reinterpret_cast<float*>(const_cast<Real*>(&sim->getNodeLevel()[0])),
                      nnodes, 0, 0, DB_DOUBLE, DB_NODECENT, NULL);
   assert(ret == 0);
   
   ret = DBPutUcdvar1(dbfile, "zoneVelocity", "mesh",
                      reinterpret_cast<float*>(const_cast<Real*>(&sim->getZoneVelocity()[0])),
                      nzones, 0, 0, DB_DOUBLE, DB_ZONECENT, NULL);
   assert(ret == 0);


   if (stepState) {
      std::vector<Real> surfaceFront(nzones, 0);
      const std::vector<int> &surfaceFrontZone = stepState->surfaceFrontZone;
      for (size_t i = 0 ; i < surfaceFrontZone.size(); ++i ){
         int zoneIndex = surfaceFrontZone[i];
         if (zoneIndex < nzones) {
            surfaceFront[ zoneIndex ] = 1;
         }
      }
   
      DBPutUcdvar1(dbfile, "surfaceZone", "mesh",
                   reinterpret_cast<float*>(const_cast<Real*>(&surfaceFront[0])),
                   nzones, 0, 0, DB_DOUBLE, DB_ZONECENT, NULL);
   }

   // FIXME
   ret = ret;
   
}

////////////////////////////////////////////////////////////////////////////////
void
OutputSilo::
writeMultiBlock( )
{
   int numDomains = domain->numDomains;
   
   DBoptlist *optlist = DBMakeOptlist(1) ;
   int blockOrigin = 0;
   DBAddOption(optlist, DBOPT_BLOCKORIGIN, &blockOrigin) ;

   // FIXME:  bad string handling
   // FIXME - there is a common way to express the meshnames and meshtypes w/o the repetition
   
   char **meshNames = new char * [numDomains];
   int   *meshTypes = new int [ numDomains ];
   for (int i = 0 ; i < numDomains; ++i) {
      std::string domainFile = getDomainFile( i );
      meshNames[i] = new char [256];
      sprintf( meshNames[i], "%s:/%s", domainFile.c_str(), "mesh" );
      meshTypes[i] = DB_UCDMESH;
   }

   std::string x = multiname + ".silo";
   int silodriver = DB_PDB;
   //int silodriver = DB_HDF5;
   
   DBfile *multidb = DBCreate(x.c_str(), DB_CLOBBER, DB_LOCAL, NULL, silodriver);
   DBPutMultimesh( multidb, "mesh", numDomains,
                   meshNames, meshTypes, optlist );

   for (int i = 0; i < numDomains; ++i) {
      delete [] meshNames[i];
   }
   delete [] meshNames;
   delete [] meshTypes;


   writeMultiVar( multidb, "nodeState");
   writeMultiVar( multidb, "nodeTimeReached");
   writeMultiVar( multidb, "nodeLevel");
   writeMultiVar( multidb, "nodeGlobalID");
   writeMultiVar( multidb, "zoneGlobalID");
   writeMultiVar( multidb, "surfaceZone");
   writeMultiVar( multidb, "zoneVelocity");
   
   DBClose(multidb);

}

////////////////////////////////////////////////////////////////////////////////
void
OutputSilo::
writeMultiVar( DBfile *multidb, std::string var)
{
   int numDomains = domain->numDomains;
   DBoptlist *optlist = DBMakeOptlist(1) ;
   int blockOrigin = 0;
   DBAddOption(optlist, DBOPT_BLOCKORIGIN, &blockOrigin) ;

   // FIXME - there is a common way to express the varnames and vartypes w/o the repetition
   // vars
   char **varNames = new char * [numDomains];
   int   *varTypes = new int [ numDomains ];
   for (int i = 0 ; i < numDomains; ++i) {
      std::string domainFile = getDomainFile( i );
      varNames[i] = new char [256];
      sprintf( varNames[i], "%s:/%s", domainFile.c_str(), var.c_str() );
      varTypes[i] = DB_UCDVAR;
   }

   DBPutMultivar( multidb, var.c_str(), numDomains, varNames, varTypes, optlist );

   for (int i = 0; i < numDomains; ++i) {
      delete [] varNames[i];
   }
   delete [] varNames;
   delete [] varTypes;

}
}
#endif // HAVE_SILO

namespace Lassen {

OutputPlanarFront::
OutputPlanarFront( std::string basefilename, int dim,  const PlanarFront &planarFront )
{
   if (dim == 2) {
      std::string filename = basefilename + ".vtk";
      FILE* fp = fopen(filename.c_str(),"w");
      
      fprintf(fp,"# vtk DataFile Version 3.0\n");
      fprintf(fp,"Facet %s\n", filename.c_str());
      fprintf(fp,"ASCII\n");
      fprintf(fp,"DATASET POLYDATA\n");

      int totalNumPoints = planarFront.facetToVertexOffset[ planarFront.nfacets ];
      
      fprintf(fp,"POINTS %d float\n", totalNumPoints);
      for(int i = 0; i < planarFront.nfacets; ++i) {
         int startPoint = planarFront.facetToVertexOffset[i];
         int endPoint = planarFront.facetToVertexOffset[i+1];
         for(int j = startPoint; j < endPoint; ++j) {
            const Point& point = planarFront.vertices[j];
            fprintf(fp, " %f %f 0  ", point.x, point.y);
         }
         fprintf(fp, "\n");
      }
      // Each facet is on a single file line.
      fprintf(fp,"LINES %d %d\n", planarFront.nfacets,
              planarFront.nfacets + totalNumPoints );
      
      int ithpt = 0;
      for(int i = 0; i < planarFront.nfacets; ++i) {
         int startPoint = planarFront.facetToVertexOffset[i];
         int endPoint = planarFront.facetToVertexOffset[i+1];
         int numPoints = endPoint - startPoint;
         fprintf(fp, "%d ", numPoints);
         for(int j = 0; j < numPoints; ++j) {
            fprintf(fp, "%d ", ithpt++);
         }
         fprintf(fp, "\n ");
      }
      assert(ithpt == totalNumPoints);
      fclose(fp);
   }
   else {
      std::string filename = basefilename + ".stl";
      FILE* fp = fopen(filename.c_str(),"w");
      fprintf(fp,"solid %s\n", filename.c_str());
      
      for(int i = 0; i < planarFront.nfacets; ++i) {
         int startPoint = planarFront.facetToVertexOffset[i];
         int endPoint = planarFront.facetToVertexOffset[i+1];
         int npoints = endPoint - startPoint;
         const Point &center = planarFront.facetCenter[i];
         for(int j = 0; j < npoints; ++j) {
            const Point& point = planarFront.vertices[startPoint + j];
            const Point& nextPoint = planarFront.vertices[startPoint + (j + 1) % (npoints)];
            fprintf(fp,"facet normal 0 0 0\n");				
            fprintf(fp,"outer loop\n");
            fprintf(fp,"vertex %f %f %f\n", center.x, center.y, center.z);
            fprintf(fp,"vertex %f %f %f\n", point.x, point.y, point.z);
            fprintf(fp,"vertex %f %f %f\n", nextPoint.x, nextPoint.y, nextPoint.z);
            fprintf(fp,"endloop\n");				
            fprintf(fp,"endfacet\n");				
         }
      }
      
      fprintf(fp,"endsolid\n");				
      fclose(fp);
   }
}

   void PrintMesh(const Mesh *mesh)
{
   for (int i =0; i <  mesh->nzones; ++i) {
      int startNode = i * mesh->zoneToNodeCount;
      std::cout << " ZONE " << i << " :\n";
      for (int j = startNode; j < startNode + mesh->zoneToNodeCount; ++j ) {
         int nodeIndex = mesh->zoneToNode[j];
         GlobalID nodeGlobalID = mesh->nodeGlobalID[ nodeIndex ];
         std::cout << " " << nodeIndex
                   << "(gid=" << nodeGlobalID << ") " 
                   << " - " << mesh->nodePoint[nodeIndex] << " Zones = ";
         for (int k = mesh->nodeToZoneOffset[nodeIndex]; k < mesh->nodeToZoneOffset[nodeIndex+1]; ++k) {
            std::cout << " " << mesh->nodeToZone[k];
         }
         std::cout << "\n";
      }
   }

}

}

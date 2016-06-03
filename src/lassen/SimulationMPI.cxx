
#include "SimulationMPI.hh"
#include "Source.hh"
#include "Output.hh"

namespace Lassen {

void gatherBoundingBox( MPI_Comm comm, const BoundingBox &bb,
                        std::vector<BoundingBox> &allBoundingBox ) ;

MPI_Datatype LASSEN_MPI_REAL;
MPI_Datatype LASSEN_MPI_GLOBALID;
MPI_Datatype LASSEN_MPI_POINT;  
MPI_Datatype LASSEN_MPI_BOUNDINGBOX; 
MPI_Datatype LASSEN_MPI_NODEDATA; 


///
/// Exchange the size of a send buffer with the neighbor ranks.  This
/// function is used as a building block for other exchange functions.
/// 
void exchangeSizes( MPI_Comm comm,
                    const std::vector<int> &neighbors,
                    const std::vector<int> &sendBufferSize,
                    std::vector<int> &recvBufferSize);

///
/// Exchange the number of entries in a send buffer vector with the
/// neighbor rank.  (Similar to the preceding function).
///
template<class T>
void exchangeSizes( MPI_Comm comm,
                    const std::vector<int> &neighbors,
                    const std::vector< std::vector<T> > &sendBuffer,
                    std::vector<int> &recvBufferSize);


///
/// Multicast the data in the send buffer to all the neighbors.
/// The results are placed in the recvBuffer.
///
template<class T>
void multiCast( MPI_Comm comm,
                const std::vector<int> &neighbors,
                const std::vector< T > &sendBuffer,
                std::vector< std::vector<T> > &recvBuffer);


///
/// Exchange the data in the send buffers to all the neighbors.  Each
/// process sends potentially different data to each of it's
/// neighbors.  The results are placed in the recvBuffer.
///
template<class T>
void exchangeBuffers( MPI_Comm comm,
                      const std::vector<int> &neighbors,
                      const std::vector< std::vector<T> > &sendBuffer,
                      std::vector< std::vector<T> > &recvBuffer);

///
/// A simple templated function that returns the correct MPI_Datatype
/// for the given type T.
///
template<typename T>
MPI_Datatype getType();


SimulationMPI::SimulationMPI()
   : SimulationParallel()
{
   // Initialize MPI data type
   LASSEN_MPI_REAL = MPI_DOUBLE;
   LASSEN_MPI_GLOBALID = MPI_LONG;
   
   MPI_Type_contiguous( 3, LASSEN_MPI_REAL, &LASSEN_MPI_POINT );
   MPI_Type_commit( &LASSEN_MPI_POINT );
   
   MPI_Type_contiguous( 2, LASSEN_MPI_POINT, &LASSEN_MPI_BOUNDINGBOX );
   MPI_Type_commit( &LASSEN_MPI_BOUNDINGBOX );

   // ensure the nodeData struct does not change out from under us.
   // 1 global ID, 3 Reals, 1 Point
   NodeData data[2];
   int blen[4] = {1,3,1,1};
   char *start = reinterpret_cast<char*>(data);
   MPI_Aint indicies[4] = {0,
                           reinterpret_cast<char*>(&data[0].level) - start,
                           reinterpret_cast<char*>(&data[0].imagePoint) - start,
                           reinterpret_cast<char*>(&data[1]) - start };
   MPI_Datatype oldtypes[4] = { LASSEN_MPI_GLOBALID,
                                LASSEN_MPI_REAL,
                                LASSEN_MPI_POINT,
                                MPI_UB };
   MPI_Type_struct( 4, blen, indicies, oldtypes, &LASSEN_MPI_NODEDATA );
   MPI_Type_commit( &LASSEN_MPI_NODEDATA );
}

////////////////////////////////////////////////////////////////////////////////
/**
   
 */

void
SimulationMPI::run()
{
   parallelInitialize();
   
   StepState stepState;
   printBanner();
   
   timers.run[0] = LassenUtil::timer();
   int isDone = 0;
   while ( !isDone ) {
      step( stepState );
      printProgress();
      int localDone = checkEndingCriteria();
      int busy = narrowBandNodes.size() > 1;
      int ldata[3] = {localDone, busy, narrowBandNodes.size() };
      int gdata[3];
      MPI_Allreduce( ldata, gdata, 3, MPI_INT, MPI_SUM, comm );
      isDone = (gdata[0] == domain->numDomains);
      numBusyDomains = gdata[1];
      numGlobalNarrowBandNodes = gdata[2];
   }
   timers.run[1] = LassenUtil::timer();

   printFinal();
}


////////////////////////////////////////////////////////////////////////////////
/**
   
 */


void
SimulationMPI::
step( StepState &stepState )
{
   // These should be constant time operations.
   stepState.nodeTempArray.resize( mesh->nnodes );
   stepState.planarFront = PlanarFront();
   
   updateSources( stepState );

   updateNarrowband( stepState );
   
   updateNearbyNodes( stepState );
   
   constructFront( stepState );

   //char name[100];
   //sprintf(name, "front%03d", cycle );
   //OutputPlanarFront(name, 3, stepState.planarFront );
   
   // PARALLEL - need to communicate the facets on the surface zones
   communicateFront( stepState );

   constructDistancesToFront( stepState );

   convertNearbyNodesToNarrowBandNodes( stepState );

   computeNextTimeStep( stepState );

   // PARALLEL - need global min
   Real localdt = dt;
   MPI_Allreduce( &localdt, &dt, 1, LASSEN_MPI_REAL, MPI_MIN, comm );
   
   computeTimeIntegral( stepState );

   // PARALLEL - need to synchronize the narrowband node data
   synchronizeNodeData();
   
   // Update timer/cycle
   this->cycle ++;
   this->time += this->dt;
}


////////////////////////////////////////////////////////////////////////////////
/**
   
 */

void 
SimulationMPI::parallelInitialize( )
{
   // FIXME:  timer's won't be right.
   Simulation::initialize();

   // all-reduce several quantities:
   // FIXME - this needs to be documented in some way,
   Real localData[4] = { minVelocity, -maxVelocity, minEdgeSize, -narrowBandWidth };
   Real globalData[4];
   MPI_Allreduce( localData, globalData, 4, LASSEN_MPI_REAL, MPI_MIN, comm );
   minVelocity =      globalData[0];
   maxVelocity =     -globalData[1];
   minEdgeSize =      globalData[2];
   narrowBandWidth = -globalData[3];
                         
   std::vector<int> boundaryNodes;
   std::vector<int> communicatingNodes;

   // Determine the set of boundary nodes, based on the connectivity of the mesh
   // no communicaiton needed.
   initializeBoundaryNodes(boundaryNodes);

   // Determine the set of communicating nodes, based on the connectivity of the mesh
   // no communicaiton needed.
   initializeCommunicatingNodes(boundaryNodes, communicatingNodes);

   // Initialize the node communicaiton
   // required communication
   initializeNodeCommunication( communicatingNodes );

   // Initialize the facet communicaiton
   // required communication
   initializeFacetCommunication( boundaryNodes );

}


////////////////////////////////////////////////////////////////////////////////
/**
   
 */

void
SimulationMPI::initializeNodeCommunication(const std::vector<int> &communicatingNodes)
{
   // Send receive the list of communicating nodes to all the neighbors
   // The nodes are communicated as a globalID
   std::vector< GlobalID >  sendBuffer(communicatingNodes.size());
   nodeCommunicationCreateMsg( communicatingNodes, sendBuffer );

   std::vector< std::vector< GlobalID > > recvBuffer(communicatingNodes.size());
   multiCast( comm, domain->neighborDomains, sendBuffer, recvBuffer );

   // maps from local node index to neighbor domain.  This will be used to setup the node communication
   std::vector< std::pair< int, int > > nodeToDomainPair;
   
   for(size_t i = 0; i < recvBuffer.size(); ++i) {
      int neighborIndex = static_cast<int>(i);
      nodeCommunicationProcessMsg( neighborIndex, recvBuffer[i], nodeToDomainPair );
   }

   nodeCommunicationComplete( nodeToDomainPair );

}

////////////////////////////////////////////////////////////////////////////////
/**
   
 */

void
SimulationMPI::initializeFacetCommunication(const std::vector<int> &boundaryNodes)
{
   Real narrowBandWidth = this->getNarrowBandWidth();
   
   // 1. determine set of potential neighbors.  This is done by comparing bounding boxes 
   // overlapDomains will contain the collection of possible overlapping domains.
   BoundingBox boundingBox;
   std::vector< BoundingBox> allBoundingBox( domain->numDomains );
   std::vector< int > overlapDomains;
   computeBoundingBox( mesh->nodePoint, mesh->nLocalNodes, boundingBox ) ;
   gatherBoundingBox( comm, boundingBox, allBoundingBox );
   findBoundingBoxIntersections( domain->domainID, allBoundingBox, narrowBandWidth, overlapDomains);

   // 2. multicast the set of positions of the boundary nodes to each
   // of the potentially overlapping domains.
   
   // FIXME: there are potential optimizations here, but this only
   // happens once, such as selectively sending points based on the
   // overlap
   std::vector< Point > sendBuffer( boundaryNodes.size() );
   std::vector< std::vector< Point > > recvBuffer( overlapDomains.size() );
   for (size_t i = 0; i < boundaryNodes.size(); ++i ) {
      int nodeIndex = boundaryNodes[i];
      sendBuffer[i] = mesh->nodePoint[nodeIndex];
   }
   multiCast( comm, overlapDomains, sendBuffer, recvBuffer );


   // 3. for each zone in the mesh, determine if is close to the boundary node of a neighbor..
   // The facetToDomainPair maps from zoneIndex to 
   std::vector< std::pair< int, int > > facetToDomainPair;

   // Quick and dirty spatial algorithm
   std::vector<Point> zoneCenter(mesh->nzones);
   computeZoneCenters( mesh, zoneCenter );
   Real boxsize = narrowBandWidth;
   SpatialGrid grid(zoneCenter, boxsize);
   Real cutoffDistance = 2 * narrowBandWidth;
   facetNeighbors.clear();
   
   std::vector< int > isFacetNeighbor(  overlapDomains.size(), 0);
   
   for (size_t i = 0; i < overlapDomains.size(); ++i) {
      const std::vector< Point > &boundaryPoint = recvBuffer[i];
      int neighborDomainID = overlapDomains[i];
      bool overlap = findFacetDomainOverlap( grid, zoneCenter, neighborDomainID,
                                             boundaryPoint,
                                             allBoundingBox[neighborDomainID],
                                             cutoffDistance,
                                             facetToDomainPair );
      if (overlap) {
         facetNeighbors.push_back( neighborDomainID );
         isFacetNeighbor[ i ] = 1;
      }
   }

   // It is possible that the facet Neighbors are not reciprical.
   // E.g. domain A might need to send facet data to domain B, but not
   // send any.  Therefore the facetNeighbors need to be updated.
   // FIXME == need to also do this in Charm
   std::vector<int> recvIsFacetNeighbor;
   exchangeSizes( comm, overlapDomains, isFacetNeighbor, recvIsFacetNeighbor);
   for (int i = 0; i < overlapDomains.size(); ++i) {
      if (recvIsFacetNeighbor[i] == 1) {
         int neighborDomainID = overlapDomains[i];
         if (std::find(facetNeighbors.begin(), facetNeighbors.end(), neighborDomainID) ==
             facetNeighbors.end()) {
            facetNeighbors.push_back(neighborDomainID);
         }
      }
   }
   
   constructFacetToDomain( facetToDomainPair );
}

////////////////////////////////////////////////////////////////////////////////

void
SimulationMPI::
communicateFront( StepState &stepState)
{
   // FIXME -- a lot of this can move to SimulationParallel
   
   // send/recv the facets of the front.
   std::vector< std::vector< Real > > sendBuffer(facetNeighbors.size());
   PlanarFront &planarFront = stepState.planarFront;
   
   // determine which facets need to be sent, and to which domain.
   // FIXME: is it better to communicate the facet center, or to recompute from the vertices?
   for( int i = 0; i < planarFront.nfacets; ++i) {
      int zoneIndex = planarFront.facetZone[i];
      int startDomain = facetToDomainOffset[ zoneIndex ];
      int endDomain = facetToDomainOffset[ zoneIndex + 1 ];
      if (startDomain == endDomain) {
         continue; // this is an entirely local facet
      }
      // Pack up the facet for communication:
      std::vector<Real> buffer;
      packFacet( planarFront, i, buffer );
      for (int j = startDomain; j < endDomain; ++j) {
         int receiverID = facetToDomain[j];
         sendBuffer[ receiverID].insert( sendBuffer[receiverID].end(), buffer.begin(), buffer.end() );
      }
   }

   std::vector< std::vector< Real > > recvBuffer;
   exchangeBuffers( comm, facetNeighbors, sendBuffer, recvBuffer );

   for (size_t i = 0; i < recvBuffer.size(); ++i) {
      if (recvBuffer[i].size() == 0 ) {
         continue;
      }
      unpackFacet( recvBuffer[i], planarFront );
   }
}

////////////////////////////////////////////////////////////////////////////////
void
SimulationMPI::
synchronizeNodeData( )
{
   // for each narrowband node
   // only narrowband nodes matter here.  level and image point and (time reached?)
   // time reached might not be necessary.

   // send/recv the node data
   std::vector< std::vector< NodeData > > sendBuffer( domain->neighborDomains.size() );
   gatherNodeData( sendBuffer );

   std::vector< std::vector< NodeData > > recvBuffer;
   exchangeBuffers( comm, domain->neighborDomains, sendBuffer, recvBuffer );

   for (size_t i = 0; i < recvBuffer.size(); ++i) {
      accumulateNodeData( recvBuffer[i] );
   }

   // sort the narroband nodes
   std::sort( narrowBandNodes.begin(), narrowBandNodes.end());
}


////////////////////////////////////////////////////////////////////////////////
// pack facet
void
SimulationMPI::
packFacet( const PlanarFront &planarFront, int facetIndex, std::vector<Real> &buffer) const
{
   int vertexBegin = planarFront.facetToVertexOffset[facetIndex];
   int vertexEnd   = planarFront.facetToVertexOffset[facetIndex + 1];
   int nVertex = vertexEnd - vertexBegin;
   buffer.push_back( static_cast<Real>(nVertex) );
   for (int i = vertexBegin; i < vertexEnd; ++i) {
      const Point &point = planarFront.vertices[i];
      buffer.push_back( point.x );
      buffer.push_back( point.y );
      buffer.push_back( point.z );
   }
   const Point &center = planarFront.facetCenter[facetIndex];
   buffer.push_back( center.x );
   buffer.push_back( center.y );
   buffer.push_back( center.z );
   buffer.push_back( planarFront.facetVelocity[facetIndex] );
}

// unpack facet
void
SimulationMPI::
unpackFacet( const std::vector<Real> &buffer, PlanarFront &planarFront)
{
   size_t index = 0;
   
   while (index < buffer.size() ) {
      int nVertex = buffer[index++];
      std::vector<Point> p(nVertex);
      for (int j = 0; j < nVertex; ++j) {
         Real x = buffer[index++];
         Real y = buffer[index++];
         Real z = buffer[index++];
         p[j] = Point( x, y, z );
      }
      Real x = buffer[index++];
      Real y = buffer[index++];
      Real z = buffer[index++];
      Point center(x,y,z);
      Real vel = buffer[index++];
      planarFront.addFacet( -1, nVertex, &p[0], center, vel );
   }
   assert(index == buffer.size());
}

   

////////////////////////////////////////////////////////////////////////////////


Real
SimulationMPI::
globalMin( Real local ) const
{
   Real global;
   MPI_Allreduce( &local, &global, 1, LASSEN_MPI_REAL, MPI_MIN, comm);
   return global;
}

Real
SimulationMPI::
globalMax( Real local ) const
{
   Real global;
   MPI_Allreduce( &local, &global, 1, LASSEN_MPI_REAL, MPI_MAX, comm);
   return global;
}


template<> MPI_Datatype getType<int>()      { return MPI_INT; }
template<> MPI_Datatype getType<double>()   { return MPI_DOUBLE; }
template<> MPI_Datatype getType<GlobalID>() { return LASSEN_MPI_GLOBALID; }
template<> MPI_Datatype getType<Point>()    { return LASSEN_MPI_POINT;  }
template<> MPI_Datatype getType<NodeData>() { return LASSEN_MPI_NODEDATA; }


// fixme -- change the name of this to exchangeInt.  so that it can be used more generally.
   
void exchangeSizes( MPI_Comm comm, const std::vector<int> &neighbors,
                    const std::vector<int> &sendBufferSize,
                    std::vector<int> &recvBufferSize)
{
   int numNeighbors = neighbors.size();
   
   std::vector< MPI_Request > request( numNeighbors * 2);
   std::vector< MPI_Status > status( numNeighbors * 2);
   
   recvBufferSize.resize( numNeighbors );
   
   for(int i = 0; i < numNeighbors; ++i) {
      MPI_Irecv( &recvBufferSize[i], 1, MPI_INT, neighbors[i], 101,
                 comm, &request[i] );
   }
   for(int i = 0; i < numNeighbors; ++i) {
      int sendsize = sendBufferSize[i];
      MPI_Isend( &sendsize, 1, MPI_INT, neighbors[i], 101,
                 comm, &request[i + numNeighbors] );
   }
   
   MPI_Waitall( numNeighbors * 2, &request[0], &status[0] );
   
}


//***********************************************************
template<class T>
void exchangeSizes( MPI_Comm comm, const std::vector<int> &neighbors,
                    const std::vector< std::vector<T> > &sendBuffer,
                    std::vector<int> &recvBufferSize)
{
   assert(neighbors.size() == sendBuffer.size());
   int numNeighbors = neighbors.size();
   std::vector<int> sendSize(  numNeighbors );
   for (int i = 0; i < numNeighbors; ++i) {
      sendSize[i]  = sendBuffer[i].size();
   }
   exchangeSizes( comm, neighbors, sendSize, recvBufferSize );
}


//***********************************************************
template<class T>
void exchangeBuffers( MPI_Comm comm, const std::vector<int> &neighbors,
                      const std::vector< std::vector<T> > &sendBuffer,
                      std::vector< std::vector<T> > &recvBuffer)
{
   size_t numNeighbors = neighbors.size();
   
   // exchange sizes
   std::vector<int> recvSizes;
   exchangeSizes( comm, neighbors, sendBuffer, recvSizes );
   assert(recvSizes.size() == numNeighbors);
   
   // allocate recv buffers
   recvBuffer.resize( numNeighbors );
   for(size_t i = 0; i < numNeighbors; ++i) {
      if (recvSizes[i] > 0) {
         recvBuffer[i].resize( recvSizes[i] );
      }
   }
   
   // exchange the data
   std::vector< MPI_Request > request( numNeighbors * 2);
   std::vector< MPI_Status > status( numNeighbors * 2);
   MPI_Datatype dtype = getType<T>();
   
   int requestIndex = 0;
   for(size_t i = 0; i < numNeighbors; ++i) {
      int recvSize = recvBuffer[i].size();
      if( recvSize > 0 ) {
         T* recvData = &(recvBuffer[i][0]);
         MPI_Irecv( recvData, recvSize, dtype, neighbors[i], 101,
                    comm, &request[requestIndex] );
         requestIndex++;
      }
   }
   for(size_t i = 0; i < numNeighbors; ++i) {
      int sendSize = sendBuffer[i].size();
      if( sendSize > 0 ) {
         T* sendData = const_cast<T*>(&(sendBuffer[i][0]));
         MPI_Isend( sendData, sendSize, dtype, neighbors[i], 101,
                    comm, &request[requestIndex] );
         requestIndex++;
      }
   }
   if(requestIndex > 0) {
      MPI_Waitall( requestIndex, &request[0], &status[0] );
   }
}

//***********************************************************
template<class T>
void multiCast( MPI_Comm comm, const std::vector<int> &neighbors,
                const std::vector< T > &sendBuffer,
                std::vector< std::vector<T> > &recvBuffer)
{
   int numNeighbors = neighbors.size();
   std::vector< std::vector< T > > tempSendBuffers( numNeighbors, sendBuffer );
   exchangeBuffers( comm, neighbors, tempSendBuffers, recvBuffer );
}

void gatherBoundingBox( MPI_Comm comm, const BoundingBox &bb,
                        std::vector<BoundingBox> &allBoundingBox )
{
   MPI_Allgather( const_cast<BoundingBox*>(&bb), 1, LASSEN_MPI_BOUNDINGBOX,
                  (&allBoundingBox[0]), 1, LASSEN_MPI_BOUNDINGBOX, comm ); 
}

} ; // namespace

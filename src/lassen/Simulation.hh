#ifndef SIMULATION_H
#define SIMULATION_H

#include <vector>
#include "LassenUtil.hh"
#include "Lassen.hh"
#include "Source.hh"

namespace Lassen {
   
/* Forward declaration */
class StepState;
class Source;

/**
 *  A class to control the level set simulation.
 *
 *  Each simulation object works over a single domain.  For
 *  multi-domain problems, the simulation object must coordinate the
 *  computation at specific times in the algorithm.

 
 */

class Simulation {
public:

   Simulation();
   virtual ~Simulation();
   
   void setZoneVelocity( const std::vector<Real> &_zoneVelocity) { zoneVelocity = _zoneVelocity; }
   void addSource( const PointSource &source ) { sources.push_back(source); }
             

   /**
    * Run the simulation from beginning to end
    */
   virtual void run();
   
   const Mesh *getMesh() const { return mesh; }
   const Domain *getDomain() const { return domain; }
   Domain *getDomain() { return domain; }
   const std::vector<PointSource>& getSources() const { return sources; }
   const std::vector<NodeState::Enum>& getNodeState() const { return nodeState; }
   const std::vector<Real>& getNodeTimeReached() const { return nodeTimeReached; }
   const std::vector<Real>& getNodeLevel() const { return nodeLevel; }
   const std::vector<Real>& getZoneVelocity() const { return zoneVelocity; }
   Real getMinVelocity() const { return minVelocity; }
   Real getMaxVelocity() const { return maxVelocity; }
   
   int getCycle() const { return cycle; }
   Real getTime() const { return time; }
   Real getTimeStep() const { return dt; }
   Real getNarrowBandWidth() const { return narrowBandWidth; }
   
protected:

   void initialize();
   void initializeVelocity();
   void initializeMeshBasedQuantities();

public:
   
   virtual bool isNodeOwner( int nodeIndex) const { return true; }
   
   
protected:
   
   /** @name Step functions.
    * Functions called during a time step 
    */
   ///@{
   
   /**
    * Take one step
    */
   
   void step( StepState &stepState);

   void updateSources( StepState &stepState );
   void updateNarrowband( StepState &stepState );
   void updateNearbyNodes( StepState &stepState );
   void constructFront( StepState &stepState );
   void constructDistancesToFront( StepState &stepState );
   void calculateLocalNormalFront2D( const Point &imagePoint,
                                     const std::vector< std::pair<Real, Point> > &field,
                                     LocalNormalFront &front);
   void convertNearbyNodesToNarrowBandNodes( StepState &stepState );
   void computeNextTimeStep( StepState &stepState );
   void computeTimeIntegral( StepState &stepState );

   virtual void printBanner() const;
   virtual void printProgress() const;
   virtual void printFinal() const;
   
   bool checkEndingCriteria() const; 

   ///@}
   
protected:

   /** @name Static step functions.
    *
    * Each static function here is called from a corresponding
    * non-static function of the same name.  The purpose for this
    * style is to make very clear which data is being accessed and
    * modified during each algorithm.
    *
    */
   ///@{
   
   
   static void
   updateSources(const Mesh *mesh,
                 Real time,
                 Real narrowBandWidth,
                 std::vector<PointSource> &sources,
                 std::vector<int> &narrowBandNodes,
                 std::vector<NodeState::Enum> &nodeState,
                 std::vector<Real> &nodeLevel,
                 std::vector<Real> &nodeTimeReached,
                 std::vector<Point> &nodeImagePoint,
                 std::vector<Real> &nodeImageVelocity,
                 bool &sourcesComplete,
                 int &numNodesUnreached);
                 

   static void
   updateNarrowband( const Mesh *mesh,
                     const Real minEdgeSize,
                     const std::vector<Real> &nodeLevel,
                     const std::vector<Real> &zoneVelocity,
                     std::vector<int> &narrowBandNodes,
                     std::vector<NodeState::Enum> &nodeState,
                     std::vector<int> &surfaceFrontZone,
                     std::vector<int> &surfaceFrontNode,
                     std::vector<int> &nodeTempArray,
                     int &numNodesUnreached);
                     
                                      
   static void
   updateNearbyNodes( const Mesh *mesh,
                      const std::vector<int> &surfaceFrontZone,
                      const std::vector<Real> &nodeLevel,
                      const std::vector<Real> &zoneVelocity,
                      std::vector<int> &nearbyNodes,
                      std::vector<NodeState::Enum> &nodeState);
      
   static void
   constructFront(const Mesh *mesh,
                  const std::vector<Real> &nodeLevel,
                  const std::vector<Real> &zoneVelocity,
                  const Real minEdgeSize,
                  const std::vector<int> &surfaceFrontZone,
                  PlanarFront &facets);


   static void
   constructFacet2D( int zoneIndex,
                     Real zoneVelocity,
                     const Point positions[4],
                     const Real levels[4],
                     PlanarFront &facets );

   static void
   constructFacet3D( int zoneIndex,
                     Real zoneVelocity,
                     const Point positions[8],
                     const Real levels[8],
                     PlanarFront &facets );

   static void
   constructDistancesToFront( const Mesh *mesh,
                              const PlanarFront &planarFront,
                              const std::vector<int> &nearbyNodes,
                              Real narrowBandWidth,
                              std::vector<NodeState::Enum> &nodeState,
                              std::vector<Real> &nodeLevel,
                              std::vector<int> &narrowBandNodes,
                              std::vector<Point> &nodeImagePoint,
                              std::vector<Real> &nodeImageVelocity);


   static void 
   computeNextTimeStep(const Mesh *mesh,
                       const std::vector<int> &narrowBandNodes,
                       const std::vector<Real> &nodeLevel,
                       const std::vector<Real> &nodeImageVelocity,
                       const std::vector<int> &surfaceFrontZone,
                       const Real minEdgeSize,
                       const Real maxVelocity,
                       const std::vector<NodeState::Enum> &nodeState,
                       Real &dt,
                       std::vector<int> &nodeTempArray);
                    

   static void
   computeTimeIntegral(const Mesh *mesh,
                       Real time,
                       Real dt,
                       const std::vector<int> &narrowBandNodes,
                       std::vector<Real> &nodeLevel,
                       std::vector<Real> &nodeTimeReached,
                       std::vector<Real> &nodeImageVelocity,
                       std::vector<Point> &nodeImagePoint);

   ///@}
   
protected:

   /**
    * The domain.
    *
    * In parallel, the domain is part of a larger whole.  The domain
    * contains about communication patterns and nearby domains.
    */
   Domain *domain;

   /**
    * The mesh, a reference to domain.mesh
    *
    */
   Mesh *mesh;

   /**
    * The source objects that initiate the algorith.
    */
   std::vector<PointSource> sources;

   /**
    * A collection of node indices that indiciate which nodes the narrow band nodes.
    *
    * The indicies are sorted, and may include local and ghost node indicies.
    */

   std::vector<int>             narrowBandNodes;

   /**
    * A vector of size mesh.nnodes, which constains the state of each node.
    */
   
   std::vector<NodeState::Enum> nodeState;

   /**
    * A vector of size mesh.nnodes, which stores the simulation time when the node was reached.
    *
    * This value is only relevant if the corresponding nodeState is
    * REACHED.
    */
   
   std::vector<Real>            nodeTimeReached;

   /**
    * A vector of size mesh.nnodes, which stores the signed distance from a node to the surface.
    
    * A negative value indicates the node is behind the surface, and
    * a positive value means the node is in front.
    */
   
   std::vector<Real>            nodeLevel;

   /**
    * The vector of size mesh.nnodes, which stores that closest point of each node to the surface.
    *
    * This value is only relevant for narrowband nodes.
    */
   
   std::vector<Point>           nodeImagePoint;

   /**
    * A vector of size mesh.nnodes, which stores the speed at which the front is traveling towards each node.
    *
    * This value is only relevant for the narrowband nodes.
    */
   
   std::vector<Real>            nodeImageVelocity;

   /**
    * A vector of size mesh.nzones, which indicates the speed at which the surface can move through that zone.
    *
    * These values are set at the beginning of the simulation and cannot change
    */
   
   std::vector<Real> zoneVelocity; 
   
   /**
    * The minimum zone velocity across all domains.
    *
    * This value is precomputed and does not change.
    */
   
   Real minVelocity;
   
   /**
    * The maximum zone velocity across all domains
    *
    * This value is precomputed and does not change.
    */
   
   Real maxVelocity;

   /**
    * The length of the smallest mesh edge in the simulation
    *
    * This value is precomputed and does not change.
    */

   Real minEdgeSize;

   /**
    * The width of the narrowband
    *
    * This is computed from the size of the largest zone in the mesh.
    * This value is precomputed and does not change.
    *
    */

   Real narrowBandWidth;

   /**
    * The next time step.
    *
    * This value is computed each timestep, and the minimum value from any domain is used.
   */
   
   Real dt;

   /**
    * The simulation time.
    *
    * This value starts at 0.0, and is incremented each step by dt.
    */
   
   Real time;

   /**
    * The number of simulations steps taken thus far.
    */
   
   int  cycle;

   /**
    * A value that indicates that all the sources have completed, across all domains
    */
   
   bool sourcesComplete;

   int numNodesUnreached;

   struct Timers {
      Real initialize[2];
      Real run[2];
   } timers;

};


 
class StepState {
public:
   
   /// The zones on the narrow band.
   /// The collection of zones that have one or more nodes on the narrow band.
   std::vector<int>    surfaceFrontZone;
   /// a node that touches a surface front zone.
   std::vector<int>    surfaceFrontNode;
   
   std::vector<int>    nearbyNodes;
   
   PlanarFront         planarFront;

   // In a few algorithms, it is convenient to have a scratch space of
   // size nnodes available to read and write to, without having to
   // allocate/deallocate it.  This array survives throught the run,
   // but the values set in one cycle are never read in the step
   // cycle.  In fact, values written in one function are never read
   // by another function.
   
   std::vector<int>    nodeTempArray;

   
};

// fixme?  Better place for this?
struct ComputeErrors {
public:
   ComputeErrors() :
      error1(0.0), error2(0.0), errorMax(0.0), errorNodeCount(0) {}
   
   Real error1;
   Real error2;
   Real errorMax;
   GlobalID errorNodeCount;
   void computeLocalErrors( const Simulation *sim);
   void completeErrors();
   void reportErrors();
};



// Problem setup functions
Real getParam( std::ifstream &f, const std::string &expected);
bool ProblemSize( std::string filename, Point &size );
bool ProblemSetup( std::string filename, Simulation &sim, Real sourceRad );

}

#endif


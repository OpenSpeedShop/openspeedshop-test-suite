


# C++ compiler
#CXX = icpc 
CXX = g++ 
CXXFLAGS = -O2 -g -DHAVE_SILO=${HAVE_SILO}

# The MPI wrapper for the C++ compiler
MPICXX = mpiicpc

# The linker flags used by the MPI wrapper compiler.  This is needed
# when compiling the Charm++ versions
#  example based on mpiicpc -show
MPILDFLAGS =  -Wl,-rpath,/usr/local/tools/mvapich-intel/lib/shared \
	      -L/usr/local/tools/mvapich-intel/lib/shared -L/usr/lib64 \
	      -L/usr/local/tools/mvapich-intel/lib -lmpich -i-dynamic

# Turn this on to generate silo output files 
#  example location for silo
HAVE_SILO = 1
SILO_DIR = /usr/gapps/silo/current/chaos_5_x86_64_ib
SILO_INC = -I$(SILO_DIR)/include
SILO_LIB = -L$(SILO_DIR)/lib -lsilo
INCDIR = $(SILO_INC)


# Location of the Charm installation
#  Note:  6.4.0 or later is required for MPI interoperability

# example location for charmdir
CHARMDIR =  ${HOME}/charm/mpi-linux-x86_64-icc
CHARMC = $(CHARMDIR)/bin/charmc
CHARMINC = -I$(CHARMDIR)/include
CHARMCFLAGS = -language charm++ -module RefineLB,CommonLBs

# Don't change below this point

all: lassen_serial lassen_mpi lassen_charm lassen_charmmpi

LIBLASSEN_SOURCES = Simulation.cxx Lassen.cxx Source.cxx \
	           Output.cxx LassenUtil.cxx Input.cxx

LIBLASSEN_OBJECTS = $(LIBLASSEN_SOURCES:.cxx=.o)

LASSEN_SERIAL_SOURCES = lassenSerial.cxx
LASSEN_SERIAL_OBJECTS = $(LASSEN_SERIAL_SOURCES:.cxx=.o)

LIBLASSENMPI_SOURCES = SimulationParallel.cxx SimulationMPI.cxx 
LIBLASSENMPI_OBJECTS = $(LIBLASSENMPI_SOURCES:.cxx=.o)

LIBLASSENCHARM_CI_SOURCES = 
LIBLASSENCHARM_CI_HEADERS = $(LIBLASSENCHARM_CI_SOURCES:.ci=.decl.h)


AR = ar csr 

.SUFFIXES: .cxx .o .ci

.cxx.o:
	$(CXX) -c $(CXXFLAGS) $(INCDIR) -o $@ $<

# common
libfront.a: $(LIBLASSEN_OBJECTS)
	$(AR) libfront.a $(LIBLASSEN_OBJECTS) 


# lassen_serial
lassen_serial: $(LASSEN_SERIAL_OBJECTS) libfront.a
	$(CXX) $(LASSEN_SERIAL_OBJECTS) -L. -lfront $(SILO_LIB) -o lassen_serial


# MPI 
lassenMPI.o: lassenMPI.cxx 
	$(MPICXX) -c $(CXXFLAGS) $(INCDIR) -o $@ $<

SimulationMPI.o: SimulationMPI.cxx
	$(MPICXX) -c $(CXXFLAGS) $(INCDIR) -o $@ $<

libfrontmpi.a: $(LIBLASSENMPI_OBJECTS)
	$(AR) libfrontmpi.a $(LIBLASSENMPI_OBJECTS) 

lassen_mpi: lassenMPI.o libfrontmpi.a libfront.a
	$(MPICXX) lassenMPI.o -L. -lfrontmpi -lfront $(SILO_LIB) -o lassen_mpi

# CHARM
SimulationCHARM.decl.h : SimulationCHARM.ci
	$(CHARMC) SimulationCHARM.ci 

SimulationCHARMMain.decl.h : SimulationCHARMMain.ci
	$(CHARMC) SimulationCHARMMain.ci 

SimulationCHARM.o : SimulationCHARM.decl.h SimulationCHARM.ci SimulationCHARM.cxx
	$(CHARMC) -c $(CXXFLAGS) $(INCDIR) SimulationCHARM.cxx -o SimulationCHARM.o 

SimulationCHARMMain.o : SimulationCHARMMain.decl.h SimulationCHARMMain.ci SimulationCHARMMain.cxx
	$(CHARMC) -c $(CXXFLAGS) $(INCDIR) SimulationCHARMMain.cxx -o SimulationCHARMMain.o 

libmoduleSimulationCHARM.a : SimulationCHARM.o SimulationParallel.o
	$(CHARMC) SimulationCHARM.o SimulationParallel.o -o libmoduleSimulationCHARM.a 

lassen_charm : libmoduleSimulationCHARM.a SimulationCHARMMain.o SimulationParallel.o libfront.a
	$(CHARMC) $(CHARMCFLAGS) $(CXXFLAGS) SimulationCHARMMain.o -L. -lfront -o lassen_charm -module SimulationCHARM $(MPILDFLAGS)  $(SILO_LIB)


# charmmpi specific
lassenCharmMPI.o : lassenCharmMPI.cxx
	$(MPICXX) -c $(CXXFLAGS) $(INCDIR) $(CHARMINC) lassenCharmMPI.cxx -o lassenCharmMPI.o 

lassen_charmmpi : libmoduleSimulationCHARM.a lassenCharmMPI.o SimulationParallel.o libfront.a
	$(CHARMC) -mpi -o lassen_charmmpi lassenCharmMPI.o $(CHARMFLAGS) $(CXXFLAGS) -L. -module SimulationCHARM  -lfront $(SILO_LIB) $(MPILDFLAGS)





clean:
	rm -f *.o lib*.a *.decl.h *.def.h lassen_serial lassen_charm lassen_mpi lassen_charmmpi

veryclean: clean
	rm -f *.silo *.core *~ *.btr *.TVD.v3breakpoints charmrun

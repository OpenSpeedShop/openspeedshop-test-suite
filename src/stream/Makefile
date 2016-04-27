MPICXX=icc
LD=icc

# For CCE module unload PrgEnv-cray PrgEnv-pgi PrgEnv-intel PrgEnv-gnu; module load PrgEnv-cray
# For ICC module unload PrgEnv-cray PrgEnv-pgi PrgEnv-intel PrgEnv-gnu; module load PrgEnv-intel
# For PGI module unload PrgEnv-cray PrgEnv-pgi PrgEnv-intel PrgEnv-gnu; module load PrgEnv-pgi  
# For GNU module unload PrgEnv-cray PrgEnv-pgi PrgEnv-intel PrgEnv-gnu; module load PrgEnv-gnu  

#CFLAGS_CRAY= -h aggress -dynamic -h PIC  -h negmsgs -h aggress -G2 -O2   -DTUNED -h omp  
CFLAGS_CRAY= -h omp -h ipa0 -c -h keepfiles -O2   -DTUNED -h omp -O ipa0 
LDFLAGS_CRAY= -h PIC -h dynamic -h omp

CFLAGS_INTEL= -c -fPIC -openmp -O2   -DTUNED 
LDFLAGS_INTEL= -Bdynamic -openmp -lmpi

CFLAGS_PGI= -fPIC -mp -O2   -DTUNED 
LDFLAGS_PGI= -dynamic -mp

CFLAGS=$(CFLAGS_INTEL)
LDFLAGS=$(LDFLAGS_INTEL)

oss:    stream.x mpi_wave.x

stream.x:   stream.o
	$(MPICXX) $(LDFLAGS) -o  $@ $<  

stream.o: stream.c
	$(MPICXX) $(CFLAGS) -o  $@ $<  

mpi_wave.x:   mpi_wave.o
	$(MPICXX) $(LDFLAGS) -o  $@ $<  

mpi_wave.o: mpi_wave.c
	$(MPICXX) $(CFLAGS) -o  $@ $<  

clean:
	rm stream.o stream.x mpi_wave.o mpi_wave.x *.s *.inst.* *.pdb *.chk.* 


TARGET = all

all:
	mpicc -o nbody -O0 -g nbody-mpi.c -lm

gnu:
	mpicc -o nbody-gnu -O0 -g nbody-mpi.c -lm

intel:
	mpiicc -o nbody-intel -O0 -g nbody-mpi.c -lm

clean:
	rm -f nbody nbody-intel nbody-gnu

veryclean:
	rm -rf nbody-survey-csvdata-* nbody*.openss cbtfAutoTopology

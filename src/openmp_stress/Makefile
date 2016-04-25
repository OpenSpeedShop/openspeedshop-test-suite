TARGET = all

all:    
	gfortran -D__GFORTRAN -g -O0 -fopenmp -ostress_omp stress_omp.f -lm -lgomp

clean:
	rm -f stress_omp

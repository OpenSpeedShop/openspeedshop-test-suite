TARGET = all

all:    
	gfortran -c -g -O2 matmul.f90
	gfortran -o matmul -lblas matmul.o -llapack


clean:
	rm -f matmul matmul.o

TARGET = all

all:    
	gfortran -c -g -O2 matmul.f90
	#gfortran -o matmul -lblas matmul.o -llapack
	gfortran -o matmul -llapack matmul.o -lblas

clean:
	rm -f matmul matmul.o

veryclean: clean
	rm -rf matmul-survey-csvdata-*  matmul-overview-csvdata-* *.openss

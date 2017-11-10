#
CC=gcc
ICC=icc

ICFLAGS = -O2 -g -I . -qopenmp
CFLAGS = -O2 -g -I . -fopenmp
ILDFLAGS = -qopenmp
LDFLAGS = -fopenmp

OBJS=	matmul.o imatmul.o

SRC=	matmul.c

all:	matmul

matmul.o: matmul.c
	$(CC) -c $(CFLAGS) matmul.c

matmul: matmul.o
	$(CC) -o matmul $(LDFLAGS) matmul.o

imatmul.o: imatmul.c
	$(ICC) -c $(ICFLAGS) imatmul.c

imatmul: imatmul.o
	$(ICC) -o imatmul $(ILDFLAGS) imatmul.o

clean:
	rm -f $(OBJS) matmul imatmul

veryclean:
	rm -rf $(OBJS) matmul imatmul *.openss slurm*.out cbtfAutoTopology attachBE_connections offline-cbtf


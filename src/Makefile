COMP = gnu
OMP = 0

ifeq ($(COMP),intel)
	CC = icc
	CCFLAGS = -g -O3 -qopenmp
endif

ifeq ($(COMP),gnu)
	CC = gcc
	CCFLAGS = -O3 -march=native -mtune=native -Wall -Wextra
	LFLAGS = -lm -lcuda -lcudart
endif

ifeq ($(COMP),gnuOMP)
	CC = gcc
	CCFLAGS = -O3 -march=native -mtune=native -fopenmp -Wall -Wextra
	LFLAGS = -lm -fopenmp -lcuda -lcudart
endif

CUFLAGS = -O3 -Xcompiler=-O3,-march=native,-mtune=native,-Wall,-Wextra -arch=sm_50

ifeq ($(OMP),1)
	CUFLAGS = -O3 -Xcompiler=-O3,-march=native,-mtune=native,-fopenmp,-Wall,-Wextra -arch=sm_50
endif

BIN = 2DSOR
OBJ = PoissonSOR2D.o PoissonSOR2D_CUDA.o main.o

all: $(BIN)


# Dependencies
main.o: main.c
PoissonSOR2D_CUDA.o: PoissonSOR2D_CUDA.c
PoissonSOR2D.o: PoissonSOR2D.c


$(BIN): $(OBJ)
	#$(CC) $(CFLAGS) -lcuda -I/opt/cuda/include $(OBJ) -o $@ $(LFLAGS)
	nvcc $(CUFLAGS) $(OBJ) -o $@

%.o: %.c
	nvcc -x cu $(CUFLAGS) -dc -c $< -o $@

clean:
	rm $(BIN) $(OBJ)

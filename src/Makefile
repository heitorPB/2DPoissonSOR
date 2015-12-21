COMP = gnu

ifeq ($(COMP),intel)
	CC = icc
	CCFLAGS = -g -O3 -qopenmp
endif

ifeq ($(COMP),gnu)
	CC = gcc
	CCFLAGS = -O3 -march=native -mtune=native -Wall -Wextra
	LFLAGS = -lm
endif

ifeq ($(COMP),gnuOMP)
	CC = gcc
	CCFLAGS = -O3 -march=native -mtune=native -fopenmp -Wall -Wextra
	LFLAGS = -lm -fopenmp
endif

BIN = 2DSOR
OBJ = main.o PoissonSOR2D.o

all: $(BIN)


# Dependencies
main.o: main.c
PoissonSOR2D.o: PoissonSOR2D.c


$(BIN): $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) -o $@ $(LFLAGS)

%.o: %.c
	$(CC) $(CCFLAGS) -c $< -o $@

clean:
	rm $(BIN) $(OBJ)

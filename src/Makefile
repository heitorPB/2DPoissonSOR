COMP = gnu

ifeq ($(COMP),intel)
	CC = icc
	CCFLAGS = -g -O3 -qopenmp
endif

ifeq ($(COMP),gnu)
	CC = gcc
	CCFLAGS = -O2 -march=native -mtune=native -fopenmp
	LFLAGS = -lm
endif

BIN = 2DSOR
OBJ = main.o PoissonSOR2D.o

all: $(BIN)

# Dependencies
main.o: main.c
PoissonSOR2D.o: PoissonSOR2D.c

#$(EXE4b): $(OBJS4b) .ALWAYSEXEC
#	$(CC) $(CCFLAGS) $(OBJS4b) -o $@ -lm
#
#$(EXE5): $(OBJS5) .ALWAYSEXEC
#	$(CC) $(CCFLAGS) $(OBJS5) -o $@ $(LBLAS) -lm

$(BIN): $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) -o $@ $(LFLAGS)

%.o: %.c
	$(CC) $(CCFLAGS) -c $< -o $@

clean:
	rm $(BIN) $(OBJ)

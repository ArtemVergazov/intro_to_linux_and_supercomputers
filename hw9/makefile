#!make

CC = mpicc
CFLAGS = -O3 -fopenmp -g
LFLAGS = -lm -lrt

MAIN1 = piprog_H.c   
MAIN2 = piprog_OMP.c 
SRC = picalc.c piutil.c 
INC = piprog.h 
SCRIPT = runmpi.sh runomp.sh runarr.sh runPImpi.sh runPIomp.sh runSERIES.sh d1.sh piloop.sh analyze.sh
RES = log

OBJ = $(SRC:.c=.o)

EXEC = pimpi  piomp 

all : $(EXEC)

pimpi : $(MAIN1) $(OBJ)
	$(CC) -o $(@) $(CFLAGS) $(^) $(LFLAGS)

piomp : $(MAIN2) $(OBJ)
	$(CC) -o $(@) $(CFLAGS) $(^) $(LFLAGS)

$(MAIN1) $(MAIN2) $(OBJ): $(INC)

test :  pimpi 
	export OMP_NUM_THREADS=2; ./piomp -a
	export OMP_NUM_THREADS=2; mpirun -np 2 -mca btl_base_warn_component_unused 0 ./pimpi -a

clean : 
	rm $(EXEC) $(OBJ)

tar :
	tar -cvzf lab9mpi.tar.gz $(MAIN1) $(MAIN2) $(SRC) $(INC) $(SCRIPT) $(RES) makefile

.PHONY: all clean tar

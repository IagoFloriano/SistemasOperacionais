DEBUG_FLAGS = -g
CFLAGS = -Wall -std=gnu99

#------------------------------------------------------------------------------
.PHONY : all clean

#------------------------------------------------------------------------------
all : 

ppos_core.o : ppos_core.c
	gcc $(CFLAGS) -c $^

#------------------------------------------------------------------------------
clean :
	$(RM) *.o

purge : clean
	$(RM) pingpong-tasks1 pingpong-tasks2 pingpong-tasks3

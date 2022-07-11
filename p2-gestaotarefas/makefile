DEBUG_FLAGS = -g
CFLAGS = -Wall -std=gnu99

#------------------------------------------------------------------------------
.PHONY : all clean

#------------------------------------------------------------------------------
all : pingpong-tasks1 pingpong-tasks2 pingpong-tasks3

pingpong-tasks1 : pingpong-tasks1.o ppos_core.o ppos_data.h ppos.h
	gcc $(CFLAGS) -o $@ $^

pingpong-tasks2 : pingpong-tasks2.o ppos_core.o ppos_data.h ppos.h
	gcc $(CFLAGS) -o $@ $^

pingpong-tasks3 : pingpong-tasks3.o ppos_core.o ppos_data.h ppos.h
	gcc $(CFLAGS) -o $@ $^

ppos_core.o : ppos_core.c
	gcc $(CFLAGS) -c $^

#------------------------------------------------------------------------------
clean :
	$(RM) *.o

purge : clean
	$(RM) pingpong-tasks1 pingpong-tasks2 pingpong-tasks3

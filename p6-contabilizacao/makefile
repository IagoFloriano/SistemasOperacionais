DEBUG_FLAGS = -g -DDEBUG
CFLAGS = -Wall -std=gnu99 # $(DEBUG_FLAGS)

#------------------------------------------------------------------------------
.PHONY : all clean

#------------------------------------------------------------------------------
all : pingpong-contab pingpong-contab-prio

debug: CFLAGS += $(DEBUG_FLAGS)
debug: all

pingpong-contab-prio: ppos_core.o pingpong-contab-prio.o queue.o
	gcc -o $@ $^

pingpong-contab: ppos_core.o pingpong-contab.o queue.o
	gcc -o $@ $^

%.o: %.c
	gcc $(CFLAGS) -c $^
#------------------------------------------------------------------------------
clean :
	$(RM) *.o

purge : clean
	$(RM) pingpong-contab pingpong-contab-prio

DEBUG_FLAGS = -g -DDEBUG
CFLAGS = -Wall -std=gnu99 # $(DEBUG_FLAGS)

#------------------------------------------------------------------------------
.PHONY : all clean

#------------------------------------------------------------------------------
all : pingpong-preempcao pingpong-preempcao-stress

debug: CFLAGS += $(DEBUG_FLAGS)
debug: all

pingpong-preempcao-stress: ppos_core.o pingpong-preempcao-stress.o queue.o
	gcc -o $@ $^

pingpong-preempcao: ppos_core.o pingpong-preempcao.o queue.o
	gcc -o $@ $^

%.o: %.c
	gcc $(CFLAGS) -c $^
#------------------------------------------------------------------------------
clean :
	$(RM) *.o

purge : clean
	$(RM) pingpong-preempcao pingpong-preempcao-stress

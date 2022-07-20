DEBUG_FLAGS = -g -DDEBUG
CFLAGS = -Wall -std=gnu99 # $(DEBUG_FLAGS)

#------------------------------------------------------------------------------
.PHONY : all clean

#------------------------------------------------------------------------------
all : pingpong-maintask

debug: CFLAGS += $(DEBUG_FLAGS)
debug: all

pingpong-maintask: ppos_core.o pingpong-maintask.o queue.o
	gcc -o $@ $^

%.o: %.c
	gcc $(CFLAGS) -c $^
#------------------------------------------------------------------------------
clean :
	$(RM) *.o

purge : clean
	$(RM) pingpong-maintask

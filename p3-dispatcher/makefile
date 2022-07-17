DEBUG_FLAGS = -g -DDEBUG
CFLAGS = -Wall -std=gnu99 # $(DEBUG_FLAGS)

#------------------------------------------------------------------------------
.PHONY : all clean

#------------------------------------------------------------------------------
all : pingpong-dispatcher

pingpong-dispatcher: ppos_core.o pingpong-dispatcher.o queue.o
	gcc -o $@ $^

%.o: %.c
	gcc $(CFLAGS) -c $^
#------------------------------------------------------------------------------
clean :
	$(RM) *.o

purge : clean
	$(RM) pingpong-dispatcher

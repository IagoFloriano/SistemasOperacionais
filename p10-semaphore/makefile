DEBUG_FLAGS = -g -DDEBUG
CFLAGS = -Wall -std=gnu99 # $(DEBUG_FLAGS)
EXE = pingpong-semaphore

#------------------------------------------------------------------------------
.PHONY : all clean

#------------------------------------------------------------------------------
all : $(EXE) pingpong-racecond

debug: CFLAGS += $(DEBUG_FLAGS)
debug: all

$(EXE): ppos_core.o $(EXE).o queue.o ppos_ipc.o
	gcc -o $@ $^

pingpong-racecond: ppos_core.o pingpong-racecond.o queue.o ppos_ipc.o
	gcc -o $@ $^

%.o: %.c
	gcc $(CFLAGS) -c $^
#------------------------------------------------------------------------------
clean :
	$(RM) *.o

purge : clean
	$(RM) $(EXE) pingpong-racecond

DEBUG_FLAGS = -g -DDEBUG
CFLAGS = -Wall -std=gnu99 # $(DEBUG_FLAGS)
EXE = pingpong-semaphore pingpong-racecond

#------------------------------------------------------------------------------
.PHONY : all clean

#------------------------------------------------------------------------------
all : $(EXE)

debug: CFLAGS += $(DEBUG_FLAGS)
debug: all

# $(EXE): ppos_core.o $(EXE).o queue.o
	# gcc -o $@ $^

%.o: %.c
	gcc $(CFLAGS) -c $^

%: ppos_core.o %.o queue.o
	gcc -o $@ $^
#------------------------------------------------------------------------------
clean :
	$(RM) *.o

purge : clean
	$(RM) $(EXE)

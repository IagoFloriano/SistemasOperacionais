DEBUG_FLAGS = -g -DDEBUG
CFLAGS = -Wall -std=gnu99 -lm
EXE = pingpong-mqueue

#------------------------------------------------------------------------------
.PHONY : all clean

#------------------------------------------------------------------------------
all : $(EXE)

debug: CFLAGS += $(DEBUG_FLAGS)
debug: all

$(EXE): ppos_core.o $(EXE).o queue.o ppos_ipc.o
	gcc -o $@ $^ $(CFLAGS)

%.o: %.c
	gcc $(CFLAGS) -c $^
#------------------------------------------------------------------------------
clean :
	$(RM) *.o

purge : clean
	$(RM) $(EXE)

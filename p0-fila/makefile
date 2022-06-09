DEBUG_FLAGS = -g
CFLAGS = -Wall -std=gnu99

#------------------------------------------------------------------------------
.PHONY : all clean

#------------------------------------------------------------------------------
all : testafila

testafila : testafila.o queue.o
	gcc $(CFLAGS) -o $@ $^


debug : testafila.o queue.o
	gcc $(CFLAGS) $(DEBUG_FLAGS) -o $@ $^

#------------------------------------------------------------------------------
clean :
	$(RM) testafila *.o

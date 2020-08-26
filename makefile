
CC = gcc
CFLAGS = -c -std=c99 -Wall
LDFLAGS=
SUBDIR = src/
SOURCES=$(SUBDIR)test.c src/queue.c
OBJECTS=$(SOURCES:.c=.o)
EXECUTABLE=test


all: $(SOURCES) $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(CC) $(LDFLAGS) $(OBJECTS) -o $@

.c.o:
	$(CC) $(CFLAGS) $< -o $@

clean:
	rm -rf $(SUBDIR)*.o $(EXECUTABLE)			

#test: test.o fifo.o
#	$(CC) test.o fifo.o -o test
#
#test.o: test.c 
#	$(CC) $(CFLAGS)	test.c
#
#fifo.o: fifo.c
#	$(CC) $(CFLAGS)	fifo.c

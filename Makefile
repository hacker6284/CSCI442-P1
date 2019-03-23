CC=g++
CFLAGS=-c -g -std=c++11
SOURCES=P1.cpp P1_Classes.cpp output.cpp Handler.cpp
OBJECTS=$(SOURCES:.cpp=.o)
LIBS=
LDFLAGS=
PROGRAM=simulator

all: $(PROGRAM)

$(PROGRAM): $(OBJECTS)
	$(CC) -o $(PROGRAM) $(OBJECTS) $(LDFLAGS) $(LIBS)

.cpp.o:
	$(CC) $(CFLAGS) $<

depend: .depend

.depend: $(SOURCES)
	rm -f .depend
	$(CC) $(CFLAGS) -MM $^ >> .depend;

include .depend

clean:
	rm -f .depend $(PROGRAM) $(OBJECTS)

run:
	./$(PROGRAM) -vta CUSTOM example_simulation.txt

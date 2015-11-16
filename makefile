CC = gcc
CFLAGS = -c
OBJECTS = main.o pattern_matching.o slist.o

DEBUG_FLAGS = -g
DEBUG_OBJECTS = main.c pattern_matching.c slist.c 

app: $(OBJECTS)
	$(CC) $(OBJECTS) -Wall -o app

debug: $(DEBUG_OBJECTS)
	$(CC) $(DEBUG_FLAGS) $(DEBUG_OBJECTS) -Wall -o app

clean:
	rm $(OBJECTS)



pattern_matching.o: pattern_matching.h pattern_matching.c
	$(CC) $(CFLAGS) pattern_matching.c

main.o: main.c
	$(CC) $(CFLAGS) main.c

slist.o: slist.h slist.c
	$(CC) $(CFLAGS) slist.c

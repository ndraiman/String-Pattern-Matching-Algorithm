CC = gcc
CFLAGS = -c
OBJECTS = slist.o main.o

app: $(OBJECTS)
	$(CC) $(OBJECTS) -Wall -o app
	
clean:
	rm $(OBJECTS)
	
slist.o: slist.c slist.H
	$(CC) $(CFLAGS) slist.c slist.H

main.o: main.c
	$(CC) $(CFLAGS) main.c

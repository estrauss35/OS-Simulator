\CC = gcc
DEBUG = -g
CFLAGS = -Wall -std=c99 -c $(DEBUG)
LFLAGS = -Wall -std=c99 $(DEBUG)
sim04 : Main.o ConfigScanner.o MetaScanner.o Sorting.o Helpers.o Scheduler.o simtimer.o
	$(CC) $(LFLAGS) Main.o ConfigScanner.o MetaScanner.o Sorting.o Helpers.o Scheduler.o simtimer.o -lpthread -o sim04
Main.o : Main.c ConfigScanner.h MetaScanner.h
	$(CC) $(CFLAGS) Main.c
ConfigScanner.o : ConfigScanner.c ConfigScanner.h Helpers.h
	$(CC) $(CFLAGS) ConfigScanner.c
MetaScanner.o : MetaScanner.c MetaScanner.h ConfigScanner.h
	$(CC) $(CFLAGS) MetaScanner.c
Sorting.o : Sorting.c Sorting.h
	$(CC) $(CFLAGS) Sorting.c
Helpers.o : Helpers.c Helpers.h
	$(CC) $(CFLAGS) Helpers.c
Scheduler.o : Scheduler.c Scheduler.h
	$(CC) $(CFLAGS) Scheduler.c
simtimer.o : simtimer.c simtimer.h
	$(CC) $(CFLAGS) simtimer.c
clean:
	\rm *.o sim04

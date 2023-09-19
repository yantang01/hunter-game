main: building.o door.o evidence.o ghost.o journal.o main.o room.o logger.o util.o hunter.o
	gcc -o main building.o door.o evidence.o ghost.o journal.o main.o room.o logger.o util.o hunter.o -lpthread

building.o: building.c defs.h
	gcc -c building.c

door.o: door.c defs.h
	gcc -c door.c

evidence.o: evidence.c defs.h
	gcc -c evidence.c

journal.o: journal.c defs.h
	gcc -c journal.c

main.o: main.c defs.h
	gcc -c main.c

room.o: room.c defs.h
	gcc -c room.c

util.o: util.c defs.h
	gcc -c util.c

hunter.o: hunter.c defs.h
	gcc -c hunter.c

run:
	./main

clean:
	rm main building.o door.o evidence.o journal.o main.o room.o util.o hunter.o

# check memory leak
check:
	valgrind ./main

# check memory leak with full details
check-full:
	valgrind --leak-check=full ./main

check-track:
	valgrind --track-origins=yes ./main
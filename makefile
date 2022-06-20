CC=gcc-9
CFLAGS=-g -W -Wall -O3 -lm -D_REENTRANT -lpthread

gen: index.o rect.o card.o queue.o split_q.o node.o gen.o
	$(CC) gen.o index.o node.o rect.o card.o queue.o split_q.o -o gen $(CFLAGS)

read: index.o rect.o card.o queue.o split_q.o node.o read.o
	$(CC) read.o index.o node.o rect.o card.o queue.o split_q.o -o read $(CFLAGS)

run: index.o rect.o card.o queue.o split_q.o node.o test.o
	$(CC) test.o index.o node.o rect.o card.o queue.o split_q.o -o run $(CFLAGS)

gen.o: gen.c index.h
	$(CC) -c gen.c $(CFLAGS)

read.o: read.c index.h
	$(CC) -c read.c $(CFLAGS)

test.o: test.c index.h
	$(CC) -c test.c $(CFLAGS)

index.o: index.c index.h card.h
	$(CC) -c index.c $(CFLAGS)

node.o: node.c index.h card.h
	$(CC) -c node.c $(CFLAGS)

rect.o: rect.c index.h
	$(CC) -c rect.c $(CFLAGS)

queue.o: queue.c index.h
	$(CC) -c queue.c $(CFLAGS)

card.o: card.c index.h card.h
	$(CC) -c card.c $(CFLAGS)

split_q.o: split_q.c index.h card.h split_q.h
	$(CC) -c split_q.c $(CFLAGS)

clean:
	rm -rf *.o run read gen

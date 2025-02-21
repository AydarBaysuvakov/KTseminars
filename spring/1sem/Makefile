CFLAGS=-g -O0 -Wall
O_FILES= main.o array.o hashtable.o parser.o wcounter.o

all: word_counter clean

word_counter: $(O_FILES)
	g++ $(O_FILES) -o wcounter $(CFLAGS)

wcounter.o: wcounter.c
	g++ -c wcounter.c

parser.o: parser.c
	g++ -c parser.c

hashtable.o: hashtable.c
	g++ -c hashtable.c

array.o: array.c
	g++ -c array.c

main.o: main.c
	g++ -c main.c

clean:
	rm -rf *.o

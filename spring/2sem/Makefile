CFLAGS=-g -O0 -Wall
O_FILES= main.o animal.o cat.o dog.o

all: animal clean

animal: $(O_FILES)
	g++ $(O_FILES) -o cat_dog $(CFLAGS)

animal.o: animal.c
	g++ -c animal.c

cat.o: cat.c
	g++ -c cat.c

dog.o: dog.c
	g++ -c dog.c

main.o: main.c
	g++ -c main.c

clean:
	rm -rf *.o

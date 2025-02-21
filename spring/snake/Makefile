CFLAGS=-g -O0 -Wall
O_FILES= main.o model.o view.o

all: snake clean

snake: $(O_FILES)
	g++ $(O_FILES) -o snake $(CFLAGS)

model.o: model.cc
	g++ -c model.cc

view.o: view.cc
	g++ -c view.cc

main.o: main.cc
	g++ -c main.cc

clean:
	rm -rf *.o

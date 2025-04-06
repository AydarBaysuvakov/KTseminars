CFLAGS=-g -O0 -Wall -lsfml-graphics -lsfml-window -lsfml-system
O_FILES= main.o model.o view.o control.o

all: snake clean

snake: $(O_FILES)
	g++ $(O_FILES) -o snake $(CFLAGS)

model.o: model.cc model.h
	g++ -c model.cc

view.o: view.cc view.h
	g++ -c view.cc

control.o: control.cc control.h
	g++ -c control.cc

main.o: main.cc
	g++ -c main.cc

clean:
	rm -rf *.o


objects = *.o
sources = main.cpp ./source/*.cpp
libs = -pthread

all: wanDB
	./wanDB

wanDB: $(objects)
	g++ -g -o wanDB $(libs) $(objects)

$(objects):
	g++ -g -Wall -c $(libs) $(sources)

clean:
	rm $(objects) wanDB

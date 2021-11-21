CC = g++
CFLAGS = -std=c++17 -Wall -Werror -Wpedantic -Wextra

all: prog

clean:
	del *.o
	del prog.exe

prog:  m.o i.o g.o p.o
	$(CC) $(CFLAGS) m.o p.o i.o g.o -o prog.exe

m.o: i.o g.o  n.o pa.o p.o
	$(CC) $(CFLAGS) -c main.cpp -o m.o

p.o: g.o n.o pa.o
	$(CC) $(CFLAGS) -c Perf_Matching.cpp -o g.o

i.o: g.o n.o pa.o
	$(CC) $(CFLAGS) -c input.cpp -o i.o

g.o: n.o pa.o
	$(CC) $(CFLAGS) -c auxiliary_classes/graph.cpp -o g.o n.o pa.o

n.o:
	$(CC) $(CFLAGS) -c auxiliary_classes/node.cpp -o n.o

pa.o:
	$(CC) $(CFLAGS) -c auxiliary_classes/partition.cpp -o pa.o


all: server client

server: server.o
	g++ server.o -std=c++20 -o server

client: client.o
	g++ client.o -std=c++20 -o client

server.o: server.cpp
	g++ -std=c++20 -c server.cpp -o server.o

client.o: client.cpp
	g++ -std=c++20 -c client.cpp -o client.o

clean:
	rm -rf *.o server client

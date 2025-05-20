all: client client_api server init destroy

client: client.o libcolorprint.a
	g++ client.o libcolorprint.a -std=c++17 -o client

client.o: client.cpp colorprint.hpp
	g++ client.cpp colorprint.hpp -c -std=c++17

client_api: client_api.o Bank.o
	g++ client_api.o Bank.o -std=c++17 -o client_api

client_api.o: client_api.cpp Bank.h
	g++ client_api.cpp -c -std=c++17

server: server.o Bank.o
	g++ server.o Bank.o -std=c++17 -o server

server.o: server.cpp Bank.h
	g++ server.cpp -c -std=c++17

init: init.o
	g++ init.o -std=c++17 -o init

init.o: init.cpp Bank.h
	g++ init.cpp -c -std=c++17

destroy: destroy.o
	g++ destroy.o -std=c++17 -o destroy

destroy.o: destroy.cpp Bank.h
	g++ destroy.cpp -c -std=c++17

Bank.o: Bank.cpp Bank.h
	g++ Bank.cpp -c -std=c++17

clean:
	rm -f *.o client init destroy server client_api

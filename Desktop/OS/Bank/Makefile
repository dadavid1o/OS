all: client_api init destroy

client_api: client_api.o Bank.o
	g++ client_api.o Bank.o -std=c++17 -o client_api

client_api.o: client_api.cpp Bank.h
	g++ client_api.cpp -c -std=c++17

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
	rm -f *.o init destroy client_api

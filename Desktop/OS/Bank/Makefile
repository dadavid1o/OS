all: client init destroy

client: client.o Bank.o
	g++ client.o Bank.o -std=c++17 -o client

client.o: client.cpp Bank.h
	g++ client.cpp -c -std=c++17

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
	rm -f *.o client init destroy

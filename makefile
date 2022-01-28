all: server client

server: server.c system.h
	gcc -pthread -o server server.c

client: client.c system.h
	gcc -o client client.c

clean:
	rm -f server
	rm -f client
	rm -f *.o
	rm -f *~

all: cli serv
cli: client.c
	gcc -o client client.c request.c -I .

serv: server.c
	gcc -o server server.c -I. -lpthread

clean:
	rm server client

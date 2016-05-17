CC = gcc
EXE = udpcli udpserv
CFLAGS = -Wall -DDEBUG

all: $(EXE)

udpserv: server.o signal.o
	$(CC) -o $@ $^
server.o: server.c
	$(CC) -o $@ -c $< $(CFLAGS)
signal.o: signal.c
	$(CC) -o $@ -c $< $(CFLAGS)

udpcli: client.o
	$(CC) -o $@ $<
client.o: client.c
	$(CC) -o $@ -c $< $(CFLAGS)

clean:
	rm -f *.o $(EXE)

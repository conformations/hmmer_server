CC=g++
CFLAGS=-I/usr/local/include -c -Wall
LDFLAGS=-L/usr/local/lib -lprotobuf -lgflags -lglog -lzmq -g3 -O0

proto: hmmer.proto
	protoc --cpp_out=. hmmer.proto

hmmer.pb.o: proto hmmer.pb.cc
	$(CC) $(CFLAGS) hmmer.pb.cc

util.o: util.cc
	$(CC) $(CFLAGS) util.cc

client.o: client.cc
	$(CC) $(CFLAGS) client.cc

server.o: server.cc
	$(CC) $(CFLAGS) server.cc

client: client.o util.o hmmer.pb.o
	$(CC) $(LDFLAGS) client.o util.o hmmer.pb.o -o client

server: server.o util.o hmmer.pb.o
	$(CC) $(LDFLAGS) server.o util.o hmmer.pb.o -o server

all: client server

clean:
	rm -f client server *.o hmmer.pb.*

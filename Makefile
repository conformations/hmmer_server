CC=g++
CFLAGS=-I/usr/local/include -c -Wall
LDFLAGS=-L/usr/local/lib -lctemplate_nothreads -lprotobuf -lgflags -lglog -lzmq -g3 -O0

proto: hmmer.proto
	protoc --cpp_out=. hmmer.proto

hmmer.pb.o: proto hmmer.pb.cc
	$(CC) $(CFLAGS) hmmer.pb.cc

util.o: util.cc
	$(CC) $(CFLAGS) util.cc

parse.o: parse.cc
	$(CC) $(CFLAGS) parse.cc

client.o: client.cc proto
	$(CC) $(CFLAGS) client.cc

server.o: server.cc proto
	$(CC) $(CFLAGS) server.cc

client: client.o util.o hmmer.pb.o
	$(CC) $(LDFLAGS) client.o util.o hmmer.pb.o -o client

server: server.o parse.o util.o hmmer.pb.o
	$(CC) $(LDFLAGS) server.o parse.o util.o hmmer.pb.o -o server

all: client server

clean:
	rm -f client server *.o hmmer.pb.*

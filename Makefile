CC=g++
CFLAGS=-I/usr/local/include -c -Wall
LDFLAGS=-L/usr/local/lib -lctemplate_nothreads -lprotobuf -lgflags -lglog -lzmq -lboost_regex -g3 -O0

proto: hmmer.proto
	protoc --cpp_out=. hmmer.proto

hmmer.pb.o: proto hmmer.pb.cc
	$(CC) $(CFLAGS) hmmer.pb.cc

proto_util.o: proto_util.cc
	$(CC) $(CFLAGS) proto_util.cc

str_util.o: str_util.cc
	$(CC) $(CFLAGS) str_util.cc

parse.o: proto parse.cc
	$(CC) $(CFLAGS) parse.cc

client.o: client.cc proto
	$(CC) $(CFLAGS) client.cc

server.o: server.cc proto
	$(CC) $(CFLAGS) server.cc

client: client.o proto_util.o hmmer.pb.o
	$(CC) $(LDFLAGS) client.o proto_util.o hmmer.pb.o -o client

server: server.o parse.o str_util.o proto_util.o hmmer.pb.o
	$(CC) $(LDFLAGS) server.o str_util.o parse.o proto_util.o hmmer.pb.o -o server

all: client server

clean:
	rm -f client server *.o hmmer.pb.*


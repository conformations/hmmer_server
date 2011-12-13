#ifndef UTIL_H_
#define UTIL_H_

#include <ostream>
#include <string>
#include <vector>

#include <google/protobuf/message.h>
#include <zmq.hpp>

// Sends a text-format protocol buffer over socket
bool proto_send(const google::protobuf::Message& m, zmq::socket_t* socket);

// Receives a text-format protocol buffer from socket
bool proto_recv(google::protobuf::Message* m, zmq::socket_t* socket);

// Writes a text-format protocol buffer to out. Expensive method--
// involves parsing, serialization, etc.
void proto_show(const google::protobuf::Message& m, std::ostream* out);

// Reads the contents of the specified file into lines
void read_file(const char* file, std::vector<std::string>* lines);

// Writes contents to a file with the specified name
void write_file(const std::string& contents, const char* file);

#endif  // UTIL_H_

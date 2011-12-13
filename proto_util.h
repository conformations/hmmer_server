#ifndef UTIL_H_
#define UTIL_H_

#include <ostream>

#include <google/protobuf/message.h>
#include <zmq.hpp>

// Sends a text-format protocol buffer over socket
bool proto_send(const google::protobuf::Message& m, zmq::socket_t* socket);

// Receives a text-format protocol buffer from socket
bool proto_recv(google::protobuf::Message* m, zmq::socket_t* socket);

// Writes a text-format protocol buffer to out. Expensive method--
// involves parsing, serialization, etc.
void proto_show(const google::protobuf::Message& m, std::ostream* out);

#endif  // UTIL_H_

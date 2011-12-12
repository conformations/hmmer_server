#ifndef UTIL_H_
#define UTIL_H_

#include <google/protobuf/message.h>
#include <zmq.hpp>

bool proto_send(const google::protobuf::Message& r, zmq::socket_t* socket);
bool proto_recv(google::protobuf::Message* r, zmq::socket_t* socket);

#endif  // UTIL_H_

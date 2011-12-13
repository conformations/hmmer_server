#include "util.h"
#include "zhelpers.hpp"

#include <iostream>
#include <string>

#include <glog/logging.h>
#include <google/protobuf/message.h>
#include <google/protobuf/text_format.h>
#include <zmq.hpp>

bool proto_send(const google::protobuf::Message& r, zmq::socket_t* socket) {
  CHECK_NOTNULL(socket);

  std::string m;
  google::protobuf::TextFormat::PrintToString(r, &m);
  return s_send(*socket, m);
}

bool proto_recv(google::protobuf::Message* r, zmq::socket_t* socket) {
  CHECK_NOTNULL(r);
  CHECK_NOTNULL(socket);

  std::string m = s_recv(*socket);
  return google::protobuf::TextFormat::ParseFromString(m, r);
}

void proto_show(const google::protobuf::Message& r, std::ostream* out) {
  CHECK_NOTNULL(out);
  
  std::string m;
  google::protobuf::TextFormat::PrintToString(r, &m);
  (*out) << m << std::endl;
}

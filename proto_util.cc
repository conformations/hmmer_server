#include "proto_util.h"
#include "zhelpers.hpp"

#include <iostream>
#include <string>

#include <glog/logging.h>
#include <google/protobuf/message.h>
#include <google/protobuf/text_format.h>
#include <snappy.h>
#include <zmq.hpp>

bool proto_send(const google::protobuf::Message& r, zmq::socket_t* socket) {
  CHECK_NOTNULL(socket);

  std::string m;
  google::protobuf::TextFormat::PrintToString(r, &m);

  std::string n;
  snappy::Compress(m.data(), m.size(), &n);
  return s_send(*socket, n);
}

bool proto_recv(google::protobuf::Message* r, zmq::socket_t* socket) {
  CHECK_NOTNULL(r);
  CHECK_NOTNULL(socket);

  std::string m = s_recv(*socket);
  std::string n;
  snappy::Uncompress(m.data(), m.size(), &n);
  return google::protobuf::TextFormat::ParseFromString(n, r);
}

void proto_show(const google::protobuf::Message& r, std::ostream* out) {
  CHECK_NOTNULL(out);
  
  std::string m;
  google::protobuf::TextFormat::PrintToString(r, &m);
  (*out) << m << std::endl;
}

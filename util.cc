#include "util.h"
#include "zhelpers.hpp"

#include <algorithm>
#include <cstdlib>
#include <fstream>
#include <iterator>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

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

void write_file(const std::string& contents, const char* file) {
  CHECK_NOTNULL(file);
  std::ofstream f(file);
  f << contents;
  f.close();
}

void read_file(const char* file, std::vector<std::string>* lines) {
  CHECK_NOTNULL(file);
  CHECK_NOTNULL(lines);

  lines->clear();

  std::string line;

  std::ifstream f(file);
  while (f.good()) {
    getline(f, line);
    lines->push_back(line);
  }
}

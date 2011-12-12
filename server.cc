#include "hmmer.pb.h"
#include "util.h"

#include <iostream>
#include <fstream>
#include <string>

#include <ctemplate/template.h>
#include <gflags/gflags.h>
#include <glog/logging.h>
#include <zmq.hpp>

DEFINE_string(send_socket, "tcp://localhost:8001", "Communication channel to server");
DEFINE_string(recv_socket, "tcp://*:8000", "Communication channel from server");
DEFINE_string(exe, "/usr/local/bin/phmmer", "hmmer3 executable");
DEFINE_string(db, "/home/hmmer/data/uniprot_sprot.fasta", "hmmer3 database");

using namespace std;

void write_file(const string& contents, char* file) {
  CHECK_NOTNULL(file);
  ofstream f(file);
  f << contents;
  f.close();
}

// TODO(cmiles) parse output in tmp_out
void parse_output(char* filename, Response* response) {
  CHECK_NOTNULL(filename);
  CHECK_NOTNULL(response);
  response->set_message("hello");
}

void process(const Request& request, Response* response) {
  CHECK_NOTNULL(response);

  char tmp_in [L_tmpnam];
  char tmp_out[L_tmpnam];
  tmpnam(tmp_in);
  tmpnam(tmp_out);

  write_file(request.sequence(), tmp_in);

  static const char pattern[] = "{{EXE}} -o {{OUT}} {{IN}} {{DB}}";
  ctemplate::StringToTemplateCache("hmmer", pattern, ctemplate::DO_NOT_STRIP);

  ctemplate::TemplateDictionary dict("process");
  dict.SetValue("DB", FLAGS_db.c_str());
  dict.SetValue("EXE", FLAGS_exe.c_str());
  dict.SetValue("IN", tmp_in);
  dict.SetValue("OUT", tmp_out);

  string cmd;
  ctemplate::ExpandTemplate("hmmer", ctemplate::DO_NOT_STRIP, &dict, &cmd);
  system(cmd.c_str());
  parse_output(tmp_out, response);

  remove(tmp_in);
  remove(tmp_out);
}

int main(int argc, char* argv[]) {
  using zmq::context_t;
  using zmq::socket_t;

  google::InitGoogleLogging(argv[0]);
  google::ParseCommandLineFlags(&argc, &argv, true);

  // Verify that the version of the protocol buffer library that we linked
  // against is compatible with the version of the headers we compiled against
  GOOGLE_PROTOBUF_VERIFY_VERSION;

  context_t ctx(1);
  socket_t sender(ctx, ZMQ_PUSH);
  socket_t receiver(ctx, ZMQ_PULL);
  sender.connect(FLAGS_send_socket.c_str());
  receiver.bind(FLAGS_recv_socket.c_str());

  Request req;
  CHECK(proto_recv(&req, &receiver));

  Response resp;
  process(req, &resp);
  CHECK(proto_send(resp, &sender));
}

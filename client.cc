#include "hmmer.pb.h"
#include "util.h"

#include <iostream>
#include <fstream>
#include <string>

#include <gflags/gflags.h>
#include <glog/logging.h>
#include <zmq.hpp>

DEFINE_string(send_socket, "tcp://localhost:8000", "Communication channel to server");
DEFINE_string(recv_socket, "tcp://*:8001", "Communication channel from server");
DEFINE_string(fasta, "", "File containing the query sequence in FASTA format");

using namespace std;

void read_sequence_or_die(const string& file, Request* r) {
  CHECK_NOTNULL(r);

  string sequence;
  string line;

  ifstream in(file.c_str());
  while(in.good()) {
    getline(in, line);
    sequence += line + "\n";
  }

  r->set_sequence(sequence);
}

int main(int argc, char* argv[]) {
  using zmq::context_t;
  using zmq::socket_t;

  google::InitGoogleLogging(argv[0]);
  google::ParseCommandLineFlags(&argc, &argv, true);

  CHECK(!FLAGS_fasta.empty()) << "Failed to provide required argument --fasta";

  // Verify that the version of the protocol buffer library that we linked
  // against is compatible with the version of the headers we compiled against
  GOOGLE_PROTOBUF_VERIFY_VERSION;

  context_t ctx(1);
  socket_t sender(ctx, ZMQ_PUSH);
  socket_t receiver(ctx, ZMQ_PULL);
  sender.connect(FLAGS_send_socket.c_str());
  receiver.bind(FLAGS_recv_socket.c_str());

  Request req;
  read_sequence_or_die(FLAGS_fasta, &req);
  CHECK(proto_send(req, &sender));

  Response resp;
  CHECK(proto_recv(&resp, &receiver));
  cout << "Received " << resp.alignments_size() << " alignments" << endl;
}

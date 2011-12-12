#include "hmmer.pb.h"
#include "util.h"

#include <iostream>
#include <fstream>
#include <string>

#include <gflags/gflags.h>
#include <glog/logging.h>
#include <zmq.hpp>

DEFINE_string(send_socket, "tcp://localhost:8001", "Communication channel to server");
DEFINE_string(recv_socket, "tcp://*:8000", "Communication channel from server");

using namespace std;

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
  resp.set_message("hello");
  CHECK(proto_send(resp, &sender));
}

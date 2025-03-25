#include "hello_server.hpp"

#include "hello.grpc.pb.h"

#include <cstdlib>
#include <iostream>

class HelloServiceImpl final : public Bug::HelloService::Service {
private:
  grpc::Status
  HelloStream(grpc::ServerContext *context,
              google::protobuf::Empty const *request,
              grpc::ServerWriter<Bug::HelloResponse> *writer) final {
    std::cout << "HelloServiceImpl - HelloStream START\n";
    grpc::Status sts{};
    while (!context->IsCancelled()) {
      Bug::HelloResponse response{};
      response.set_value("Hello World");
      bool written = writer->Write(response);
      if (!written) {
        std::cout << "Write failure, break\n";
        sts = grpc::Status{grpc::StatusCode::CANCELLED, "Write failure, break"};
        break;
      }
      std::cout << "Write done\n";
      std::this_thread::sleep_for(std::chrono::seconds{1});
    }
    std::cout << "HelloServiceImpl - HelloStream END\n";
    return sts;
  }
};

int main() {
  std::cout << "gRPC version: " << grpc_version_string() << "\n";
  auto service = std::make_unique<HelloServiceImpl>();
  auto server = std::make_unique<HelloServer>("0.0.0.0:50051", service.get());
  server->start();
  server->wait();
  // server dtor
  // service dtor
}

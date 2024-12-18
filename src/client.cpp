#include "hello_client.hpp"

#include <iostream>
#include <thread>

void helloStream(Bug::HelloService::Stub* stub)
{
   while (true)
   {
      std::cout << "Client_A - HelloStream START\n";
      grpc::ClientContext context {};
      google::protobuf::Empty request {};
      Bug::HelloResponse response {};
      auto reader = stub->HelloStream(&context, request);
      while (true)
      {
         bool ok = reader->Read(&response);
         if (!ok)
         {
            std::cout << "Read failure, break\n";
            break;
         }
         std::cout << "Read done\n";
      }
      std::cout << "Client_A - HelloStream END\n";
      std::this_thread::sleep_for(std::chrono::seconds{1});
   }
}

int main()
{
   HelloClient client {"0.0.0.0:50051"};
   auto* stub = client.stub();
   auto t = std::thread(&helloStream, stub);
   t.join();
}

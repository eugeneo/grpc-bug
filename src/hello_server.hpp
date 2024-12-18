#pragma once

#include <grpc/grpc.h>
#include <grpcpp/server_builder.h>

#include <string>
#include <thread>

class HelloServer final
{
public:
   explicit HelloServer(std::string const& listenAddr, grpc::Service* service)
   {
      std::ignore = m_builder.AddListeningPort(listenAddr, grpc::InsecureServerCredentials());
      std::ignore  = m_builder.RegisterService(service);
   }

   ~HelloServer()
   {
      stop();
   }

   void start()
   {
      m_server     = m_builder.BuildAndStart();
      m_threadWait = std::thread([this] {
         wait();
      });
   }

   void wait()
   {
      if (m_server)
      {
         m_server->Wait();
      }
   }

   void stop()
   {
      if (m_server)
      {
         m_server->Shutdown();
         m_threadWait.join();
      }
   }

private:
   std::unique_ptr< grpc::Server > m_server;
   grpc::ServerBuilder m_builder;
   std::thread m_threadWait;
};

#pragma once

#include "hello.grpc.pb.h"

#include <grpc/grpc.h>
#include <grpcpp/channel.h>
#include <grpcpp/create_channel.h>

#include <memory>

class HelloClient
{
public:
   explicit HelloClient(std::string serverAddr)
   {
      grpc::ChannelArguments args {};
      args.SetInt(GRPC_ARG_INITIAL_RECONNECT_BACKOFF_MS, 1000);
      args.SetInt(GRPC_ARG_MIN_RECONNECT_BACKOFF_MS, 1000);
      args.SetInt(GRPC_ARG_MAX_RECONNECT_BACKOFF_MS, 1000);

      m_channel = grpc::CreateCustomChannel(serverAddr, grpc::InsecureChannelCredentials(), args);
      m_stub = Bug::HelloService::NewStub(m_channel);
   }

   auto* stub() noexcept { return m_stub.get(); }

private:
   std::shared_ptr< grpc::Channel > m_channel;
   std::unique_ptr< Bug::HelloService::Stub > m_stub;
};

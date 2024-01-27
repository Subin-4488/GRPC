#include <chrono>
#include <iostream>
#include <string>
#include <mutex>

#include <grpc/grpc.h>
#include <grpcpp/security/server_credentials.h>
#include <grpcpp/server.h>
#include <grpcpp/server_builder.h>
#include <grpcpp/server_context.h>

#include "chat.grpc.pb.h"

using grpc::Server;
using grpc::ServerBuilder;
using grpc::ServerContext;
using grpc::ServerReader;
using grpc::ServerReaderWriter;
using grpc::ServerWriter;
using grpc::Status;

using chatproto::ChatMessage;
using chatproto::ChatService;

class ChatServer final : public ChatService::Service
{
public:
      explicit ChatServer();
      ~ChatServer() override;
      ;
      ::grpc::Status communicate(::grpc::ServerContext *, ::grpc::ServerReaderWriter<::chatproto::ChatMessage, ::chatproto::ChatMessage> *) override;
  private:
      std::mutex lock;
};

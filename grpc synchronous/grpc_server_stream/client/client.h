#include <iostream>
#include <grpc/grpc.h>
#include <grpcpp/channel.h>
#include <grpcpp/client_context.h>
#include <grpcpp/create_channel.h>
#include <grpcpp/security/credentials.h>

#include "mx.grpc.pb.h"

using grpc::Channel;
using grpc::ChannelInterface;
using grpc::ClientContext;
using grpc::ClientReader;
using grpc::ClientReaderWriter;
using grpc::ClientWriter;
using grpc::Status;

using ServerStreamEX::EvenFlag;
using ServerStreamEX::RandomGenerator;
using ServerStreamEX::RandomNumber;

class Client
{
public:
      Client(std::shared_ptr<grpc::ChannelInterface> channel);
      void GenerateRandomNumber(EvenFlag &);

private:
      std::unique_ptr<RandomGenerator::Stub> stub_;
};

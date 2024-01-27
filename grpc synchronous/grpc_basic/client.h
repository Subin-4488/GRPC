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

using Greet::Greetings;
using Greet::GreetUser;
using Greet::PersonData;
using Greet::User;

class Client
{
public:
      Client(std::shared_ptr<grpc::ChannelInterface> channel);
      void WishUser(PersonData&, Greetings*);

private:
      std::unique_ptr<GreetUser::Stub> stub_;
};
#include <iostream>
#include <memory>
#include <string>

#include <grpc/support/log.h>
#include <grpcpp/grpcpp.h>

#include "mx.grpc.pb.h"

using grpc::Channel;
using grpc::ClientAsyncResponseReader;
using grpc::ClientContext;
using grpc::CompletionQueue;
using grpc::Status;

using Greet::Greetings;
using Greet::GreetUser;
using Greet::PersonData;
using Greet::User;

class Client
{
public:
      Client(std::shared_ptr<grpc::Channel> channel);
      void WishUser(PersonData, Greetings *);
      ~Client()
      {
            completionQueue.Shutdown();
      }

private:
      std::unique_ptr<GreetUser::Stub> stub_;
      ClientContext context;
      CompletionQueue completionQueue;

      Status status;
      void *tag;
      bool ok;
};

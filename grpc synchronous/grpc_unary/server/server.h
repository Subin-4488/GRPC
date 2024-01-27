#include <grpc/grpc.h>
#include <grpcpp/security/server_credentials.h>
#include <grpcpp/server.h>
#include <grpcpp/server_builder.h>
#include <grpcpp/server_context.h>

#include <iostream>
#include <string>

#include "mx.grpc.pb.h"

using grpc::Server;
using grpc::ServerBuilder;
using grpc::ServerContext;
using grpc::ServerReader;
using grpc::ServerReaderWriter;
using grpc::ServerWriter;
using grpc::Status;

using Greet::Greetings;
using Greet::GreetUser;
using Greet::PersonData;
using Greet::User;

class GreetServer final : public GreetUser::Service
{

public:
    explicit GreetServer();
    ~GreetServer() override;
    ::grpc::Status WishUser(::grpc::ServerContext *, const ::Greet::PersonData *, ::Greet::Greetings *) override;

private:
    PersonData personData;
    Greetings greetings;
};